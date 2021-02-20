#include "UsrDef.h"
#include "TimeTick.h"
#include "ParameterPro.h"
#include "CompCtr.h"
#include "TempSample.h"
#include "Signal.h"

static unsigned char AreaUpPro(unsigned char u8AD, const unsigned char *TempTab);
static unsigned char AreaDownPro(unsigned char u8AD, const unsigned char *TempTab);
static void LimCurrent(unsigned char ProtAD, unsigned char ExitAD, unsigned char StopAD);
static void LimCurrHeat(void);
static void LimCurrCool(void);
static void WorkOff(void);
static void WorkFan(void);
static void LimitMaxFreCool(void);
static void WorkCool(void);
static void WorkHeat(void);
static void WorkDry(unsigned char T1_Ts);
static void CompStop(void);
static void CompRun(void);
static void ErrDisplay(void);
static void GetErrBit(unsigned char mdata, unsigned char num, unsigned char end);
static void SeerFreqDeal(void);
static void CalFreq(void);
static void LimitMaxFreHeat(void);
static void LoadControl(void);
static unsigned char GetRealFreq(unsigned char freq);

unsigned short u16CompAccel;
unsigned short u16CompDecel;
unsigned char u8SeerFreq;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void CompInit(void)
{
  u8CompSta = COMP_STOP;
  u16CompSpd = FST_RUN_DELAY;
  unCompErrFlgs1.u8Byte = 0;
  unCompRunFlgs2.u8Byte = 0;
  unCompRunFlgs2.bits.NoPlat = 0;

  u8LastTrgFreq = 0;
  u8RunCtrFreq = 0;
  u8RealRunFreq = 0;
  u8EnergyFreq = 0;
  u8LimitFreq = 0;
  u8DgTrgHz = 0;

  u8T5Prot5s = 0;
  u8T5Freq120s = 120;
  u8CurrProt2s = 200;
  u16CompStopTim = 240;    //�ϵ�û��ѹ��������,�Ƿ���Ҫ�����ͷ���λ���,���ܿ�ѹ����?
  u8DefrostExit = 50;
  u8ValveDlyTim = 0;

  u8InRateFreq = 0xFF;     //�ڻ���������
  u8InRateOutFan = 0xFF;
  u8InRatePMV = 0xFF;
  u8InRateTest = 0;

  u16ROFreqPlatTim = 0;
  u16ROLowFreqTim = u16RetOilTim1;

  u16CompAccel = 60;
  u16CompDecel = 60;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void CompCtr(void)
{
  ///if(unInSta.bits.FastTest)       //������������ӱ���
  /* ��������������, ���������ӱ��� */
  ///if ((unInSta.bits.FastTest) || (SeerTestNo != u8InRateTest) )
  if ((unInSta.bits.SelfChk)  || 
      (unInSta.bits.FastTest) || 
      (SeerTestNo != u8InRateTest) )
  {
    if(u16CompStopTim < 240)
    {
      u16CompStopTim = 241;
    }
  }

  //Ҫ��ͣѹ�����Ĺ���, ��λ������־
  if (unInErr1.u8Byte       || 
      unSysErrFlgs.u8Byte   || 
      unSysErrFlgs1.u8Byte  || 
      unSysErrFlgs2.u8Byte  || 
      unCompRunFlgs1.u8Byte)
  {
    fgProtErr = 1;
  }
  else
  {
    fgProtErr = 0;
  }

  if((unSysErrFlgs.u8Byte == 0)  && 
     (unSysErrFlgs2.u8Byte == 0) && 
     (fgCompErrSta == 0)         && 
     (unFreqProt.u8Byte == 0))
  {
    u8ErrHoldTim = 0;                     //���д��󼰱����ָ�ʱ��,���й��Ͻ��30s��ָ�����
  }

  ProtTP();                               //ͨ�ñ���:����

  if(C_ModeHeat == u8InSetMode)           //ͨ�ñ���:����,����ģʽ������������
  {
    if(SeerTestMax != u8InRateTest)       //��������ģʽ������Ƶ��Ч
    {
      LimCurrHeat();
    }
    else
    {
      unCompRunFlgs2.bits.NoPlat = 1;     //����ƽ̨
    }
  }
  else
  {
    LimCurrCool();
  }

  WorkOff();
  WorkFan();
  WorkCool();
  WorkHeat();
  LoadControl();                          //��ʱ���������ͨ�����ݻ��������

  u8EnergyFreq = GetRealFreq(u8FreqSet);  //���ó�����Ƶ��

  if ( (SeerTestNo != u8InRateTest ) && ( C_ModeOff != u8InSetMode ) )
  {
    if ( 0 != u8SeerFreq )
    {
      u8EnergyFreq = (u8SeerFreq >> 1);
    }
  }

  u8RunFreqGrade = GetFreqGrade(u8RunCtrFreq);

  //SEERFreqDeal();               //��������ģʽ,���ݹ�������Ƶ�ʿ���
  //FreqMinChange();              //��С����Ƶ�����ƣ�
  if(unFlgs2.bits.Defrost || (C_ModeDefrost == u8InSetMode) || (unCompRunFlgs2.bits.DefrostExit)) //��˪���ȼ�������Ƶ
  {
    //�����ֳ�˪�˳����Ϳ���
    if(unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unCompRunFlgs1.u8Byte)
    {
      u8TargetFreq = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
    else
    {
      u8EnergyFreq = GetRealFreq(u8FreqSet);        //���ó�����Ƶ��
      u8TargetFreq = u8EnergyFreq;
      u16ROFreqPlatTim = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
  }
  else if(u8DgTrgHz >= u8MinFreq)                   //С��������� 10Hz��Eeprom�涨����СƵ��
  {
    if(unSysErrFlgs.bits.I2cErr || fgCompErrSta)
    {
      u8TargetFreq = 0;
    }
    else
    {
      u8TargetFreq = u8DgTrgHz;
    }
  }
  /* 
  else if((SeerTestNo != u8InRateTest))
  {
    if(0 != u8EnergyFreq)
    {
      SEERFreqDeal();                               //��������ģʽ,���ݹ�������Ƶ�ʿ��� 
    }
    else
    {
      u8TargetFreq = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
  }
  */
  else if(0 != u8EnergyFreq)                        //�����ܼ������������Ƶ��
  {
    if(unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unCompRunFlgs1.u8Byte)
    {
      u8TargetFreq = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
    else
    {
      u8TargetFreq = u8EnergyFreq;
      
      //if(SeerTestNo != u8InRateTest)             //�ڻ��������Բ��������
      //{
      //  u16ROLowFreqTim = u16RetOilTim1;
      //  unCompRunFlgs2.bits.RetOil = 0;
      //}
      //���»��ʹ�DRģʽ����,������
      //if(unCompRunFlgs2.bits.RetOil)
      //{
      //  u8EnergyFreq = 0;                        //����,��ѹ����ֹͣ,����3����������
      //  u16ROLowFreqTim = u16RetOilTim1;
      //  unCompRunFlgs2.bits.RetOil = 0;
      //}

      if(unSeltFlgs1.bits.RetOilSelt)              // �������
      {
        if((u8RunCtrFreq >= u8MinFreq) && (u8RunCtrFreq < u8RetOilFreq1))
        {
          if(0 == u16ROLowFreqTim)
          {
            unCompRunFlgs2.bits.RetOil = 1;
            u16ROFreqPlatTim = u16RetOilTim2;
          }
        }
        else
        {
          if(0 == unCompRunFlgs2.bits.RetOil)
          {
            u16ROLowFreqTim = u16RetOilTim1;
          }
        }

        if(unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unCompRunFlgs1.u8Byte)
        {
          u8TargetFreq = 0;
          u16ROLowFreqTim = u16RetOilTim1;
          unCompRunFlgs2.bits.RetOil = 0;
        }
        else
        {
          if(unCompRunFlgs2.bits.RetOil)
          {
            if(u16ROFreqPlatTim)
            {
              u8TargetFreq = u8RetOilFreq2;
            }
            else
            {
              u8TargetFreq = u8EnergyFreq;
              u16ROLowFreqTim = u16RetOilTim1;
              unCompRunFlgs2.bits.RetOil = 0;
            }

            if((C_ModeDefrost == u8InSetMode) || (unFreqLimFlgs.u8Byte) || unFlgs2.bits.Defrost)
            {
              //�����ֳ�˪���������¶���Ƶ���������¶���Ƶ��������Ƶ��������Ƶ����ѹ��Ƶ�˳����Ϳ���
              u8TargetFreq = u8EnergyFreq;
              u16ROFreqPlatTim = 0;
              u16ROLowFreqTim = u16RetOilTim1;
              unCompRunFlgs2.bits.RetOil = 0;
            }
          }
          else
          {
            u8TargetFreq = u8EnergyFreq;
          }
        }
      }
    }
  }
  else                              //����debugҲ���������Ƶ��
  {
    u8TargetFreq = 0;
    u16ROLowFreqTim = u16RetOilTim1;
    unCompRunFlgs2.bits.RetOil = 0;
  }

  if(u8PowerOnDly < 33)             //����Դ�̵�����ʱ
  {
    u8TargetFreq = 0;
  }

  if((COMP_STOP == u8CompSta)       &&
     (SeerTestNo == u8InRateTest)   &&
     (1 == unSeltFlgs1.bits.PMVSelt))
  {
    if(0 == u16TrgPulse)           //Ŀ�꿪��Ϊ0,���ܿ�ѹ��
    {
      u8TargetFreq = 0;
    }
  }

  //OutTrgFreqDeal()
  if(0 != u8TargetFreq)
  {
    if((COMP_STOP == u8CompSta) && (u8CompDly <= 120))
    {
      return;
    }

    u8LimitFreq = u8TargetFreq;         //����Ŀ��Ƶ����Ƶ
    if(SeerTestNo == u8InRateTest)      //���������޵�ѹ��Ƶ
    {
      if(1 == u8VoltLimArea)            //��ѹ��Ƶ,���������������
      {
        if(u8LimitFreq > u8VoltLimFreq1)
        {
          u8LimitFreq = u8VoltLimFreq1;
        }
      }
      else if(2 == u8VoltLimArea)
      {
        if(u8LimitFreq > u8VoltLimFreq2)
        {
          u8LimitFreq = u8VoltLimFreq2;
        }
      }
      else
      {
        ;
      }
    }

    //������ֵ�ѹ������������������������������Ƶ,���ܵڶ�Ӧƽ̨
    if(unFreqLimFlgs.u8Byte || unCompRunFlgs2.bits.NoPlat)
    {
      if(u8LimitFreq < u8PlatFreq0)
      {
        //Ŀ��Ƶ��С�ڵ�0��ƽ̨Ƶ��
        unFlgs1.bits.Plat0RunOk = 1;
      }

      if(u8LimitFreq < u8PlatFreq1)
      {
        //Ŀ��Ƶ��С�ڵ�1��ƽ̨Ƶ��
        unFlgs1.bits.Plat1RunOk = 1;
      }

      //������ֵ�ѹ������������������������������Ƶ,���ܵ�2��ƽ̨
      if(unFreqLimFlgs.u8Byte)
      {
        unFlgs1.bits.Plat2RunOk = 1;
      }

      if(unFlgs1.bits.PlatRun)
      {
        //����ƽ̨����,��ƽ̨����ʱ���ֵ�ѹ������������������������������Ƶ,����ƽ̨
        unFlgs1.bits.PlatRun = 0;

        u16CompSpd = 0;
        if(u8RunCtrFreq == u8PlatFreq0)
        {
          unFlgs1.bits.Plat0RunOk = 1;
        }
        if(u8RunCtrFreq == u8PlatFreq1)
        {
          unFlgs1.bits.Plat1RunOk = 1;
        }
        else if(u8RunCtrFreq == u8PlatFreq2)
        {
          unFlgs1.bits.Plat2RunOk = 1;
        }
        else
        {
          ;
        }
      }
    }

    if(0 == unFlgs1.bits.Plat0RunOk)
    {
      //Ҫ���ܵ�0��ƽ̨��Ŀ��Ƶ��С��ƽ̨Ƶ��,����ƽ̨Ƶ��
      if(u8LimitFreq < u8PlatFreq0)
      {
        u8LimitFreq = u8PlatFreq0;
      }
    }
    else if(0 == unFlgs1.bits.Plat1RunOk)
    {
      //Ҫ���ܵ�һ��ƽ̨��Ŀ��Ƶ��С��ƽ̨Ƶ��,����ƽ̨Ƶ��
      if(u8LimitFreq < u8PlatFreq1)
      {
        u8LimitFreq = u8PlatFreq1;
      }
    }
    else
    {
      ;
    }

    CompRun();          //��������Ƶ����,��ʽ����ѹ������
  }
  else
  {
    CompStop();
  }

  //unCompRunFlgs2.bits.NoPlat = 0;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DefrostCtr(void)
{
  unsigned short  u16ExitTime;

  if(C_ModeHeat != u8InSetMode)
  {
    u16InDefrostTim = 0;
    unFlgs2.bits.Defrost = 0;
    unCompRunFlgs2.bits.DefrostExit = 0;
    unDevFlgs.bits.Defrost7DW = 0;
    unDevFlgs.bits.Defrost7UP = 0;
    unCompRunFlgs2.bits.DefrostIn = 0;

    return;
  }

  if(unDevFlgs.bits.Defrost7DW)
  {
    u16ExitTime = 180 + 40;
  }
  else if(unDevFlgs.bits.Defrost7UP)
  {
    u16ExitTime = 120 + 40;
  }
  else
  {
    u16ExitTime = 600 + 40;                 //����40��,��ͨ���л�ʱ��
  }

  if(u8DefrostDly3Min < 0xFF)
  {
    u8DefrostDly3Min++;
  }

  //====��˪��������====
  if(unCompRunFlgs2.bits.DefrostExit)
  {
    //if(SEERTestNO != u8InRateTest)
    //{
    //  if(u8DefrostDly3Min >= 15)          //��������,15���,ֱ���˳���˪
    //  {
    //    u8DefrostDly3Min = 250;
    //  }
    //}

    if(u8DefrostDly3Min > (u8DefrostExitT + 10))
    {
      unCompRunFlgs2.bits.DefrostExit = 0;

      u16InDefrostTim = 0;
      u16CompStopTim = 240;
      u8DefrostExit = 0;
      if(u8StopFanDly >= 30)
      {
        u8StopFanDly = 28;
      }
    }

    unCompRunFlgs2.bits.NoPlat = 0;
    return;
  }
  else if(unFlgs2.bits.Defrost)       //��˪�������
  {
    u16InDefrostTim++;
    if(u8TempT3 <= u8T3DefrostExit2)
    {
      u8DefrostDly3Min = 0;
    }

    // ��˪�˳���������˪ʱ��ﵽ 10���ӻ��������T3�ﵽ���˳���˪
    if((u8TempT3 > u8T3DefrostExit1) || (u8DefrostDly3Min > 80) || (u16InDefrostTim >= u16ExitTime))
    {
      u8DefrostDly3Min = 0;
      unFlgs2.bits.Defrost = 0;
      unCompRunFlgs2.bits.DefrostExit = 1;
      fgDefrostStop = 1;              //��˪����ͣ��
    }

    unCompRunFlgs2.bits.NoPlat = 1;   //��˪,����ƽ̨
    return;
  }
  //if(u8DefrostExit < 40)            //��˪����,����ƽ̨
  //{
  //  if(SEERTestNO != u8InRateTest)  //��˪����,����ƽ̨
  //  {
  //    unFlgs1.bits.Plat0RunOk = 1;
  //    unFlgs1.bits.Plat1RunOk = 1;
  //    unFlgs1.bits.Plat2RunOk = 1;
  //  }
  //}

  unCompRunFlgs2.bits.DefrostIn = 0;

  //==================��˪���봦��================
  if(unTempErrFlgs.bits.T3Err)  //T3����,ǿ�ƻ�˪
  {
    if(u16CompRunTim > MINUTE_MACRO(40))
    {
      if(u8TempT4 < TEMP_MACRO(7))
      {
        unDevFlgs.bits.Defrost7DW = 1;

        goto HERE112;
      }
    }
    else
    {
      unDevFlgs.bits.Defrost7DW = 0;
    }

    if(u16CompRunTim > MINUTE_MACRO(90))
    {
      if(u8TempT4 >= TEMP_MACRO(7))
      {
        unDevFlgs.bits.Defrost7UP = 1;

        goto HERE112;
      }
    }
    else
    {
      unDevFlgs.bits.Defrost7UP = 0;
    }

    if(u16InDefrostTim > MINUTE_MACRO(10))
    {
      return;
    }
  }
  else
  {
    unDevFlgs.bits.Defrost7DW = 0;
    unDevFlgs.bits.Defrost7UP = 0;
  }

  //if((COMP_RUN == u8CompStatus)&&(u8TempT3 < TEMP_MACRO(0)))
  if(COMP_RUN == u8CompSta)
  {
    if(u16InDefrostTim < 0xFFFF)
    {
      u16InDefrostTim++;
    }
  }

  //�õ���СT3ֵ 
  ///if(u16CompRunTim < MINUTE_MACRO(10))     //ѹ�����Ƿ�ֹͣ��,�Ƿ�Ҫ���µ��������10���Ӻ�
  if(u16InDefrostTim < MINUTE_MACRO(10))
  {
    u8TempT3Min = 255;
  }

  if((u16InDefrostTim >= MINUTE_MACRO(10)) && (u16InDefrostTim <= MINUTE_MACRO(15)))
  {
    if(u8TempT3 < u8TempT3Min)
    {
      u8T3MinDly++;
      if(u8T3MinDly >= 2)     //������1S,�൱��2�β���ֵ
      {
        u8T3MinDly = 0;
        u8TempT3Min = u8TempT3;
      }
    }
    else
    {
      u8T3MinDly = 0;
    }
  }
  else
  {
    u8T3MinDly = 0;
  }

  if(COMP_STOP == u8CompSta)
  {
    if(u16InDefrostTim < MINUTE_MACRO(10))
    {
      u16InDefrostTim = 0;
    }
  }

  //===============����1=============
  if (u16InDefrostTim >= MINUTE_MACRO(29))
  {
    if(u8TempT3 < u8T3DefrostIn1)
    {
      if(u8TempT3 + u8T3DefrostSUB1 <= u8TempT3Min)
      {
        goto HERE112;
      }
    }
  }

  //===============����2==============
  if(u16InDefrostTim >= MINUTE_MACRO(35))
  {
    if(u8TempT3 < u8T3DefrostIn2)
    {
      if(u8TempT3 + u8T3DefrostSUB2 <= u8TempT3Min)
      {
        goto HERE112;
      }
    }
  }

  //===============����3===============
  if(u16InDefrostTim >= MINUTE_MACRO(29))
  {
    if(u8TempT3 < u8T3DefrostIn3)
    {
      if(u8DefrostDly3Min >= MINUTE_MACRO(3))
      {
        goto HERE112;
      }
    }
    else
    {
      u8DefrostDly3Min = 0;
    }
  }
  else
  {
    u8DefrostDly3Min = 0;
  }

  //===============����4===============
  if(u16InDefrostTim >= MINUTE_MACRO(120))
  {
    if(u8TempT3 < TEMP_MACRO(-15))
    {
HERE112:                //���뻯˪
      u16InDefrostTim = 0;
      unFlgs2.bits.Defrost = 1;
      unCompRunFlgs2.bits.DefrostExit = 0;
      u8DefrostDly3Min = 0;
      //u16CompStopTim = 240;
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SeerDefrostCtr(void)
{

}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned char AreaCompare(unsigned char u8AD, const unsigned char *TempTab, unsigned char Area)
{
  unsigned char up_temp = 0, down_temp = 0;         //����/�½��¶ȵ�

  up_temp = Area * 2;
  up_temp += 1;                   //Area *2 +1:�õ���ǰ��������Ϸ����¶ȵ��ַ
  down_temp = up_temp - 3;          //�õ���ǰ��������·����¶ȵ��ַ (0����,���������ж�,���Բ��ÿ��Ǹ���)

  up_temp = *(TempTab + up_temp);     //�õ���ǰ��������Ϸ����¶ȵ�
  down_temp = *(TempTab + down_temp);   //�õ���ǰ��������·����¶ȵ�

  if(0 == Area)               //0�����14�������⴦��,ֻ��һ������
  {
    if(u8AD >= up_temp)
    {
      Area = AreaUpPro(u8AD, TempTab);
    }
  }
  //else if(14 == Area)
  //{
  //  if(u8AD <= down_temp)
  //    {
  //    Area = AreaDownPro(u8AD,TempTab);
  //    }
  //}
  else
  {
    if(u8AD >= up_temp)
    {
      Area = AreaUpPro(u8AD, TempTab);
    }
    else if(u8AD < down_temp)
    {
      Area = AreaDownPro(u8AD, TempTab);
    }
  }

  return(Area);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned char AreaUpPro(unsigned char u8AD, const unsigned char *TempTab)
{
  unsigned char index = 0, end_data = 0;
  unsigned char Area = 0;

  for(index = 0; index < 29; index++) //���������Ч�ı���¶�ֵ,�ҵ�0xFF��Ϊ��Чֵ����,���28������
  {
    end_data = *TempTab;
    if(0xFF == end_data)
    {
      break;
    }
    TempTab++;
  }

  TempTab = TempTab - index;          //�����㷨
  end_data = index >> 1;

  for(index = 0; index < end_data; index++)
  {
    TempTab++;
    if(u8AD >= *(TempTab))
    {
      Area++;
    }
    TempTab++;
  }
  return(Area);
}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned char AreaDownPro(unsigned char u8AD, const unsigned char *TempTab)
{
  unsigned char index = 0, end_data = 0;
  unsigned char Area = 0;

  for(index = 0; index < 29; index++) //���������Ч�ı���¶�ֵ,�ҵ�0xFF��Ϊ��Чֵ����,���28������
  {
    end_data = *TempTab;
    if(0xFF == end_data)
    {
      break;
    }

    TempTab++;
  }

  Area = index >> 1;
  end_data = Area;
  TempTab--;

  for(index = 0; index < end_data; index++)
  {
    TempTab--;
    if(u8AD < * (TempTab))
    {
      Area--;
    }

    TempTab--;
  }

  return(Area);
}


/**
  * @brief  : ��ǰƵ�� * 0.9
  * @param  : ���뵱ǰ����Ƶ��
  * @retval : ���:��Ƶ���Ƶ������
  */
unsigned char LimitFreq(unsigned char freq)
{
  unsigned short ii;
  unsigned char i;

  if(u8RunFreqGrade <= 1) //��ǰ����Ƶ�ʵȼ�
  {
    return(0);    //��F1������,����Ƶ,���ѹ����
  }

  ii = freq;
  ii = ii * 9;
  ii = ii / 10;
  i = (unsigned char)ii;    //��������Ƶ�ʽ���90%���ȵ���Ƶ
  i = GetFreqGrade(i);  //i:������Ƶ���Ƶ�ʵȼ�

  if(i < 1)
  {
    i = 1;
  }

  if(i < u8FreqSet) //ҪС�ڵ�ǰƵ�ʵȼ�,������Ƶ,��ֹƵ�ʷ�������
  {
    return(i);
  }
  else
  {
    return(u8FreqSet);
  }
}


/**
  * @brief  : ��ǰ����Ƶ��ת����Ƶ�����еȼ�
  * @param  : ����ʵ��Ƶ��
  * @retval : ���Ƶ�ʵȼ�
  */
unsigned char GetFreqGrade(unsigned char freq)
{
  unsigned char index;
  unsigned char *pFreqData;

  pFreqData = &TabRunFreq[25];    //Eeprom��ȡ��Ƶ�ʱ�,������
  for(index = 25; index > 0; index--) //Eeprom��Ƶ�ʵȼ�ֻ��25
  {
    if(freq >= *pFreqData)
    {
      freq = *pFreqData;
      break;
    }

    pFreqData--;
  }

  pFreqData--;
  while(freq == *pFreqData)     //������г�����ͬ,������С�Ǹ�Ϊ׼
  {
    if(0 == index)
    {
      break;
    }

    index--;
    pFreqData--;
  }

  if(0 == index)
  {
    if(freq > 3)                    //��һֱ������һ��Ƶ��,����ڲ�������3Hz, ���յ�һ��Ƶ����
    {
      index = 1;
    }
  }

  return(index);
}

/**
  * @brief  : ���ȵ�����Ƶ
  * @param  : 
  * @retval : 
  */
unsigned char const TabCurrHeatT[] = {TEMP_MACRO(5), TEMP_MACRO(6), TEMP_MACRO(9), TEMP_MACRO(10), TEMP_MACRO(14), TEMP_MACRO(15), 255, 255};
unsigned char *const TabCurrHeat[] = {&u8InEECurrentHeat1I, &u8InEECurrentHeat2I, &u8InEECurrentHeat3I, &u8InEECurrentHeat4I};
void LimCurrHeat(void)
{
  unsigned char crrent_in;
  unsigned char crrent_exit;

  u8CurrArea = AreaCompare(u8TempT4, (unsigned char*)TabCurrHeatT, u8CurrArea);
  if(u8CurrArea > 3)
  {
    u8CurrArea = 3;
  }

  crrent_in =  *TabCurrHeat[u8CurrArea];
  crrent_exit = crrent_in - u8InEECurrentHeatExit;
  LimCurrent(crrent_in, crrent_exit, u8InEECurrentHeatStop);
}

/**
  * @brief  : ���������Ƶ
  * @param  : 
  * @retval : 
  */
unsigned char const TabCurrCoolT[] = {TEMP_MACRO(32), TEMP_MACRO(33), TEMP_MACRO(37), TEMP_MACRO(38), TEMP_MACRO(44), TEMP_MACRO(45), TEMP_MACRO(49), TEMP_MACRO(50), 255, 255};
unsigned char *const TabCurrCool[] = {&u8InEECurrentCool1I, &u8InEECurrentCool2I, &u8InEECurrentCool3I, &u8InEECurrentCool4I, &u8InEECurrentCool5I};
void LimCurrCool(void)
{
  unsigned char crrent_in;
  unsigned char crrent_exit;

  u8CurrArea = AreaCompare(u8TempT4, (unsigned char*)TabCurrCoolT, u8CurrArea);
  if(u8CurrArea > 4)
  {
    u8CurrArea = 4;
  }

  crrent_in =  *TabCurrCool[u8CurrArea];
  crrent_exit = crrent_in - u8InEECurrentCoolExit;
  LimCurrent(crrent_in, crrent_exit, u8InEECurrentCoolStop);
}

/**
  * @brief  : ��������
  * @param  : ����ֵ
  * @retval : 
  */
void LimCurrent(unsigned char ProtAD, unsigned char ExitAD, unsigned char StopAD)
{
  if(u8AcCurrRMS_T10 >= StopAD)
  {
    unSysErrFlgs.bits.OverCurrErr  = 1;
  }
  else if(u8AcCurrRMS_T10 >= ProtAD)
  {
    unFreqLimFlgs.bits.CurrLim = 1;

    if(u8CurrProt2s >= 20)
    {
      u8CurrProt2s = 0;

      u8FreqSet = LimitFreq(u8RunCtrFreq);
      if(0 == u8FreqSet)
      {
        fgCurrentStop = 1;
      }
    }
  }
  else if(u8AcCurrRMS_T10 < ExitAD)
  {
    u8CurrProt2s = 200;
    unFreqLimFlgs.bits.CurrLim = 0;

    if(u8ErrHoldTim >= ERR_RST_TIM)
    {
      unSysErrFlgs.bits.OverCurrErr = 0;
    }
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void WorkOff(void)
{
  if(C_ModeOff == u8InSetMode)
  {
    u8FreqSet = 0;                //��������
    fgModeChangeStop = 1;         //ģʽ�л�ͣ��
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void WorkFan(void)
{
  if(C_ModeFan == u8InSetMode)
  {
    u8FreqSet = 0;            //��������
    fgModeChangeStop = 1;     //ģʽ�л�ͣ��
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
unsigned char *const TAB_FreqCool[] = 
{
&u8StartFreCool1, 
&u8StartFreCool2, 
&u8StartFreCool3, 
&u8StartFreCool4, 
&u8StartFreCool5, 
&u8StartFreCool6, 
&u8StartFreCool7, 
&u8StartFreCool8, 
&u8StartFreCool9, 
&u8StartFreCool10};
unsigned char const TAB_T1_Ts_Cool[] = 
{
-3 + 3 + 1, 
-2 + 3 + 1, 
-2 + 3 + 1, 
0 + 3 + 1, 
0 + 3 + 1, 
1 + 3 + 1, 
1 + 3 + 1, 
2 + 3 + 1, 
2 + 3 + 1, 
3 + 3 + 1, 
3 + 3 + 1, 
4 + 3 + 1, 
4 + 3 + 1, 
5 + 3 + 1, 
5 + 3 + 1, 
6 + 3 + 1, 
6 + 3 + 1, 
7 + 3 + 1, 
255, 
255};
void WorkCool(void)
{
  unsigned char temp_diff = 0, temp_set_middle = 0;
  unsigned char i = 0;

  if((C_ModeCool != u8InSetMode) && (C_ModeDry != u8InSetMode) && (C_ModeForce != u8InSetMode))
  {
    fgProtT1 = 0;
    unFreqLimFlgs.bits.T2TempLLim = 0;
    unFreqLimFlgs.bits.T3TempLim = 0;
    unCompRunFlgs2.bits.FirstInCool = 0;
    if((u8ErrHoldTim >= ERR_RST_TIM) || (u16CompStopTim >= MINUTE_MACRO(4)))   //���Խ��T3���±���,�ڻ������̵����Ҷ��Ĺ���
    {
      fgProtCoolT2 = 0;
      fgProtCoolT3 = 0;
    }
    return;
  }

  if(0 == unCompRunFlgs2.bits.FirstInCool)
  {
    unCompRunFlgs2.bits.FirstInCool = 1;

    u8FreqSet = 0;          //ֹͣѹ����
    u8EnergyFreq = 0;
    fgModeChangeStop = 1;   //ģʽ�л�ͣ��

    u8T2Prot5s = 250;
    u8T2Freq60s = 250;
    u8T3Prot5s = 250;       //��������,���������ж�һ�� ����ѹ��������,�ٱ����ص�
    u8T3Freq60s = 250;

    u8T1_TsArea = 0;        //�״�,�������ж�
    u8MaxFreqArea = 0;      //�״�,�������ж�
    u8CurrArea = 0;

    unFreqLimFlgs.bits.CurrLim = 0;
    unCompRunFlgs2.bits.ErrT2Lim = 0;
    fgT1TsStop = 0;             //�ж����,��ȡ������ͣ����־
    u8T4MinFreqLim = 0;
    u8T2ErrCompRunTim = 3;
    return;
  }

  LimitMaxFreCool();            //���Ƶ������
  ProtT2Cool();
  ProtT3Cool();

  //temp_diff = u8TempT1 + 3;   //���������������, ����õ��²�����:+3��T1�������ʱ����������������
  temp_diff = u8TempT1 + 3 + 1;
  temp_set_middle = TEMP_MACRO(u8InSetTemp);

  if(temp_diff > temp_set_middle)
  {
    temp_diff = temp_diff - temp_set_middle;
  }
  else
  {
    temp_diff = 0;
  }

  if(C_ModeDry == u8InSetMode)
  {
    WorkDry(temp_diff);
    u8T1_TsArea = 0;
    u8MaxFreqArea = 0;          //�״�,�������ж�(��ʪת����ʱ,�����ж�)
  }
  else
  {
    fgProtT1 = 0;
    u8T1_TsArea = AreaCompare(temp_diff, (unsigned char*)TAB_T1_Ts_Cool, u8T1_TsArea);
    if(u8T1_TsArea > 9)
    {
      u8T1_TsArea = 9;
    }

    if(unFreqProt.u8Byte || (u16CompStopTim < MINUTE_MACRO(4)))
    {
      u8FreqSet = 0;
      u8T1_TsArea = 0;                              //ѹ���������ڼ�,ǿ������Ϊ0,����������
    }
    //else if(0xFF != u8InRateFreq)                 //�ڻ���������
    //{
    //  u8FreqSet = 20;
    //  u8MaxFreqLim = 20;
    //}
    //else if(SeerTestNo != u8InRateTest) 
    //{
    //  SEERFreqDeal();                             //��������ģʽ,���ݹ�������Ƶ�ʿ���
    //}
    else if ( unInSta.bits.SelfChk ) 
    {
      u8FreqSet = 1;
      u8MaxFreqLim = 9;
      unCompRunFlgs2.bits.NoPlat = 1;
      ///u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
      u16CompAccel = (unsigned short)(600 / 2 + 1);
      unFlgs1.bits.Plat0RunOk = 1;
      unFlgs1.bits.Plat1RunOk = 1;
      unFlgs1.bits.Plat2RunOk = 1;
    }
    else if (unInSta.bits.FastTest)                //�̼��̶ܹ���58Hz,ע��E�������ı������Ӱ��
    {
      ///u8FreqSet = 9;
      u8MaxFreqLim = 9;
      unCompRunFlgs2.bits.NoPlat = 1;
      u16CompAccel = (unsigned short)(600 / 2 + 1);
      unFlgs1.bits.Plat0RunOk = 1;
      unFlgs1.bits.Plat1RunOk = 1;
      unFlgs1.bits.Plat2RunOk = 1;

      for ( i = 0; i < 26; i++ )
      {
      	if ( TabRunFreq[i] > 58 )
        {
        	u8FreqSet = i;
        	break;
        }
        else
        {
        	u8FreqSet = i;
        }
      }

    }
    else if (COMP_STOP == u8CompSta)             //ѹ����ֹͣ->����
    {
      u8FreqAreaT3Min = 0;
      if((u8T1_TsArea < 2) || (unFreqLimFlgs.u8Byte & 0xFE)) //��Ƶ�ʹﲻ���¶�,����������, 20200513, Leo
      {
        u8FreqSet = 0;
        u8T1_TsArea = 0;
      }
      else
      {
        u8FreqSet = *TAB_FreqCool[u8T1_TsArea];
        u8T1_TsAreaPre = u8T1_TsArea;
      }
    }
    else if(SeerTestNo != u8InRateTest)
    {
      SeerFreqDeal();                         //��������ģʽ,���ݹ�������Ƶ�ʿ���
    }
    else if(C_ModeForce == u8InSetMode)       //ǿ��������F2,������ƵӰ��
    {
      u8FreqSet = 2;
    }
    else
    {
      CalFreq();                              //������������Ƶ��
    }
  }

  if(u8FreqSet > u8MaxFreqLim)                //������Ƶ�Գ�ʪҲ��Ч
  {
    u8FreqSet = u8MaxFreqLim;
    if(0 == u8MaxFreqLim)
    {
      fgT4LimStop = 1;
    }
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void SeerFreqDeal(void)
{
  //if (u8InRateTest)
  if(SeerTestNo != u8InRateTest)
  {
    if(C_ModeHeat == u8InSetMode )
    {
      //����
      pU8TabChkAddr = &EeSysPara.Elem.HEAT_TEST_FREQ_A;    //contine addr
      u8SeerFreq = *(pU8TabChkAddr + u8InRateTest - 1);
      //u8EnergyFreq = u8SeerFreq>>1;
      //u8TargetFreq = u8SeerFreq >> 1;
    }
    else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
    {
      //����
      pU8TabChkAddr = &EeSysPara.Elem.COOL_TEST_FREQ_A;    //contine addr
      u8SeerFreq = *(pU8TabChkAddr + u8InRateTest - 1);
      //u8EnergyFreq = u8SeerFreq>>1;
      //u8TargetFreq = u8SeerFreq >> 1;
      //u8InTrgfreqDot = (u8SeerFreq%2)*50;         //0.5Hz
    }
    else
    {
      //u8EnergyFreq = 0;
    }
  }
}

/**
  * @brief  : 
  * @param  : u8T1_TsArea, ��ǰ�������� A=9 B=8 C=7 D=6 E=5 F=4 G=3 H=2 I=1 J=0
  * @retval : 
  */
void CalFreq(void)
{
  unsigned char add_grade, trg_grade;
  unsigned char curr_freq;
  unsigned char *pEepromFreq;
  unsigned char tempset_middle;

  trg_grade = u8RunFreqGrade;                //u8RunFreqGrade:��ǰ����Ƶ�ʵȼ�
  pEepromFreq = &TabRunFreq[trg_grade];      //�Ե�ǰƵ�ʵ�Ϊ��׼,������һ�׶ε�Ƶ�ʵ�

  if(u8T1_TsArea > u8T1_TsAreaPre)           //����ǰһ���²�����
  {
    add_grade = 1;
    u8FreqAreaT3Min = 0;

INCFREQ:                                     //���ϼӵ�
    if(0 == (unFreqLimFlgs.u8Byte & 0xFE ))  //��֤����Ƶ������Ƶ
    {
      while(add_grade)
      {
        curr_freq = *pEepromFreq;
        while(curr_freq == *pEepromFreq)
        {
          if(trg_grade >= COMP_MAXFREQTAP)   //���25��
          {
            add_grade = 1;
            break;
          }

          trg_grade++;
          pEepromFreq++;
        }

        add_grade--;
        if(trg_grade > u8FreqSet)
        {
          u8FreqSet = trg_grade ;
        }
      }
    }
  }
  else if(u8T1_TsArea < u8T1_TsAreaPre)   //С��ǰһ���²�����
  {
    u8FreqAreaT3Min = 0;
    
DECFREQ:                                  //���¼���
    curr_freq = *pEepromFreq;
    while(curr_freq == *pEepromFreq)
    {
      if (trg_grade <= 1)
      {
        trg_grade = 1;
        break;
      }

      trg_grade--;
      pEepromFreq--;
    }

    if(trg_grade < u8FreqSet)
    {
      u8FreqSet = trg_grade;
    }
  }
  else                                   //����ǰһ���²�����
  {
    if(u8FreqAreaT3Min >= (3 * 60))      //ÿ3min����һ���²�����
    {
      u8FreqAreaT3Min = 0;

      if(u8T1_TsArea == 9)               //A ��3��
      {
        add_grade = 3;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea >= 5)          //b-e ��2��
      {
        add_grade = 2;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea >= 3)          // f-g ��1��
      {
        add_grade = 1;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea == 2)   //H ���ֲ���
      {
        u8F1RunT60Min = 0;
      }
      else if(u8T1_TsArea == 1)   //I �½�һ��
      {
        u8F1RunT60Min = 0;
        goto DECFREQ;
      }
      else if(u8T1_TsArea == 0)   //J ��F1����һ��Сʱ��������T1-TS����,��ѹ�����ر�
      {
        u8FreqSet = 1;
        //tempset_middle = TEMP_MACRO(u8InSetTemp); 
        
        if(u8F1RunT60Min >= 60)                   //1min++, ǿ��1Сʱͣ��
        {
          u8FreqSet = 0;
          fgT1TsStop = 1;
        }

        if(u8InSetMode == C_ModeHeat)
        {
          tempset_middle = TEMP_MACRO(u8InSetTemp + HEAT_DLT_TEMP);
          if(u8TempT1 > (tempset_middle + 2 + 2))  // 2*2, ����ͣ��
          {
            u8FreqSet = 0;
            fgT1TsStop = 1;
          }
        }
        else
        {
          tempset_middle = TEMP_MACRO(u8InSetTemp);
          if((u8TempT1 + 2 + 2) < tempset_middle)  // 2*2, ����ͣ��
          {
            u8FreqSet = 0;
            fgT1TsStop = 1;
          }
        }
      }
      else
      {
        u8F1RunT60Min = 0;
      }
    }
  }

  u8T1_TsAreaPre = u8T1_TsArea;               //�жϽ���,ˢ����������
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
unsigned char *const TabHeatStart[] = 
{
&u8StartFreHeat1, 
&u8StartFreHeat2,
&u8StartFreHeat3, 
&u8StartFreHeat4, 
&u8StartFreHeat5, 
&u8StartFreHeat6, 
&u8StartFreHeat7, 
&u8StartFreHeat8, 
&u8StartFreHeat9, 
&u8StartFreHeat10};
unsigned char const TabT1_Ts_Heat[] = 
{1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 255, 255};
void WorkHeat(void)
{
  unsigned char temp_diff = 0,temp_set_middle = 0,defrost_in_tim = 0;
  unsigned char i = 0;

  //if(C_ModeHeat != u8InSetMode)           //������ģʽ,��T2���±�����־���
  if((C_ModeHeat != u8InSetMode) && (C_ModeForceHeat != u8InSetMode)) 
  {
    if(u8ErrHoldTim >= ERR_RST_TIM)
    {
      fgProtHeatT2 = 0;
    }

    unFreqLimFlgs.bits.T2TempHLim = 0;
    unCompRunFlgs2.bits.FirstInHeat = 0;
    return;
  }

  if(0 == unCompRunFlgs2.bits.FirstInHeat)  //��һ�ν�������ģʽ,���ҵ�����Ƶ�ʵ�
  {
    unCompRunFlgs2.bits.FirstInHeat = 1;

    u8FreqSet = 0;                  //ֹͣѹ����
    u8EnergyFreq = 0;
    fgModeChangeStop = 1;           //ģʽ�л�ͣ��

    u8T2Prot5s = 250;
    u8T2Freq60s = 250;
    u8T2Lim180s = 0;
    u8T4MinFreqLim = 0;

    u8CurrArea = 0;
    u8MaxFreqArea = 0;              //�״�,�������ж�
    u8T1_TsArea = 0;
    u8T1_TsAreaH = 9;               //�״�����,�½����ж�

    fgT1TsStop = 0;                 //�ж����,��ȡ������ͣ����־
    unFreqLimFlgs.bits.CurrLim = 0;
    unCompRunFlgs2.bits.ErrT2Lim = 0;
    return;
  }

  if((SeerTestMax != u8InRateTest) && (C_ModeForceHeat != u8InSetMode))   //��������ģʽT4��Ƶ��Ч
  {
    LimitMaxFreHeat();                      //�������Ƶ������
  }
  
  ProtT2Heat();

  if(SeerTestNo != u8InRateTest)            //��˪����ʱ�䴦��: �ֲ���ģʽ������ģʽ
  {
    defrost_in_tim = u8DefrostInRateT;
  }
  else
  {
    defrost_in_tim = u8DefrostInT;
  }

  if(unCompRunFlgs2.bits.DefrostExit)       //��˪�˳�
  {
    if(SeerTestNo != u8InRateTest)          //��˪����ʱ�䴦��: �ֲ���ģʽ������ģʽ
    {
      u8FreqSet = u8FreqDefrostTest2;
    }
    else
    {
      u8FreqSet = 0;
    }

    //u8FreqSet = 0;                        //��˪�˳�ͣѹ����
    unFlgs.bits.OutFanCtr = 1;              //����������
    if(u8DefrostDly3Min > u8DefrostExitT)
    {
      unFlgs.bits.FWVCtr = 1;
    }

    return;
  }
  else if(unFlgs2.bits.Defrost)             //��˪����
  {
    fgProtHeatT2 = 0;
    unFreqLimFlgs.u8Byte = 0;

    if(u16InDefrostTim < (defrost_in_tim + 10))
    {
      if(SeerTestNo != u8InRateTest)        //��˪����ʱ�䴦��: �ֲ���ģʽ������ģʽ
      {
        u8FreqSet = u8FreqDefrostTest1;
      }
      else
      {
        u8FreqSet = 0;
      }

      //u8FreqSet = 0;                      //���뻯˪ǰͣѹ����
      if(u16InDefrostTim >= (defrost_in_tim + 8))
      {
        u16CompStopTim = 250;
      }
    }
    else if(u16CompStopTim < MINUTE_MACRO(4))
    {
      u8FreqSet = 0;
    }
    else if(unFreqProt.u8Byte)              //��˪���̳��ֹ��ϱ�����־
    {
      u8FreqSet = 0;
    }
    else if(SeerTestNo != u8InRateTest)
    {
      u8FreqSet = u8FreqDefrostTest;        //��˪Ƶ��ʹ�������������Ƶ��(��������)
    }
    else
    {
      u8FreqSet = u8FreDefrost; //u8T4MaxCoolFreq4; //��˪Ƶ��ʹ�������������Ƶ��  
    }

    if(u16InDefrostTim > (defrost_in_tim))  //����F3,����5��,��任��ͨ��������
    {
      unCompRunFlgs2.bits.DefrostIn = 1;
      unFlgs.bits.OutFanCtr = 0;
      unFlgs.bits.FWVCtr = 0;
    }

    return;                                 //��˪�ӹ��̲���Ҫ�ټ�����������, ֱ�ӷ���
  }

  temp_diff = (u8TempT1 - HEAT_DLT_TEMP * 2 + 4 * 2); //���������������, ����õ��²�����:+12��Ӳ�ԵĲ���6��
  temp_set_middle = TEMP_MACRO(u8InSetTemp);

  if(temp_diff > temp_set_middle)                     //���ע��u8InSetTemp�����ı����ʽ:60+x+x
  {
    temp_diff = temp_diff - temp_set_middle;
  }
  else
  {
    temp_diff = 0;
  }

  u8T1_TsAreaH = AreaCompare(temp_diff, (unsigned char*)TabT1_Ts_Heat, u8T1_TsAreaH);
  if(u8T1_TsAreaH > 9)
  {
    u8T1_TsAreaH = 9;
  }
  u8T1_TsArea = 9 - u8T1_TsAreaH;           //��������Ҫ������

  if(unFreqProt.u8Byte || (u16CompStopTim < MINUTE_MACRO(4)))
  {
    u8FreqSet = 0;
    u8T1_TsAreaH = 9;                       //ѹ���������ڼ�,ǿ������Ϊ0,����������
  }
  //else if(0xFF != u8InRateFreq)
  //{
  //  u8FreqSet = 20;
  //  u8MaxFreqLim = 20;
  //}
  else if(unInSta.bits.FastTest)            //�̼��̶ܹ���58Hz,ע��E�������ı������Ӱ��
  {
    ///u8FreqSet = 9;
    u8MaxFreqLim = 9;

    unCompRunFlgs2.bits.NoPlat = 1;
    u16CompAccel = (unsigned short)(600 / 2 + 1);
    unFlgs1.bits.Plat0RunOk = 1;
    unFlgs1.bits.Plat1RunOk = 1;
    unFlgs1.bits.Plat2RunOk = 1;

    for ( i = 0; i < 26; i++ )
    {
    	if ( TabRunFreq[i] > 58 )
      {
      	u8FreqSet = i - 1;
      	break;
      }
      else
      {
      	u8FreqSet = i;
      }
    }
  }
  else if(COMP_STOP == u8CompSta)
  {
    u8FreqAreaT3Min = 0;
    if((u8T1_TsArea < 1) || (unFreqLimFlgs.u8Byte & 0xFE ))   //��Ƶ�ʹﲻ���¶�,����������, 20200513, Leo
    {
      u8FreqSet = 0;
      u8T1_TsArea = 0;                                       //ѹ���������ڼ�,ǿ������Ϊ0,����������
      u8T1_TsAreaH = 9;
    }
    else
    {
      u8FreqSet = *TabHeatStart[u8T1_TsArea];
      u8T1_TsAreaPre = u8T1_TsArea;
    }
  }
  else if(SeerTestNo != u8InRateTest)
  {
    SeerFreqDeal();                         //��������ģʽ,���ݹ�������Ƶ�ʿ���
  }
  else if(C_ModeForceHeat == u8InSetMode)   //ǿ��������F4,������ƵӰ��
  {
    u8FreqSet = 4;
  }
  else
  {
    CalFreq();
  }

  if(u8FreqSet > u8MaxFreqLim)  //�������������ȼ������������Ƶ��
  {
    u8FreqSet = u8MaxFreqLim;
    if(0 == u8MaxFreqLim)
    {
      fgT4LimStop = 1;
    }
  }
}

/**
  * @brief  : �������Ƶ������
  * @param  : 
  * @retval : 
  */
unsigned char const TabMaxFreqHeatLim[] =
{
  TEMP_MACRO(-16), TEMP_MACRO(-15), TEMP_MACRO(-12), 
  TEMP_MACRO(-11), TEMP_MACRO(-8), TEMP_MACRO(-7), 
  TEMP_MACRO(-4), TEMP_MACRO(-3), TEMP_MACRO(0), 
  TEMP_MACRO(1),  TEMP_MACRO(5), TEMP_MACRO(6), 
  TEMP_MACRO(11), TEMP_MACRO(12), TEMP_MACRO(14), 
  TEMP_MACRO(15), TEMP_MACRO(16), TEMP_MACRO(17), 
  TEMP_MACRO(18), TEMP_MACRO(19),TEMP_MACRO(21), 
  TEMP_MACRO(22), TEMP_MACRO(24), TEMP_MACRO(25), 
  TEMP_MACRO(27), TEMP_MACRO(28), TEMP_MACRO(33), 
  TEMP_MACRO(34), 255, 255
};
unsigned char *const TabMaxFreHeat[] =
{
  &TabRunFreq[25], &TabRunFreq[24], &TabRunFreq[23], 
  &TabRunFreq[22], &TabRunFreq[21], &u8T4MaxHeatFreq9, 
  &u8T4MaxHeatFreq8, &u8T4MaxHeatFreq7,&u8T4MaxHeatFreq6, 
  &u8T4MaxHeatFreq5, &u8T4MaxHeatFreq4, &u8T4MaxHeatFreq3, 
  &u8T4MaxHeatFreq2, &u8T4MaxHeatFreq1
};
void LimitMaxFreHeat(void)
{
  u8MaxFreqArea = AreaCompare(u8TempT4, (unsigned char*)TabMaxFreqHeatLim, u8MaxFreqArea);
  if(u8MaxFreqArea >= 14)
  {
    u8MaxFreqArea = 14;
    u8MaxFreqLim = 0;
  }
  else
  {
    if(u8MaxFreqArea <= 4)
    {
      u8MaxFreqLim = 25 - u8MaxFreqArea;
    }
    else
    {
      u8MaxFreqLim = *TabMaxFreHeat[u8MaxFreqArea];
    }
  }

  if((C_InFanSH == u8InFanMode) || (C_InFanH == u8InFanMode) || (C_InFanTurbo == u8InFanMode) )
  {
    if(25 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F25LmtTemp)            //��ע��E������u8T1f25�����ݸ�ʽ
      {
        u8MaxFreqLim = 24;
      }
    }

    if(24 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F24LmtTemp)            //��ע��E������u8T1f24�����ݸ�ʽ
      {
        u8MaxFreqLim = 23;
      }
    }

    if(23 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F23LmtTemp)            //��ע��E������u8T1f23�����ݸ�ʽ
      {
        u8MaxFreqLim = 22;
      }
    }

    if(22 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F22LmtTemp)
      {
        u8MaxFreqLim = 21;
      }
    }

    if(21 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F21LmtTemp)
      {
        u8MaxFreqLim = u8T4MaxHeatFreq9;
      }
    }
  }
  else
  {
    if(u8MaxFreqLim > u8T4MaxHeatFreq9)
    {
      u8MaxFreqLim = u8T4MaxHeatFreq9;
    }

    if( (C_InFanMM == u8InFanMode) || (C_InFanM == u8InFanMode) )      //�з���Ƶ
    {
      if(u8MaxFreqLim > u8T4MidLimHeatFreq1)
      {
        u8MaxFreqLim = u8T4MidLimHeatFreq1;
      }
    }

    if( (C_InFanSL == u8InFanMode) || (C_InFanL == u8InFanMode) )     //�ͷ���Ƶ
    {
      if(u8MaxFreqLim > u8T4LowLimHeatFreq2)
      {
        u8MaxFreqLim = u8T4LowLimHeatFreq2;
      }
    }
  }
  //if(C_InFanSL == u8InFanMode)      //���� ��ƵF3 ,���ٲ��Ǹ߷�,�����Ѿ��������ض�����������
  //{
  //    unCompRunFlgs2.bits.NoPlat = 1;         //ȡ��ƽ̨

  //  if(u8MaxFreqLim > 3)
  //    {
  //    u8MaxFreqLim = 3;
  //    }

  //    if(unCompRunFlgs2.bits.RetOil)
  //    {
  //    unCompRunFlgs2.bits.RetOil = 0;       //ȡ������
  //    u16ROLowFreqTim = u16RetOilTim1;
  //    }
  //}

  if(fgErrT2)                           //T2����������
  {
    if (u8TempT1 > TEMP_MACRO(20))
    {
      unCompRunFlgs2.bits.ErrT2Lim = 1;
    }
    else if(u8TempT1 < TEMP_MACRO(18))
    {
      unCompRunFlgs2.bits.ErrT2Lim = 0;
    }

    if(unCompRunFlgs2.bits.ErrT2Lim)
    {
      if(u8MaxFreqLim > 4)
      {
        u8MaxFreqLim = 4;
      }
    }
    else
    {
      if(u8MaxFreqLim > 12)
      {
        u8MaxFreqLim = 12;
      }
    }
  }
  else
  {
    unCompRunFlgs2.bits.ErrT2Lim = 0;
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
//unsigned char const TabT1_Ts_Dry[]= {0+3, 1+3, 2+3, 3+3, 4+3, 5+3, 255, 255};       //��ʪ�²�����
unsigned char const TabT1_Ts_Dry[] = 
{
0 + 3 + 1, 
1 + 3 + 1, 
2 + 3 + 1, 
3 + 3 + 1, 
4 + 3 + 1, 
5 + 3 + 1, 
255, 255}; //��ʪ�²�����
unsigned char const TabFreqDry[] = {1, 3, 5, 7};
void WorkDry(unsigned char T1_Ts)
{
  u8T1_TsArea = AreaCompare(T1_Ts, (unsigned char*)TabT1_Ts_Dry, u8T1_TsArea);

  if(u8T1_TsArea > 3)
  {
    u8T1_TsArea = 3;
  }

  if(COMP_RUN == u8CompSta)
  {
    if(u8TempT1 < TEMP_MACRO(10))   //T1����������,С��10��C
    {
      fgProtT1 = 1;
    }
  }
  else
  {
    if(u8TempT1 >= TEMP_MACRO(12))  //T1�������ָ�,����12��C
    {
      fgProtT1 = 0;
    }
  }

  if(unFreqProt.u8Byte || (u16CompStopTim < MINUTE_MACRO(4)))
  {
    //����Ƶ����,ֹͣ�������
    u8FreqSet = 0;
    u8T1_TsArea = 0;               //ѹ���������ڼ�,ǿ������Ϊ0,����������
  }
  else
  {
    if(0 == (unFreqLimFlgs.u8Byte & 0xFE ))
    {
      u8FreqSet = TabFreqDry[u8T1_TsArea];
    }
  }

  //if(C_InFanSL == u8InFanMode)     //����ģʽ��Ƶ
  //{
  //  if(u8FreqSet > 3)
  //  {
  //    u8FreqSet = 3;
  //  }
  //}
}


/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
unsigned char const TabMaxFreCoolLim[] =
{
  TEMP_MACRO(20), TEMP_MACRO(22), 
  TEMP_MACRO(29), TEMP_MACRO(30), 
  TEMP_MACRO(32), TEMP_MACRO(33), 
  TEMP_MACRO(37), TEMP_MACRO(38), 
  TEMP_MACRO(44), TEMP_MACRO(45), 
  TEMP_MACRO(49), TEMP_MACRO(50), 
  TEMP_MACRO(54), TEMP_MACRO(55), 
  255, 255
};
unsigned char *const TabMaxFreCool[7] =
{
  &u8T4MaxCoolFreq7, 
  &u8T4MaxCoolFreq6, 
  &u8T4MaxCoolFreq5, 
  &u8T4MaxCoolFreq4, 
  &u8T4MaxCoolFreq3, 
  &u8T4MaxCoolFreq2, 
  &u8T4MaxCoolFreq1
};
void LimitMaxFreCool(void)
{
  u8MaxFreqArea = AreaCompare(u8TempT4, (unsigned char*)TabMaxFreCoolLim, u8MaxFreqArea);
  if(u8MaxFreqArea >= 7)
  {
    u8MaxFreqArea = 7;
    u8MaxFreqLim = 0;
  }
  else
  {
    u8MaxFreqLim = *TabMaxFreCool[u8MaxFreqArea];
  }

  //if(C_InFanSL == u8InFanMode) //���� ��ƵF3
  //{
  //  if(u8MaxFreqLim > 3)
  //  {
  //    u8MaxFreqLim = 3;
  //  }
  //}

  if(fgErrT2)                    //T2����������
  {
    if(u8TempT1 >= TEMP_MACRO(25))
    {
      unCompRunFlgs2.bits.ErrT2Lim = 1;
    }
    else if(u8TempT1 < TEMP_MACRO(23))
    {
      unCompRunFlgs2.bits.ErrT2Lim = 0;
    }

    if(unCompRunFlgs2.bits.ErrT2Lim)
    {
      if(u8MaxFreqLim > 12)
      {
        u8MaxFreqLim = 12;
      }
    }
    else
    {
      if(u8MaxFreqLim > 4)
      {
        u8MaxFreqLim = 4;
      }
    }

    if(COMP_RUN == u8CompSta)
    {
      if(u8T2ErrCompRunTim > 33)      //30����
      {
        u8T2ErrCompRunTim = 0;
        u8MaxFreqLim = 0;
      }
    }
    else
    {
      if(u8T2ErrCompRunTim >= 3)     //3����
      {
        u8T2ErrCompRunTim = 3;
      }
      else
      {
        u8MaxFreqLim = 0;
      }
    }
  }
  else
  {
    unCompRunFlgs2.bits.ErrT2Lim = 0;
    u8T2ErrCompRunTim = 3;
  }

  if(unTempErrFlgs.bits.T3Err)      //T3����������
  {
    if(u8MaxFreqLim > 14)
    {
      u8MaxFreqLim = 14;
    }
  }
}

/**
  * @brief  : ��ͨ������
  * @param  : 
  * @retval : 
  */
void LoadControl(void)
{
  if(unFlgs2.bits.Defrost || unCompRunFlgs2.bits.DefrostExit) //��˪�ڼ��������
  {
    u8ValveDlyTim = 0;
    return;
  }

  //��������
  if(COMP_RUN == u8CompSta)
  {
    u8StopFanDly = 0;
    unFlgs.bits.OutFanCtr = 1; //ѹ��������,����ͬʱ����
  }
  else
  {
    if( (u8StopFanDly > 30)||(unInSta.bits.FastTest))
    {
      unFlgs.bits.OutFanCtr = 0;
    }
  }

  //��ͨ������
  if(fgProtErr)
  {
    unFlgs.bits.FWVCtr = 0;  //���̹ر���ͨ��,����
    u8ValveDlyTim = 0;
    return;
  }

  if((0 != u8ValveDlyTim) && (0 == unInSta.bits.FastTest))
  {
    ;
  }
  ///else if(u8InSetMode == C_ModeHeat)
  else if ( ( C_ModeHeat == u8InSetMode ) || ( C_ModeForceHeat == u8InSetMode ) )
  {
    unFlgs.bits.FWVCtr = 1;
  }
  else
  {
    unFlgs.bits.FWVCtr = 0;
  }

  if(COMP_RUN == u8CompSta)
  {
    u8ValveDlyTim = 120;
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void CompStop(void)
{
  if(COMP_RUN == u8CompSta)
  {
    u8StopFanDly = 0;
    u16CompSpd = STOP_DELAY;        //5s

    if(unSeltFlgs2.bits.CompProTime && (C_ModeHeat == u8InSetMode))
    {
      //����ѹ����4���ӱ�����ѡ
      u16CompStopTim = 0;
    }
    else
    {
      u16CompStopTim = 60;
    }

    ErrDisplay();
  }

  u8LimitFreq = 0;                  //Ƶ������
  u8LastTrgFreq = 0;
  u8RunCtrFreq = 0;
  u8RealRunFreq = 0;

  u16CompRunTim = 0;
  u8CompDly = 115;
  u8CompSta = COMP_STOP;

  unFlgs1.bits.Plat0RunOk = 0;      //����3������ƽ̨
  unFlgs1.bits.Plat1RunOk = 0;
  unFlgs1.bits.Plat2RunOk = 0;

}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void CompRun(void)
{
  if(0 == u16CompSpd)                   //Ƶ�ʿ��Ƶ���ʱ��,��������Ƶ���� 100ms--
  {
    u8LastTrgFreq = u8LimitFreq;        //��һ״̬������Ƶ��, ��Ƶ�����ⲿ����

    if( (SeerTestNo != u8InRateTest)   ||
        (C_ModeDefrost == u8InSetMode) ||
        unFlgs2.bits.Defrost)           //�������Ի��߻�˪
    {
      u8MinRunFreq = u8MinFreq;         //u8MinFreq:Eepromд�����С����Ƶ��
    }

    if(u8LastTrgFreq < u8MinRunFreq)      //С����С����Ƶ��
    {
      if(u8DgTrgHz < u8MinFreq)          //���԰�Ŀ��Ƶ��С�ڳ���涨����СƵ��
      {
        u8LastTrgFreq = u8MinRunFreq;     //С����СƵ��,������СƵ�ʹ���,��ͣ��
      }
    }//��˵���������ʱƵ��С��E���涨����СƵ���ҵ��԰�Ƶ��С�ڳ���涨����СƵ����ô�Ż�ͣ����E������СƵ��ֻ�������ܵ�ʱ����Ч��

    if(u8MinForceFreq && (SeerTestNo == u8InRateTest))      //�ض�����,ǿ����Ƶ��10����,��������ʱ��Ч
    {
      u8LastTrgFreq = u8MinForceFreq;
    }

    if(u8LastTrgFreq)                                       //����Ƶ����
    {
      if(u8RunCtrFreq > u8LastTrgFreq)
      {
        u8RunCtrFreq--;
      }
      else if(u8RunCtrFreq < u8LastTrgFreq)
      {
        if(COMP_STOP == u8CompSta)
        {
          u8RunCtrFreq = u8StartFreq;
        }
        else
        {
          u8RunCtrFreq++;
        }
      }
      else
      {
        ;
      }

      //����Ƶ�����趨, ���ݲ�ͬ��ƽ̨��Eepromѡ��ͬ�ĵȴ�ʱ��
      if(u8RunCtrFreq < u8PlatFreq0)         //С�ڵ�0ƽ̨ 30Hz
      {
        if(u8RunCtrFreq < u8LastTrgFreq)
        {
          u16CompSpd = (unsigned int)u8FreqUpSpd0;
          if(0 != u16CompSpd)
          {
            u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }
        else if(u8RunCtrFreq > u8LastTrgFreq)
        {
          if(0 == unFreqLimFlgs.bits.CurrLim)
          {
            u16CompSpd = (unsigned int)u8FreqDwSpd0;
          }
          else
          {
            u16CompSpd = (unsigned int)u8FreqDwSpd1;
          }

          if(0 != u16CompSpd)
          {
            u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }
        else  //u8RunCtrFreq = u8LastTrgFreq
        {
          if(unFlgs1.bits.Plat0RunOk)
          {
            u16CompSpd = (unsigned int)u8FreqDwSpd1;
            if(0 != u16CompSpd)
            {
              u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
          else
          {
            if(0 == unFreqLimFlgs.bits.CurrLim)
            {
              u16CompSpd = (unsigned int)u8FreqDwSpd0;
            }
            else
            {
              u16CompSpd = (unsigned int)u8FreqDwSpd1;
            }

            if(0 != u16CompSpd)
            {
              u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
        }

        unFlgs1.bits.PlatRun = 0;
      }
      else if(u8RunCtrFreq == u8PlatFreq0)               //���ڵ�0ƽ̨ 30Hz
      {
        if(0 == unFlgs1.bits.Plat0RunOk)
        {
          u16CompSpd = u16PlatHold0+PLAT0_DRV_LAG_MAIN;

          unFlgs1.bits.Plat0RunOk = 1;
          unFlgs1.bits.PlatRun = 1;
        }
        else
        {
          if(u8RunCtrFreq < u8LastTrgFreq)
          {
            u16CompSpd = (unsigned int)u8FreqUpSpd1;
            if(0 != u16CompSpd)
            {
              u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
          else if(u8RunCtrFreq > u8LastTrgFreq)
          {
            if(0 == unFreqLimFlgs.bits.CurrLim)
            {
              u16CompSpd = (unsigned int)u8FreqDwSpd0;
            }
            else
            {
              u16CompSpd = (unsigned int)u8FreqDwSpd1;
            }

            if(0 != u16CompSpd)
            {
              u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
          else
          {
            u16CompSpd = (unsigned int)u8FreqUpSpd1;
            if(0 != u16CompSpd)
            {
              u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }

          unFlgs1.bits.PlatRun = 0;
        }
      }
      else if(u8RunCtrFreq < u8PlatFreq1)        //С�ڵ�1ƽ̨ 42Hz
      {
        if(u8RunCtrFreq < u8LastTrgFreq)
        {
          u16CompSpd = (unsigned int)u8FreqUpSpd1;
          if(0 != u16CompSpd)
          {
            u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }
        else
        {
          u16CompSpd = (unsigned int)u8FreqDwSpd1;
          if(0 != u16CompSpd)
          {
            u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }

        unFlgs1.bits.PlatRun = 0;
      }
      else if(u8RunCtrFreq == u8PlatFreq1)       //���ڵ�1ƽ̨ 42Hz
      {
        if(0 == unFlgs1.bits.Plat1RunOk)
        {
          u16CompSpd = u16PlatHold1+PLAT1_DRV_LAG_MAIN;

          unFlgs1.bits.Plat1RunOk = 1;
          unFlgs1.bits.PlatRun = 1;
        }
        else
        {
          if(u8RunCtrFreq < u8LastTrgFreq)
          {
            u16CompSpd = (unsigned int)u8FreqUpSpd2;
            if(0 != u16CompSpd)
            {
              u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
          else
          {
            u16CompSpd = (unsigned int)u8FreqDwSpd1;
            if(0 != u16CompSpd)
            {
              u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }

          unFlgs1.bits.PlatRun = 0;
        }
      }
      else if(u8RunCtrFreq < u8PlatFreq2)        //С�ڵ�2ƽ̨ 62Hz
      {
        if(u8RunCtrFreq < u8LastTrgFreq)
        {
          u16CompSpd = (unsigned int)u8FreqUpSpd2;
          if(0 != u16CompSpd)
          {
            u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }
        else
        {
          u16CompSpd = (unsigned int)u8FreqDwSpd2;
          if(0 != u16CompSpd)
          {
            u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }

        unFlgs1.bits.PlatRun = 0;
      }
      else if(u8RunCtrFreq == u8PlatFreq2)       //���ڵ�2ƽ̨ 62Hz
      {
        if(0 == unFlgs1.bits.Plat2RunOk)
        {
          u16CompSpd = u16PlatHold2+PLAT2_DRV_LAG_MAIN;
          unFlgs1.bits.Plat2RunOk = 1;
          unFlgs1.bits.PlatRun = 1;
        }
        else
        {
          if(u8RunCtrFreq < u8LastTrgFreq)
          {
            u16CompSpd = (unsigned int)u8FreqUpSpd3;
            if(0 != u16CompSpd)
            {
              u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }
          else
          {
            u16CompSpd = (unsigned int)u8FreqDwSpd2;
            if(0 != u16CompSpd)
            {
              u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
            }
          }

          unFlgs1.bits.PlatRun = 0;
        }
      }
      else                        //ƽ̨����,��������Ŀ��Ƶ��
      {
        if(u8RunCtrFreq < u8LastTrgFreq)
        {
          u16CompSpd = (unsigned int)u8FreqUpSpd3;
          if(0 != u16CompSpd)
          {
            u16CompAccel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }
        else
        {
          u16CompSpd = (unsigned int)u8FreqDwSpd3;
          if(0 != u16CompSpd)
          {
            u16CompDecel = (unsigned short)(600 / u16CompSpd + 1);
          }
        }

        unFlgs1.bits.PlatRun = 0;
      }

      u8CompSta = COMP_RUN;
      unLimStop.u8Byte = 0;
    }
    else
    {
      CompStop();
    }
  }
}

/**
  * @brief  : ͨ��Ƶ������  �õ�ʵ������Ƶ��ֵ
  * @param  : 
  * @retval : 
  */
unsigned char GetRealFreq(unsigned char freq)
{
  unsigned char *pFreqData;

  pFreqData = TabRunFreq;
  pFreqData = pFreqData + freq;

  if(0 == freq)
  {
    return(0);
  }
  //if((0xFF != u8InRateFreq)&&(0 == unFlgs2.bits.Defrost)&&(0 == unCompRunFlgs2.bits.DefrostExit))  //��˪ʱ,������Ƶ��
  //{
  //  return(u8InRateFreq);
  //}
  //else
  //{
  return(*pFreqData);
  //}
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void GetErrBit(unsigned char mdata, unsigned char num, unsigned char end)
{
  unsigned char i = 0;
  for(i = 1; i <= end; i++)
  {
    if(mdata & 0x01)
    {
      num = num + i;
      u8ErrStop = num;
      break;
    }
    mdata = mdata >> 1;
  }
}

/**
  * @brief  : 
  * @param  : 
  * @retval : 
  */
void ErrDisplay(void)
{
  /*
  1-100: main error, 
  101-130: comp error, 
  131-160: fan error, 
  161-171: pfc error, 
  others: reserve
  */
  //GetErrBit(unTempProtFlgs.u8Byte,1,8);
  GetErrBit(unInErr1.u8Byte, 9, 8);
  GetErrBit(unInErr2.u8Byte, 17, 8);
  GetErrBit(unSysErrFlgs.u8Byte, 25, 8);
  GetErrBit(unFreqProt.u8Byte, 33, 8);
  GetErrBit(unCompErrFlgs1.u8Byte, 41, 8);
  GetErrBit(unCompErrFlgs2.u8Byte, 49, 8);
}

