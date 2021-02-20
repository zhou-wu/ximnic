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
  u8EvapMidTempAd = 102;//114;  //T2=23℃
  u8IndoorTempAd = 102;//114;   //T1=23℃
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
  
  if(unSeltFlgs1.bits.VLimSelt && (u8TimPowerOn >= 100))   //配置电压限频 上电10s后有效
  {
    if(unSeltFlgs1.bits.ACVoltSelt)       //交流限频
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
        if(u8Temp < u8VoltLim1)       //190V Eeprom参数
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
        if(u8Temp < u8VoltLim2)         //180V Eeprom参数
        {
          unFreqLimFlgs.bits.VoltLim = 1;
          u8VoltLimArea = 2;
          u8VoltLimitTim = 0;
        }
        else if(u8Temp >= u8VoltRst1)   //200V Eeprom参数
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
          if(u8Temp >= u8VoltRst1)      //200V Eeprom参数
          {
            unFreqLimFlgs.bits.VoltLim = 0;
            u8VoltLimArea = 0;
          }
          else if(u8Temp >= u8VoltRst2) //190V Eeprom参数
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

    if(unSeltFlgs1.bits.ACVoltSelt)     //交流电压限频
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
    //无电压限频功能
    unFreqLimFlgs.bits.VoltLim = 0;
    unSysErrFlgs.bits.ACVoltLowErr = 0;
    u8VoltLimArea = 0;
    u8VoltLimitTim = 0;
  }

  //电流关机保护,一次侧电流保护
  if(u8AcCurrRMS_T10 >= u8ACCurrOver)      //Eeprom里的电流比较值是实际值的10倍,提高精度
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

  //检查PFC错误:快检模式下检查母线电压低于300V报PFC故障,30s后自恢复; Eeprom可选择禁止/激活此功能
  if(unInSta.bits.FastTest && unSeltFlgs2.bits.PfcErrSel)   //PFC故障检测使能,“1”有效 Eeprom参数
  {
    if(u16CompRunTim >= 10)
    {
      if(u8DcVoltAd < 150)         //二次侧电压是AD值:150 根据AD=Vdc/2计算的Vdc=300V
      {
        if(u8PfcErrDly > 100)
        {
          if(0 == unSysErrFlgs2.bits.PfcErr)
          {
            u8ErrHoldTim = 0;     //此故障是30s后自恢复,计时与其他故障有所区别
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

  //PFC故障恢复,在出现错误30s后恢复
  if(u8ErrHoldTim >= ERR_RST_TIM)
  {
    u8PfcErrDly = 0;
    unSysErrFlgs2.bits.PfcErr = 0;
  }

  // 低压保护(高电平保护)
  if((C_ModeDefrost == u8InSetMode) || unFlgs2.bits.Defrost)
  {
    u16QuitDefrostTim = 480;           //化霜期间及化霜结束8分钟不检测低压开关
    unFlgs2.bits.LowPressEnable = 0;
  }

  if(C_ModeHeat == u8InSetMode)
  {
    if((u8TempT4 < TEMP_MACRO(-8)) || unTempErrFlgs.bits.T4Err) // 当T4传感器故障或者T4≤-8℃,不再检测低压
    {
      unFlgs2.bits.LowPressEnable = 0;
    }
    else if((u8TempT4 < TEMP_MACRO(0)) && (COMP_RUN == u8CompSta) && (u16CompRunTim < 300))   // -8℃<T4<0℃,且压缩机启动5min之内不检测低压
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
  * @brief  : 排气温度保护与限频
  * @param  :
  * @retval :
  */
void ProtTP(void)
{
  if(unTempErrFlgs.bits.T5Err)            //排气温度传感器已经损坏或错误, 不执行保护动作
  {
    unFreqLimFlgs.bits.T5TempLim = 0;
    fgProtT5 = 0;

    return;
  }

  if(0 == fgProtT5)                           //排气保护与停机分支
  {
    if(u8TempT5 >= PROTECT_T5_HIGH)           //排气高于110℃
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
    if(u8TempT5 < PROTECT_T5_RST)                //排气低于90℃
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

  if(u8TempT5 >= PROTECT_T5_KEEP)                //排气限频分支,大于100℃保持
  {
    //unFreqLimFlgs.bits.TdTempLim = 1;
    if(u8TempT5 >= PROTECT_T5_LIMT)              //大于105℃立即降频
    {
      unFreqLimFlgs.bits.T5TempLim = 1;
      if(u8T5Freq120s >= 120)
      {
        u8T5Freq120s = 0;
        if(u8RunFreqGrade > 1)                   //最低限到第一档频率
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
  * @brief  : 蒸发器防冻结保护(制冷)T2
  * @param  : 
  * @retval : 
  */
void ProtT2Cool(void)
{
  if(fgErrT2)                                 //T2温度传感器已经损坏或错误,不执行保护动作
  {
    unFreqLimFlgs.bits.T2TempLLim = 0;
    fgProtCoolT2 = 0;

    return;
  }

  if(0 == fgProtCoolT2)                       //防冻结保护与停机分支
  {
    if(u8TempT2 < PROTECT_T2_COOL_LOW)        //T2 < 0℃
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
    if(u8TempT2 >= PROTECT_T2_COOL_RST)       //T2 >= 5℃
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

  if(u8TempT2 < PROTECT_T2_COOL_LIMT)         //限频分支, T2 < 4℃
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
  else if(u8TempT2 >= PROTECT_T2_COOL_LIMTRST)//T2 > 7℃
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
  * @brief  : 冷凝器高温保护
  * @param  : 
  * @retval : 
  */
void ProtT3Cool(void)
{
  unsigned char temp_limit, temp_stop, temp_reset;

  if(unTempErrFlgs.bits.T3Err)            //T3温度传感器已经损坏或错误, 不执行保护动作
  {
    fgProtCoolT3 = 0;
    unFreqLimFlgs.bits.T3TempLim = 0;
    return;
  }

  if(SeerTestNo != u8InRateTest)
  {
    temp_limit = PROTECT_T23_HEAT_HIGH;   //65°C  能力测试
  }
  else
  {
    temp_limit = u8T3HighLmtTemp;         //eeprom, 注意数据与Chigo的温度表匹配!!!
  }

  temp_stop = temp_limit + 5 + 5;
  temp_reset =  temp_limit - 3 - 3;

  if(0 == fgProtCoolT3)                   //保护与关机分支
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

  if(u8TempT3 >= temp_reset)              //限频分支
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
  * @brief  : 蒸发器高温保护(制热)
  * @param  : 
  * @retval : 
  */
void ProtT2Heat(void)
{
  if(fgErrT2)                                 //T2温度传感器已经损坏或错误, 不执行保护动作
  {
    fgProtHeatT2 = 0;
    unFreqLimFlgs.bits.T2TempHLim = 0;
    return;
  }

  if(0 == fgProtHeatT2)
  {
    if(u8TempT2 >= PROTECT_T2_HEAT_HIGH)      //65℃
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
    if(u8TempT2 < u8T2HighLmtTemp)           //此参数的数值定义eeprom
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

  if(u8TempT2 >= u8T2HighLmtTemp)         //E方  52℃
  {
    //unFreqLimFlgs.bits.TeTempHLim = 1;
    if(u8TempT2 >= PROTECT_T2_HEAT_DEC)   //55℃ //限频分支
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

