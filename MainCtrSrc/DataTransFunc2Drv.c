#include "UsrDef.h"
#include "TimeTick.h"
#include "DgComm.h"
#include "Signal.h"
#include "FanCtr.h"
#include "InOutComm.h"
#include "ParameterPro.h"
#include "CompCtr.h"
#include "DevCtr.h"
#include "MonitorCommSub.h"

unsigned char u8PFCEnFlg, u8ErrCnt;
unsigned char u8MtrErrCode, 
              u8FMErrCode, 
              u8MtrFBSta, 
              u8FMFBSta, 
              u8ErrCode, 
              u8CompErrClr, 
              u8FanErrClr, 
              u8Timer1msfg;
unsigned char DrvEepPara[222],/* *pMain2Drv,*pDrv2Main,*/ Main2Drv[30],Drv2Main[30];
unsigned char Drv2SysBuff[20];
unsigned long u32PFC_AC_Curr;
unsigned char u8LytranCompErrCode;
unsigned char u8LytranFanErrCode;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DataMainDrvCommInit(unsigned char *pMain2DrvBuf, unsigned char *pDrv2MainBuf)
{
#if 0
  if ( pMain2DrvBuf && pDrv2MainBuf )
  { 
    pMain2Drv = pMain2DrvBuf;
    pDrv2Main = pDrv2MainBuf;
  }
  else
  {
    pMain2Drv = 0;
    pDrv2Main = 0;
  }
#endif
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void Main2DrvLytran(signed long *p2CmpDrvBuf, signed long *p2FanDrvBuf)
{
  unsigned short u16Tmp = 0;

  if ( (!p2CmpDrvBuf) || (!p2FanDrvBuf) )
  {
    return;
  }

#if 0   ///debug only
  if (u8ErrCode < 101)
  {
    u8ErrCode = u8ErrStop;
  }

  if (25 == u8Force25HZTim )
  {
    u8ErrCode = 0;
  }

  Main2Drv[0] = u16Dg2Drv;

  if (u8PreHeatStopTim)
  {
    u16Tmp = 0;
  }
  else
  {
    ///u16Tmp = (unsigned short)((unsigned short)u8RunCtrFreq * 60 + (u8RunCtrFreqDot >> 1) + (u8RunCtrFreqDot / 10)); // u8RunCtrFreq * 60 + u8RunCtrFreqDot / 100 * 60
    u16Tmp = u8RunCtrFreq;
  }
#endif

  u16Tmp = u8RunCtrFreq;    ///debug only
  if ( CTR_FROM_PC == fgCompCtrSrc )
  {
    u16Tmp = u8PC2CompHz;
  }
  
  ///Main2Drv[1] = (unsigned char)(u16Tmp & 0x00FF);
  ///Main2Drv[2] = (unsigned char)(u16Tmp >> 8);
  u16Tmp *= 10;
  p2CmpDrvBuf[1] = u16Tmp;
  ///DgLedOn();
  
  Main2Drv[3] = (unsigned char)(u16CompAccel & 0x00FF);
  Main2Drv[4] = (unsigned char)(u16CompAccel >> 8);
  ///u16Tmp = u16CompAccel;
  ///u16Tmp *= 10;
  ///p2CmpDrvBuf[0] = u16Tmp;
  Main2Drv[5] = (unsigned char)(u16CompDecel & 0x00FF);
  Main2Drv[6] = (unsigned char)(u16CompDecel >> 8);
  ///u16Tmp = u16CompDecel;
  ///u16Tmp *= 10;
  ///p2CmpDrvBuf[0] = u16Tmp;
  if ( u16CompAccel > 0 )
  {
    u16Tmp = u16CompAccel;
  }
  else if ( u16CompDecel > 0 )
  {
    u16Tmp = u16CompDecel;
  }
  else
  {;}
  u16Tmp *= 10;
  p2CmpDrvBuf[0] = u16Tmp;
  p2CmpDrvBuf[0] = 10;       ///debug only
  
  Main2Drv[7] = 0;

  // Fan motor speed transfer.
  ///p2FanDrvBuf[0] = 100;
  u16Tmp = u16TrgFanSpd;  
  Main2Drv[8] = (unsigned char)(u16Tmp & 0x00ff);
  Main2Drv[9] = (unsigned char)(u16Tmp >> 8);
  u16Tmp *= 10;
  if ( CTR_FROM_PC == fgOutFanCtrSrc )
  {
    u16Tmp = u8PC2FanSpd;
    if ( u16Tmp > 3 )
    {
      u16Tmp *= 100;
    }
  }
  p2FanDrvBuf[1] = u16Tmp;

  Main2Drv[10] = 0;   //风机转向设置
  Main2Drv[11] = 0;

  if ((u8InRateTest) && (unSeltFlgs2.bits.TorqueOFF == 1))  //若转矩控制选择为1,则关闭转矩补偿
  {
    Main2Drv[12] = 0; //为0表示关闭转矩补偿
    p2CmpDrvBuf[4] = 0x08;
  }
  else
  {
    Main2Drv[12] = 1;
    p2CmpDrvBuf[4] = 0x00;
  }

  Main2Drv[13] = u8InRateTest;

  if (u8InRateTest || ( 0 == u8ZGPFCEnFlg ))
  {
    //能力测试要求关PFC且频率小于关PFC频率    或者    能力测试要求关PFC且内机目标频率小于关PFC频率
    //if ((unSeltFlgs3.bits.SeerPFCA_OFF == 1) && (((u8SeerFreq>>1) < U8_TEST_PFC_OFF_FREQ)||(u8ZGinCtrlTrgHzOut < U8_TEST_PFC_OFF_FREQ)))
    //{
    //  u8PFCEnFlg = 0;
    //}
    //else
    //{
    //  u8PFCEnFlg = 1;
    //}

    ///if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
    if((C_ModeCool == u8InSetModeBuf) || (C_ModeDry == u8InSetModeBuf) || (C_ModeForce == u8InSetModeBuf))
    {
      switch(u8InRateTest)
      {
        case 1://SeerTestMin
          if (unSeltFlgs3.bits.SeerPFCA_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 2://SeerTestMid
          if (unSeltFlgs3.bits.SeerPFCB_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 3://SeerTestRat
          if (unSeltFlgs3.bits.SeerPFCC_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 4://SeerTestMax
          if (unSeltFlgs3.bits.SeerPFCD_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 5://SeerTest5
          if (unSeltFlgs3.bits.SeerPFCE_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 6://SeerTest6
          if (unSeltFlgs3.bits.SeerPFCF_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 7://SeerTest7
          if (unSeltFlgs3.bits.SeerPFCG_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        default://SeerTestNO
          u8PFCEnFlg = 1;
          break;
      }
    }
    else//if(C_ModeHeat == u8InSetMode)
    {
      switch(u8InRateTest)
      {
        case 1://SeerTestMin
          if (unSeltFlgs6.bits.SeerPFCA_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 2://SeerTestMid
          if (unSeltFlgs6.bits.SeerPFCB_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 3://SeerTestRat
          if (unSeltFlgs6.bits.SeerPFCC_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 4://SeerTestMax
          if (unSeltFlgs6.bits.SeerPFCD_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 5://SeerTest5
          if (unSeltFlgs6.bits.SeerPFCE_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 6://SeerTest6
          if (unSeltFlgs6.bits.SeerPFCF_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        case 7://SeerTest7
          if (unSeltFlgs6.bits.SeerPFCG_OFF == 1)
          {
            u8PFCEnFlg = 0;
          }
          else
          {
            u8PFCEnFlg = 1;
          }
          break;

        default://SeerTestNO
          u8PFCEnFlg = 1;
          break;
      }
    }
  }
  else
  {
    //cool,dry,forcecool
    u8PFCEnFlg = 1;//驱动根据实际情况开关
  }

  // PFC enable flag setting
  Main2Drv[14] = u8PFCEnFlg;
  p2CmpDrvBuf[3] = u8PFCEnFlg;
  p2CmpDrvBuf[3] = 1;       ///debug only

  /*WARMING MODE*/
  if(0 == u8TrgHeatDuty)
  {
    Main2Drv[15] = 0;
  }
  else
  {
    Main2Drv[15] = 1;
  }

  /* Fast test flag setting */
  ///if (1 == unInSta.bits.FastTest)
  if ((1 == unInSta.bits.FastTest)||(unInSta.bits.SelfChk))
  {
    Main2Drv[16] = 1;
  }
  else
  {
    if((u8MtrFBSta != DRIVE_STATUS_RUN) && (u8FMFBSta != FM_STATUS_RUN))
    {
      Main2Drv[16] = 0;
    }
  }

  p2CmpDrvBuf[4] = 0;  ///debug only
  ///p2CmpDrvBuf[5] = 0;
  ///p2CmpDrvBuf[6] =;

  /* Running mode flag setting  */
  Main2Drv[17] = u8InSetMode;
  if ( C_ModeHeat == u8InSetMode )
  {
    p2CmpDrvBuf[7] = 0;
  }
  else if ( C_ModeCool==u8InSetMode )
  {
    p2CmpDrvBuf[7] = 1;
  }
  else
  {;}
}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DrvLytran2Main(signed long *pCompDrv2Buf, signed long *pFanDrv2Buf)
{
  signed long s32Tmp = 0;
  unsigned char u8Tmp = 0;
  unsigned short u16Tmp = 0;
  ByteTypeDef unTmpL;
  ByteTypeDef unTmpH;
  
  u8RealRunFreq = Drv2Main[0];
  s32Tmp = pCompDrv2Buf[2];
  s32Tmp /= 10;
  u8RealRunFreq = (unsigned char)s32Tmp;
  
  u8RealRunDot = Drv2Main[1];
  u8RealRunInterger = u8RealRunFreq;
  if (u8RealRunDot >= 50)
  {
    u8RealRunInterger++;
  }

  // Compressor running status transfer: RUN STOP & PROTECT
  u8MtrFBSta = Drv2Main[2];  ///???
  // Fan motor running speed feedback.
  if(u16TrgFanSpd)
  {
    u16FanFBSpd = ((unsigned short)Main2Drv[4] << 8) + (unsigned short)Main2Drv[3];

    s32Tmp = pFanDrv2Buf[1];
    s32Tmp /= 10;
    u16Tmp = (unsigned short)s32Tmp;
    Drv2Main[4] = (unsigned char)(u16Tmp>>8);
    Drv2Main[3] = (unsigned char)(u16Tmp);
    u16FanFBSpd = ((unsigned short)Drv2Main[4] << 8) + (unsigned short)Drv2Main[3];  //原来写的是主控给驱动的风速指令
  }
  else
  {
    u16FanFBSpd = 0;
  }

  // Fan motor running status transfer: RUN STOP & PROTECT
  u8FMFBSta = Drv2Main[5];   ///???

  if (unSeltFlgs1.bits.ACVoltSelt)    //暂无
  {
    // AC voltage  value transform to ad value y = (x - 40)/1.284
    u8AcVoltAd = Drv2Main[6];
    // AC voltage RMS value
    //u8AcVoltRMS_60V = Drv2Main[7];
  }
  else
  {
    u8AcVoltAd = 0;
    //u8AcVoltRMS_60V = 0;
  }
  u8AcVoltAd = pCompDrv2Buf[5];
  
  u8HalfDcVoltRMS = Drv2Main[8]; //  Vdc / 2
  u8HalfDcVoltFil = Drv2Main[9];
  s32Tmp = pCompDrv2Buf[6];
  s32Tmp /= 2;
  u8HalfDcVoltRMS = u8HalfDcVoltFil = (unsigned char)s32Tmp;
  u8DcVoltAd = u8HalfDcVoltFil;

  u8AcCurrRMS_T10 = Drv2Main[10];
  s32Tmp = pCompDrv2Buf[4];
  ///s32Tmp *= 100; ???
  u8AcCurrRMS_T10 = (unsigned char)s32Tmp;
  if ( 0 == unSeltFlgs1.bits.FanSelt )
  {
    //AC fan pay : 0.3A
    if (u8AcCurrRMS_T10 <= 252)
    {
      u8AcCurrRMS_T10 += 3;
    }
  }
  // Input current RMS value transform to ad value y =(x - 0.92) / 0.117
  if (u8AcCurrRMS_T10 > 9)
  {
    u32PFC_AC_Curr = (unsigned short)u8AcCurrRMS_T10 * 10 - 85;
    u32PFC_AC_Curr = u32PFC_AC_Curr * 10 / 117;
  }
  else
  {
    u32PFC_AC_Curr = 0;
  }
  u8AcCurrAd = (unsigned char)u32PFC_AC_Curr;

  // Compressor and PFC error code
  ///u8MtrErrCode = Drv2Main[11];
  s32Tmp = pCompDrv2Buf[1];
  u8Tmp = (unsigned char)s32Tmp;
  switch(u8Tmp)
  {
    case 0x04: ///IPM-FO
      break;
    case 0x08: ///变频器软件过流
      break;
    case 0x09: ///速度推定异常
      break;
    case 0x10: ///缺相保护
      u8MtrErrCode = LOW_CURR_2_LOSS_PHS;
      break;
    case 0x11: ///EE数据错误
      break;
    case 0x12: ///EE初始化错误
      break;
    case 0x14: ///母线电压欠压
      u8MtrErrCode = VDC_DECT_2_LOW_VOLT;
      break;
    case 0x18: ///母线电压过压
      u8MtrErrCode = VDC_DECT_2_OVER_VOLT;
      break;
    case 0x20: ///检出AD偏置异常
      break;
    case 0x24: ///直流电压突变
      break;
    case 0x38: ///压缩机失速
      u8MtrErrCode = FREQ_DECT_2_LOSS_SPD;
      break;
    case 0x44: ///PFC软件过流
      break;
    case 0x4C: ///PFC硬件过流
      break;
    default:
      u8MtrErrCode = 0;
      break;
  }
  
  // Fan motor error code
  u8FMErrCode = Drv2Main[12];
  s32Tmp = pFanDrv2Buf[2];
  u16Tmp = (unsigned short)s32Tmp;
  unTmpL.u8Byte = (unsigned char)u16Tmp;
  unTmpH.u8Byte = (unsigned char)(u16Tmp>>8);
  if ( unTmpL.bits.no0 )
  {
    ///变频器软件过电流
  }
  else if ( unTmpL.bits.no1 )
  {
    ///PFC输入软件过电流
  }
  else if ( unTmpL.bits.no2 )
  {
    ///变频器硬件过电流检出
  }
  else if ( unTmpL.bits.no3 )
  {
    ///PFC输入硬件过电流检出
  }
  else if ( unTmpL.bits.no4 )
  {
    ///直流过电压
  }
  else if ( unTmpL.bits.no5 )
  {
    ///直流低电压
  }
  else if ( unTmpL.bits.no6 )
  {
    ///直流电压突变
  }
  else if ( unTmpL.bits.no7 )
  {
    ///速度推定异常
  }
  else if ( unTmpH.bits.no0 )
  {
    ///速度控制异常
  }
  else if ( unTmpH.bits.no1 )
  {
    ///缺相保护
  }
  else if ( unTmpH.bits.no2 )
  {
    ///检出AD偏置异常
  }
  else if ( unTmpH.bits.no3 )
  { 
    ///AD采样电路误差大
  }
  else if ( unTmpH.bits.no4 )
  {
    ///EE数据错误
  }
  else if ( unTmpH.bits.no5 )
  {
    ///EE初始化错误
  }
  else if ( unTmpH.bits.no6 )
  {
    ///变频器软件过电流
  }
  else if ( unTmpH.bits.no7 )
  {
    ///PFC输入软件过电流
  }
  else
  {}
  
  //
  u8PFCRunFlg = Drv2Main[13];
  u8PFCRunFlg = pCompDrv2Buf[7];

  u8LytranCompErrCode = (unsigned char)pCompDrv2Buf[1];
  u8LytranFanErrCode = (unsigned char)pFanDrv2Buf[2];
}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
void fb_bldc64_err(void)
{
  if((0 != u8MtrErrCode) || (0 != u8FMErrCode))
  {
    ///fgCompErrSta = 1; ///debug only
#if 0   ///debug only
    if ((0 == fgCompErrFix) && (0 == fgFmErrFix))
    {
      if ((u8ErrHoldTim > ERR_RST_TIM) && (u8CompSta == COMP_STOP))    //2013111901 clear Err after u8CompSta is 0
      {
        if(0 != u8MtrErrCode)
        {
          u8MtrErrCode = 0;
          u8CompErrClr = 1;
        }
        if(0 != u8FMErrCode)
        {
          u8FMErrCode = 0;
          u8FanErrClr = 1;
        }

        fgCompErrSta = 0;
        unCompErrFlgs1.u8Byte = 0;
        unCompErrFlgs2.u8Byte = 0;
      }
    }
#endif
  }

  switch (u8MtrErrCode)  //ex
  {
    case PEAK_RMS_2_OVER_CURR:            /* Compress over current */
    case PIN_VOLT_2_OVER_CURR:            /* High impedance stop for over current*/
    case PIN_START_2_CURR_ERR:            /* Inverter sampling circuit error */
      {
        unCompErrFlgs2.bits.CpOvCurrErr = 1;
      }
      break;
    case RMS_30S_2_OVER_LOAD:
      {
        unCompErrFlgs2.bits.MCEErr = 1;
      }
      break;
    case VDC_DECT_2_LOW_VOLT:
      {
        unCompErrFlgs1.bits.LoVoltErr = 1;
      }
      break;
    case VDC_DECT_2_OVER_VOLT:
      {
        //unCompErrFlgs2.bits.OvVoltErr = 1;
      }
      break;
    case LOW_CURR_2_LOSS_PHS:
      {
        /* lose phase*/
        unCompErrFlgs1.bits.PhsLossErr = 1;
      }
      break;
    case STOP_REASON_INSTANT_VOLTSTOP1:
      {
        unCompErrFlgs1.bits.LoVoltErr = 1;    //the Power Source lower than threshold
      }
      break;
    case VDC_LOW_2_VOLT_DROP:
    case STOP_REASON_INSTANT_VOLTSTOP2:
    case STOP_REASON_INSTANT_VOLTDROP:
      {
        unCompErrFlgs2.bits.VoltDrop = 1;
      }
      break;
    case DLT_THETA_2_LOSS_STEP:
    case FREQ_DECT_2_LOSS_SPD:             /* speed loss error */
      {
        unCompErrFlgs1.bits.SpeedErr = 1;
      }
      break;
    case HC_STOP_CONV_OVR_CURR_SW:
    case PFC_RMS_2_OVER_CURR:
    case PFC_CMP_2_OVER_CURR:
    case STOP_REASON_CNV_CURERR_PF:       /* pfc overcurrent  */
    case PFC_RMS_2_OVER_LOAD:
    case PFC_START_2_CURR_ERR:
      {
        unCompErrFlgs1.bits.PfcErr = 1;
      }
      break;
    case IPM_DECT_2_FO:
      {
        /* High impedance stop for over current*/
        unCompErrFlgs1.bits.FoErr = 1;
      }
      break;
    case BEMF_DECT_2_ZERO_SPD:
      {
        unCompErrFlgs2.bits.ZeroSpdErr = 1;
      }
      break;
    default:
      {
        if((u8FMErrCode) && (0 == u8MtrErrCode))            //gui8FMStopCode
        {
          if(u8FMErrCode == VDC_DECT_2_LOW_VOLT)
          {
            //unCompErrFlgs1.bits.LoVoltErr = 1;
          }
          else
          {
            unSysErrFlgs.bits.DCFanSpdErr = 1;
          }
        }
      }
      break;
  }

  /*
  注意:打开usCompErrCntTim、usFmErrCntTim计数代码
  if (unInSta.bits.FastTest)
  {
    u8ErrCnt = 1;
  }
  else
  {
    u8ErrCnt = ERR_CNT;
  }

  if ((u8MtrErrCode) && (u8MtrErrCode != VDC_LOW_2_VOLT_DROP) && (u8MtrErrCode != VDC_DECT_2_LOW_VOLT)
    && (u8MtrErrCode != VDC_DECT_2_OVER_VOLT) && (u8MtrErrCode != STOP_REASON_INSTANT_VOLTSTOP2))
  {
    if (fgCompErrSet)
    {
      if (u8CompErrCnt >= u8ErrCnt)
      {
        u8CompErrCnt = u8ErrCnt;
        fgCompErrFix = 1;
      }
    }
    else
    {
      u8CompErrCnt++;
      fgCompErrSet = 1;
    }
  }
  else
  {
    fgCompErrSet = 0;
    if (u8CompErrCnt)
    {
      if (usCompErrCntTim >= TIME2S_30MIN)  //900
      {
        usCompErrCntTim = 0;
        u8CompErrCnt = 0;
      }
    }
    else
    {
      usCompErrCntTim = 0;
    }
  }

  if ((u8FMErrCode) && (u8FMErrCode != VDC_DECT_2_LOW_VOLT) && (u8FMErrCode != VDC_DECT_2_OVER_VOLT))
  {
    if (fgFmErrSet)
    {
      if (u8FmErrCnt >= u8ErrCnt)
      {
        u8FmErrCnt = u8ErrCnt;
        fgFmErrFix = 1;
      }
    }
    else
    {
      u8FmErrCnt++;
      fgFmErrSet = 1;
    }
  }
  else
  {
    fgFmErrSet = 0;
    if (u8FmErrCnt)
    {
      if (usFmErrCntTim >= TIME2S_30MIN)
      {
        usFmErrCntTim = 0;
        u8FmErrCnt = 0;
      }
    }
    else
    {
      usFmErrCntTim = 0;
    }
  }

  */
}


