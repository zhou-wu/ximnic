#include "UsrDef.h"
#include "DevCtr.h"
#include "InOutComm.h"
#include "TempSample.h"
#include "DataTransFunc2Drv.h"
#include "TimeTick.h"
#include "ParameterPro.h"
#include "SelfChk.h"
#include "Display.h"
#include "MonitorCommSub.h"
#include "MiscFunc.h"
#include "Test.h"
#include "FanCtr.h"    ///
#include "CompCtr.h"   ///
#include "Signal.h"    ///
#include "TimeTick.h"  ///
#include "DgComm.h"    ///
#include "PmvCtr.h"    ///
#include "SelfChk.h"

static void VarsInit(void);
static void DevInit(void);
static void DevCtr(void);
static void DevDrv(void);

unsigned char u8WaitPFCTim;
unsigned char u8InRateTestPre;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void MainCtrInit(void)
{
  unsigned char u8InitAdSampling = 0;

  VarsInit();
  EeParamPro();
  FanInit();
  DevInit();
  CompInit();
  SignalInit();
  InOutCommInit();
  TimTickInit();
  DgCommInit();
  PmvInit(); 
  //APfcInit();
  TempSampleInit();
  for(u8InitAdSampling = 0; u8InitAdSampling < 20; u8InitAdSampling++)
  {
    u8AdcTim = 4;
    TempSampleSrv();
  }
  //SimurtInit();
  
  //DgLedOn();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void MainCtrTaskPro(void)
{
  static unsigned char fgInit = 0;

  if ( 0 == fgInit )
  {
    MainCtrInit();
    fgInit = 0;
  }
  
  FanCtr();
  PmvCtr();
  TempSampleSrv();
  SignalSrv();
  DgCommSrv();
  OutCommRec();
  OutCommSend();
  fb_bldc64_err();      //放在CompCtrl()之前
  CompCtr();
  FanCtr();
  //PmvCtrl();
  DevCtr();
  DevDrv();
  SelfChkPro();
  DisplayPro();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void VarsInit(void)
{
  unSysErrFlgs.u8Byte = 0;    //内外机通信,预留,交流电压过低,预留,E方故障,直流风机失速,预留,过电流
  unFreqProt.u8Byte = 0;      //预留,制冷T3保护,TP保护,T2高温保护,T2防冻结保护,IPM高温保护,预留,预留
  unSysErrFlgs2.u8Byte = 0;   //预留
  unCompErrFlgs1.u8Byte = 0;  //Fo保护,缺相保护,失速保护,低电压保护,PFC保护,预留,预留,预留
  unCompErrFlgs2.u8Byte = 0;  //过电压保护,零速保护,电压跌落,MCE故障,CP过电流,E方故障,启动故障,直流风机失速
  unCompRunFlgs1.u8Byte = 0;  //预留,预留,预留,预留,预留,预留,压缩机错误状态
  unInErr1.u8Byte = 0;        //内机风机失速故障,室内EE故障,预留,预留,预留,预留,预留
  unInErr2.u8Byte = 0;        //T1传感器故障,T2传感器故障,T2B传感器故障,内机过零,室内外机通信故障,水满保护,蒸发器高温保护,蒸发器防冻结保护
  unTempErrFlgs.u8Byte = 0;
  unFixErrFlgs.u8Byte = 0;
  unFixSetFlgs.u8Byte = 0;
  unCompRunFlgs2.u8Byte = 0;
  unOutDustFlgs01.u8Byte = 0;
  unFreqLimFlgs.u8Byte = 0;
  unTempProtFlgs.u8Byte = 0;
  unOutSta.u8Byte = 0;
  unDevFlgs.u8Byte = 0;
  unSysErrFlgs.u8Byte = 0;
  unSysErrFlgs1.u8Byte = 0;
  unFlgs.u8Byte = 0;
  unFlgs1.u8Byte = 0;
  unFlgs2.u8Byte = 0;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DevInit(void)
{
  FOUR_WAY_PORT_OUT;
  FOUR_WAY_OFF;
  POWER_PORT_OUT;
//CompressPTC_PORT_OUT;
  ChassisPTC_PORT_OUT;
//CompressPTC_OFF;
  ChassisPTC_OFF;
  u8DgFWValve = 0;
  u8ZGInCtrFWValve = 0;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DevCtr(void)
{
  /*
    if(0 == unFlgs2.bits.DCVlowPRO)     //直流母线电压正常
    {
      if(u8DcVoltageAd < u8DcStop)      //直流母线电压低于限定值170VDC
      {
        if(u8DcVoltageDly > 80)
        {
          if(u8PowerOnDelay > 21)
          {
            u8PowerOnDelay = 21;      //断开负载
          }
        }
        if(u8DcVoltageDly > 180)      //1.8s
        {
          unFlgs2.bits.DCVlowPRO = 1;
          unFreqLimFlgs.bits.VoltLim = 0;
          u8VoltLimArea = 0;
        }
      }
      else
      {
        u8DcVoltageDly = 0;
      }
    }
    else                    //母线电压过低故障恢复值190VDC
    {
      u8DcVoltageDly = 0;
      if(u8DcVoltageAd >= u8DcRTS)
      {
        unFlgs2.bits.DCVlowPRO = 0;
      }
      else
      {
        u8PowerOnDelay = 0;
      }
    }
  */

  if(u8PowerOnDly >= TIME100MS_2S)    //上电2秒后才开电源继电器
  {
    POWER_ON;
    u8PowerOnFlg = 1;
  }
  else
  {
    POWER_OFF;
  }

  if(u8DgFWValve)                    //四通阀调试参数
  {
    if(1 == u8DgFWValve)
    {
      unFlgs.bits.FWVCtr = 1;
    }
    else if(2 == u8DgFWValve)
    {
      unFlgs.bits.FWVCtr = 0;
    }
    else
    {
      unFlgs.bits.FWVCtr = 0;
    }
  }
  else
  {
    //u8DbgFWValve为0,表示小板无调试四通阀请求
    if(unSysErrFlgs.bits.InOutCommErr)   //内机通讯错误,四通阀根据小板的调试频率决定开关状态
    {
      if(u8DgTrgHz < u8MinFreq)
      {
        unFlgs.bits.FWVCtr = 0;
      }
    }
  }

  //无能力测试标志,电加热带控制
  if(SeerTestNo == u8InRateTest)
  {
    if(unTempErrFlgs.bits.T4Err)
    {
      unFlgs.bits.PTCCtr = 1;
      u8HeatTime = 0;
    }
    else if(u8TempT4 > TEMP_MACRO(8)) //T4>8℃,关闭电加热带
    {
      if(unFlgs.bits.PTCCtr)
      {
        u8HeatTime = 0;
      }
      unFlgs.bits.PTCCtr = 0;
    }
    else if(C_ModeOff != u8InSetMode)
    {
      if(u8TempT4 <= TEMP_MACRO(3))
      {
        unFlgs.bits.PTCCtr = 1;
      }

      if(0 == unFlgs.bits.PTCCtr)
      {
        u8HeatTime = 0;
      }
      else
      {
        u8HeatTime = 8;     //8min
      }
    }
    else if(u8TempT4 < TEMP_MACRO(1))
    {
      if(0 == unFlgs.bits.PTCCtr)
      {
        u8HeatTime = 0;
      }
      unFlgs.bits.PTCCtr = 1;
    }
    else if(unFlgs.bits.PTCCtr)
    {
      if(u8HeatTime > 5)        //5min
      {
        u8HeatTime = 0;
        unFlgs.bits.PTCCtr = 0;
      }
    }
    else if(u8TempT4 <= TEMP_MACRO(3))  //T4 < 3℃
    {
      if(u8HeatTime > 60)       //60min
      {
        u8HeatTime = 0;
        unFlgs.bits.PTCCtr = 1;
      }
    }
  }
  else
  {
    unFlgs.bits.PTCCtr = 0;
    u8HeatTime = 0;
  }
  //底盘电加热控制
  if( ((C_ModeHeat == u8InSetMode)||unFlgs2.bits.Defrost)
     && (u8TempT4 < TEMP_MACRO(-3)) && (u8CompSta == COMP_RUN) )
  {
     unFlgs1.bits.ChassisHeat = 1;//底盘电加热开
  }
  if( (C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode)
     || (u8TempT4 > TEMP_MACRO(0)) || (u8CompSta == COMP_STOP) )
  {
     unFlgs1.bits.ChassisHeat = 0;//底盘电加热关
  }
  //压机预热
  if( ((u8TempT4 <= TEMP_MACRO(1))||unTempErrFlgs.bits.T4Err)
     && ((u8TempT5 <= 1)||unTempErrFlgs.bits.T5Err)
     && (u8CompSta == COMP_STOP) )
  {
    u8TrgHeatDuty = 1;//预热开
  }
  if( (u8TempT4 >= TEMP_MACRO(3)) || (u8TempT5 >= 3) || (u8TargetFreq != 0)
      || (unTempErrFlgs.bits.T4Err && unTempErrFlgs.bits.T5Err) )
  {
      if(u8TrgHeatDuty)
      {
          u8PreHeatStopTim = 10;//delay 1s then trun of comp
      }
      u8TrgHeatDuty = 0;
  }
  if (fgPCBAChk)
  {
    unFlgs.bits.FWVCtr = 1;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DevDrv(void)
{
  if (unInSta.bits.SelfChk )
  {
    return;
  }

  if(u8PowerOnDly < 30)
  {
    FOUR_WAY_OFF;
    //CompressPTC_OFF;          //压缩机电加热带
    return;
  }

  if(unSeltFlgs2.bits.ValveDir)
  {
    //四通阀反向
    if(unFlgs.bits.FWVCtr)     //四通阀输出
    {
      FOUR_WAY_OFF;
    }
    else
    {
      FOUR_WAY_ON;
    }
  }
  else
  {
    //四通阀正向
    if(unFlgs.bits.FWVCtr)     //四通阀输出
    {
      FOUR_WAY_ON;
    }
    else
    {
      FOUR_WAY_OFF;
    }
  }

  if(unFlgs.bits.PTCCtr)         //电加热输出
  {
    //CompressPTC_ON;
  }
  else
  {
    //CompressPTC_OFF;
  }

  if(unFlgs1.bits.ChassisHeat)   //底盘电加热
  {
    //ChassisPTC_ON;
  }
  else
  {
    //ChassisPTC_OFF;
  }

  //四通阀开启时序
  if(unFlgs2.bits.FWVCtrlBak != unFlgs.bits.FWVCtr)
  {
    u8CompDly = 0;
    unFlgs2.bits.FWVCtrlBak = unFlgs.bits.FWVCtr;
  }

  //PTC开启时序
  if(unFlgs2.bits.PTCCtrlBak != unFlgs.bits.PTCCtr)
  {
    u8CompDly = 0;
    unFlgs2.bits.PTCCtrlBak = unFlgs.bits.PTCCtr;
  }
}


