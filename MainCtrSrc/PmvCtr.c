#include "UsrDef.h"
#include "PmvCtr.h"
#include "ParameterPro.h"
#include "MiscFunc.h"

static void StepDirect(void);
static unsigned char TempRegJdg(unsigned char u8Temp, unsigned char u8CurrReg, unsigned char *u8TabDn[], unsigned char *u8TabUp[], unsigned char u8Len);
static void PmvInitPls(void);

union UNION_PMVFASTDIR_FLAG unPmvFastDirFlgs;
unsigned char u8TrgDiscPipeTemp;
unsigned char u8PMVCoolT5Zone, u8PMVHeatT5Zone, u8PMVCoolT5ZoneTmp, u8PMVHeatT5ZoneTmp;
unsigned char *u8PMVCoolT5RegTabDn[U8_PMV_COOL_T5_ZONE_LEN] =
{
  //TempReg
  {&u8CoolDntemp1},   //0
  {&u8CoolDntemp2},   //1
  {&u8CoolDntemp3},   //2
  {&u8CoolDntemp4},   //3
  {&u8CoolDntemp5},   //4
  {&u8CoolDntemp6},   //5
  {&u8CoolDntemp7},   //6
  {&u8CoolDntemp8},   //7
  {&u8CoolDntemp9}    //8
};
unsigned char *u8PMVCoolT5RegTabUp[U8_PMV_COOL_T5_ZONE_LEN] =
{
  //TempReg
  {&u8CoolUptemp1},   //0
  {&u8CoolUptemp2},   //1
  {&u8CoolUptemp3},   //2
  {&u8CoolUptemp4},   //3
  {&u8CoolUptemp5},   //4
  {&u8CoolUptemp6},   //5
  {&u8CoolUptemp7},   //6
  {&u8CoolUptemp8},   //7
  {&u8CoolUptemp9}    //8
};
unsigned char *u8PMVHeatT5RegTabDn[U8_PMV_HEAT_T5_ZONE_LEN] =
{
  //TempReg
  {&u8HeatDntemp1},   //0
  {&u8HeatDntemp2},   //1
  {&u8HeatDntemp3},   //2
  {&u8HeatDntemp4},   //3
  {&u8HeatDntemp5},   //4
  {&u8HeatDntemp6},   //5
  {&u8HeatDntemp7},   //6
  {&u8HeatDntemp8},   //7
  {&u8HeatDntemp9}    //8
};
unsigned char *u8PMVHeatT5RegTabUp[U8_PMV_HEAT_T5_ZONE_LEN] =
{
  //TempReg
  {&u8HeatUptemp1},   //0
  {&u8HeatUptemp2},   //1
  {&u8HeatUptemp3},   //2
  {&u8HeatUptemp4},   //3
  {&u8HeatUptemp5},   //4
  {&u8HeatUptemp6},   //5
  {&u8HeatUptemp7},   //6
  {&u8HeatUptemp8},   //7
  {&u8HeatUptemp9}    //8
};

extern const unsigned char TabPmv[];
extern const unsigned char TabPmvDircet[];

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void PmvInit(void)
{
  if (unSeltFlgs1.bits.PMVSelt)
  {
    PMV_PORT_OUT;
    StepDrv(0x00);
    u8StepTim = STEP_PPS;           //32ms励磁周期
    u16DeltaPulse = RESET_PULSE;
    u16CurrPulse = RESET_PULSE;
    u16TrgPulse = 0;
    u8PulseCnt = 7;                 //阀闭方向步进:从7到0
    unFlgs1.bits.PmvRunDir = 0;     //阀闭方向回转
    unFlgs2.bits.PmvReset = 0;
    u8StepExcitTim = STEP_START_EXCIT;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void PmvCtr(void)
{
  if (unInSta.bits.SelfChk )
  {
    unSeltFlgs1.bits.PMVSelt = 0;
    PMV_PORT_OUT;
    return;
  }
  
  if(u8TimPowerOn < 33)
  {
    PmvInit();
    return;
  }

  //2013051707 通信不上20秒再开膨胀阀
  if (unSeltFlgs1.bits.PMVSelt)       //标配电子膨胀阀
  {
    if (u8StepTim >= STEP_PPS)        //励磁速度计时1ms++, 32ms
    {
      u8StepTim = 0;
      if (0 != u16DeltaPulse)         //当需要的步进数还没有到时,不判断目标开度和当前开度的差值
      {
        if (u8StepExcitTim == 0)      //膨胀阀开始动作,先在上次动作的停止相(当前起始相)励磁0.5s
        {
          StepDirect();
        }

        //根据硬件连接执行电子膨胀阀输出,驱动对应IO口电平,PmvCfgDir参数由Eeprom加载
        if(0 == unSeltFlgs2.bits.PmvCfgDir)
        {
          //正向,硬件按IO顺序连接
          StepDrv(TabPmv[u8PulseCnt]);
        }
        else
        {
          //反向,硬件按IO逆序连接
          StepDrv(TabPmvDircet[u8PulseCnt]);
        }
      }
      else          //执行到与目标开度相同,重新判断是否更新目标开度并计算差值
      {
        if (0 == u8StepExcitTim)  //膨胀阀在结束相保持励磁0.5s完成,计算下一次动作的脉冲数
        {
          //0.5s励磁时间到
          StepDrv(0x00);  //drv set off //膨胀阀本次动作完成,驱动端口置高电平

          //目标开度计算:u16TrgPulse = 化霜开度,或小板调试开度,或主程序控制开度
          if(unFlgs2.bits.Defrost || (C_ModeDefrost == u8InSetMode)) //化霜优先级高于锁频
          {
            //化霜
            u16TrgPulse = u16DefrostPulse;
            unFlgs2.bits.PmvReset = 1;        //化霜时电子膨胀阀先复位再开到化霜开度
          }
          else if (u16DgTrgStepPls)
          {
            //小板控制
            u16TrgPulse = u16DgTrgStepPls;
          }
          else
          {
            if(COMP_STOP == u8CompSta)
            {
              //停机
              if(unFlgs2.bits.PmvReset)
              {
                unFlgs2.bits.PmvReset = 0;
                u16TrgPulse = 0;       //压机关闭时有复位要求,则将目标开度设为0即可
              }
              else
              {
                u16TrgPulse = DEFAULT_PULSE;     //复位完成后走到默认开度
              }
            }
            else
            {
              //开机
              //=============================down is seer test===============================//
              if(u8InRatePMV != 0xFF)
              {
                //新锁频,内机或者显示板锁频
                u16TrgPulse = u8InRatePMV;
                u16TrgPulse = u16TrgPulse << 3;
              }
              else if(SeerTestNo != u8InRateTest)
              {
                if(u8InRateTest == SeerTestMin)
                {
                  //最小能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MIN_RATI_PLSH); //u16HeatSEERMinPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MIN_RATI_PLSH); //u16CoolSEERMinPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestMid)
                {
                  //中间能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MID_RATI_PLSH); //u16HeatSEERMidPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MID_RATI_PLSH); //u16CoolSEERMidPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestRat)
                {
                  //额定能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_RATI_PLSH);   //u16HeatRatPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_RATI_PLSH);   //u16CoolRatPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestMax)
                {
                  //最大能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MAX_RATI_PLSH); //u16HeatSEERMaxPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MAX_RATI_PLSH); //u16CoolSEERMaxPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest5)//第5锁频
                {
                  //5能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS5H);  //u16HeatSEER5Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS5H);  //u16CoolSEER5Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest6)//第6锁频
                {
                  //6能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS6H);  //u16HeatSEER6Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS6H);  //u16CoolSEER6Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest7)//第7锁频
                {
                  //7能力测试
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //制热
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS7H);  // u16HeatSEER7Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //制冷,除湿,强制制冷
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS7H);  //u16CoolSEER7Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
              }
              //=============================up is seer test===============================//
              else
              {
                unFlgs2.bits.PmvReset = 1;             //每次压机停止后都复位电子膨胀阀
                if(u16CompRunTim >=  MINUTE_MACRO(3))  //开机3分钟后根据排气温度控制膨胀阀开度
                {
                  if(u8PmvRegulateTim >= PMV_REG)      //每2分钟修正一次
                  {
                    u8PmvRegulateTim = 0;
                    if(0 == unTempErrFlgs.bits.T5Err)
                    {
                      if((u8InSetMode == C_ModeCool) || (u8InSetMode == C_ModeDry) || (u8InSetMode == C_ModeForce))
                      {
                        //u8PMVCoolT5Zone = TempRegJdg(u8TempTP, u8PMVCoolT5Zone, *u8PMVCoolT5RegTab, U8_PMV_COOL_T5_ZONE_LEN);
                        //u8PMVCoolT5ZoneTmp = TempRegJdg(u8TempTP, u8PMVCoolT5Zone, *u8PMVCoolT5RegTab, U8_PMV_COOL_T5_ZONE_LEN);
                        u8PMVCoolT5ZoneTmp = TempRegJdg(u8TempT5, u8PMVCoolT5Zone, u8PMVCoolT5RegTabDn, u8PMVCoolT5RegTabUp, U8_PMV_COOL_T5_ZONE_LEN);
                        if(u8PMVCoolT5ZoneTmp >= u8PMVCoolT5Zone)
                        {
                          u8PMVCoolT5Zone = u8PMVCoolT5ZoneTmp;
                        }
                        else
                        {
                          u8PMVCoolT5Zone--;
                        }

                        pU16TabChkAddr = &u16CoolPlat1Pulse;   //contine addr
                        u16TrgPulse = *(pU16TabChkAddr + (unsigned int)u8PMVCoolT5Zone);

                      }
                      else if(u8InSetMode == C_ModeHeat)
                      {
                        //u8PMVHeatT5Zone = TempRegJdg(u8TempTP, u8PMVHeatT5Zone, *u8PMVHeatT5RegTab, U8_PMV_HEAT_T5_ZONE_LEN);
                        //u8PMVHeatT5ZoneTmp = TempRegJdg(u8TempTP, u8PMVHeatT5Zone, *u8PMVHeatT5RegTab, U8_PMV_HEAT_T5_ZONE_LEN);
                        u8PMVHeatT5ZoneTmp = TempRegJdg(u8TempT5, u8PMVHeatT5Zone, u8PMVHeatT5RegTabDn, u8PMVHeatT5RegTabUp, U8_PMV_HEAT_T5_ZONE_LEN);
                        if(u8PMVHeatT5ZoneTmp >= u8PMVHeatT5Zone)
                        {
                          u8PMVHeatT5Zone = u8PMVHeatT5ZoneTmp;
                        }
                        else
                        {
                          u8PMVHeatT5Zone--;
                        }

                        pU16TabChkAddr = &u16HeatPlat1Pulse;   //contine addr
                        u16TrgPulse = *(pU16TabChkAddr + (unsigned int)u8PMVHeatT5Zone);

                      }
                    }
                    else
                    {
                      //排气传感器故障,则跑最高档
                      u16TrgPulse = MAX_PULSE;   //480P
                    }
                  }
                }
                else
                {
                  //开机确定初始开度
                  PmvInitPls();
                }

                if(u16TrgPulse > MAX_PULSE)
                {
                  u16TrgPulse = MAX_PULSE;   //480P
                }
                if(u16TrgPulse < MIN_PULSE)
                {
                  u16TrgPulse = MIN_PULSE;   //80P
                }
              }
            }//end of COMP_STOP != u8CompStatus

          }//end of 主控程序
        }//u16TrgPulse赋值完毕,下一步计算偏差


        //开度偏差计算
        if(u16CurrPulse != u16TrgPulse)
        {
          u8StepExcitTim = STEP_START_EXCIT;

          if(0 == u16TrgPulse)             //复位:目标值u16TrgPulse=0; 当前值u16CurrPulse!=0;
          {
            if(0 != u16CurrPulse)
            {
              u16CurrPulse += CLOSE_CLR_PULSE;   //在当前开度的基础上,再多走160P。方向另外判断
            }
          }

          if(u16CurrPulse  < u16TrgPulse)
          {
            u16DeltaPulse = u16TrgPulse - u16CurrPulse;
            unFlgs1.bits.PmvRunDir = 1;     //阀开方向
          }
          else
          {
            u16DeltaPulse = u16CurrPulse - u16TrgPulse;
            unFlgs1.bits.PmvRunDir = 0;     //阀闭方向
          }
        }
      }//end of !u16CurrPulse
    }//end of u8StepTim < STEP_PPS
  }

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void PmvInitPls(void)
{
  if((u8InSetMode == C_ModeCool) || (u8InSetMode == C_ModeDry) || (u8InSetMode == C_ModeForce))
  {
    //制冷,除湿,强制制冷
    u16TrgPulse = u16CoolInitPulse; //Eeprom参数
    u8PMVCoolT5Zone = 6;
  }
  else if (u8InSetMode == C_ModeHeat)
  {
    //制热
    u16TrgPulse = u16HeatInitPulse; //Eeprom参数
    u8PMVHeatT5Zone = 6;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned char TempRegJdg(unsigned char u8Temp, unsigned char u8CurrReg, unsigned char *u8TabDn[], unsigned char *u8TabUp[], unsigned char u8Len)
{
  // Temperature Region
#if(0)
  unsigned char u8CNT, u8TmpLow, u8TmpHigh, u8NewReg;
  u8NewReg = u8CurrReg;
  u8TmpLow = *u8TabDn[u8CurrReg];//*u8Tab[u8CurrReg][0];
  u8TmpHigh = *u8TabUp[u8CurrReg];//*u8Tab[u8CurrReg][1];

  if (u8Temp >= u8TmpHigh)
  {
    for (u8CNT = u8CurrReg; u8CNT < u8Len; u8CNT++)
    {
      u8TmpHigh = *u8TabUp[u8CNT];//*u8Tab[u8CNT][1];

      if (u8Temp >= u8TmpHigh)
      {
        u8NewReg = u8CNT + 1;
      }
      else
      {
        break;
      }
    }
  }
  else if (u8Temp <= u8TmpLow)
  {
    for (u8CNT = u8CurrReg; u8CNT > 0; u8CNT--)
    {
      u8TmpLow = *u8TabDn[u8CNT];//*u8Tab[u8CNT][0];

      if (u8Temp <= u8TmpLow)
      {
        u8NewReg = u8CNT - 1;
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    u8NewReg = u8CurrReg;
  }

  return u8NewReg;
#endif

#if(1)
  unsigned char u8CNT, u8NewReg;

  u8NewReg = u8CurrReg;
  for(u8CNT = 0; u8CNT < 9; u8CNT++)
  {
    if(u8NewReg == u8CNT)
    {
      if(u8Temp >= *u8TabUp[u8CNT])
      {
        u8NewReg++;
      }
      else
      {
        return u8NewReg;
      }
    }
    else
    {
      if(u8Temp < *u8TabDn[u8CNT])
      {
        u8NewReg = u8CNT;
        return u8NewReg;
      }
    }
  }
  return u8NewReg;

#endif
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void StepDirect(void)
{
  if (unFlgs1.bits.PmvRunDir)
  {
    //open
    u16CurrPulse++;
    //uiCurrPulse++;

    u8PulseCnt++;
    if (u8PulseCnt >= 8)
    {
      u8PulseCnt = 0;
    }
  }
  else
  {
    //off
    u16CurrPulse--;
    //uiCurrPulse--;

    u8PulseCnt--;
    if(u8PulseCnt >= 8)
    {
      u8PulseCnt = 7;
    }
  }

  u16DeltaPulse--;
  if (u16DeltaPulse == 0)
  {
    //停止励磁时间
    u8StepExcitTim = STEP_STOP_EXCIT;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void StepDrv(unsigned char u8PortLevel)
{
  if(u8PortLevel & 0x01)
  {
    PMVA_A = 1;
  }
  else
  {
    PMVA_A = 0;
  }

  if(u8PortLevel & 0x02)
  {
    PMVA_B = 1;
  }
  else
  {
    PMVA_B = 0;
  }

  if(u8PortLevel & 0x04)
  {
    PMVA_C = 1;
  }
  else
  {
    PMVA_C = 0;
  }

  if(u8PortLevel & 0x08)
  {
    PMVA_D = 1;
  }
  else
  {
    PMVA_D = 0;
  }
}


const unsigned char TabPmv[] =
{
  //阀开方向步进:0到7,阀闭方向步进:7到0
  0x01,   //0   //00000001----A相通电
  0x03,   //1   //00000011----AB相通电
  0x02,   //2   //00000010----B相通电
  0x06,   //3   //00000110----B~A相通电
  0x04,   //4   //00000100----~A相通电
  0x0c,   //5   //00001100----~A~B相通电
  0x08,   //6   //00001000----~B相通电
  0x09    //7   //00001001----~BA相通电
};

const unsigned char TabPmvDircet[] =
{
  //阀开方向步进:0到7,阀闭方向步进:7到0
  0x09,   //0   //00000001----A相通电
  0x08,   //1   //00000011----AB相通电
  0x0c,   //2   //00000010----B相通电
  0x04,   //3   //00000110----B~A相通电
  0x06,   //4   //00000100----~A相通电
  0x02,   //5   //00001100----~A~B相通电
  0x03,   //6   //00001000----~B相通电
  0x01    //7   //00001001----~BA相通电
};

