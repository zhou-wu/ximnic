#include "UsrDef.h"
#include "InOutComm.h"
#include "DgComm.h"
#include "FanCtr.h"
#include "DataTransFunc2Drv.h"
#include "PmvCtr.h"
#include "Signal.h"
#include "MiscFunc.h"   ///

union UNION_DGCOMMCTR_FLAGS unDgCommCtr;
union UNION_DGSET_FLAGS unDgSetFlgs;
unsigned char u8DgCommRst1s;
unsigned char u8DgCommRstTim;
unsigned char DgCommBuf[U8_DBG_LEN];
static unsigned char u8DgCommByteCnt;
static unsigned char u8DgCommBitCnt;
static unsigned char u8DgTrgHzOut;
static unsigned char u8DgTrgHzDotOut;
static unsigned short u16DgFanSpdOut;
static unsigned char u8DgFWValveOut;
static unsigned short u16DgTrgStepPulseOut;
static unsigned char u8DgTrgHzIn;
static unsigned char u8DgTrgHzDotIn;
static unsigned short u16DgFanSpdIn;
static unsigned char u8DgFWValveIn;
static unsigned short u16DgTrgStepPulseIn;
static unsigned char u8CommNum;
static unsigned char u8DgDisp;
static unsigned char u8FrcDefrost;
static unsigned short u16FanSpdDisp, 
                      u16FanFluxDisp,
                      u16CompFluxDisp;
static unsigned char  u8ErrCodeP, 
                      u8ErrCodeF, 
                      u8ErrCodeE, 
                      u8ErrCodeH, 
                      u8LmtCodeL, 
                      u8DgDisp, 
                      u8ErrCode1, 
                      u8ErrCode2;
unsigned char u8ZGInCtrTrgHzOut;
unsigned short u16ZGInCtrlFanSpd;
unsigned short u16Dg2Drv;
union UNION_DGCOMM_FLAGS unDgCommFlgs;

/* 有待考究 */
volatile unsigned char *gp_sci1_tx_address;               /* SCI1 transmit buffer address */
volatile unsigned short g_sci1_tx_count;                  /* SCI1 transmit data number */
volatile unsigned char *gp_sci1_rx_address;               /* SCI1 receive buffer address */
volatile unsigned short g_sci1_rx_count;                  /* SCI1 receive data number */
volatile unsigned short g_sci1_rx_length;                 /* SCI1 receive data length */

static void DgJUDGE(void);
static void ErrDispDeal(void);

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DgCommInit(void)
{
#if 0
  ICU.IER[IER_ICU_IRQ3].BIT.IEN3 = 0; // Disable IRQ3
  //DBGCOMM_CLK_PORT_IN;              //
  DBGCOMM_CLK_PORT_MODE;

  MPC.PWPR.BIT.B0WI = 0;              //
  MPC.PWPR.BIT.PFSWE = 1;             //
  MPC.PB4PFS.BIT.ISEL = 1;            // PB4 used as IRQ3 input
  MPC.PWPR.BIT.PFSWE = 0;             //
  MPC.PWPR.BIT.B0WI = 1;              //

  ICU.IRQCR[3].BIT.IRQMD = 2;         // Set rise edge
  ICU.IPR[IPR_ICU_IRQ3].BIT.IPR = 7;  //
  ICU.IER[IER_ICU_IRQ3].BIT.IEN3 = 1; // Enalbe IRQ3
#endif

  unDgCommFlgs.u8Byte = 0;
  u8DgCommBitCnt = 0;
  u8DgCommByteCnt = 0;
  u8InDoorSetTim = 10;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DgCommSrv(void)
{
  unsigned short u16DgTmp1 = 0;
  unsigned char i = 0;

  if (u8DgCommRst1s > 50)    //1S更改为5S
  {
    //1s Not communication from debugger
    unDgCommFlgs.u8Byte = 0;
    u8DgCommBitCnt = 0;
    u8DgCommByteCnt = 0;
  }
  if (u8DgCommRstTim > 10)   //5s更改为10s
  {
    u8DgTrgHzOut = 0;
    u8DgTrgHzDotOut = 0;
    u16DgFanSpdOut = 0;
    u8DgFWValveOut = 0;
    u16DgTrgStepPulseOut = 0;
    u16DgInFanSpd = 0;
  }
  if(u8InDoorSetTim > 10)
  {
    u8DgTrgHzIn = 0;
    u8DgTrgHzDotIn = 0;
    u16DgFanSpdIn = 0;
    u8DgFWValveIn = 0;
    u16DgTrgStepPulseIn = 0;
  }

  if (unDgCommFlgs.bits.DgDatRec)
  {
    //get data
    unDgCommFlgs.bits.DgDatRec = 0;

    if ((DgCommBuf[0] == 0x96) && (DgCommBuf[1] == 0xAC) && (DgCommBuf[u8CommNum - 1] == 0x5A))
    {
      if (DgCommBuf[u8CommNum] == CommChkSum(DgCommBuf, u8CommNum))
      {
        {
          if(u8DgTrgHzOut != DgCommBuf[3])
          {
            u8DgTrgHzOut = DgCommBuf[3];
            unDgCommCtr.bits.DgOutFreq = 1;
          }

          u8DgTrgHzDot = DgCommBuf[4];
          if (DgCommBuf[4])
          {
            u8DgDisp = DgCommBuf[4];
          }
          else
          {
            u8DgDisp = 0;
          }

          u16DgTmp1 = 0;
          u16DgTmp1 = ((unsigned short)DgCommBuf[10] << 8) + (unsigned short)DgCommBuf[9];  //PA 室外风机风速设定0-1599
          if (u16DgFanSpdOut != u16DgTmp1)
          {
            u16DgFanSpdOut = u16DgTmp1;
            unDgCommCtr.bits.DgOutFan = 1;
          }

          u16DgTmp1 = ((unsigned short)DgCommBuf[7] << 8) + (unsigned short)DgCommBuf[6];


          if ((u16DgTmp1 >= 50) && (u16DgTmp1 <= 519))
          {
            if (u16DgTrgStepPulseOut != u16DgTmp1)
            {
              u16DgTrgStepPulseOut = u16DgTmp1;
              unDgCommCtr.bits.DgOutPmv = 1;
            }
          }
          u16DgTmp1 = 0;

          if (u8DgFWValveOut != DgCommBuf[8])
          {
            u8DgFWValveOut = DgCommBuf[8];
            unDgCommCtr.bits.DgOutFw = 1;
          }

          if (u8DgDefrost != DgCommBuf[13])
          {
            u8DgDefrost = DgCommBuf[13];
            if ((5 == u8DgDefrost) && (C_ModeHeat == u8InSetMode))
            {
              u8FrcDefrost = 1;
            }
          }

          u16DgTmp1 = ((unsigned short)DgCommBuf[12] << 8) + (unsigned short)DgCommBuf[11]; //SA 室内风机风速设定0-2000
          if (u16DgInFanSpd != u16DgTmp1)
          {
            u16DgInFanSpd = u16DgTmp1;
            unDgCommCtr.bits.DgInFan = 1;
          }

          if ((u8CommNum - 4) >= 12)
          {
            u16Dg2Drv = DgCommBuf[14];      //debug
          }

          // 第五字节是取消限频和故障设定
          u8DgCanLimit = DgCommBuf[5];

          unDgSetFlgs.u8Byte = 0;

          if (1 == u8DgCanLimit)
          {
            unDgSetFlgs.bits.NoSendErr = 1; 
          }
          else if (2 == u8DgCanLimit)
          {
            unDgSetFlgs.bits.NoVoltLim = 1;
          }
          else if (3 == u8DgCanLimit)
          {
            unDgSetFlgs.bits.NoCurrLim = 1;
          }
        }

        ErrDispDeal();

        i = 34;
        DgCommBuf[0] = 0x69;
        DgCommBuf[1] = 0XAC;
        DgCommBuf[2] = i;

        DgCommBuf[3] = u8TempT1;
        DgCommBuf[4] = u8TempT2;
        DgCommBuf[5] = u8TempT3;
        DgCommBuf[6] = u8TempT4;
        DgCommBuf[7] = u8TempT5  + 50;
        DgCommBuf[8] = u8TempIpm;

        DgCommBuf[9] = (unsigned char)(u16FanFBSpd & 0x00ff);
        DgCommBuf[10] = (unsigned char)(u16FanFBSpd >> 8);
        DgCommBuf[11] = u8AcCurrAd;
        DgCommBuf[12] = u8AcVoltAd;
        DgCommBuf[13] = u8HalfDcVoltFil;

        DgCommBuf[14] = (unsigned char)(u16CurrPulse & 0x00ff);   //pmv
        DgCommBuf[15] = (unsigned char)(u16CurrPulse >> 8);
        DgCommBuf[16] = u8InSetTemp;
        DgCommBuf[17] = u8EnergyFreq;  //FT
        DgCommBuf[18] = u8RealRunFreq; //Fr
        DgCommBuf[19] = u8RunCtrFreq; //OT
        DgCommBuf[20] = u8LastTrgFreq; //OC
        DgCommBuf[21] = u8InSetMode;   //od

        u16FanFluxDisp = (unsigned short)Drv2SysBuff[U8_FAN_FLUX] * 8;
        u16CompFluxDisp = (unsigned short)Drv2SysBuff[U8_COMP_FLUX] * 8;
        DgCommBuf[22] = (unsigned char)(u16FanFluxDisp & 0x00ff);
        DgCommBuf[23] = (unsigned char)(u16FanFluxDisp >> 8);
        DgCommBuf[24] = (unsigned char)(u16CompFluxDisp & 0x00ff);
        DgCommBuf[25] = (unsigned char)(u16CompFluxDisp >> 8);
        DgCommBuf[26] = u8LmtCodeL;
        DgCommBuf[27] = u8ErrCode;
        DgCommBuf[28] = u8ErrCode1;
        DgCommBuf[29] = u8ErrCode2;
        DgCommBuf[30] = u8T1_TsArea;
        DgCommBuf[31] = u8T1_TsAreaPre;
        DgCommBuf[32] = u8TempT2b;
        DgCommBuf[33] = 0; //u8IndoorRunFanSpeed;
        DgCommBuf[34] = 0; //u8OutStatus;
        DgCommBuf[35] = u8TrgDiscPipeTemp;
        DgCommBuf[36] = 0; //u8DefCndtn;     //1-6
        u8CommNum = i + 4;
        DgCommBuf[i + 3] = 0x5A;
        DgCommBuf[i + 4] = CommChkSum(DgCommBuf, i + 4);
        unDgCommFlgs.bits.DgCommSta = 1;
        unDgCommFlgs.bits.DgDatRec = 0;
        unDgCommFlgs.bits.DgSendHold = 0;
        
        u8DgCommByteCnt = 0;
        u8DgCommBitCnt = 0;
      }
      else
      {
        unDgCommFlgs.bits.DgCommSta = 0;
        unDgCommFlgs.bits.DgDatRec = 0;
        unDgCommFlgs.bits.DgSendHold = 0;

        u8DgCommByteCnt = 0;
        u8DgCommBitCnt = 0;
      }
    }
    else
    {
      unDgCommFlgs.bits.DgCommSta = 0;
      unDgCommFlgs.bits.DgDatRec = 0;
      unDgCommFlgs.bits.DgSendHold = 0;

      u8DgCommByteCnt = 0;
      u8DgCommBitCnt = 0;
    }
  }
  DgJUDGE();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void ErrDispDeal(void)
{
  unsigned char u8Tmp = 0;
  u8ErrCodeE = 0;
  u8ErrCodeH = 0;
  u8ErrCodeP = 0;
  u8ErrCodeF = 0;
  u8LmtCodeL = 0;
  u8ErrCode1 = 0;
  u8ErrCode2 = 0;

  u8Tmp = 0;
  //1st data(low 4 bits)
  //L1-L15
  if ((VDC_DECT_2_LOW_VOLT == u8MtrErrCode) || (VDC_DECT_2_LOW_VOLT == u8FMErrCode))
  {
    //L1:压缩机/风机电压过低
    u8LmtCodeL = 0x01;
  }
  else if ((VDC_DECT_2_OVER_VOLT == u8MtrErrCode) || (VDC_DECT_2_OVER_VOLT == u8FMErrCode))
  {
    //L2:压缩机/风机电压过高
    u8LmtCodeL = 0x02;
  }
  else if ((u8MtrErrCode == START_UP_MISS) || (u8MtrErrCode == DLT_THETA_2_LOSS_STEP) || (u8MtrErrCode == RMS_30S_2_OVER_LOAD))
  {
    //L4:压缩机启动故障、同步故障、MCE故障
    u8LmtCodeL = 0x04;
  }
  else if (u8MtrErrCode == BEMF_DECT_2_ZERO_SPD)
  {
    //L5:压缩机零速故障
    u8LmtCodeL = 0x05;
  }
  else if (u8MtrErrCode == LOW_CURR_2_LOSS_PHS)
  {
    //L7:压缩机缺相故障
    u8LmtCodeL = 0x07;
  }
  else if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
  {
    //L8:压缩机失速故障
    u8LmtCodeL = 0x08;
  }
  else if(unFreqLimFlgs.bits.CurrLim)
  {
    //LF:电流限频
    u8LmtCodeL = 0x0F;
  }
  else if (unFreqLimFlgs.bits.IpmTempLim)
  {
    //LE:IPM模块高温限频
    u8LmtCodeL = 0x0E;
  }
  else if (unFreqLimFlgs.bits.VoltLim)
  {
    //LA:电压限频
    u8LmtCodeL = 0x0A;
  }
  else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
  {
    //LB:蒸发器温度限频
    u8LmtCodeL = 0x0B;
  }
  else if (unFreqLimFlgs.bits.T3TempLim)
  {
    //LC:冷凝器温度限频
    u8LmtCodeL = 0x0C;
  }
  else if (unFreqLimFlgs.bits.T5TempLim)
  {
    //LD:排气温度限频
    u8LmtCodeL = 0x0D;
  }

  //2nd data(high 4 bits)
  if (0x01 == u8LmtCodeL)
  {
    if ((VDC_DECT_2_OVER_VOLT == u8MtrErrCode) || (VDC_DECT_2_OVER_VOLT == u8FMErrCode))
    {
      //L2:压缩机/风机电压过高
      u8LmtCodeL |= 0x20;
    }
    else if ((u8MtrErrCode == START_UP_MISS) || (u8MtrErrCode == DLT_THETA_2_LOSS_STEP) || (u8MtrErrCode == RMS_30S_2_OVER_LOAD))
    {
      //L4:压缩机启动故障、同步故障、MCE故障
      u8LmtCodeL |= 0x40;
    }
    else if (u8MtrErrCode == BEMF_DECT_2_ZERO_SPD)
    {
      //L5:压缩机零速故障
      u8LmtCodeL |= 0x50;
    }
    else if (u8MtrErrCode == LOW_CURR_2_LOSS_PHS)
    {
      //L7:压缩机缺相故障
      u8LmtCodeL |= 0x70;
    }
    else if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
    {
      //L8:压缩机失速故障
      u8LmtCodeL |= 0x80;
    }
    else if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x02 == u8LmtCodeL)
  {
    if ((u8MtrErrCode == START_UP_MISS) || (u8MtrErrCode == DLT_THETA_2_LOSS_STEP) || (u8MtrErrCode == RMS_30S_2_OVER_LOAD))
    {
      //L4:压缩机启动故障、同步故障、MCE故障
      u8LmtCodeL |= 0x40;
    }
    else if (u8MtrErrCode == BEMF_DECT_2_ZERO_SPD)
    {
      //L5:压缩机零速故障
      u8LmtCodeL |= 0x50;
    }
    else if (u8MtrErrCode == LOW_CURR_2_LOSS_PHS)
    {
      //L7:压缩机缺相故障
      u8LmtCodeL |= 0x70;
    }
    else if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
    {
      //L8:压缩机失速故障
      u8LmtCodeL |= 0x80;
    }
    else if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x04 == u8LmtCodeL)
  {
    if (u8MtrErrCode == BEMF_DECT_2_ZERO_SPD)
    {
      //L5:压缩机零速故障
      u8LmtCodeL |= 0x50;
    }
    else if (u8MtrErrCode == LOW_CURR_2_LOSS_PHS)
    {
      //L7:压缩机缺相故障
      u8LmtCodeL |= 0x70;
    }
    else if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
    {
      //L8:压缩机失速故障
      u8LmtCodeL |= 0x80;
    }
    else if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x05 == u8LmtCodeL)
  {
    if (u8MtrErrCode == LOW_CURR_2_LOSS_PHS)
    {
      //L7:压缩机缺相故障
      u8LmtCodeL |= 0x70;
    }
    else if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
    {
      //L8:压缩机失速故障
      u8LmtCodeL |= 0x80;
    }
    else if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x07 == u8LmtCodeL)
  {
    if (u8MtrErrCode == FREQ_DECT_2_LOSS_SPD)
    {
      //L8:压缩机失速故障
      u8LmtCodeL |= 0x80;
    }
    else if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x08 == u8LmtCodeL)
  {
    if(unFreqLimFlgs.bits.CurrLim)
    {
      //LF:电流限频
      u8LmtCodeL = 0xF0;
    }
    else if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x0F == u8LmtCodeL)
  {
    if (unFreqLimFlgs.bits.IpmTempLim)
    {
      //LE:IPM模块高温限频
      u8LmtCodeL |= 0xE0;
    }
    else if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x0E == u8LmtCodeL)
  {
    if (unFreqLimFlgs.bits.VoltLim)
    {
      //LA:电压限频
      u8LmtCodeL |= 0xA0;
    }
    else if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x0A == u8LmtCodeL)
  {
    if (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim)
    {
      //LB:蒸发器温度限频
      u8LmtCodeL |= 0xB0;
    }
    else if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x0B == u8LmtCodeL)
  {
    if (unFreqLimFlgs.bits.T3TempLim)
    {
      //LC:冷凝器温度限频
      u8LmtCodeL |= 0xC0;
    }
    else if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }
  else if (0x0C == u8LmtCodeL)
  {
    if (unFreqLimFlgs.bits.T5TempLim)
    {
      //LD:排气温度限频
      u8LmtCodeL |= 0xD0;
    }
  }

  //P1-P3F
  if (unSysErrFlgs.bits.OverCurrErr || unCompErrFlgs2.bits.CpOvCurrErr)
  {
    //P3:过流保护
    u8ErrCodeP = 0x03;
  }
  else if (fgProtT5)
  {
    //P4
    u8ErrCodeP = 0x04;
  }
  else if (fgProtCoolT3)
  {
    //P5
    u8ErrCodeP = 0x05;
  }
  else if(unCompErrFlgs1.bits.FoErr)  //压缩机IPM 模块保护
  {
    //P6
    u8ErrCodeP = 0x06;
  }
  else if(fgProtCoolT2)
  {
    //P7
    u8ErrCodeP = 0x07;
  }
  else if(fgIPMProt)
  {
    //P8
    u8ErrCodeP = 0x08;
  }
  else if(unSysErrFlgs.bits.DCFanSpdErr)
  {
    //P9
    u8ErrCodeP = 0x09;
  }
  else if(fgProtHeatT2)
  {
    //P11
    u8ErrCodeP = 0x11;
  }
  else if ((VDC_DECT_2_LOW_VOLT == u8MtrErrCode) || (VDC_DECT_2_LOW_VOLT == u8FMErrCode))
  {
    //P11
    //u8ErrCodeP = 0x11;
  }
  else if ((VDC_DECT_2_OVER_VOLT == u8MtrErrCode) || (VDC_DECT_2_OVER_VOLT == u8FMErrCode))
  {
    //P12
    //u8ErrCodeP = 0x12;
  }
  else if (STOP_REASON_INSTANT_VOLTSTOP1 == u8MtrErrCode)
  {
    //P13
    //u8ErrCodeP = 0x13;
  }
  else if (STOP_REASON_INSTANT_VOLTSTOP2 == u8MtrErrCode)
  {
    //P14
    //u8ErrCodeP = 0x14;
  }
  else if (STOP_REASON_INSTANT_VOLTDROP == u8MtrErrCode)
  {
    //P15
    //u8ErrCodeP = 0x15;
  }
  /*
  else if (fgDryT1LowPro)
  {//P21
    u8ErrCodeP = 0x21;
  }
  else if (fgInHeatT2 || fgInCoolT2)
  {//P22
    u8ErrCodeP = 0x22;
  }
  else if (fgCoolT4HighPro || fgHeatT4HighPro || fgT4Freq0Stop)
  {//P24
    u8ErrCodeP = 0x24;
  }
  else if (fgSysErrTPAbnormal)
  {//P28
    u8ErrCodeP = 0x28;
  }
  else if (fgSysErrT3Abnormal)
  {//P29
    u8ErrCodeP = 0x29;
  }
  else if (fgHighPress)
  {//P30
    u8ErrCodeP = 0x30;
  }
  else if (fgLowPress)
  {//P31
    u8ErrCodeP = 0x31;
  }
  */

  if(unSysErrFlgs.bits.DrvCommErr)
  {
    //H10:压缩机通信故障
    u8ErrCodeH = 0x10;
  }
  //H1-H4,H6-H3F
  if (DLT_THETA_2_LOSS_STEP == u8MtrErrCode)
  {
    //H19
    //u8ErrCodeH = 0x19;
  }
  else if ((VDC_LOW_2_VOLT_DROP != u8MtrErrCode) && (u8MtrErrCode) && (u8MtrErrCode <= 8))
  {
    //H21-H28 (do not use H5)
    u8ErrCodeH = u8MtrErrCode + 0x20;
  }
  else if ((u8MtrErrCode >= 66) && (u8MtrErrCode <= 71))
  {
    //H31-H36
    u8ErrCodeH = u8MtrErrCode - 17;
  }

  if (unSysErrFlgs.bits.I2cErr)
  {
    //E10
    u8ErrCodeE = 0x10;
  }
  //E1-E3F
  else if (unSysErrFlgs.bits.InOutCommErr)
  {
    //E2
    u8ErrCodeE = 0x02;
  }
  else if (unTempErrFlgs.bits.T4Err)  //T4
  {
    //E4
    u8ErrCodeE = 0x04;
  }
  else if (unTempErrFlgs.bits.T3Err)  //T3
  {
    //E6
    u8ErrCodeE = 0x06;
  }
  else if (fgErrInEE)
  {
    //E7
    u8ErrCodeE = 0x07;
  }
  else if (unTempErrFlgs.bits.T5Err)  //TP,T5
  {
    //E5
    u8ErrCodeE = 0x05;
  }
  else if (unSysErrFlgs.bits.ACVoltLowErr)
  {
    //E9:一次侧电压过低
    u8ErrCodeE = 0x09;
  }
  else if (fgErrInFan)
  {
    //E11
    u8ErrCodeE = 0x11;
  }
  else if (unTempErrFlgs.bits.TipmErr)
  {
    //E12
    u8ErrCodeE = 0x12;
  }
  else if (DLT_THETA_2_LOSS_STEP == u8FMErrCode)
  {
    //E20
    u8ErrCodeE = 0x20;
  }
  else if (START_UP_MISS == u8FMErrCode)
  {
    //E29
    u8ErrCodeE = 0x29;
  }
  else if (u8FMErrCode && (u8FMErrCode <= 8))
  {
    //E21-E28
    u8ErrCodeE = 0x20 + u8FMErrCode;
  }

  //F
  /*
  if ()
  {//F6
    //u8ErrCodeF = 6;
  }
  else if ()
  {//F4
    //u8ErrCodeF = 4;
  }
  else if ()
  {//F9:
    //u8ErrCodeF = 0x9;
  }
  */

  //u8ErrCode1.bit7-bit6: 00:E,01:H,02:P,03:F
  if (u8ErrCodeE)
  {
    u8ErrCode1 = u8ErrCodeE;
    u8Tmp = 1;
  }
  else if (u8ErrCodeH)
  {
    u8ErrCode1 = u8ErrCodeH + 0x40;
    u8Tmp = 2;
  }
  else if (u8ErrCodeP)
  {
    u8ErrCode1 = u8ErrCodeP + 0x80;
    u8Tmp = 3;
  }
  else if (u8ErrCodeF)
  {
    u8ErrCode1 = u8ErrCodeF + 0xC0;
  }

  //u8ErrCode2.bit7-bit6: 00:E,01:H,02:P,03:F
  if (1 == u8Tmp)
  {
    if (u8ErrCodeH)
    {
      u8ErrCode2 = u8ErrCodeH + 0x40;
    }
    else if (u8ErrCodeP)
    {
      u8ErrCode2 = u8ErrCodeP + 0x80;
    }
    else if (u8ErrCodeF)
    {
      u8ErrCode2 = u8ErrCodeF + 0xC0;
    }
  }
  else if (2 == u8Tmp)
  {
    if (u8ErrCodeP)
    {
      u8ErrCode2 = u8ErrCodeP + 0x80;
    }
    else if (u8ErrCodeF)
    {
      u8ErrCode2 = u8ErrCodeF + 0xC0;
    }
  }
  else if (3 == u8Tmp)
  {
    if (u8ErrCodeF)
    {
      u8ErrCode2 = u8ErrCodeF + 0xC0;
    }
  }
}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DgJUDGE(void)
{
  u8DgTrgHzDot = u8DgTrgHzDotOut;

  if (unDgCommCtr.bits.DgOutFreq)
  {
    u8DgTrgHz = u8DgTrgHzOut;        //小板设置频率
  }
  else if (u8ZGInCtrTrgHzOut)
  {
    u8DgTrgHz = u8ZGInCtrTrgHzOut;   //室内外通信//锁频测试压缩机目标频率
  }
  else
  {
    u8DgTrgHz = 0;
  }

  if (unDgCommCtr.bits.DgOutFan)
  {
    u16DgFanSpd = u16DgFanSpdOut;    //小板设置风速
  }
  else if(u16ZGInCtrlFanSpd)
  {
    u16DgFanSpd = u16ZGInCtrlFanSpd; //室内外通信风速
  }
  else if(u8ACFanCtr)
  {
    if(u8ACFanCtr == 3)
    {
      //高风
      u16DgFanSpd = 3;
    }
    else
    {
      //低风
      u16DgFanSpd = 1;
    }
  }
  else
  {
    u16DgFanSpd = 0;
  }

  if (unDgCommCtr.bits.DgOutPmv)
  {
    u16DgTrgStepPls = u16DgTrgStepPulseOut;   //小板设置膨胀阀开度
  }
  else if (u16ZGInCtrTrgStepPulse)
  {
    u16DgTrgStepPls = u16ZGInCtrTrgStepPulse;//室内外通信//锁频测试电子膨胀阀开度
  }
  else
  {
    u16DgTrgStepPls = 0;
  }

  if (unDgCommCtr.bits.DgOutFw)
  {
    u8DgFWValve = u8DgFWValveOut;  //小板设置四通阀开关
  }
  else
  {
    u8DgFWValve = u8ZGInCtrFWValve;//室内外通信四通阀开关
  }

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void r_icu_irq3_interrupt_Srv(void)
{
#if TESTCPU_MODE == 1
  PORTA.PODR.BIT.B4 = 1;
#endif
  //DELAY5US
  u8DgCommRstTim = 0;
  u8DgCommRst1s = 0;
  if (unDgCommFlgs.bits.DgCommSta)
  {
    // Send.
    if (unDgCommFlgs.bits.DgSendHold == 0)
    {
      DBGCOMM_DATA_PORT_OUT;
      if (DgCommBuf[u8DgCommByteCnt] & 0x01)
      {
        DBGCOMM_DATA_H;
      }
      else
      {
        DBGCOMM_DATA_L;
      }
      DgCommBuf[u8DgCommByteCnt] >>= 1;
      u8DgCommBitCnt++;
      if (u8DgCommBitCnt > 7)
      {
        u8DgCommBitCnt = 0;
        u8DgCommByteCnt++;
        if (u8DgCommByteCnt > u8CommNum)
        {
          unDgCommFlgs.bits.DgSendHold = 1;
        }
      }
    }
    else
    {
      DBGCOMM_DATA_PORT_IN;

      unDgCommFlgs.bits.DgCommSta = 0;   //rec
      unDgCommFlgs.bits.DgDatRec = 0;
      unDgCommFlgs.bits.DgSendHold = 0;

      u8DgCommBitCnt = 0;
      u8DgCommByteCnt = 0;
    }
  }
  else
  {
    // Rece.
    DBGCOMM_DATA_PORT_IN;
    DgCommBuf[u8DgCommByteCnt] >>= 1;
    if (DBGCOMM_DATA_PORT)
    {
      DgCommBuf[u8DgCommByteCnt] |= 0x80;
    }
    else
    {
      DgCommBuf[u8DgCommByteCnt] &= 0x7f;
    }
    u8DgCommBitCnt++;
    if (u8DgCommBitCnt > 7)
    {
      u8DgCommBitCnt = 0;

      if (DgCommBuf[0] == 0x96)
      {
        u8DgCommByteCnt++;
        if (u8DgCommByteCnt > 1)
        {
          u8CommNum = DgCommBuf[2] + 4;
          if (u8CommNum > U8_DBG_LEN - 1)
          {
            u8CommNum = U8_DBG_LEN - 1;
          }

          if (u8DgCommByteCnt > u8CommNum)
          {
            unDgCommFlgs.bits.DgCommSta = 0;
            unDgCommFlgs.bits.DgDatRec = 1;
            unDgCommFlgs.bits.DgSendHold = 0;

            u8DgCommBitCnt = 0;
            u8DgCommByteCnt = 0;
          }
        }
      }
      else
      {
        u8DgCommBitCnt = 0;
        u8DgCommByteCnt = 0;
      }
    }
  }
  //DELAY5US;
#if TESTCPU_MODE == 1
  PORTA.PODR.BIT.B4 = 0;
#endif
}


