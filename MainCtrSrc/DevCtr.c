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
  fb_bldc64_err();      //����CompCtrl()֮ǰ
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
  unSysErrFlgs.u8Byte = 0;    //�����ͨ��,Ԥ��,������ѹ����,Ԥ��,E������,ֱ�����ʧ��,Ԥ��,������
  unFreqProt.u8Byte = 0;      //Ԥ��,����T3����,TP����,T2���±���,T2�����ᱣ��,IPM���±���,Ԥ��,Ԥ��
  unSysErrFlgs2.u8Byte = 0;   //Ԥ��
  unCompErrFlgs1.u8Byte = 0;  //Fo����,ȱ�ౣ��,ʧ�ٱ���,�͵�ѹ����,PFC����,Ԥ��,Ԥ��,Ԥ��
  unCompErrFlgs2.u8Byte = 0;  //����ѹ����,���ٱ���,��ѹ����,MCE����,CP������,E������,��������,ֱ�����ʧ��
  unCompRunFlgs1.u8Byte = 0;  //Ԥ��,Ԥ��,Ԥ��,Ԥ��,Ԥ��,Ԥ��,ѹ��������״̬
  unInErr1.u8Byte = 0;        //�ڻ����ʧ�ٹ���,����EE����,Ԥ��,Ԥ��,Ԥ��,Ԥ��,Ԥ��
  unInErr2.u8Byte = 0;        //T1����������,T2����������,T2B����������,�ڻ�����,�������ͨ�Ź���,ˮ������,���������±���,�����������ᱣ��
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
    if(0 == unFlgs2.bits.DCVlowPRO)     //ֱ��ĸ�ߵ�ѹ����
    {
      if(u8DcVoltageAd < u8DcStop)      //ֱ��ĸ�ߵ�ѹ�����޶�ֵ170VDC
      {
        if(u8DcVoltageDly > 80)
        {
          if(u8PowerOnDelay > 21)
          {
            u8PowerOnDelay = 21;      //�Ͽ�����
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
    else                    //ĸ�ߵ�ѹ���͹��ϻָ�ֵ190VDC
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

  if(u8PowerOnDly >= TIME100MS_2S)    //�ϵ�2���ſ���Դ�̵���
  {
    POWER_ON;
    u8PowerOnFlg = 1;
  }
  else
  {
    POWER_OFF;
  }

  if(u8DgFWValve)                    //��ͨ�����Բ���
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
    //u8DbgFWValveΪ0,��ʾС���޵�����ͨ������
    if(unSysErrFlgs.bits.InOutCommErr)   //�ڻ�ͨѶ����,��ͨ������С��ĵ���Ƶ�ʾ�������״̬
    {
      if(u8DgTrgHz < u8MinFreq)
      {
        unFlgs.bits.FWVCtr = 0;
      }
    }
  }

  //���������Ա�־,����ȴ�����
  if(SeerTestNo == u8InRateTest)
  {
    if(unTempErrFlgs.bits.T4Err)
    {
      unFlgs.bits.PTCCtr = 1;
      u8HeatTime = 0;
    }
    else if(u8TempT4 > TEMP_MACRO(8)) //T4>8��,�رյ���ȴ�
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
    else if(u8TempT4 <= TEMP_MACRO(3))  //T4 < 3��
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
  //���̵���ȿ���
  if( ((C_ModeHeat == u8InSetMode)||unFlgs2.bits.Defrost)
     && (u8TempT4 < TEMP_MACRO(-3)) && (u8CompSta == COMP_RUN) )
  {
     unFlgs1.bits.ChassisHeat = 1;//���̵���ȿ�
  }
  if( (C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode)
     || (u8TempT4 > TEMP_MACRO(0)) || (u8CompSta == COMP_STOP) )
  {
     unFlgs1.bits.ChassisHeat = 0;//���̵���ȹ�
  }
  //ѹ��Ԥ��
  if( ((u8TempT4 <= TEMP_MACRO(1))||unTempErrFlgs.bits.T4Err)
     && ((u8TempT5 <= 1)||unTempErrFlgs.bits.T5Err)
     && (u8CompSta == COMP_STOP) )
  {
    u8TrgHeatDuty = 1;//Ԥ�ȿ�
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
    //CompressPTC_OFF;          //ѹ��������ȴ�
    return;
  }

  if(unSeltFlgs2.bits.ValveDir)
  {
    //��ͨ������
    if(unFlgs.bits.FWVCtr)     //��ͨ�����
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
    //��ͨ������
    if(unFlgs.bits.FWVCtr)     //��ͨ�����
    {
      FOUR_WAY_ON;
    }
    else
    {
      FOUR_WAY_OFF;
    }
  }

  if(unFlgs.bits.PTCCtr)         //��������
  {
    //CompressPTC_ON;
  }
  else
  {
    //CompressPTC_OFF;
  }

  if(unFlgs1.bits.ChassisHeat)   //���̵����
  {
    //ChassisPTC_ON;
  }
  else
  {
    //ChassisPTC_OFF;
  }

  //��ͨ������ʱ��
  if(unFlgs2.bits.FWVCtrlBak != unFlgs.bits.FWVCtr)
  {
    u8CompDly = 0;
    unFlgs2.bits.FWVCtrlBak = unFlgs.bits.FWVCtr;
  }

  //PTC����ʱ��
  if(unFlgs2.bits.PTCCtrlBak != unFlgs.bits.PTCCtr)
  {
    u8CompDly = 0;
    unFlgs2.bits.PTCCtrlBak = unFlgs.bits.PTCCtr;
  }
}


