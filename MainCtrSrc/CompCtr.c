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
  u16CompStopTim = 240;    //上电没有压缩机保护,是否需要等膨胀阀复位完毕,才能开压缩机?
  u8DefrostExit = 50;
  u8ValveDlyTim = 0;

  u8InRateFreq = 0xFF;     //内机能力测试
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
  ///if(unInSta.bits.FastTest)       //快检跳过三分钟保护
  /* 快检或者能力测试, 跳过三分钟保护 */
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

  //要求停压缩机的故障, 置位保护标志
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
    u8ErrHoldTim = 0;                     //运行错误及报警恢复时间,所有故障解除30s后恢复重启
  }

  ProtTP();                               //通用保护:排气

  if(C_ModeHeat == u8InSetMode)           //通用保护:电流,其他模式保护另作调用
  {
    if(SeerTestMax != u8InRateTest)       //低温制热模式电流限频无效
    {
      LimCurrHeat();
    }
    else
    {
      unCompRunFlgs2.bits.NoPlat = 1;     //不跑平台
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
  LoadControl();                          //暂时解决制冷四通阀短暂换向的问题

  u8EnergyFreq = GetRealFreq(u8FreqSet);  //查表得出运行频率

  if ( (SeerTestNo != u8InRateTest ) && ( C_ModeOff != u8InSetMode ) )
  {
    if ( 0 != u8SeerFreq )
    {
      u8EnergyFreq = (u8SeerFreq >> 1);
    }
  }

  u8RunFreqGrade = GetFreqGrade(u8RunCtrFreq);

  //SEERFreqDeal();               //能力测试模式,根据工况单独频率控制
  //FreqMinChange();              //最小运行频率限制？
  if(unFlgs2.bits.Defrost || (C_ModeDefrost == u8InSetMode) || (unCompRunFlgs2.bits.DefrostExit)) //化霜优先级高于锁频
  {
    //当出现除霜退出回油控制
    if(unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unCompRunFlgs1.u8Byte)
    {
      u8TargetFreq = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
    else
    {
      u8EnergyFreq = GetRealFreq(u8FreqSet);        //查表得出运行频率
      u8TargetFreq = u8EnergyFreq;
      u16ROFreqPlatTim = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
  }
  else if(u8DgTrgHz >= u8MinFreq)                   //小板调试优先 10Hz或Eeprom规定的最小频率
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
      SEERFreqDeal();                               //能力测试模式,根据工况单独频率控制 
    }
    else
    {
      u8TargetFreq = 0;
      u16ROLowFreqTim = u16RetOilTim1;
      unCompRunFlgs2.bits.RetOil = 0;
    }
  }
  */
  else if(0 != u8EnergyFreq)                        //自由跑计算的理论能需频率
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
      
      //if(SeerTestNo != u8InRateTest)             //内机能力测试不允许回油
      //{
      //  u16ROLowFreqTim = u16RetOilTim1;
      //  unCompRunFlgs2.bits.RetOil = 0;
      //}
      //以下回油从DR模式剥离,待考虑
      //if(unCompRunFlgs2.bits.RetOil)
      //{
      //  u8EnergyFreq = 0;                        //回油,把压缩机停止,利用3分钟来回油
      //  u16ROLowFreqTim = u16RetOilTim1;
      //  unCompRunFlgs2.bits.RetOil = 0;
      //}

      if(unSeltFlgs1.bits.RetOilSelt)              // 进入回油
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
              //当出现除霜、蒸发器温度限频、冷凝器温度限频、排气限频、电流限频、电压限频退出回油控制
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
  else                              //既无debug也无外机运行频率
  {
    u8TargetFreq = 0;
    u16ROLowFreqTim = u16RetOilTim1;
    unCompRunFlgs2.bits.RetOil = 0;
  }

  if(u8PowerOnDly < 33)             //主电源继电器延时
  {
    u8TargetFreq = 0;
  }

  if((COMP_STOP == u8CompSta)       &&
     (SeerTestNo == u8InRateTest)   &&
     (1 == unSeltFlgs1.bits.PMVSelt))
  {
    if(0 == u16TrgPulse)           //目标开度为0,不能开压机
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

    u8LimitFreq = u8TargetFreq;         //根据目标频率限频
    if(SeerTestNo == u8InRateTest)      //能力测试无电压限频
    {
      if(1 == u8VoltLimArea)            //电压限频,区间在外边已做好
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

    //如果出现电压、电流、排气、冷凝器、蒸发器限频,不跑第对应平台
    if(unFreqLimFlgs.u8Byte || unCompRunFlgs2.bits.NoPlat)
    {
      if(u8LimitFreq < u8PlatFreq0)
      {
        //目标频率小于第0个平台频率
        unFlgs1.bits.Plat0RunOk = 1;
      }

      if(u8LimitFreq < u8PlatFreq1)
      {
        //目标频率小于第1个平台频率
        unFlgs1.bits.Plat1RunOk = 1;
      }

      //如果出现电压、电流、排气、冷凝器、蒸发器限频,不跑第2个平台
      if(unFreqLimFlgs.u8Byte)
      {
        unFlgs1.bits.Plat2RunOk = 1;
      }

      if(unFlgs1.bits.PlatRun)
      {
        //正在平台运行,在平台运行时出现电压、电流、排气、冷凝器、蒸发器限频,不跑平台
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
      //要求跑第0个平台当目标频率小于平台频率,运行平台频率
      if(u8LimitFreq < u8PlatFreq0)
      {
        u8LimitFreq = u8PlatFreq0;
      }
    }
    else if(0 == unFlgs1.bits.Plat1RunOk)
    {
      //要求跑第一个平台当目标频率小于平台频率,运行平台频率
      if(u8LimitFreq < u8PlatFreq1)
      {
        u8LimitFreq = u8PlatFreq1;
      }
    }
    else
    {
      ;
    }

    CompRun();          //保护及限频结束,正式进入压机运行
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
    u16ExitTime = 600 + 40;                 //增加40秒,四通阀切换时间
  }

  if(u8DefrostDly3Min < 0xFF)
  {
    u8DefrostDly3Min++;
  }

  //====化霜动作处理====
  if(unCompRunFlgs2.bits.DefrostExit)
  {
    //if(SEERTestNO != u8InRateTest)
    //{
    //  if(u8DefrostDly3Min >= 15)          //能力测试,15秒后,直接退出化霜
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
  else if(unFlgs2.bits.Defrost)       //化霜处理过程
  {
    u16InDefrostTim++;
    if(u8TempT3 <= u8T3DefrostExit2)
    {
      u8DefrostDly3Min = 0;
    }

    // 化霜退出条件：化霜时间达到 10分钟或室外管温T3达到则退出化霜
    if((u8TempT3 > u8T3DefrostExit1) || (u8DefrostDly3Min > 80) || (u16InDefrostTim >= u16ExitTime))
    {
      u8DefrostDly3Min = 0;
      unFlgs2.bits.Defrost = 0;
      unCompRunFlgs2.bits.DefrostExit = 1;
      fgDefrostStop = 1;              //化霜结束停机
    }

    unCompRunFlgs2.bits.NoPlat = 1;   //化霜,不跑平台
    return;
  }
  //if(u8DefrostExit < 40)            //化霜结束,不跑平台
  //{
  //  if(SEERTestNO != u8InRateTest)  //化霜结束,不跑平台
  //  {
  //    unFlgs1.bits.Plat0RunOk = 1;
  //    unFlgs1.bits.Plat1RunOk = 1;
  //    unFlgs1.bits.Plat2RunOk = 1;
  //  }
  //}

  unCompRunFlgs2.bits.DefrostIn = 0;

  //==================化霜进入处理================
  if(unTempErrFlgs.bits.T3Err)  //T3故障,强制化霜
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

  //得到最小T3值 
  ///if(u16CompRunTim < MINUTE_MACRO(10))     //压缩机是否停止后,是否要重新登启动后的10分钟后
  if(u16InDefrostTim < MINUTE_MACRO(10))
  {
    u8TempT3Min = 255;
  }

  if((u16InDefrostTim >= MINUTE_MACRO(10)) && (u16InDefrostTim <= MINUTE_MACRO(15)))
  {
    if(u8TempT3 < u8TempT3Min)
    {
      u8T3MinDly++;
      if(u8T3MinDly >= 2)     //抗干扰1S,相当于2次采样值
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

  //===============条件1=============
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

  //===============条件2==============
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

  //===============条件3===============
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

  //===============条件4===============
  if(u16InDefrostTim >= MINUTE_MACRO(120))
  {
    if(u8TempT3 < TEMP_MACRO(-15))
    {
HERE112:                //进入化霜
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
  unsigned char up_temp = 0, down_temp = 0;         //上升/下降温度点

  up_temp = Area * 2;
  up_temp += 1;                   //Area *2 +1:得到当前区间的向上方向温度点地址
  down_temp = up_temp - 3;          //得到当前区间的向下方向温度点地址 (0区间,不用向下判断,所以不用考虑负数)

  up_temp = *(TempTab + up_temp);     //得到当前区间的向上方向温度点
  down_temp = *(TempTab + down_temp);   //得到当前区间的向下方向温度点

  if(0 == Area)               //0区间和14区间特殊处理,只有一个方向
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

  for(index = 0; index < 29; index++) //逐个查找有效的表格温度值,找到0xFF认为有效值结束,最多28个数据
  {
    end_data = *TempTab;
    if(0xFF == end_data)
    {
      break;
    }
    TempTab++;
  }

  TempTab = TempTab - index;          //查找算法
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

  for(index = 0; index < 29; index++) //逐个查找有效的表格温度值,找到0xFF认为有效值结束,最多28个数据
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
  * @brief  : 当前频率 * 0.9
  * @param  : 输入当前运行频率
  * @retval : 输出:限频后的频率序列
  */
unsigned char LimitFreq(unsigned char freq)
{
  unsigned short ii;
  unsigned char i;

  if(u8RunFreqGrade <= 1) //当前运行频率等级
  {
    return(0);    //在F1基础上,再限频,则关压缩机
  }

  ii = freq;
  ii = ii * 9;
  ii = ii / 10;
  i = (unsigned char)ii;    //根据输入频率进行90%幅度的限频
  i = GetFreqGrade(i);  //i:返回限频后的频率等级

  if(i < 1)
  {
    i = 1;
  }

  if(i < u8FreqSet) //要小于当前频率等级,才能限频,防止频率反而高了
  {
    return(i);
  }
  else
  {
    return(u8FreqSet);
  }
}


/**
  * @brief  : 当前运行频率转换成频率序列等级
  * @param  : 输入实际频率
  * @retval : 输出频率等级
  */
unsigned char GetFreqGrade(unsigned char freq)
{
  unsigned char index;
  unsigned char *pFreqData;

  pFreqData = &TabRunFreq[25];    //Eeprom读取的频率表,倒序查表
  for(index = 25; index > 0; index--) //Eeprom的频率等级只有25
  {
    if(freq >= *pFreqData)
    {
      freq = *pFreqData;
      break;
    }

    pFreqData--;
  }

  pFreqData--;
  while(freq == *pFreqData)     //如果序列出现相同,则以最小那个为准
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
    if(freq > 3)                    //若一直减到第一级频率,而入口参数大于3Hz, 按照第一级频率跑
    {
      index = 1;
    }
  }

  return(index);
}

/**
  * @brief  : 制热电流限频
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
  * @brief  : 制冷电流限频
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
  * @brief  : 电流保护
  * @param  : 电流值
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
    u8FreqSet = 0;                //能需清零
    fgModeChangeStop = 1;         //模式切换停机
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
    u8FreqSet = 0;            //能需清零
    fgModeChangeStop = 1;     //模式切换停机
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
    if((u8ErrHoldTim >= ERR_RST_TIM) || (u16CompStopTim >= MINUTE_MACRO(4)))   //尝试解决T3高温保护,内机保护继电器乱动的故障
    {
      fgProtCoolT2 = 0;
      fgProtCoolT3 = 0;
    }
    return;
  }

  if(0 == unCompRunFlgs2.bits.FirstInCool)
  {
    unCompRunFlgs2.bits.FirstInCool = 1;

    u8FreqSet = 0;          //停止压缩机
    u8EnergyFreq = 0;
    fgModeChangeStop = 1;   //模式切换停机

    u8T2Prot5s = 250;
    u8T2Freq60s = 250;
    u8T3Prot5s = 250;       //进入制冷,可以立刻判断一次 避免压缩机开了,再保护关掉
    u8T3Freq60s = 250;

    u8T1_TsArea = 0;        //首次,上升沿判断
    u8MaxFreqArea = 0;      //首次,上升沿判断
    u8CurrArea = 0;

    unFreqLimFlgs.bits.CurrLim = 0;
    unCompRunFlgs2.bits.ErrT2Lim = 0;
    fgT1TsStop = 0;             //判断完毕,就取消达温停机标志
    u8T4MinFreqLim = 0;
    u8T2ErrCompRunTim = 3;
    return;
  }

  LimitMaxFreCool();            //最大频率限制
  ProtT2Cool();
  ProtT3Cool();

  //temp_diff = u8TempT1 + 3;   //按照制冷能需规则, 计算得到温差区间:+3是T1能需计算时将负数拉回坐标轴
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
    u8MaxFreqArea = 0;          //首次,上升沿判断(抽湿转制冷时,重新判断)
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
      u8T1_TsArea = 0;                              //压缩机保护期间,强制区间为0,上升沿趋势
    }
    //else if(0xFF != u8InRateFreq)                 //内机能力测试
    //{
    //  u8FreqSet = 20;
    //  u8MaxFreqLim = 20;
    //}
    //else if(SeerTestNo != u8InRateTest) 
    //{
    //  SEERFreqDeal();                             //能力测试模式,根据工况单独频率控制
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
    else if (unInSta.bits.FastTest)                //商检跑固定的58Hz,注意E方参数改变带来的影响
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
    else if (COMP_STOP == u8CompSta)             //压缩机停止->启动
    {
      u8FreqAreaT3Min = 0;
      if((u8T1_TsArea < 2) || (unFreqLimFlgs.u8Byte & 0xFE)) //限频和达不到温度,不允许启动, 20200513, Leo
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
      SeerFreqDeal();                         //能力测试模式,根据工况单独频率控制
    }
    else if(C_ModeForce == u8InSetMode)       //强制制冷跑F2,不受限频影响
    {
      u8FreqSet = 2;
    }
    else
    {
      CalFreq();                              //计算理论运行频率
    }
  }

  if(u8FreqSet > u8MaxFreqLim)                //各种限频对抽湿也有效
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
      //制热
      pU8TabChkAddr = &EeSysPara.Elem.HEAT_TEST_FREQ_A;    //contine addr
      u8SeerFreq = *(pU8TabChkAddr + u8InRateTest - 1);
      //u8EnergyFreq = u8SeerFreq>>1;
      //u8TargetFreq = u8SeerFreq >> 1;
    }
    else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
    {
      //制冷
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
  * @param  : u8T1_TsArea, 当前运行区间 A=9 B=8 C=7 D=6 E=5 F=4 G=3 H=2 I=1 J=0
  * @retval : 
  */
void CalFreq(void)
{
  unsigned char add_grade, trg_grade;
  unsigned char curr_freq;
  unsigned char *pEepromFreq;
  unsigned char tempset_middle;

  trg_grade = u8RunFreqGrade;                //u8RunFreqGrade:当前运行频率等级
  pEepromFreq = &TabRunFreq[trg_grade];      //以当前频率档为基准,查找下一阶段的频率点

  if(u8T1_TsArea > u8T1_TsAreaPre)           //大于前一个温差区间
  {
    add_grade = 1;
    u8FreqAreaT3Min = 0;

INCFREQ:                                     //往上加档
    if(0 == (unFreqLimFlgs.u8Byte & 0xFE ))  //保证无限频才能升频
    {
      while(add_grade)
      {
        curr_freq = *pEepromFreq;
        while(curr_freq == *pEepromFreq)
        {
          if(trg_grade >= COMP_MAXFREQTAP)   //最多25档
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
  else if(u8T1_TsArea < u8T1_TsAreaPre)   //小于前一个温差区间
  {
    u8FreqAreaT3Min = 0;
    
DECFREQ:                                  //往下减档
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
  else                                   //等于前一个温差区间
  {
    if(u8FreqAreaT3Min >= (3 * 60))      //每3min更新一次温差区间
    {
      u8FreqAreaT3Min = 0;

      if(u8T1_TsArea == 9)               //A 增3档
      {
        add_grade = 3;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea >= 5)          //b-e 增2档
      {
        add_grade = 2;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea >= 3)          // f-g 增1档
      {
        add_grade = 1;
        u8F1RunT60Min = 0;
        goto INCFREQ;
      }
      else if(u8T1_TsArea == 2)   //H 保持不变
      {
        u8F1RunT60Min = 0;
      }
      else if(u8T1_TsArea == 1)   //I 下降一档
      {
        u8F1RunT60Min = 0;
        goto DECFREQ;
      }
      else if(u8T1_TsArea == 0)   //J 以F1运行一个小时或者满足T1-TS条件,则压缩机关闭
      {
        u8FreqSet = 1;
        //tempset_middle = TEMP_MACRO(u8InSetTemp); 
        
        if(u8F1RunT60Min >= 60)                   //1min++, 强制1小时停机
        {
          u8FreqSet = 0;
          fgT1TsStop = 1;
        }

        if(u8InSetMode == C_ModeHeat)
        {
          tempset_middle = TEMP_MACRO(u8InSetTemp + HEAT_DLT_TEMP);
          if(u8TempT1 > (tempset_middle + 2 + 2))  // 2*2, 达温停机
          {
            u8FreqSet = 0;
            fgT1TsStop = 1;
          }
        }
        else
        {
          tempset_middle = TEMP_MACRO(u8InSetTemp);
          if((u8TempT1 + 2 + 2) < tempset_middle)  // 2*2, 达温停机
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

  u8T1_TsAreaPre = u8T1_TsArea;               //判断结束,刷新能需区间
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

  //if(C_ModeHeat != u8InSetMode)           //非制热模式,将T2高温保护标志清除
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

  if(0 == unCompRunFlgs2.bits.FirstInHeat)  //第一次进入制热模式,先找到启动频率点
  {
    unCompRunFlgs2.bits.FirstInHeat = 1;

    u8FreqSet = 0;                  //停止压缩机
    u8EnergyFreq = 0;
    fgModeChangeStop = 1;           //模式切换停机

    u8T2Prot5s = 250;
    u8T2Freq60s = 250;
    u8T2Lim180s = 0;
    u8T4MinFreqLim = 0;

    u8CurrArea = 0;
    u8MaxFreqArea = 0;              //首次,上升沿判断
    u8T1_TsArea = 0;
    u8T1_TsAreaH = 9;               //首次制热,下降沿判断

    fgT1TsStop = 0;                 //判断完毕,就取消达温停机标志
    unFreqLimFlgs.bits.CurrLim = 0;
    unCompRunFlgs2.bits.ErrT2Lim = 0;
    return;
  }

  if((SeerTestMax != u8InRateTest) && (C_ModeForceHeat != u8InSetMode))   //低温制热模式T4限频无效
  {
    LimitMaxFreHeat();                      //制热最大频率限制
  }
  
  ProtT2Heat();

  if(SeerTestNo != u8InRateTest)            //化霜进入时间处理: 分测试模式和正常模式
  {
    defrost_in_tim = u8DefrostInRateT;
  }
  else
  {
    defrost_in_tim = u8DefrostInT;
  }

  if(unCompRunFlgs2.bits.DefrostExit)       //化霜退出
  {
    if(SeerTestNo != u8InRateTest)          //化霜进入时间处理: 分测试模式和正常模式
    {
      u8FreqSet = u8FreqDefrostTest2;
    }
    else
    {
      u8FreqSet = 0;
    }

    //u8FreqSet = 0;                        //化霜退出停压缩机
    unFlgs.bits.OutFanCtr = 1;              //室外风机开启
    if(u8DefrostDly3Min > u8DefrostExitT)
    {
      unFlgs.bits.FWVCtr = 1;
    }

    return;
  }
  else if(unFlgs2.bits.Defrost)             //化霜过程
  {
    fgProtHeatT2 = 0;
    unFreqLimFlgs.u8Byte = 0;

    if(u16InDefrostTim < (defrost_in_tim + 10))
    {
      if(SeerTestNo != u8InRateTest)        //化霜进入时间处理: 分测试模式和正常模式
      {
        u8FreqSet = u8FreqDefrostTest1;
      }
      else
      {
        u8FreqSet = 0;
      }

      //u8FreqSet = 0;                      //进入化霜前停压缩机
      if(u16InDefrostTim >= (defrost_in_tim + 8))
      {
        u16CompStopTim = 250;
      }
    }
    else if(u16CompStopTim < MINUTE_MACRO(4))
    {
      u8FreqSet = 0;
    }
    else if(unFreqProt.u8Byte)              //化霜过程出现故障保护标志
    {
      u8FreqSet = 0;
    }
    else if(SeerTestNo != u8InRateTest)
    {
      u8FreqSet = u8FreqDefrostTest;        //化霜频率使用制冷最大允许频率(能力测试)
    }
    else
    {
      u8FreqSet = u8FreDefrost; //u8T4MaxCoolFreq4; //化霜频率使用制冷最大允许频率  
    }

    if(u16InDefrostTim > (defrost_in_tim))  //降到F3,持续5秒,则变换四通阀和外风机
    {
      unCompRunFlgs2.bits.DefrostIn = 1;
      unFlgs.bits.OutFanCtr = 0;
      unFlgs.bits.FWVCtr = 0;
    }

    return;                                 //化霜子过程不需要再计算制热能需, 直接返回
  }

  temp_diff = (u8TempT1 - HEAT_DLT_TEMP * 2 + 4 * 2); //按照制热能需规则, 计算得到温差区间:+12是硬性的补偿6℃
  temp_set_middle = TEMP_MACRO(u8InSetTemp);

  if(temp_diff > temp_set_middle)                     //务必注意u8InSetTemp变量的表达形式:60+x+x
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
  u8T1_TsArea = 9 - u8T1_TsAreaH;           //制热区间要反过来

  if(unFreqProt.u8Byte || (u16CompStopTim < MINUTE_MACRO(4)))
  {
    u8FreqSet = 0;
    u8T1_TsAreaH = 9;                       //压缩机保护期间,强制区间为0,上升沿趋势
  }
  //else if(0xFF != u8InRateFreq)
  //{
  //  u8FreqSet = 20;
  //  u8MaxFreqLim = 20;
  //}
  else if(unInSta.bits.FastTest)            //商检跑固定的58Hz,注意E方参数改变带来的影响
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
    if((u8T1_TsArea < 1) || (unFreqLimFlgs.u8Byte & 0xFE ))   //限频和达不到温度,不允许启动, 20200513, Leo
    {
      u8FreqSet = 0;
      u8T1_TsArea = 0;                                       //压缩机保护期间,强制区间为0,上升沿趋势
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
    SeerFreqDeal();                         //能力测试模式,根据工况单独频率控制
  }
  else if(C_ModeForceHeat == u8InSetMode)   //强制制冷跑F4,不受限频影响
  {
    u8FreqSet = 4;
  }
  else
  {
    CalFreq();
  }

  if(u8FreqSet > u8MaxFreqLim)  //计算出来的能需等级高于最大限制频率
  {
    u8FreqSet = u8MaxFreqLim;
    if(0 == u8MaxFreqLim)
    {
      fgT4LimStop = 1;
    }
  }
}

/**
  * @brief  : 制热最大频率限制
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
      if(u8TempT1 >= u8T1F25LmtTemp)            //须注意E方参数u8T1f25的数据格式
      {
        u8MaxFreqLim = 24;
      }
    }

    if(24 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F24LmtTemp)            //须注意E方参数u8T1f24的数据格式
      {
        u8MaxFreqLim = 23;
      }
    }

    if(23 == u8MaxFreqLim)
    {
      if(u8TempT1 >= u8T1F23LmtTemp)            //须注意E方参数u8T1f23的数据格式
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

    if( (C_InFanMM == u8InFanMode) || (C_InFanM == u8InFanMode) )      //中风限频
    {
      if(u8MaxFreqLim > u8T4MidLimHeatFreq1)
      {
        u8MaxFreqLim = u8T4MidLimHeatFreq1;
      }
    }

    if( (C_InFanSL == u8InFanMode) || (C_InFanL == u8InFanMode) )     //低风限频
    {
      if(u8MaxFreqLim > u8T4LowLimHeatFreq2)
      {
        u8MaxFreqLim = u8T4LowLimHeatFreq2;
      }
    }
  }
  //if(C_InFanSL == u8InFanMode)      //静音 限频F3 ,风速不是高风,本身已经屏蔽了特定工况运行了
  //{
  //    unCompRunFlgs2.bits.NoPlat = 1;         //取消平台

  //  if(u8MaxFreqLim > 3)
  //    {
  //    u8MaxFreqLim = 3;
  //    }

  //    if(unCompRunFlgs2.bits.RetOil)
  //    {
  //    unCompRunFlgs2.bits.RetOil = 0;       //取消回油
  //    u16ROLowFreqTim = u16RetOilTim1;
  //    }
  //}

  if(fgErrT2)                           //T2传感器故障
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
//unsigned char const TabT1_Ts_Dry[]= {0+3, 1+3, 2+3, 3+3, 4+3, 5+3, 255, 255};       //除湿温差区间
unsigned char const TabT1_Ts_Dry[] = 
{
0 + 3 + 1, 
1 + 3 + 1, 
2 + 3 + 1, 
3 + 3 + 1, 
4 + 3 + 1, 
5 + 3 + 1, 
255, 255}; //除湿温差区间
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
    if(u8TempT1 < TEMP_MACRO(10))   //T1传感器保护,小于10°C
    {
      fgProtT1 = 1;
    }
  }
  else
  {
    if(u8TempT1 >= TEMP_MACRO(12))  //T1传感器恢复,大于12°C
    {
      fgProtT1 = 0;
    }
  }

  if(unFreqProt.u8Byte || (u16CompStopTim < MINUTE_MACRO(4)))
  {
    //有限频故障,停止能需输出
    u8FreqSet = 0;
    u8T1_TsArea = 0;               //压缩机保护期间,强制区间为0,上升沿趋势
  }
  else
  {
    if(0 == (unFreqLimFlgs.u8Byte & 0xFE ))
    {
      u8FreqSet = TabFreqDry[u8T1_TsArea];
    }
  }

  //if(C_InFanSL == u8InFanMode)     //静音模式限频
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

  //if(C_InFanSL == u8InFanMode) //静音 限频F3
  //{
  //  if(u8MaxFreqLim > 3)
  //  {
  //    u8MaxFreqLim = 3;
  //  }
  //}

  if(fgErrT2)                    //T2传感器故障
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
      if(u8T2ErrCompRunTim > 33)      //30分钟
      {
        u8T2ErrCompRunTim = 0;
        u8MaxFreqLim = 0;
      }
    }
    else
    {
      if(u8T2ErrCompRunTim >= 3)     //3分钟
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

  if(unTempErrFlgs.bits.T3Err)      //T3传感器故障
  {
    if(u8MaxFreqLim > 14)
    {
      u8MaxFreqLim = 14;
    }
  }
}

/**
  * @brief  : 四通阀控制
  * @param  : 
  * @retval : 
  */
void LoadControl(void)
{
  if(unFlgs2.bits.Defrost || unCompRunFlgs2.bits.DefrostExit) //化霜期间独立控制
  {
    u8ValveDlyTim = 0;
    return;
  }

  //外风机控制
  if(COMP_RUN == u8CompSta)
  {
    u8StopFanDly = 0;
    unFlgs.bits.OutFanCtr = 1; //压缩机启动,外风机同时启动
  }
  else
  {
    if( (u8StopFanDly > 30)||(unInSta.bits.FastTest))
    {
      unFlgs.bits.OutFanCtr = 0;
    }
  }

  //四通阀控制
  if(fgProtErr)
  {
    unFlgs.bits.FWVCtr = 0;  //立刻关闭四通阀,故障
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
      //制热压缩机4分钟保护可选
      u16CompStopTim = 0;
    }
    else
    {
      u16CompStopTim = 60;
    }

    ErrDisplay();
  }

  u8LimitFreq = 0;                  //频率清零
  u8LastTrgFreq = 0;
  u8RunCtrFreq = 0;
  u8RealRunFreq = 0;

  u16CompRunTim = 0;
  u8CompDly = 115;
  u8CompSta = COMP_STOP;

  unFlgs1.bits.Plat0RunOk = 0;      //清零3个启动平台
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
  if(0 == u16CompSpd)                   //频率控制调整时间,控制升降频曲线 100ms--
  {
    u8LastTrgFreq = u8LimitFreq;        //上一状态的运行频率, 限频已在外部做好

    if( (SeerTestNo != u8InRateTest)   ||
        (C_ModeDefrost == u8InSetMode) ||
        unFlgs2.bits.Defrost)           //能力测试或者化霜
    {
      u8MinRunFreq = u8MinFreq;         //u8MinFreq:Eeprom写入的最小运行频率
    }

    if(u8LastTrgFreq < u8MinRunFreq)      //小于最小运行频率
    {
      if(u8DgTrgHz < u8MinFreq)          //调试板目标频率小于程序规定的最小频率
      {
        u8LastTrgFreq = u8MinRunFreq;     //小于最小频率,则按照最小频率工作,不停机
      }
    }//因此当自由运行时频率小于E方规定的最小频率且调试板频率小于程序规定的最小频率那么才会停机。E方的最小频率只对自由跑的时候有效。

    if(u8MinForceFreq && (SeerTestNo == u8InRateTest))      //特定工况,强制跑频率10分钟,能力测试时无效
    {
      u8LastTrgFreq = u8MinForceFreq;
    }

    if(u8LastTrgFreq)                                       //升降频处理
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

      //升降频速率设定, 根据不同的平台从Eeprom选择不同的等待时间
      if(u8RunCtrFreq < u8PlatFreq0)         //小于第0平台 30Hz
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
      else if(u8RunCtrFreq == u8PlatFreq0)               //等于第0平台 30Hz
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
      else if(u8RunCtrFreq < u8PlatFreq1)        //小于第1平台 42Hz
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
      else if(u8RunCtrFreq == u8PlatFreq1)       //等于第1平台 42Hz
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
      else if(u8RunCtrFreq < u8PlatFreq2)        //小于第2平台 62Hz
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
      else if(u8RunCtrFreq == u8PlatFreq2)       //等于第2平台 62Hz
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
      else                        //平台结束,正常运行目标频率
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
  * @brief  : 通过频率序列  得到实际运行频率值
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
  //if((0xFF != u8InRateFreq)&&(0 == unFlgs2.bits.Defrost)&&(0 == unCompRunFlgs2.bits.DefrostExit))  //化霜时,不锁定频率
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

