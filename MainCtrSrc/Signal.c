#include "UsrDef.h"
#include "TempSample.h"
#include "TimeTick.h"
#include "ParameterPro.h"
#include "Signal.h"
#include "CompCtr.h"

unsigned char u8AcCurrRMS_T10;
unsigned char u8HalfDcVoltFil;
unsigned char u8HalfDcVoltRMS;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SignalInit(void)
{
  u8TempT3 = TEMP_MACRO(30);
  u8TempT4 = TEMP_MACRO(30);
  u8TempT5 = 60;
  u8TempIpm = 60;

  u8T4TempAd = 114;
  u8T3TempAd = 114;
  u8AcVoltAd = 140;      //220Vac
  u8T5TempAd = 33;
  u8AcCurrAd = 0;

  u8AcCurrRMS_T10 = 0;
  u8HalfDcVoltFil = 150;
  u8TempT3Min = 255;

#if 0
  u8InSetMode = C_ModeHeat;
  u8EvapMidTempAd = 102;//114;  //T2=23��
  u8IndoorTempAd = 102;//114;   //T1=23��
  u8InSetTemp = 21;
  u8TempT2 = Temp2Value(25);
  u8TempT1 = Temp2Value(25);
#endif

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SignalSrv(void)
{
  unsigned char u8Temp;

  //PCBACheck();

  ///unSeltFlgs1.bits.VLimSelt = 0;  ///debug only
  
  if(unSeltFlgs1.bits.VLimSelt && (u8TimPowerOn >= 100))   //���õ�ѹ��Ƶ �ϵ�10s����Ч
  {
    if(unSeltFlgs1.bits.ACVoltSelt)       //������Ƶ
    {
      //u8Temp = u8AcVoltAd;
      u8Temp = u8DcVoltAd;
    }
    else
    {
      u8Temp = u8DcVoltAd;
    }

    if(u8VoltLimPeriod >= 2)
    {
      u8VoltLimPeriod = 0;

      if(0 == u8VoltLimArea)          //no limit
      {
        if(u8Temp < u8VoltLim1)       //190V Eeprom����
        {
          unFreqLimFlgs.bits.VoltLim = 1;
          u8VoltLimArea = 1;
        }
        else
        {
          u8VoltLimitTim = 0;
        }
      }
      else if(1 == u8VoltLimArea)       //first limit
      {
        if(u8Temp < u8VoltLim2)         //180V Eeprom����
        {
          unFreqLimFlgs.bits.VoltLim = 1;
          u8VoltLimArea = 2;
          u8VoltLimitTim = 0;
        }
        else if(u8Temp >= u8VoltRst1)   //200V Eeprom����
        {
          if(u8VoltLimitTim > MINUTE_MACRO(3))
          {
            unFreqLimFlgs.bits.VoltLim = 0;
            u8VoltLimArea = 0;
          }
        }
      }
      else if(2 == u8VoltLimArea)       //second limit
      {
        if(u8VoltLimitTim > MINUTE_MACRO(3))
        {
          if(u8Temp >= u8VoltRst1)      //200V Eeprom����
          {
            unFreqLimFlgs.bits.VoltLim = 0;
            u8VoltLimArea = 0;
          }
          else if(u8Temp >= u8VoltRst2) //190V Eeprom����
          {
            unFreqLimFlgs.bits.VoltLim = 1;
            u8VoltLimArea = 1;
            u8VoltLimitTim = 0;
          }
        }
      }
      else
      {
        unFreqLimFlgs.bits.VoltLim = 0;
        u8VoltLimArea = 0;
        u8VoltLimitTim = 0;
      }
    }

    if(unSeltFlgs1.bits.ACVoltSelt)     //������ѹ��Ƶ
    {
    }
    else
    {
      unSysErrFlgs.bits.ACVoltLowErr = 0;
      u8VoltStopTim = 0;
    }
  }
  else
  {
    //�޵�ѹ��Ƶ����
    unFreqLimFlgs.bits.VoltLim = 0;
    unSysErrFlgs.bits.ACVoltLowErr = 0;
    u8VoltLimArea = 0;
    u8VoltLimitTim = 0;
  }

  //�����ػ�����,һ�β��������
  if(u8AcCurrRMS_T10 >= u8ACCurrOver)      //Eeprom��ĵ����Ƚ�ֵ��ʵ��ֵ��10��,��߾���
  {
    unSysErrFlgs.bits.OverCurrErr = 1;
  }
  else
  {
    if(u8ErrHoldTim >= ERR_RST_TIM)
    {
      unSysErrFlgs.bits.OverCurrErr = 0;
    }
  }

  //���PFC����:���ģʽ�¼��ĸ�ߵ�ѹ����300V��PFC����,30s���Իָ�; Eeprom��ѡ���ֹ/����˹���
  if(unInSta.bits.FastTest && unSeltFlgs2.bits.PfcErrSel)   //PFC���ϼ��ʹ��,��1����Ч Eeprom����
  {
    if(u16CompRunTim >= 10)
    {
      if(u8DcVoltAd < 150)         //���β��ѹ��ADֵ:150 ����AD=Vdc/2�����Vdc=300V
      {
        if(u8PfcErrDly > 100)
        {
          if(0 == unSysErrFlgs2.bits.PfcErr)
          {
            u8ErrHoldTim = 0;     //�˹�����30s���Իָ�,��ʱ������������������
          }
          unSysErrFlgs2.bits.PfcErr = 1;//unCompErrFlgs1.bits.PfcErr
        }
      }
      else
      {
        u8PfcErrDly = 0;
      }
    }
    else
    {
      u8PfcErrDly = 0;
    }
  }
  else
  {
    u8PfcErrDly = 0;
    unSysErrFlgs2.bits.PfcErr = 0;
  }

  //PFC���ϻָ�,�ڳ��ִ���30s��ָ�
  if(u8ErrHoldTim >= ERR_RST_TIM)
  {
    u8PfcErrDly = 0;
    unSysErrFlgs2.bits.PfcErr = 0;
  }

  // ��ѹ����(�ߵ�ƽ����)
  if((C_ModeDefrost == u8InSetMode) || unFlgs2.bits.Defrost)
  {
    u16QuitDefrostTim = 480;           //��˪�ڼ估��˪����8���Ӳ�����ѹ����
    unFlgs2.bits.LowPressEnable = 0;
  }

  if(C_ModeHeat == u8InSetMode)
  {
    if((u8TempT4 < TEMP_MACRO(-8)) || unTempErrFlgs.bits.T4Err) // ��T4���������ϻ���T4��-8��,���ټ���ѹ
    {
      unFlgs2.bits.LowPressEnable = 0;
    }
    else if((u8TempT4 < TEMP_MACRO(0)) && (COMP_RUN == u8CompSta) && (u16CompRunTim < 300))   // -8��<T4<0��,��ѹ��������5min֮�ڲ�����ѹ
    {
      unFlgs2.bits.LowPressEnable = 0;
    }
    else
    {
      unFlgs2.bits.LowPressEnable = 1;
    }
  }
  else
  {
    unFlgs2.bits.LowPressEnable = 1;
  }
}

/**
  * @brief  : �����¶ȱ�������Ƶ
  * @param  :
  * @retval :
  */
void ProtTP(void)
{
  if(unTempErrFlgs.bits.T5Err)            //�����¶ȴ������Ѿ��𻵻����, ��ִ�б�������
  {
    unFreqLimFlgs.bits.T5TempLim = 0;
    fgProtT5 = 0;

    return;
  }

  if(0 == fgProtT5)                           //����������ͣ����֧
  {
    if(u8TempT5 >= PROTECT_T5_HIGH)           //��������110��
    {
      if(u8T5Prot5s >= 50)
      {
        u8T5Prot5s = 0;
        fgProtT5 = 1;
      }
    }
    else
    {
      u8T5Prot5s = 0;
    }
  }
  else
  {
    if(u8TempT5 < PROTECT_T5_RST)                //��������90��
    {
      if(u8T5Prot5s >= 50)
      {
        u8T5Prot5s = 0;
        fgProtT5 = 0;
      }
    }
    else
    {
      u8T5Prot5s = 0;
    }
  }

  if(u8TempT5 >= PROTECT_T5_KEEP)                //������Ƶ��֧,����100�汣��
  {
    //unFreqLimFlgs.bits.TdTempLim = 1;
    if(u8TempT5 >= PROTECT_T5_LIMT)              //����105��������Ƶ
    {
      unFreqLimFlgs.bits.T5TempLim = 1;
      if(u8T5Freq120s >= 120)
      {
        u8T5Freq120s = 0;
        if(u8RunFreqGrade > 1)                   //����޵���һ��Ƶ��
        {
          u8FreqSet = LimitFreq(u8RunCtrFreq);
        }
      }
    }
  }
  else
  {
    unFreqLimFlgs.bits.T5TempLim = 0;
    u8T5Freq120s = 120;
  }
}

/**
  * @brief  : �����������ᱣ��(����)T2
  * @param  : 
  * @retval : 
  */
void ProtT2Cool(void)
{
  if(fgErrT2)                                 //T2�¶ȴ������Ѿ��𻵻����,��ִ�б�������
  {
    unFreqLimFlgs.bits.T2TempLLim = 0;
    fgProtCoolT2 = 0;

    return;
  }

  if(0 == fgProtCoolT2)                       //�����ᱣ����ͣ����֧
  {
    if(u8TempT2 < PROTECT_T2_COOL_LOW)        //T2 < 0��
    {
      if(u8T2Prot5s >= 20)                    //2s
      {
        u8T2Prot5s = 0;
        fgProtCoolT2 = 1;
      }
    }
    else
    {
      u8T2Prot5s = 0;
    }
  }
  else
  {
    if(u8TempT2 >= PROTECT_T2_COOL_RST)       //T2 >= 5��
    {
      if(u8T2Prot5s >= 20)
      {
        u8T2Prot5s = 0;
        fgProtCoolT2 = 0;
        unFreqLimFlgs.bits.T2TempLLim = 0;
      }
    }
    else
    {
      u8T2Prot5s = 0;
    }
  }

  if(u8TempT2 < PROTECT_T2_COOL_LIMT)         //��Ƶ��֧, T2 < 4��
  {
    unFreqLimFlgs.bits.T2TempLLim = 1;
    if(u8T2Freq60s >= 60)                     //60s
    {
      u8T2Freq60s = 0;
      u8FreqSet = LimitFreq(u8RunCtrFreq);
      if(0 == u8FreqSet)
      {
        fgCoolT2Stop = 1;
      }
    }
  }
  else if(u8TempT2 >= PROTECT_T2_COOL_LIMTRST)//T2 > 7��
  {
    unFreqLimFlgs.bits.T2TempLLim = 0;
    u8T2Freq60s = 60;
  }
  else
  {
    u8T2Freq60s = 60;
  }
}


/**
  * @brief  : ���������±���
  * @param  : 
  * @retval : 
  */
void ProtT3Cool(void)
{
  unsigned char temp_limit, temp_stop, temp_reset;

  if(unTempErrFlgs.bits.T3Err)            //T3�¶ȴ������Ѿ��𻵻����, ��ִ�б�������
  {
    fgProtCoolT3 = 0;
    unFreqLimFlgs.bits.T3TempLim = 0;
    return;
  }

  if(SeerTestNo != u8InRateTest)
  {
    temp_limit = PROTECT_T23_HEAT_HIGH;   //65��C  ��������
  }
  else
  {
    temp_limit = u8T3HighLmtTemp;         //eeprom, ע��������Chigo���¶ȱ�ƥ��!!!
  }

  temp_stop = temp_limit + 5 + 5;
  temp_reset =  temp_limit - 3 - 3;

  if(0 == fgProtCoolT3)                   //������ػ���֧
  {
    if(u8TempT3 >= temp_stop)
    {
      if(u8T3Prot5s >= 50)                //5s
      {
        u8T3Prot5s = 0;
        fgProtCoolT3 = 1;
      }
    }
    else
    {
      u8T3Prot5s = 0;
    }
  }
  else
  {
    if(u8TempT3 < temp_reset)
    {
      if(u8T3Prot5s >= 30)                //3s
      {
        u8T3Prot5s = 0;
        fgProtCoolT3 = 0;
      }
    }
    else
    {
      u8T3Prot5s = 0;
    }
  }

  if(u8TempT3 >= temp_reset)              //��Ƶ��֧
  {
    //unFreqLimFlgs.bits.T3TempLim = 1;
    if(u8TempT3 >= temp_limit)
    {
      unFreqLimFlgs.bits.T3TempLim = 1;
      if(u8T3Freq60s >= 60)
      {
        u8T3Freq60s = 0;

        if(u8RunFreqGrade > 1)
        {
          u8FreqSet = LimitFreq(u8RunCtrFreq);
        }
      }
    }
  }
  else
  {
    unFreqLimFlgs.bits.T3TempLim = 0;
    u8T3Freq60s = 80;
  }
}

/**
  * @brief  : ���������±���(����)
  * @param  : 
  * @retval : 
  */
void ProtT2Heat(void)
{
  if(fgErrT2)                                 //T2�¶ȴ������Ѿ��𻵻����, ��ִ�б�������
  {
    fgProtHeatT2 = 0;
    unFreqLimFlgs.bits.T2TempHLim = 0;
    return;
  }

  if(0 == fgProtHeatT2)
  {
    if(u8TempT2 >= PROTECT_T2_HEAT_HIGH)      //65��
    {
      if(u8T2Prot5s >= 25)
      {
        u8T2Prot5s = 0;
        fgProtHeatT2 = 1;
      }
    }
    else
    {
      u8T2Prot5s = 0;
    }
  }
  else
  {
    if(u8TempT2 < u8T2HighLmtTemp)           //�˲�������ֵ����eeprom
    {
      if(u8T2Prot5s >= 25)
      {
        u8T2Prot5s = 0;
        fgProtHeatT2 = 0;
      }
    }
    else
    {
      u8T2Prot5s = 0;
    }
  }

  if(u8TempT2 >= u8T2HighLmtTemp)         //E��  52��
  {
    //unFreqLimFlgs.bits.TeTempHLim = 1;
    if(u8TempT2 >= PROTECT_T2_HEAT_DEC)   //55�� //��Ƶ��֧
    {
      unFreqLimFlgs.bits.T2TempHLim = 1;
      if(u8T2Freq60s >= 20)
      {
        u8T2Freq60s = 0;

        if(u8RunFreqGrade > 1)
        {
          u8T2Lim180s = 0;
          u8FreqSet = LimitFreq(u8RunCtrFreq);
        }
        else
        {
          u8T2Freq60s = 20;
          if(u8T2Lim180s > 180)
          {
            u8FreqSet = 0;
            fgHeatT2Stop = 1;
          }
        }
      }
    }
    else
    {
      u8T2Lim180s = 0;
      u8T2Freq60s = 20;
    }
  }
  else
  {
    unFreqLimFlgs.bits.T2TempHLim = 0;
    u8T2Freq60s = 60;
  }
}

