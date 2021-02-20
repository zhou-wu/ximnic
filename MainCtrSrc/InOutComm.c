#include "UsrDef.h"
#include "InOutComm.h"
#include "DgComm.h"
#include "DevCtr.h"
#include "ParameterPro.h"
#include "PmvCtr.h"
#include "FanCtr.h"
#include "DataTransFunc2Drv.h"
#include "Signal.h"
#include "TempSample.h"
#include "Signal.h"
#include "MiscFunc.h"

#define INCOMM_TXD_PORT_MODE    (PORTB.PMR.BIT.B5=1)

static union
{
  unsigned char u8Byte;
  struct
  {
    unsigned char RDatRec : 1;
    unsigned char no1   : 1;
    unsigned char no2   : 1;
    unsigned char no3   : 1;
    unsigned char no4   : 1;
    unsigned char no5   : 1;
    unsigned char no6   : 1;
    unsigned char no7   : 1;
  } bits;
} unOutCommRxdFlgs;

static union
{
  unsigned char u8Byte;
  struct
  {
    unsigned char TDelay : 1;
    unsigned char no1   : 1;
    unsigned char no2   : 1;
    unsigned char no3   : 1;
    unsigned char no4   : 1;
    unsigned char no5   : 1;
    unsigned char no6   : 1;
    unsigned char no7   : 1;
  } bits;
} unOutCommTxdFlgs;

union UNION_INIT_FLAGS unInitFlgs;
unsigned char u8TimInitComm;
unsigned char u8InOutCommTxdTim;
unsigned char u8InDoorSetTim;
unsigned char u8InOutCommErrTim;
unsigned short u16ZGInCtrFanSpd;
unsigned char u8ZGInCtrFWValve;
unsigned char InOutComBuf[U8_COMM_BUF_SIZE];
unsigned char u8InDoorSetTim;
unsigned char u8ACFanCtr;
unsigned short u16ZGInCtrTrgStepPulse;
unsigned char u8Recving;
unsigned char u8InOutCommBufCnt;
unsigned char InOutCommBuf[U8_COMM_BUF_SIZE];
unsigned char u8SendOrder1;
unsigned char u8SendOrder2;
unsigned char u8SendOrder3;
unsigned char u8InInst;
unsigned char u8PowerTypeNew;
unsigned char u8ZGPFCEnFlg;       //志高PFC使能标志 1:开PFC,0:关PFC
unsigned char u8OutCommTxdTim;
unsigned char u8InOutCmdLen;      ///u8INCOMMDATALEN

static void SendDelay(void);
static void OutComRecZG(void);
static void OutComRecZG40(void);
static void RecDecode40(void);
static void RecDecode(void);
static void OutComSend(void);
static void OutComSend40(void);
static void F_Uart5_SendEnabe(void);

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void InOutCommInit(void)
{
  unsigned char i = 0;
  
  for(i = 0; i < U8_COMM_BUF_SIZE; i++)
  {
    InOutComBuf[i] = 0;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommRec(void)
{
  ///INCOMM_RXD_PORT_IN; // #define INCOMM_RXD_PORT_IN      (PORTB.PDR.BIT.B6 = 0)
  if(u8InOutCommErrTim >= 20)
  {
    unPmvFastDirFlgs.bits.Fistincommok = 1;
    unPmvFastDirFlgs.bits.InDoorPmvRstOk = 0;
  }

  if ( unInSta.bits.SelfChk )
  {
    if ( u8InOutCommErrTim >= 7 )
    {
      if (0 == unDgSetFlgs.bits.NoSendErr)
      {
        unSysErrFlgs.bits.InOutCommErr = 1;
      }
    }
  }
  else
  {
    if (u8InOutCommErrTim >= 60)
    {
      //1min error
      if (0 == unDgSetFlgs.bits.NoSendErr)
      {
        //小板通信修改
        unSysErrFlgs.bits.InOutCommErr = 1;
      }
    }
  }

  if (unInitFlgs.bits.InitTim)
  {
    unInitFlgs.bits.InitTim = 0;
    u8TimInitComm = 0;
  }
  //增加当接收到一个以上字节后,若隔一段时间没有接收到下一个字节,则重新初始化通信
  if(u8Recving) // // 2012-06-01
  {
    if(u8InOutCommBufCnt >= 1)
    {
      if(u8TimInitComm > 30) // 30MS
      {
        u8TimInitComm = 0;
        u8InOutCommBufCnt = 0;
        InOutCommInit();
      }
    }
    else
    {
      u8TimInitComm = 0;
    }
  }
  else
  {
    u8TimInitComm = 0;
  }

  if (unOutCommRxdFlgs.bits.RDatRec)
  {
    unOutCommRxdFlgs.bits.RDatRec = 0;
    u8Recving = 0;
    switch(InOutCommBuf[0])
    {
      case U8_COMM_CODE:
        {
          unPmvFastDirFlgs.bits.Fistincommok = 1;
          RecDecode();
        }
        break;
      case U8_COMM40_HEAD_IN2OUT:
        {
          unPmvFastDirFlgs.bits.Fistincommok = 1;
          RecDecode40();
        }
        break;
      default:
        {
          if (u8InOutCommErrTim >= 60)
          {
            //indoor and outdoor communication error for 2min
            unSysErrFlgs.bits.InOutCommErr = 1;
          }
        }
        break;
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void RecDecode(void)
{
  if ((InOutCommBuf[15] != 0x55) && (InOutCommBuf[15] != 0xA5))
  {
    //Data error.
    //DI();
    InOutCommInit();
    //EI();
  }
  /*  else if(u8InInst==1)
  {
    if (InOutCommBuf[15] != 0xA5)
      { //Data error.
        //DI();
        InOutCommInit();
        //EI();
      }
    }
  */
  else if (InOutCommBuf[14] != CommChkSum(&InOutCommBuf[1], 13))
  {
    //Check sum error.
    u8InOutCommBufCnt = 0;
    //DI();
    InOutCommInit();
    //EI();
  }
  else
  {
    // Data is ok!
    OutComRecZG();
    u8SendOrder1 = 0xAA;
    u8SendOrder2 = 0x00;
    u8SendOrder3 = 0x00;
    SendDelay();
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void RecDecode40(void)
{
  if (InOutCommBuf[39] != CommChkSum(&InOutCommBuf[0], 39))
  {
    //Check sum error.
    u8InOutCommBufCnt = 0;
    //DI();
    InOutCommInit();
    //EI();
  }
  else
  {
    // Data is ok!
    OutComRecZG40();
    u8SendOrder1 = U8_COMM40_HEAD_OUT2IN;
    SendDelay();
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComRecZG40(void)
{
  ByteTypeDef undata_tmp;

  u8InInst = InOutCommBuf[3];             //室内外通信命令

  /// 正常模式
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    undata_tmp.u8Byte = 0;
    // = InOutCommBuf[4];                         //内机机型/内核
    u8PowerTypeNew = InOutCommBuf[5];             //当前室内机机型

    undata_tmp.u8Byte = InOutCommBuf[25];         //测试模式状态控制字
    unInSta.bits.FastTest = undata_tmp.bits.no0;  //快检
    unInSta.bits.SelfChk = undata_tmp.bits.no1;
    unInSta.bits.RationRun = undata_tmp.bits.no7; //进入能力测试//额定测试
    if (unInSta.bits.RationRun)                   //进入能力测试//额定测试
    {
      //unSeltFlgs3.u8Byte = undata_tmp.bits.no6; //PFC控制使能,与协议定义相反 //1:让驱动自行判断
      u8ZGPFCEnFlg = !undata_tmp.bits.no6;        //PFC控制使能,与协议定义相反 //1: 让驱动自行判断
      // = InOutCommBuf[26];                      //能力测试模式区域代码
      u8InRateTest = InOutCommBuf[27];            //能力测试模式测试点代码

      if ( ( u8InRateTest != u8InRateTestPre ) || 
           ( u8InSetModeBufPre != u8InSetModeBuf ))
      {
        u8WaitPFCTim = 150;
        u8InRateTestPre = u8InRateTest;
        u8InSetModeBufPre = u8InSetModeBuf;
      }
    }
    else
    {
      ///unSeltFlgs3.u8Byte = 0;
      ///unSeltFlgs6.u8Byte = 0;
      
      u8ZGPFCEnFlg = 1;
      u8InRateTest = SeerTestNo;
      u8InRateTestPre = u8InRateTest;
      u8WaitPFCTim = 0;
    }
    
    undata_tmp.u8Byte = InOutCommBuf[6];       //内机设定模式
    u8InSetModeBuf = undata_tmp.u8Byte & 0x0F; //&0b00001111;
    ///u8InSetModeBufPre = u8InSetModeBuf;
    if ( u8WaitPFCTim > 0 )
    {
      u8InSetMode = C_ModeOff;
    }
    else
    {
      u8InSetMode = u8InSetModeBuf;
    }
     
    // = undata_tmp.bits.no4;           //内机设定模式,静音
    // = undata_tmp.bits.no5;           //内机设定模式,睡眠
    // = undata_tmp.bits.no6;           //内机设定模式,经济
    // = undata_tmp.bits.no7;           //内机设定模式,强力

    undata_tmp.u8Byte = InOutCommBuf[7];//内机控制命令
    // = undata_tmp.bits.no0;           //强制收冷媒
    // = undata_tmp.bits.no1;           //强制化霜
    // = undata_tmp.bits.no2;           //快速制冷
    // = undata_tmp.bits.no3;           //快速制热
    // = undata_tmp.bits.no3;           //压机预热
    // = InOutCommBuf[8];               //预留
    unInitFlgs.bits.TempAD = 0;
    //LH K082 12-18K_1.0.0a_190930
    u8InSetTemp = InOutCommBuf[9] >> 1; //用户设定温度,设定温度*2
    u8TempT1 = InOutCommBuf[10];        //室内环境温度值T1,(温度+30)*2,分辨率0.5℃,偏置-30℃
    u8TempT2 = InOutCommBuf[11];        //室内蒸发器中部温度T2,(温度+30)*2,分辨率0.5℃,偏置-30℃
    u8TempT2b = InOutCommBuf[12];       //室内蒸发器出口温度T2B,(温度+30)*2,分辨率0.5℃,偏置-30℃
    // = InOutCommBuf[13];              //制冷模式温度补偿,(温度+30)*2,默认发60,分辨率0.5℃,偏置-30℃
    // = InOutCommBuf[14];              //制热模式温度补偿,(温度+30)*2,默认发60,分辨率0.5℃,偏置-30℃
    // = InOutCommBuf[15];              //室内湿度
    // = InOutCommBuf[16];              //其他温度

    u8InFanMode = InOutCommBuf[17];     //内风机运行档位
    // = InOutCommBuf[18];              //内风机实际转速,实际转速/10 rpm,交流风机发00H

    u8InErr1 = 0x00;
    u8InErr2 = 0x00;
    undata_tmp.u8Byte = InOutCommBuf[19];  //室内保护状态1
    fgProtInT2L = undata_tmp.bits.no0;     //蒸发器低温保护
    //fgProtInT2H = undata_tmp.bits.no1;   //蒸发器高温保护
    // = undata_tmp.bits.no2;              //室内防冷风

    undata_tmp.u8Byte = InOutCommBuf[20];  //室内保护状态2
    //
    undata_tmp.u8Byte = InOutCommBuf[21];  //室内故障停机状态1
    fgErrT1 = undata_tmp.bits.no0;         //T1传感器故障
    fgErrT2 = undata_tmp.bits.no1;         //T2传感器故障
    fgErrT2b = undata_tmp.bits.no2;        //T2B传感器故障
    //fgInCommErr = undata_tmp.bits.no3;   //室内外机通信故障
    //fgWaterFull = undata_tmp.bits.no4;   //水满保护
    fgErrInEE = undata_tmp.bits.no5;       //室内EE故障
    fgErrInFan = undata_tmp.bits.no6;      //内机风机失速故障
    //fgErrZero = undata_tmp.bits.no7;     //过零信号故障

    undata_tmp.u8Byte = InOutCommBuf[22];  //室内故障停机状态2

    // = InOutCommBuf[23];                 //预留
    // = InOutCommBuf[24];                 //内机功能状态

    if((InOutCommBuf[28] != 0) && (u8ZGInCtrTrgHzOut != InOutCommBuf[28])) //锁频测试压缩机目标频率
    {
      u8ZGInCtrTrgHzOut = InOutCommBuf[28];
      unDgCommCtr.bits.DgOutFreq = 0;
    }
    if ((InOutCommBuf[29] != 0) && (u16ZGInCtrFanSpd != ((unsigned short)InOutCommBuf[29] * 10)) ) //锁频测试外风机目标转速
    {
      u16ZGInCtrFanSpd = (unsigned short)InOutCommBuf[29] * 10;
      unDgCommCtr.bits.DgOutFan = 0;
    }
    if ((InOutCommBuf[30] >= 12) && (InOutCommBuf[30] <= 129))             //锁频测试电子膨胀阀开度
    {
      if (u16ZGInCtrTrgStepPulse != ((unsigned short)InOutCommBuf[30] << 2) )
      {
        u16ZGInCtrTrgStepPulse = (unsigned short)InOutCommBuf[30] << 2;
        unDgCommCtr.bits.DgOutPmv = 0;
      }
    }
    undata_tmp.u8Byte = InOutCommBuf[31];       //锁频测试外设控制状态字
    if (u8ZGInCtrFWValve != undata_tmp.bits.no0)//四通阀
    {
      u8ZGInCtrFWValve = undata_tmp.bits.no0;
      unDgCommCtr.bits.DgOutFw = 0;
    }
    // = undata_tmp.bits.no1;        //电磁阀SV1
    // = undata_tmp.bits.no2;        //电磁阀SV2
    // = undata_tmp.bits.no3;        //底盘电加热
    // = undata_tmp.bits.no4;        //曲轴加热带
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComRecZG(void)
{
  unsigned char data_tmp;
  // unsigned char u8TempT1HeatCmps;
  u8InInst = InOutCommBuf[2];               //室内外通信命令

  if(u8InInst == 0)
  {
    //00命令
    u8PowerTypeNew = InOutCommBuf[3];       //当前室内机机型
    u8InSetMode = InOutCommBuf[4];
    //u8ZGInCtrPowerMod= InOutCommBuf[5];   //ZG室内主控通信协议,室内拨码状态

    data_tmp = InOutCommBuf[6];             //外机主控,仅接收内风挡和制热温度补偿选择
    data_tmp >>= 4;
    u8InFanMode = data_tmp & 0x07;

    u8InErr1 = InOutCommBuf[7];
    if(u8InErr1 & 0x01)   //室内风机失速
    {
      fgErrInFan = 1;
    }
    else
    {
      fgErrInFan = 0;
    }

    if(u8InErr1 & 0x02)   //室内Eeprom错误
    {
      fgErrInEE = 1;
    }
    else
    {
      fgErrInEE = 0;
    }
    //Byte8:室内机保护状态2
    u8InErr2 = InOutCommBuf[8];
    if(u8InErr2 & 0x01)   //T3故障
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x02)   //T2或T2B故障
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x04)   //T1故障
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x10)   //室内外通信故障
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x20)   //水满保护
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x40)   //蒸发器高温保护
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x80)   //蒸发器低温保护
    {

    }
    else
    {

    }
    unInitFlgs.bits.TempAD = 1;//40字节协议与16字节协议温度值兼容
    u8InSetTemp = InOutCommBuf[9];
    //= InOutCommBuf[10];     //室内拨码状态
    u8T1TempAd = InOutCommBuf[11];

    if(C_ModeHeat == u8InSetMode)
    {
      //制热T2
      u8T2TempAd = InOutCommBuf[12];
    }
    else
    {
      //制冷T2B
      u8T2bTempAd = InOutCommBuf[12];
    }

    data_tmp = InOutCommBuf[13];
    if(data_tmp & 0x01)   //快检
    {
      unInSta.bits.FastTest = 1;
    }

  }
  else
  {
    //01命令
    data_tmp = InOutCommBuf[5];
    if(data_tmp & 0x04)   //PFC控制使能
    {
      u8ZGPFCEnFlg = 1;   //1:让驱动自行判断
    }
    else
    {
      u8ZGPFCEnFlg = 0;   //0:当目标频率小于参数表关PFC频率时关PFC
    }

    //Byte6:室外机状态控制
    data_tmp = InOutCommBuf[6];
    unInitFlgs.bits.TempAD = 1;     //40字节协议与16字节协议温度值兼容
    if(C_ModeHeat == u8InSetMode)   //T2和T2B互补传输
    {
      //制热T2B
      u8T2bTempAd = InOutCommBuf[12];
    }
    else
    {
      //制冷T2
      u8T2TempAd = InOutCommBuf[12];
    }
  }
  /*
  if (u8InTrgFreq)
  { // Cancel pre-heater
    u8TrgHeatDuty = 0;
  }
  */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommSend(void)
{
  ///INCOMM_TXD_PORT_OUT; #define INCOMM_TXD_PORT_OUT     (PORTB.PDR.BIT.B5 = 1)
  if (u8OutCommTxdTim == 0)
  {
    if (unOutCommTxdFlgs.bits.TDelay)
    {
      unOutCommTxdFlgs.bits.TDelay = 0;
      switch(u8SendOrder1)
      {
        case U8_COMM_CODE:
          {
            OutComSend();
            F_Uart5_SendEnabe();
          }
          break;
        case U8_COMM40_HEAD_OUT2IN:
          {
            OutComSend40();
            F_Uart5_SendEnabe();
          }
          break;
        default:
          {
            u8InOutCommBufCnt = 0;
            InOutCommInit();
          }
          break;
      }
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComSend(void)
{
  unsigned char i = 0;
  
  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    InOutCommBuf[i] = 0;
  }
  
  //ZG
  InOutCommBuf[0] = 0xAA; //Flow begin.
  InOutCommBuf[1] = 0;  //outdoor address

  if(u8InInst == 0)
  {
    //00命令,回复主机第一帧数据
    InOutCommBuf[2] = 0;
    InOutCommBuf[3] = u8PowerTypeNew;   //根据室内发送给室外的机型回传
    InOutCommBuf[4] = u8RealRunInterger;//压缩机实际频率 (311为u8RunCtrFreq)

    /*
    if (unInStatus.bits.FastTest)
    {
      u8CompStopTim = U8_COMP_STOP_TIM;
      u8FreqStopTim = U8_COMP_STOP_TIM;
    }
    */
    InOutCommBuf[5] = 0;//当前室外机保护状态1
    InOutCommBuf[6] = 0;//当前室外机保护状态2


    if(unTempErrFlgs.bits.T3Err)
    {
      //冷凝器温度传感器故障T3
      InOutCommBuf[5] |= 0x01;
    }
    if(unTempErrFlgs.bits.T5Err)
    {
      //排气温度传感器故障T5
      InOutCommBuf[5] |= 0x02;
    }
    if(unSysErrFlgs.bits.DrvCommErr)
    {
      //与IPM模块通信故障
      InOutCommBuf[5] |= 0x04;
    }
    if(unCompErrFlgs1.bits.FoErr)
    {
      //模块保护
      InOutCommBuf[5] |= 0x10;
    }
    if((unCompErrFlgs1.bits.LoVoltErr) || (unCompErrFlgs1.bits.PfcErr) || (unSysErrFlgs.bits.ACVoltLowErr) || (unCompErrFlgs2.bits.OvVoltErr))
    {
      //室外过压、欠压保护
      InOutCommBuf[5] |= 0x20;
    }
    if(unTempErrFlgs.bits.T4Err)
    {
      //室外温度传感器故障T4
      InOutCommBuf[5] |= 0x40;
    }
    if(unSysErrFlgs.bits.OverCurrErr)
    {
      //电流保护
      InOutCommBuf[5] |= 0x80;
    }


    if((unCompErrFlgs2.bits.CpOvCurrErr) || (unCompErrFlgs2.bits.MCEErr)
        || (unCompErrFlgs1.bits.PhsLossErr) || (unCompErrFlgs2.bits.VoltDrop) || (unCompErrFlgs1.bits.SpeedErr)
        || (unCompErrFlgs2.bits.ZeroSpdErr))
    {
      //压缩机驱动保护
      InOutCommBuf[6] |= 0x01;
    }
    if (fgTpProtFix)
    {
      //排气温度过高
      InOutCommBuf[6] |= 0x02;
    }
    if(unSysErrFlgs2.bits.ProtHP)
    {
      //高压保护
      InOutCommBuf[6] |= 0x10;
    }
    if(unSysErrFlgs2.bits.ProtLP)
    {
      //低压保护
      InOutCommBuf[6] |= 0x20;
    }
    if (unSysErrFlgs.bits.DCFanSpdErr)
    {
      //直流室外风机失速
      InOutCommBuf[6] |= 0x40;
    }
    if ((unSysErrFlgs.bits.I2cErr) || (unCompErrFlgs2.bits.E2Err))
    {
      //E方故障
      InOutCommBuf[6] |= 0x08;
    }

    //外机给内机发固定值,兼容内机主控程序
    //InOutCommBuf[7] = u8CondMidTempAd;
    InOutCommBuf[7] = TEMP_MACRO(25);//u8TempT3;//实际温度*2+60
    //InOutCommBuf[8] = u8OutdoorTempAd;
    InOutCommBuf[8] = TEMP_MACRO(25);//u8TempT4;//实际温度*2+60
    //InOutCommBuf[9] = u8DiscPipeTempAd;
    InOutCommBuf[9] = 50;//u8TempTP;//实际温度
    InOutCommBuf[10] = 0;//u8AcCurrentRMS_T10 / 5;  //u8AcCurrentRMS_T10为外机电流*10 //一次侧电流 传输值 = 实际值*2
    //InOutCommBuf[11] = u8HalfDcVoltRMS;   //dc volt
    InOutCommBuf[11] = 110;             //AC电压 通顺杰方案没有交流电压检测 //一次侧电压 传输值 = 实际值/2
    InOutCommBuf[12] = u8InSetMode;       //mode当前室外机运转模式
    if(unCompRunFlgs2.bits.RetOil)
    {
      //回油
      InOutCommBuf[13] |= 0x02;
    }
  }
  else if(1 == u8InInst)
  {
    InOutCommBuf[2] = 1;
    InOutCommBuf[3] = TabAD2IpmTemp[u8TempIpm] - 50;            //IPM模块实际温度*2-50
    InOutCommBuf[4] = 0;                //二次侧电流
    InOutCommBuf[5] = u8HalfDcVoltFil >> 1;       //直流母线电压 传输值 = 实际值/4
    InOutCommBuf[6] = (unsigned char)(u16CurrPulse >> 3);  //电子膨胀阀当前开度

    u16CurFanSpeed = ((unsigned short)Drv2Main[4] << 8) + (unsigned short)Drv2Main[3];

    InOutCommBuf[7] = u16CurFanSpeed / 10;       //直流风机当前转速
    InOutCommBuf[8] = 0;

    if(u8PowerOnFlg == 1)
    {
      InOutCommBuf[8] |= 0x01; //室外主继电器
    }
    if(unFlgs.bits.FWVCtr)
    {
      InOutCommBuf[8] |= 0x02; //四通阀
    }
    else
    {
      InOutCommBuf[8] &= 0xFD;
    }
    if(u8TrgFan == FanH)
    {
      InOutCommBuf[8] |= 0x04; //高风
    }
    else if(u8TrgFan == FanL)
    {
      InOutCommBuf[8] |= 0x10; //低风
    }
    else
    {
      InOutCommBuf[8] &= 0xE7;
    }

    InOutCommBuf[9] = 0;

    if(unSysErrFlgs.bits.InOutCommErr)
    {
      //室内外机通信故障
      InOutCommBuf[9] |= 0x01; //室内外通信故障
    }
    if(unSysErrFlgs.bits.DCFanSpdErr)
    {
      InOutCommBuf[9] |= 0x02;  //直流风机保护
    }
    //if(fgIPMProt)
    //{
    //  InOutCommBuf[9]|= 0x04; //IPM模块高温保护
    //}
    //if()
    //{//IPM模块故障且不可恢复
    //  InOutCommBuf[9] |= 0x08;
    //}
    if(fgCompErrFix)
    {
      InOutCommBuf[9] |= 0x10;  //驱动故障且不可恢复
    }

    //if()
    //{//正在除霜中
    //  InOutCommBuf[10] |= 0x01;
    //}

    //if()
    //{//正在复位中
    //  InOutCommBuf[10] |= 0x02;
    //}

  }
  InOutCommBuf[14] = CommChkSum(&InOutCommBuf[1], 13);
  InOutCommBuf[15] = 0x55;  // EOF.

  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 16;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComSend40(void)
{
  unsigned char i = 0;
  ByteTypeDef undata_tmp;

  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    InOutCommBuf[i] = 0;
  }
  
  InOutCommBuf[0] = U8_COMM40_HEAD_OUT2IN;  //Flow begin.
  InOutCommBuf[1] = U8_COMM40_ADDR_OUTDOOR; //outdoor address
  InOutCommBuf[2] = U8_COMM40_ADDR_INDOOR;
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    //正常模式
    InOutCommBuf[3] = U8_COMM40_COMMAND_NORMAL;   //室内外通信命令
    InOutCommBuf[4] = u8PowerTypeNew;             //根据室内发送给室外的机型回传 //外机机型/能力
    //InOutCommBuf[5] = ;                         //原始总能需
    //InOutCommBuf[6] = ;                         //T4修正能需
    //InOutCommBuf[7] = ;                         //T2/T2B修正能需

    InOutCommBuf[8] = u8TempT3;           //冷凝器中部管温T3,(温度+30)*2
    InOutCommBuf[9] = u8TempT4;           //室外环境温度T4,(温度+30)*2
    InOutCommBuf[10] = u8TempT5;          //压缩机排气温度T5,实际温度值,无偏置
    InOutCommBuf[11] = TabAD2IpmTemp[u8TempIpm];  //IPM温度T6,实际温度值,无偏置
    //InOutCommBuf[12] = ;          //预留温度T7
    //InOutCommBuf[13] = ;          //预留温度T8
    //InOutCommBuf[14] = ;          //T2/T2B平均温度,(温度+30)*2

    if (u8InSetMode <= C_ModeDry)
    {
      InOutCommBuf[15] = u8InSetMode;       //mode当前室外机运转模式
    }
    else if (u8InSetMode == C_ModeForce)
    {
      InOutCommBuf[15] = C_ModeCool;
      InOutCommBuf[15] |= 0X80; //0b10000000;
    }
    else if (unFlgs2.bits.Defrost)
    {
      InOutCommBuf[15] = C_ModeHeat;
    }
    else
    {
      InOutCommBuf[15] = u8InSetMode;
    }

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unFlgs2.bits.Defrost;  //正在除霜中
    undata_tmp.bits.no1 = unCompRunFlgs2.bits.RetOil;  //回油
    InOutCommBuf[16] = undata_tmp.u8Byte;       //室外机运行状态

    InOutCommBuf[17] = u8EnergyFreq;            //当前压缩机目标频率
    InOutCommBuf[18] = u8RealRunInterger;       //压缩机实际频率
    InOutCommBuf[19] = u16FanFBSpd / 10;        //当前外风机实际转速,实际转速/10
    InOutCommBuf[20] = (unsigned char)(u16CurrPulse >> 2);   //当前电子膨胀阀开度,实际开度/4

    //InOutCommBuf[21] = u8AcVoltageAd;         //一次侧电压,实际电压/2,
    InOutCommBuf[22] = u8AcCurrRMS_T10 / 5;     //一次侧电流,实际电流*2,分辨率0.5A
    InOutCommBuf[23] = u8HalfDcVoltFil >> 1;    //二次侧电压,实际电压/4,
    //InOutCommBuf[24] = ;    //二次侧电流,实际电流*4,分辨率0.25A

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unFreqLimFlgs.bits.CurrLim;    //一次侧/二次侧电流限频
    undata_tmp.bits.no1 = unFreqLimFlgs.bits.VoltLim;  //一次侧/二次侧电压限频
    undata_tmp.bits.no2 = unFreqLimFlgs.bits.T2TempHLim | unFreqLimFlgs.bits.T2TempLLim;   //蒸发器高/低温限频
    undata_tmp.bits.no3 = unFreqLimFlgs.bits.T3TempLim;//T3冷凝器高温限频
    //undata_tmp.bits.no4 = ;    //T4外环境温度限频
    undata_tmp.bits.no5 = unFreqLimFlgs.bits.T5TempLim;//T5压缩机排气温度限频
    undata_tmp.bits.no6 = unFreqLimFlgs.bits.IpmTempLim;    //T6IPM模块温度限频
    //undata_tmp.bits.no7 = ;//高压压力限频
    InOutCommBuf[25] = undata_tmp.u8Byte; //室外机限频状态1

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;    //内风机低风限频
    //undata_tmp.bits.no1 = ;    //内风机中风限频
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[26] = undata_tmp.u8Byte; //室外机限频状态2

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unTempErrFlgs.bits.T3Err;    //T3冷凝器温度传感器故障
    undata_tmp.bits.no1 = unTempErrFlgs.bits.T4Err;    //T4外环境传感器故障
    undata_tmp.bits.no2 = unTempErrFlgs.bits.T5Err;    //T5压缩机排气传感器故障
    undata_tmp.bits.no3 = unTempErrFlgs.bits.TipmErr;  //T6IPM模块温度传感器故障
    undata_tmp.bits.no4 = ((unSysErrFlgs.bits.I2cErr) | (unCompErrFlgs2.bits.E2Err));  //室外EEPROM故障
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[27] = undata_tmp.u8Byte; //室外机故障状态1

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unSysErrFlgs.bits.OverCurrErr;   //一次侧/二次侧电流过流保护
    undata_tmp.bits.no1 = ( (unSysErrFlgs.bits.ACVoltLowErr) | (unCompErrFlgs2.bits.OvVoltErr) );    //一次侧/二次侧电压过压欠/压限频
    if((fgProtCoolT2) | (fgProtHeatT2))
    {
      undata_tmp.bits.no2 = 1; //蒸发器高/低温保护
    }
    else
    {
      undata_tmp.bits.no2 = 0; //蒸发器高/低温保护
    }
    undata_tmp.bits.no3 = fgProtCoolT3;  //T3冷凝器高温保护
    undata_tmp.bits.no4 = fgProtT5;      //T5压缩机排气温度过高保护
    undata_tmp.bits.no5 = fgIPMProt;     //T6IPM模块温度过高保护
    undata_tmp.bits.no6 = unSysErrFlgs2.bits.ProtHP;  //高压压力保护
    undata_tmp.bits.no7 = unSysErrFlgs2.bits.ProtLP;  //低压压力保护
    InOutCommBuf[28] = undata_tmp.u8Byte; //室外机故障状态2

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unSysErrFlgs.bits.DCFanSpdErr;   //直流风机失速
    //undata_tmp.bits.no1 = ;    //防台风保护
    undata_tmp.bits.no2 = unSysErrFlgs.bits.InOutCommErr; //室外通讯故障
    undata_tmp.bits.no3 = unSysErrFlgs.bits.DrvCommErr;  //主控与DSP通讯故障
    ///undata_tmp.bits.no4 = unCompErrFlgs1.bits.FoErr; //IPM模块保护
    undata_tmp.bits.no4 = (unCompErrFlgs1.u8Byte||unCompErrFlgs2.u8Byte)?1:0; /// Leo
    //undata_tmp.bits.no5 = ;  //相序保护
    //undata_tmp.bits.no6 = ;  //模式冲突
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[29] = undata_tmp.u8Byte; //室外机故障状态3

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;
    //undata_tmp.bits.no1 = ;
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[30] = undata_tmp.u8Byte; //室外机故障状态4

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;
    //undata_tmp.bits.no1 = ;
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[30] = undata_tmp.u8Byte; //测试模式控制状态字

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no7 = unDgCommCtr.bits.DgInFan;
    InOutCommBuf[31] = undata_tmp.u8Byte; //锁频测试内风机转速

    undata_tmp.u8Byte = 0x00;
    if (u8TrgFan == FanL)
    {
      undata_tmp.bits.no0 = 1;   //外低风
      undata_tmp.bits.no1 = 0;   //外高风
    }
    else if (u8TrgFan == FanH)
    {
      undata_tmp.bits.no0 = 0;   //外低风
      undata_tmp.bits.no1 = 1;   //外高风
    }
    else
    {
      undata_tmp.bits.no0 = 0;   //外低风
      undata_tmp.bits.no1 = 0;   //外高风
    }
    //undata_tmp.bits.no0 = ;    //外低风
    //undata_tmp.bits.no1 = ;    //外高风
    undata_tmp.bits.no2 = unFlgs.bits.FWVCtr; //四通阀
    //undata_tmp.bits.no3 = ;    //底盘加热
    undata_tmp.bits.no4 = unFlgs.bits.PTCCtr; //曲轴加热
    //undata_tmp.bits.no5 = ;    //sv1电磁阀
    //undata_tmp.bits.no6 = ;    //sv2电磁阀
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[32] = u16DgInFanSpd / 10;
    InOutCommBuf[33] = undata_tmp.u8Byte; //外机外设输出状态,用于商检
  }
  InOutCommBuf[39] = CommChkSum(&InOutCommBuf[0], 39);
  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 40;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SendDelay(void)
{
  u8InOutCommErrTim = 0;
  unSysErrFlgs.bits.InOutCommErr = 0;
  unOutCommRxdFlgs.u8Byte = 0;
  unOutCommTxdFlgs.u8Byte = 0;
  u8InOutCommTxdTim = 5;            //Delay 50ms to send.
  unOutCommTxdFlgs.bits.TDelay = 1;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void F_Uart5_SendEnabe(void)
{ 
#if 0  ///原来有
  INCOMM_TXD_PORT_MODE;
  SCI5.SCR.BIT.TIE = 1;
  SCI5.SCR.BIT.TE  = 1;     // enable tx
  SCI5.SCR.BIT.TEIE = 1;
  SCI5.SCR.BIT.RE = 0;      //disable recieving
#endif
}

//============================== Leo =================================

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommDec(unsigned char *pRxBuf)
{
  ByteTypeDef unTmp;
  unTmp.u8Byte = 0;

  ///DgLedBlink();

  u8InInst = pRxBuf[3];                           //室内外通信命令

  /// 正常模式
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    unTmp.u8Byte = 0;
    // = pRxBuf[4];                               //内机机型/内核
    u8PowerTypeNew = pRxBuf[5];                   //当前室内机机型

    unTmp.u8Byte = pRxBuf[25];                    //测试模式状态控制字
    unInSta.bits.FastTest = unTmp.bits.no0;       //快检
    unInSta.bits.SelfChk = unTmp.bits.no1;
    unInSta.bits.RationRun = unTmp.bits.no7;      //进入能力测试//额定测试
    if (unInSta.bits.RationRun)                   //进入能力测试//额定测试
    {
      //unSeltFlgs3.u8Byte = undata_tmp.bits.no6; //PFC控制使能,与协议定义相反 //1:让驱动自行判断
      u8ZGPFCEnFlg = !unTmp.bits.no6;             //PFC控制使能,与协议定义相反 //1: 让驱动自行判断
      // = pRxBuf[26];                            //能力测试模式区域代码
      u8InRateTest = pRxBuf[27];                  //能力测试模式测试点代码

      if ( ( u8InRateTest != u8InRateTestPre ) || 
           ( u8InSetModeBufPre != u8InSetModeBuf ))
      {
        u8WaitPFCTim = 150;
        u8InRateTestPre = u8InRateTest;
        u8InSetModeBufPre = u8InSetModeBuf;
      }
    }
    else
    {
      ///unSeltFls3.u8Byte = 0;
      ///unSeltFls6.u8Byte = 0;
      
      u8ZGPFCEnFlg = 1;
      u8InRateTest = SeerTestNo;
      u8InRateTestPre = u8InRateTest;
      u8WaitPFCTim = 0;
    }
    
    unTmp.u8Byte = pRxBuf[6];             //内机设定模式
    u8InSetModeBuf = unTmp.u8Byte & 0x0F; //&0b00001111;
    ///u8InSetModeBufPre = u8InSetModeBuf;
    if ( u8WaitPFCTim > 0 )
    {
      u8InSetMode = C_ModeOff;
    }
    else
    {
      u8InSetMode = u8InSetModeBuf;
    }
     
    // = unTmp.bits.no4;           //内机设定模式,静音
    // = unTmp.bits.no5;           //内机设定模式,睡眠
    // = unTmp.bits.no6;           //内机设定模式,经济
    // = unTmp.bits.no7;           //内机设定模式,强力

    unTmp.u8Byte = pRxBuf[7];//内机控制命令
    // = unTmp.bits.no0;           //强制收冷媒
    // = unTmp.bits.no1;           //强制化霜
    // = unTmp.bits.no2;           //快速制冷
    // = unTmp.bits.no3;           //快速制热
    // = unTmp.bits.no3;           //压机预热
    // = pRxBuf[8];                //预留
    unInitFlgs.bits.TempAD = 0;
    u8InSetTemp = pRxBuf[9] >> 1;  //用户设定温度,设定温度*2
    u8TempT1 = pRxBuf[10];         //室内环境温度值T1,(温度+30)*2,分辨率0.5℃,偏置-30℃
    u8TempT2 = pRxBuf[11];         //室内蒸发器中部温度T2,(温度+30)*2,分辨率0.5℃,偏置-30℃
    u8TempT2b = pRxBuf[12];        //室内蒸发器出口温度T2B,(温度+30)*2,分辨率0.5℃,偏置-30℃
    // = pRxBuf[13];               //制冷模式温度补偿,(温度+30)*2,默认发60,分辨率0.5℃,偏置-30℃
    // = pRxBuf[14];               //制热模式温度补偿,(温度+30)*2,默认发60,分辨率0.5℃,偏置-30℃
    // = pRxBuf[15];               //室内湿度
    // = pRxBuf[16];               //其他温度

    u8InFanMode = pRxBuf[17];      //内风机运行档位
    // = pRxBuf[18];               //内风机实际转速,实际转速/10 rpm,交流风机发00H

    u8InErr1 = 0x00;
    u8InErr2 = 0x00;
    unTmp.u8Byte = pRxBuf[19];        //室内保护状态1
    fgProtInT2L = unTmp.bits.no0;     //蒸发器低温保护
    //fgProtInT2H = unTmp.bits.no1;   //蒸发器高温保护
    // = unTmp.bits.no2;              //室内防冷风

    unTmp.u8Byte = pRxBuf[20];        //室内保护状态2
    //
    unTmp.u8Byte = pRxBuf[21];        //室内故障停机状态1
    fgErrT1 = unTmp.bits.no0;         //T1传感器故障
    fgErrT2 = unTmp.bits.no1;         //T2传感器故障
    fgErrT2b = unTmp.bits.no2;        //T2B传感器故障
    //fgInCommErr = unTmp.bits.no3;   //室内外机通信故障
    //fgWaterFull = unTmp.bits.no4;   //水满保护
    fgErrInEE = unTmp.bits.no5;       //室内EE故障
    fgErrInFan = unTmp.bits.no6;      //内机风机失速故障
    //fgErrZero = unTmp.bits.no7;     //过零信号故障

    unTmp.u8Byte = pRxBuf[22];        //室内故障停机状态2

    // = pRxBuf[23];                  //预留
    // = pRxBuf[24];                  //内机功能状态

    if((pRxBuf[28] != 0) && (u8ZGInCtrTrgHzOut != pRxBuf[28])) //锁频测试压缩机目标频率
    {
      u8ZGInCtrTrgHzOut = pRxBuf[28];
      unDgCommCtr.bits.DgOutFreq = 0;
    }
    if ((pRxBuf[29] != 0) && (u16ZGInCtrFanSpd != ((unsigned short)pRxBuf[29] * 10)) ) //锁频测试外风机目标转速
    {
      u16ZGInCtrFanSpd = (unsigned short)pRxBuf[29] * 10;
      unDgCommCtr.bits.DgOutFan = 0;
    }
    if ((pRxBuf[30] >= 12) && (pRxBuf[30] <= 129))             //锁频测试电子膨胀阀开度
    {
      if (u16ZGInCtrTrgStepPulse != ((unsigned short)pRxBuf[30] << 2) )
      {
        u16ZGInCtrTrgStepPulse = (unsigned short)pRxBuf[30] << 2;
        unDgCommCtr.bits.DgOutPmv = 0;
      }
    }
    unTmp.u8Byte = pRxBuf[31];             //锁频测试外设控制状态字
    if (u8ZGInCtrFWValve != unTmp.bits.no0)//四通阀
    {
      u8ZGInCtrFWValve = unTmp.bits.no0;
      unDgCommCtr.bits.DgOutFw = 0;
    }
    // = unTmp.bits.no1;                   //电磁阀SV1
    // = unTmp.bits.no2;                   //电磁阀SV2
    // = unTmp.bits.no3;                   //底盘电加热
    // = unTmp.bits.no4;                   //曲轴加热带
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommEnc(unsigned char *pTxBuf)
{
  unsigned char i = 0;
  unsigned short u16Tmp = 0;
  
  ByteTypeDef unTmp;

  unTmp.u8Byte = 0;
  
  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    pTxBuf[i] = 0;
  }
  
  pTxBuf[0] = U8_COMM40_HEAD_OUT2IN;  //Flow begin.
  pTxBuf[1] = U8_COMM40_ADDR_OUTDOOR; //outdoor address
  pTxBuf[2] = U8_COMM40_ADDR_INDOOR;
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    //正常模式
    pTxBuf[3] = U8_COMM40_COMMAND_NORMAL;   //室内外通信命令
    pTxBuf[4] = u8PowerTypeNew;             //根据室内发送给室外的机型回传 //外机机型/能力
    //pTxBuf[5] = ;                         //原始总能需
    //pTxBuf[6] = ;                         //T4修正能需
    //pTxBuf[7] = ;                         //T2/T2B修正能需

    pTxBuf[8] = u8TempT3;                   //冷凝器中部管温T3,(温度+30)*2
    pTxBuf[9] = u8TempT4;                   //室外环境温度T4,(温度+30)*2
    pTxBuf[10] = u8TempT5;                  //压缩机排气温度T5,实际温度值,无偏置
    pTxBuf[11] = TabAD2IpmTemp[u8TempIpm];  //IPM温度T6,实际温度值,无偏置
    //pTxBuf[12] = ;                        //预留温度T7
    //pTxBuf[13] = ;                        //预留温度T8
    //pTxBuf[14] = ;                        //T2/T2B平均温度,(温度+30)*2

    if (u8InSetMode <= C_ModeDry)
    {
      pTxBuf[15] = u8InSetMode;             //mode当前室外机运转模式
    }
    else if (u8InSetMode == C_ModeForce)
    {
      pTxBuf[15] = C_ModeCool;
      pTxBuf[15] |= 0X80; //0b10000000;
    }
    else if (unFlgs2.bits.Defrost)
    {
      pTxBuf[15] = C_ModeHeat;
    }
    else
    {
      pTxBuf[15] = u8InSetMode;
    }

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unFlgs2.bits.Defrost;             //正在除霜中
    unTmp.bits.no1 = unCompRunFlgs2.bits.RetOil;       //回油
    pTxBuf[16] = unTmp.u8Byte;                         //室外机运行状态

    pTxBuf[17] = u8EnergyFreq;                         //当前压缩机目标频率
    pTxBuf[18] = u8RealRunInterger;                    //压缩机实际频率
    u16Tmp = u16FanFBSpd;
    u16Tmp /= 10;
    pTxBuf[19] = (unsigned char)u16Tmp;                //当前外风机实际转速,实际转速/10
    pTxBuf[20] = (unsigned char)(u16CurrPulse >> 2);   //当前电子膨胀阀开度,实际开度/4

    //pTxBuf[21] = u8AcVoltageAd;                      //一次侧电压,实际电压/2,
    pTxBuf[22] = u8AcCurrRMS_T10 / 5;                  //一次侧电流,实际电流*2,分辨率0.5A
    pTxBuf[23] = u8HalfDcVoltFil >> 1;                 //二次侧电压,实际电压/4,
    //pTxBuf[24] = ;    //二次侧电流,实际电流*4,分辨率0.25A

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unFreqLimFlgs.bits.CurrLim;       //一次侧/二次侧电流限频
    unTmp.bits.no1 = unFreqLimFlgs.bits.VoltLim;       //一次侧/二次侧电压限频
    unTmp.bits.no2 = unFreqLimFlgs.bits.T2TempHLim | unFreqLimFlgs.bits.T2TempLLim;   //蒸发器高/低温限频
    unTmp.bits.no3 = unFreqLimFlgs.bits.T3TempLim;     //T3冷凝器高温限频
    //unTmp.bits.no4 = ;                               //T4外环境温度限频
    unTmp.bits.no5 = unFreqLimFlgs.bits.T5TempLim;     //T5压缩机排气温度限频
    unTmp.bits.no6 = unFreqLimFlgs.bits.IpmTempLim;    //T6IPM模块温度限频
    //undata_tmp.bits.no7 = ;                          //高压压力限频
    pTxBuf[25] = unTmp.u8Byte;                         //室外机限频状态1

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;                               //内风机低风限频
    //unTmp.bits.no1 = ;                               //内风机中风限频
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[26] = unTmp.u8Byte;                         //室外机限频状态2

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unTempErrFlgs.bits.T3Err;         //T3冷凝器温度传感器故障
    unTmp.bits.no1 = unTempErrFlgs.bits.T4Err;         //T4外环境传感器故障
    unTmp.bits.no2 = unTempErrFlgs.bits.T5Err;         //T5压缩机排气传感器故障
    unTmp.bits.no3 = unTempErrFlgs.bits.TipmErr;       //T6IPM模块温度传感器故障
    unTmp.bits.no4 = ((unSysErrFlgs.bits.I2cErr) | (unCompErrFlgs2.bits.E2Err));  //室外EEPROM故障
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    pTxBuf[27] = unTmp.u8Byte; //室外机故障状态1
    u16DgTmp0 = unTmp.u8Byte;  ///
      
    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unSysErrFlgs.bits.OverCurrErr;    //一次侧/二次侧电流过流保护
    unTmp.bits.no1 = ( (unSysErrFlgs.bits.ACVoltLowErr) | (unCompErrFlgs2.bits.OvVoltErr) );    //一次侧/二次侧电压过压欠/压限频
    if((fgProtCoolT2) | (fgProtHeatT2))
    {
      unTmp.bits.no2 = 1;                             //蒸发器高/低温保护
    }
    else
    {
      unTmp.bits.no2 = 0;                             //蒸发器高/低温保护
    }
    unTmp.bits.no3 = fgProtCoolT3;                    //T3冷凝器高温保护
    unTmp.bits.no4 = fgProtT5;                        //T5压缩机排气温度过高保护
    unTmp.bits.no5 = fgIPMProt;                       //T6IPM模块温度过高保护
    unTmp.bits.no6 = unSysErrFlgs2.bits.ProtHP;       //高压压力保护
    unTmp.bits.no7 = unSysErrFlgs2.bits.ProtLP;       //低压压力保护
    pTxBuf[28] = unTmp.u8Byte;                        //室外机故障状态2
    u16DgTmp1 = unTmp.u8Byte;  ///
    
    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unSysErrFlgs.bits.DCFanSpdErr;   //直流风机失速
    //unTmp.bits.no1 = ;                              //防台风保护
    unTmp.bits.no2 = unSysErrFlgs.bits.InOutCommErr;  //室外通讯故障
    unTmp.bits.no3 = unSysErrFlgs.bits.DrvCommErr;    //主控与DSP通讯故障
    ///unTmp.bits.no4 = unCompErrFlgs1.bits.FoErr;    //IPM模块保护
    unTmp.bits.no4 = (unCompErrFlgs1.u8Byte||unCompErrFlgs2.u8Byte)?1:0; /// Leo
    //unTmp.bits.no5 = ;                              //相序保护
    //unTmp.bits.no6 = ;                              //模式冲突
    //unTmp.bits.no7 = ;
    pTxBuf[29] = unTmp.u8Byte;                        //室外机故障状态3
    u16DgTmp2 = unTmp.u8Byte;  ///

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;
    //unTmp.bits.no1 = ;
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[30] = unTmp.u8Byte; //室外机故障状态4

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;
    //unTmp.bits.no1 = ;
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[30] = unTmp.u8Byte; //测试模式控制状态字

    unTmp.u8Byte = 0x00;
    unTmp.bits.no7 = unDgCommCtr.bits.DgInFan;
    pTxBuf[31] = unTmp.u8Byte; //锁频测试内风机转速

    unTmp.u8Byte = 0x00;
    if (u8TrgFan == FanL)
    {
      unTmp.bits.no0 = 1;     //外低风
      unTmp.bits.no1 = 0;     //外高风
    }
    else if (u8TrgFan == FanH)
    {
      unTmp.bits.no0 = 0;     //外低风
      unTmp.bits.no1 = 1;     //外高风
    }
    else
    {
      unTmp.bits.no0 = 0;   //外低风
      unTmp.bits.no1 = 0;   //外高风
    }
    //unTmp.bits.no0 = ;    //外低风
    //unTmp.bits.no1 = ;    //外高风
    unTmp.bits.no2 = unFlgs.bits.FWVCtr; //四通阀
    //unTmp.bits.no3 = ;    //底盘加热
    unTmp.bits.no4 = unFlgs.bits.PTCCtr; //曲轴加热
    //unTmp.bits.no5 = ;    //sv1电磁阀
    //unTmp.bits.no6 = ;    //sv2电磁阀
    //unTmp.bits.no7 = ;
    pTxBuf[32] = u16DgInFanSpd / 10;
    pTxBuf[33] = unTmp.u8Byte; //外机外设输出状态,用于商检
  }
  pTxBuf[39] = CommChkSum(&pTxBuf[0], 39);
  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 40;
  SendDelay();
}



