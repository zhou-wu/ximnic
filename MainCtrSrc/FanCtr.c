#include "UsrDef.h"
#include "FanCtr.h"
#include "CompCtr.h"
#include "TempSample.h"
#include "InOutComm.h"
#include "TimeTick.h"
#include "ParameterPro.h"
#include "MiscFunc.h"

static void CoolDCFAN_5Dang(void);
static void HeatDCFAN_5Dang(void);
static void FanDrv(void);
static unsigned short Pdiv16(unsigned short mdata);

unsigned char u8ODOutFan10sTim;
unsigned short u16ODTwoHourTim;
unsigned short u16DgFanSpd;
unsigned char u8DownSpdTim;
unsigned short u16TrgFanSpd;
unsigned short u16FanFBSpd;

const unsigned char TAB_CoolDCFAN[] =
{
  TEMP_MACRO(17), 
  TEMP_MACRO(19), 
  TEMP_MACRO(20), 
  TEMP_MACRO(22),
  TEMP_MACRO(23), 
  TEMP_MACRO(25), 
  TEMP_MACRO(26), 
  TEMP_MACRO(28), 
  255, 
  255
};

const unsigned char TAB_HeatDCFAN[] =
{
  TEMP_MACRO(10), 
  TEMP_MACRO(12), 
  TEMP_MACRO(13), 
  TEMP_MACRO(15),
  TEMP_MACRO(16), 
  TEMP_MACRO(18), 
  TEMP_MACRO(19), 
  TEMP_MACRO(21), 
  255, 
  255
};

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void FanInit(void)
{
  if (unSeltFlgs1.bits.FanSelt)
  {
    //直流风机
    u16ODTwoHourTim = 0;
    u8ODOutFan10sTim = 0;
    unOutDustFlgs01.u8Byte = 0x10;
  }
  else
  {
    //交流风机
    //AC_FAN_HI_PORT_OUT;
    //AC_FAN_HI_OFF;
    //AC_FAN_LOW_PORT_OUT;
    //AC_FAN_LOW_OFF;
  }

  u8TrgFan = FanOFF;
  u16DgFanSpd = 0;
  u16ZGInCtrFanSpd = 0;
  //u8Initfancam = 0xfe;
  u8TrgFanBak = FanOFF;
  u8ACFanSwhDly = 0;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void FanCtr(void)
{
  /*
  if (u8InOutCommErrTim >= (60+30))
  {//common err process

    u8TrgFan = FanOFF;
    u8DownSpdTim = TIME1S_150S;
  }
  */

  if(u8PowerOnDly < 32)         //电容未充满,不允许输出
  {
    u8TrgFan = FanOFF;
  }
  else if ((u16DgFanSpd <= DC_FAN_DBG_MAX_SPD) && (u16DgFanSpd >= DC_FAN_DBG_MIN_SPD) && (unFlgs2.bits.Defrost == 0))
  {
    //小板调试优先于主控逻辑; 200rpm<调试板设定风速<1200rpm
    if(unSeltFlgs1.bits.FanSelt)
    {
      //DC FAN
      u8DownSpdTim = TIME1S_150S;
      u8TrgFan = FanBug;        //直流风机先置位调试标志, 然后去Eeprom取实际转速
    }
    else
    {
      //AC FAN
      u8DownSpdTim = TIME1S_150S;

      //交流风机直接将小板的转速转化成固定的风挡
      if (u16DgFanSpd >= 800)
      {
        //设定风速>=800,高风  // >=3
        u8TrgFan = FanH;  //FanH=4
      }
      else if (u16DgFanSpd >= 500)
      {
        //设定风速>=500,中风  //2
        u8TrgFan = FanM;  //FanM=3
      }
      else
      {
        //设定风速>=350,低风  //1
        u8TrgFan = FanL;  //FanL=2
      }
    }
  }
  else  //主控逻辑接管外风机
  {
    if(unSysErrFlgs.bits.InOutCommErr)
    {
      if((u8DgTrgHz >= u8MinFreq) && (0 == unSysErrFlgs.bits.I2cErr))
      {
        //内机通信故障时有调试频率且外机Eeprom无故障则强制为高风
        u8TrgFan = FanH;
      }
      else
      {
        u8TrgFan = FanOFF;
        u8DCFanArea = 0;
      }
    }
    else
    {
      if(1 == unFlgs.bits.OutFanCtr)
      {
        //外风机控制跟随压缩机,具体执行根据外环温判断
        if(0 == unTempErrFlgs.bits.T4Err)
        {
          //T4外环温正常,外风机按逻辑控制
          if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
          {
            if(unSeltFlgs1.bits.FanSelt)
            {
              //DC FAN
              CoolDCFAN_5Dang();
              if(u8DCFanArea >= 5)
              {
                u8DCFanArea = 5;
              }
              u8TrgFan = 5 - u8DCFanArea;
            }
            else
            {
              //AC FAN
              //AC fan, T4
              if(FanH == u8TrgFan)
              {
                //高风
                if(u8TempT4 < TEMP_MACRO(26))
                {
                  u8TrgFan = FanM;
                }
              }
              else if(FanM == u8TrgFan)
              {
                //中风
                if(u8TempT4 >= TEMP_MACRO(28))
                {
                  u8TrgFan = FanH;
                }
                else if(u8TempT4 < TEMP_MACRO(20))
                {
                  u8TrgFan = FanL;
                }
              }
              else if(FanL == u8TrgFan)
              {
                //低风
                if(u8TempT4 >= TEMP_MACRO(22))
                {
                  u8TrgFan = FanM;
                }
              }
              else
              {
                //初始状态
                if(u8TempT4 >= TEMP_MACRO(28))
                {
                  u8TrgFan = FanH;
                }
                else if(u8TempT4 >= TEMP_MACRO(22))
                {
                  u8TrgFan = FanM;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
            }

            if((1 == unSeltFlgs1.bits.LCoolSelt) && (0 == unTempErrFlgs.bits.T3Err))
            {
              //低温制冷:可选功能,默认开启
              if(u8TempT4 <= TEMP_MACRO(15))
              {
                //室外温度小于等于15度且压缩机运行,自动进入低温制冷功能
                if(COMP_RUN == u8CompSta)
                {
                  u8LowCoolTim = 60;
                  unDevFlgs.bits.LowCoolFanEn = 1;
                }
              }
              else if(u8TempT3 >= TEMP_MACRO(20))
              {
                //T4>15℃且T3>=20℃
                if(0 == u8LowCoolTim)
                {
                  //持续1min,退出低温制冷
                  unDevFlgs.bits.LowCoolFanEn = 0;
                }
              }
              else
              {
                u8LowCoolTim = 60;
              }

              //低温制冷,T3控制外风机
              if(unDevFlgs.bits.LowCoolFanEn)
              {
                if(FanOFF != u8TrgFanBak)
                {
                  if(u8TempT3 < TEMP_MACRO(0))
                  {
                    u8TrgFan = FanOFF;
                    u8TrgFanBak = u8TrgFan;
                  }
                  else
                  {
                    u8TrgFanBak = u8TrgFan;
                  }
                }
                else
                {
                  if(u8TempT3 >= TEMP_MACRO(20))
                  {
                    u8TrgFanBak = u8TrgFan;
                  }
                  else
                  {
                    u8TrgFan = FanOFF;
                    u8TrgFanBak = u8TrgFan;
                  }
                }
              }
            }

            //=============================down is seer test===============================//
            if(unSeltFlgs1.bits.FanSelt)
            {
              //DC FAN:能力测试,直流风机按对应风挡对应的转速输出
              if(SeerTestMin == u8InRateTest)
              {
                u8TrgFan = FanCMIN;
              }
              else if(SeerTestMid == u8InRateTest)
              {
                u8TrgFan = FanCMID;
              }
              else if(SeerTestRat == u8InRateTest)
              {
                u8TrgFan = FanCRAT;
              }
              else if(SeerTestMax == u8InRateTest)
              {
                u8TrgFan = FanCMAX;
              }
              else if(SeerTest5 == u8InRateTest)
              {
                u8TrgFan = FanC5;
              }
              else if(SeerTest6 == u8InRateTest)
              {
                u8TrgFan = FanC6;
              }
              else if(SeerTest7 == u8InRateTest)
              {
                u8TrgFan = FanC7;
              }
              else
              {
                ;
              }
            }
            else
            {
              //所有能力测试,交流风机强制高风输出
              //if(SeerTestNO != u8InRateTest)
              //{
              //  u8TrgFan = FanH;
              //}
              if(SeerTestMin == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanA)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestMid == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanB)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestRat == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanC)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestMax == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanD)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest5 == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanE)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest6 == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanF)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest7 == u8InRateTest)
              {
                if(unSeltFlgs4.bits.CTestACFanG)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else
              {
                ;
              }
            }
            //=============================up is seer test===============================//
          }
          else if ((u8InSetMode == C_ModeHeat) || (C_ModeForceHeat == u8InSetMode) )  /// Leo
          {
            //heat
            //add  end
            if (unSeltFlgs1.bits.FanSelt)
            {
              //DC fan process
              HeatDCFAN_5Dang();
              if(u8DCFanArea >= 5)
              {
                u8DCFanArea = 5;
              }
              u8TrgFan = u8DCFanArea + 1;
            }
            else
            {
              //AC FAN HEAT MINRATI
              // AC fan, T4
              if(FanH == u8TrgFan)
              {
                if(u8TempT4 >= TEMP_MACRO(12))
                {
                  u8TrgFan = FanM;
                }
              }
              else if(FanM == u8TrgFan)
              {
                if(u8TempT4 >= TEMP_MACRO(17))
                {
                  //T4>17°,低风
                  u8TrgFan = FanL;
                }
                else if(u8TempT4 < TEMP_MACRO(10))
                {
                  //T4<10°,高风
                  u8TrgFan = FanH;
                }
              }
              else if(FanL == u8TrgFan)
              {
                //低风
                if(u8TempT4 < TEMP_MACRO(15))
                {
                  //T4<15°,中风
                  u8TrgFan = FanM;
                }
              }
              else
              {
                if(u8TempT4 > TEMP_MACRO(17))
                {
                  //T4>17°,低风
                  u8TrgFan = FanL;
                }
                else if(u8TempT4 > TEMP_MACRO(12))
                {
                  //T4>12°,中风
                  u8TrgFan = FanM;
                }
                else
                {
                  //高风
                  u8TrgFan = FanH;
                }
              }
            }//end of AC FAN
            
            //=============================down is seer test===============================//
            if(unSeltFlgs1.bits.FanSelt)
            {
              //DC FAN:能力测试直流风机按具体风挡对应的转速输出
              if(SeerTestMin == u8InRateTest)
              {
                u8TrgFan = FanHMIN;
              }
              else if(SeerTestMid == u8InRateTest)
              {
                u8TrgFan = FanHMID;
              }
              else if(SeerTestRat == u8InRateTest)
              {
                u8TrgFan = FanHRAT;
              }
              else if(SeerTestMax == u8InRateTest)
              {
                u8TrgFan = FanHMAX;
              }
              else if(SeerTest5 == u8InRateTest)
              {
                u8TrgFan = FanH5;
              }
              else if(SeerTest6 == u8InRateTest)
              {
                u8TrgFan = FanH6;
              }
              else if(SeerTest7 == u8InRateTest)
              {
                u8TrgFan = FanH7;
              }
              else
              {
                ;
              }
            }
            else
            {
              //所有能力测试,交流风机强制高风输出
              //if(SeerTestNO != u8InRateTest)
              //{
              //  u8TrgFan = FanH;
              //}
              if(SeerTestMin == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanA)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestMid == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanB)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestRat == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanC)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTestMax == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanD)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest5 == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanE)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest6 == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanF)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else if(SeerTest7 == u8InRateTest)
              {
                if(unSeltFlgs5.bits.HTestACFanG)
                {
                  u8TrgFan = FanH;
                }
                else
                {
                  u8TrgFan = FanL;
                }
              }
              else
              {
                ;
              }
            }
            //=============================up is seer test===============================//
          }
        }
        else
        {
          //室外温度传感器T4故障吹高风
          u8TrgFan = FanH;
        }
      }
      else                                //外风机标志无效,判断调试小板是否有压缩机频率
      {
        if((u8DgTrgHz >= u8MinFreq) && (0 == unSysErrFlgs.bits.I2cErr))
        {
          u8TrgFan = FanH;                //有调试频率且外机Eeprom无故障则强制为高风
        }
        else
        {
          u8TrgFan = FanOFF;
          u8DCFanArea = 0;
        }
      }//end of no FAN
    }//end of no comErr
    //}
  }

  //停机退出低温制冷功能
  if(COMP_STOP == u8CompSta)
  {
    u8LowCoolTim = 60;
    unDevFlgs.bits.LowCoolFanEn = 0;
  }

  //AC FAC:一档或两档风输出,中间停顿1s
  //FanDelay();

  //风机逻辑已处理好,按照交直流两个版本输出
  FanDrv();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void CoolDCFAN_5Dang(void)
{
  u8DCFanArea = AreaCompare(u8TempT4, TAB_CoolDCFAN, u8DCFanArea);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void HeatDCFAN_5Dang(void)
{
  u8DCFanArea = AreaCompare(u8TempT4, TAB_HeatDCFAN, u8DCFanArea);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void FanDrv(void)
{
  if (unInSta.bits.SelfChk )
  {
    return; 
  }
  
  if (unSeltFlgs1.bits.FanSelt)
  {
    //DC FAN:根据风挡标志读取Eeprom 对应的转速

    if(FanH == u8TrgFan)
    {
      //1
      u16TrgFanSpd = u16FanHighSpd;
    }
    else if(FanM == u8TrgFan)
    {
      //2
      u16TrgFanSpd = u16FanMidSpd;
    }
    else if(FanL == u8TrgFan)
    {
      //3
      u16TrgFanSpd = u16FanLowSpd;
    }
    else if(FanSL == u8TrgFan)
    {
      //4
      u16TrgFanSpd = u16FanSLowSpd;
    }
    else if(FanSSL == u8TrgFan)
    {
      //5
      u16TrgFanSpd = u16FanSSLowSpd;
    }
    else if (FanBug == u8TrgFan)
    {
      //6
      u16TrgFanSpd = Pdiv16(u16DgFanSpd);
    }
    //=============================down is seer test===============================//
    else if(FanHMIN == u8TrgFan)
    {
      //7
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_MIN_RATI_SPDH);
    }
    else if(FanHMID == u8TrgFan)
    {
      //8
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_MID_RATI_SPDH);
    }
    else if(FanHRAT == u8TrgFan)
    {
      //9
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_RATI_SPDH);
    }
    else if(FanHMAX == u8TrgFan)
    {
      //10
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_MAX_RATI_SPDH);
    }
    else if(FanH5 == u8TrgFan)
    {
      //11
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_TEST_SPD5H);
    }
    else if(FanH6 == u8TrgFan)
    {
      //12
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_TEST_SPD6H);
    }
    else if(FanH7 == u8TrgFan)
    {
      //13
      u16TrgFanSpd = BytesToSingleWord(&U8_HEAT_TEST_SPD7H);
    }
    else if(FanCMIN == u8TrgFan)
    {
      //14
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_MIN_RATI_SPDH);
    }
    else if(FanCMID == u8TrgFan)
    {
      //15
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_MID_RATI_SPDH);
    }
    else if(FanCRAT == u8TrgFan)
    {
      //16
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_RATI_SPDH);
    }
    else if(FanCMAX == u8TrgFan)
    {
      //17
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_MAX_RATI_SPDH);
    }
    else if(FanC5 == u8TrgFan)
    {
      //18
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_TEST_SPD5H);
    }
    else if(FanC6 == u8TrgFan)
    {
      //19
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_TEST_SPD6H);
    }
    else if(FanC7 == u8TrgFan)
    {
      //20
      u16TrgFanSpd = BytesToSingleWord(&U8_COOL_TEST_SPD7H);
    }
    else
    {
      u16TrgFanSpd = 0;
    }
    //=============================up is seer test===============================//

    //发送给驱动的直流风机风速,Eeprom 存储的是按照每转所经历的时间来判定直流风机的风速
    if(FanBug == u8TrgFan)
    {
      u16TrgFanSpd = u16DgFanSpd;
    }
    else
    {
      u16TrgFanSpd = Pdiv16(u16TrgFanSpd);
    }

    //直流风机故障恢复条件
    if(u8ErrHoldTim >= ERR_RST_TIM)
    {
      unSysErrFlgs.bits.DCFanSpdErr = 0;
    }

    if(unCompErrFlgs1.bits.FanCurrent || unCompErrFlgs1.bits.FanPhsLoss || unCompErrFlgs1.bits.FanZer0 || unCompErrFlgs2.bits.FanErr)
    {
      unSysErrFlgs.bits.DCFanSpdErr = 1;
    }
  }
  else//(unSeltFlgs1.bits.FanSelt == 0)
  {
    //AC FAN:一档或两档风输出,中间停顿1s
    if (unSeltFlgs1.bits.ACFanSelt)
    {
      //单档风机
      if (u8TrgFan)
      {
        //AC_FAN_HI_ON;
        u8TrgFanOK = 1;
      }
      else
      {
        //AC_FAN_HI_OFF;
        u8TrgFanOK = 0;
      }
    }
    else
    {
      //2档风速
      if(u8TrgFanOK != u8TrgFan)
      {
        u8CompDly = 0;
        u8ACFanSwhDly = 10;
        u8TrgFanOK = u8TrgFan;
      }

      //风挡切换使能,通过延时时间实现
      if((FanH == u8TrgFan) || (FanM == u8TrgFan))
      {
        //高风
        //AC_FAN_LOW_OFF;
        if(0 == u8ACFanSwhDly)
        {
          //AC_FAN_HI_ON;
        }
      }
      else if(FanL == u8TrgFan)
      {
        //低风
        //AC_FAN_HI_OFF;
        if(0 == u8ACFanSwhDly)
        {
          //AC_FAN_LOW_ON;
        }
      }
      else
      {
        //AC_FAN_HI_OFF;
        //AC_FAN_LOW_OFF;
      }
    }
  }

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned short Pdiv16(unsigned short mdata)
{
  unsigned short u16DgTmp3 = 0;
  unsigned short u16DgTmp2 = 0;

  if(0 == mdata)
  {
    return(0);
  }

  //转速 = 300000/转速设置值
  u16DgTmp3 = 30000 / mdata; // 先用30000除以转速值,得到商
  u16DgTmp3 *= 10;          // 因为是30000除,不是300000,所以商再乘以10,得到设定风速的时间
  u16DgTmp2 = 30000 % mdata; 
  u16DgTmp2 *= 10;
  u16DgTmp2 /= mdata;       // 余数*10/转速设置值
  u16DgTmp3 += u16DgTmp2;
  return(u16DgTmp3);
}

