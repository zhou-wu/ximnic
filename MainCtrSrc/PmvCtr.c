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
    u8StepTim = STEP_PPS;           //32ms��������
    u16DeltaPulse = RESET_PULSE;
    u16CurrPulse = RESET_PULSE;
    u16TrgPulse = 0;
    u8PulseCnt = 7;                 //���շ��򲽽�:��7��0
    unFlgs1.bits.PmvRunDir = 0;     //���շ����ת
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

  //2013051707 ͨ�Ų���20���ٿ����ͷ�
  if (unSeltFlgs1.bits.PMVSelt)       //����������ͷ�
  {
    if (u8StepTim >= STEP_PPS)        //�����ٶȼ�ʱ1ms++, 32ms
    {
      u8StepTim = 0;
      if (0 != u16DeltaPulse)         //����Ҫ�Ĳ�������û�е�ʱ,���ж�Ŀ�꿪�Ⱥ͵�ǰ���ȵĲ�ֵ
      {
        if (u8StepExcitTim == 0)      //���ͷ���ʼ����,�����ϴζ�����ֹͣ��(��ǰ��ʼ��)����0.5s
        {
          StepDirect();
        }

        //����Ӳ������ִ�е������ͷ����,������ӦIO�ڵ�ƽ,PmvCfgDir������Eeprom����
        if(0 == unSeltFlgs2.bits.PmvCfgDir)
        {
          //����,Ӳ����IO˳������
          StepDrv(TabPmv[u8PulseCnt]);
        }
        else
        {
          //����,Ӳ����IO��������
          StepDrv(TabPmvDircet[u8PulseCnt]);
        }
      }
      else          //ִ�е���Ŀ�꿪����ͬ,�����ж��Ƿ����Ŀ�꿪�Ȳ������ֵ
      {
        if (0 == u8StepExcitTim)  //���ͷ��ڽ����ౣ������0.5s���,������һ�ζ�����������
        {
          //0.5s����ʱ�䵽
          StepDrv(0x00);  //drv set off //���ͷ����ζ������,�����˿��øߵ�ƽ

          //Ŀ�꿪�ȼ���:u16TrgPulse = ��˪����,��С����Կ���,����������ƿ���
          if(unFlgs2.bits.Defrost || (C_ModeDefrost == u8InSetMode)) //��˪���ȼ�������Ƶ
          {
            //��˪
            u16TrgPulse = u16DefrostPulse;
            unFlgs2.bits.PmvReset = 1;        //��˪ʱ�������ͷ��ȸ�λ�ٿ�����˪����
          }
          else if (u16DgTrgStepPls)
          {
            //С�����
            u16TrgPulse = u16DgTrgStepPls;
          }
          else
          {
            if(COMP_STOP == u8CompSta)
            {
              //ͣ��
              if(unFlgs2.bits.PmvReset)
              {
                unFlgs2.bits.PmvReset = 0;
                u16TrgPulse = 0;       //ѹ���ر�ʱ�и�λҪ��,��Ŀ�꿪����Ϊ0����
              }
              else
              {
                u16TrgPulse = DEFAULT_PULSE;     //��λ��ɺ��ߵ�Ĭ�Ͽ���
              }
            }
            else
            {
              //����
              //=============================down is seer test===============================//
              if(u8InRatePMV != 0xFF)
              {
                //����Ƶ,�ڻ�������ʾ����Ƶ
                u16TrgPulse = u8InRatePMV;
                u16TrgPulse = u16TrgPulse << 3;
              }
              else if(SeerTestNo != u8InRateTest)
              {
                if(u8InRateTest == SeerTestMin)
                {
                  //��С��������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MIN_RATI_PLSH); //u16HeatSEERMinPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MIN_RATI_PLSH); //u16CoolSEERMinPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestMid)
                {
                  //�м���������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MID_RATI_PLSH); //u16HeatSEERMidPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MID_RATI_PLSH); //u16CoolSEERMidPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestRat)
                {
                  //���������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_RATI_PLSH);   //u16HeatRatPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_RATI_PLSH);   //u16CoolRatPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTestMax)
                {
                  //�����������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_MAX_RATI_PLSH); //u16HeatSEERMaxPulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_MAX_RATI_PLSH); //u16CoolSEERMaxPulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest5)//��5��Ƶ
                {
                  //5��������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS5H);  //u16HeatSEER5Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS5H);  //u16CoolSEER5Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest6)//��6��Ƶ
                {
                  //6��������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS6H);  //u16HeatSEER6Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS6H);  //u16CoolSEER6Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
                else if(u8InRateTest == SeerTest7)//��7��Ƶ
                {
                  //7��������
                  if(u8InSetMode == C_ModeHeat)
                  {
                    //����
                    u16TrgPulse = BytesToSingleWord(&U8_HEAT_TEST_PLS7H);  // u16HeatSEER7Pulse;
                  }
                  else if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
                  {
                    //����,��ʪ,ǿ������
                    u16TrgPulse = BytesToSingleWord(&U8_COOL_TEST_PLS7H);  //u16CoolSEER7Pulse;
                  }

                  unFlgs2.bits.PmvReset = 1;
                }
              }
              //=============================up is seer test===============================//
              else
              {
                unFlgs2.bits.PmvReset = 1;             //ÿ��ѹ��ֹͣ�󶼸�λ�������ͷ�
                if(u16CompRunTim >=  MINUTE_MACRO(3))  //����3���Ӻ���������¶ȿ������ͷ�����
                {
                  if(u8PmvRegulateTim >= PMV_REG)      //ÿ2��������һ��
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
                      //��������������,������ߵ�
                      u16TrgPulse = MAX_PULSE;   //480P
                    }
                  }
                }
                else
                {
                  //����ȷ����ʼ����
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

          }//end of ���س���
        }//u16TrgPulse��ֵ���,��һ������ƫ��


        //����ƫ�����
        if(u16CurrPulse != u16TrgPulse)
        {
          u8StepExcitTim = STEP_START_EXCIT;

          if(0 == u16TrgPulse)             //��λ:Ŀ��ֵu16TrgPulse=0; ��ǰֵu16CurrPulse!=0;
          {
            if(0 != u16CurrPulse)
            {
              u16CurrPulse += CLOSE_CLR_PULSE;   //�ڵ�ǰ���ȵĻ�����,�ٶ���160P�����������ж�
            }
          }

          if(u16CurrPulse  < u16TrgPulse)
          {
            u16DeltaPulse = u16TrgPulse - u16CurrPulse;
            unFlgs1.bits.PmvRunDir = 1;     //��������
          }
          else
          {
            u16DeltaPulse = u16CurrPulse - u16TrgPulse;
            unFlgs1.bits.PmvRunDir = 0;     //���շ���
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
    //����,��ʪ,ǿ������
    u16TrgPulse = u16CoolInitPulse; //Eeprom����
    u8PMVCoolT5Zone = 6;
  }
  else if (u8InSetMode == C_ModeHeat)
  {
    //����
    u16TrgPulse = u16HeatInitPulse; //Eeprom����
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
    //ֹͣ����ʱ��
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
  //�������򲽽�:0��7,���շ��򲽽�:7��0
  0x01,   //0   //00000001----A��ͨ��
  0x03,   //1   //00000011----AB��ͨ��
  0x02,   //2   //00000010----B��ͨ��
  0x06,   //3   //00000110----B~A��ͨ��
  0x04,   //4   //00000100----~A��ͨ��
  0x0c,   //5   //00001100----~A~B��ͨ��
  0x08,   //6   //00001000----~B��ͨ��
  0x09    //7   //00001001----~BA��ͨ��
};

const unsigned char TabPmvDircet[] =
{
  //�������򲽽�:0��7,���շ��򲽽�:7��0
  0x09,   //0   //00000001----A��ͨ��
  0x08,   //1   //00000011----AB��ͨ��
  0x0c,   //2   //00000010----B��ͨ��
  0x04,   //3   //00000110----B~A��ͨ��
  0x06,   //4   //00000100----~A��ͨ��
  0x02,   //5   //00001100----~A~B��ͨ��
  0x03,   //6   //00001000----~B��ͨ��
  0x01    //7   //00001001----~BA��ͨ��
};

