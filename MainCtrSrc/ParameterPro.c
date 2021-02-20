#ifndef _PARA_C_
#define _PARA_C_
#endif

#include "UsrDef.h"
///#include "eeprom.h"
#include "ParameterPro.h"
#include "DataTransFunc2Drv.h"
#include "DevCtr.h"
#include "MiscFunc.h"

static void ReadMainParam3(void);
static void ReadMainParam(unsigned short Address);
static void EepParaDeal(void);

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void EeParamPro(void)
{
  unsigned char u8Cnt = 0;

  unSysErrFlgs.bits.I2cErr = 0;
  
  for ( u8Cnt = 0; u8Cnt < 3; u8Cnt++ )
  {
    u8EER_F = 0;
    ReadMainParam3();
    ReadMainParam(U16_MAIN_EEP_START_ADDR);   //Ԥ�����ز�����ʼ��ַ0x00F8

    if ( 0 == u8EER_F )
    {
      break;
    }
    else
    {
      u8EER_F = 0;
      ReadMainParam3();
      ReadMainParam(U16_MAIN_EEP_START_ADDR); //Ԥ�����ز�����ʼ��ַ0x00F8
    }

    if ( 0 == u8EER_F )
    {
      break;
    }
  }

  EepParaDeal();

  //ucMinFreq=1;

  if(0 != u8EER_F)
  {
    unSysErrFlgs.bits.I2cErr = 1;
    //u8CoolStopCurr = 95;     //defaul 9.5A ucCoolStopCurr
    //u88HeatStopCurr = 95;    //defaul 9.5A ucHeatStopCur
  }

  s16IPM_k1 = (signed  short)((signed  short)U8_IPM_CHANGE_TEMP - (signed  short)U8_IPM_TEMP2VALUE(25)) * 256 / ((signed  short)U8_IPM_CHANGE_TEMP_AD - (signed  short)U8_IPM_MIN_TEMP_AD);
  s16IPM_b1 = (signed  short)U8_IPM_CHANGE_TEMP - (signed  short)(((signed long)s16IPM_k1 * (signed long)U8_IPM_CHANGE_TEMP_AD) >> 8);
  s16IPM_k2 = (signed  short)((signed  short)U8_IPM_TEMP2VALUE(105) - (signed  short)U8_IPM_CHANGE_TEMP) * 256 / ((signed  short)U8_IPM_MAX_TEMP_AD - (signed  short)U8_IPM_CHANGE_TEMP_AD);
  s16IPM_b2 = (signed  short)U8_IPM_CHANGE_TEMP - (signed  short)(((signed long)s16IPM_k2 * (signed long)U8_IPM_CHANGE_TEMP_AD) >> 8);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void EepParaDeal(void)
{
  unSeltFlgs4.u8Byte = COOL_TEST_AC_FAN;
  unSeltFlgs5.u8Byte = HEAT_TEST_AC_FAN;

  unSeltFlgs1.u8Byte = u8SeltFlgs1;
  unSeltFlgs3.u8Byte = u8SeltFlgs3;
  unSeltFlgs2.u8Byte = u8SeltFlgs2;
  unSeltFlgs6.u8Byte = HEAT_RAT_PFC;

  unSeltFlgs7.u8Byte = COOL_RAT_DEFROST;
  unSeltFlgs8.u8Byte = HEAT_RAT_DEFROST;

  unSeltFlgs1.bits.FanSelt = 1;  ///debug only
  //��ֱ�����ѡ��:�ڴ˴���E��֮���������ֵ,����������ص�Ӳ����ʼ������,��Ҫ�Ƶ���
  if (unSeltFlgs1.bits.FanSelt)
  {
    Main2Drv[18] |= 0x01;
  }
  else
  {
    Main2Drv[18] &= 0xFE;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void ReadMainParam(unsigned short Address)
{
  unsigned char i = 0;
  
  u8I2cFlg = 1;

  for (i = 0; i < 2; i++)
  {
    ///Sequ_RD(Address, EEP_SYS_PARA_LEN, u8EepSysPara );  ///ԭ���� //���ز�������510 
    if (u8EepSysPara[0] != EEPROM_SN)
    {
      unEEWrtFlgs.bits.I2cProt = 1;
    }
    //s_delay(1000);      // 0.1ms
    
    if (0 == unEEWrtFlgs.bits.I2cProt)
    {
      break;
    }
  }

  if (1 == unEEWrtFlgs.bits.I2cProt)
  {
    u8EER_F = 1;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void ReadMainParam3(void)
{
  unsigned char i = 0;
  u8I2cFlg = 0;
  
  //=======================��һ֡========================
  for ( i = 0; i < 2; i++ )
  {
    ///Sequ_RD(U16_MAIN_EEP_START1_ADDR, EEP_SYS_PARA_LEN1,&u8ID_CODE1); ///ԭ���� //���ز�������510
    if(10 != u8ID_CODE1)
    {
      unEEWrtFlgs.bits.I2cProt = 1;
      return;
    }
    //s_delay(1000);      // 0.1ms
    if (0 == unEEWrtFlgs.bits.I2cProt)
    {
      break;
    }
  }

  if(0 == u8ACCurrOver)
  {
    unEEWrtFlgs.bits.I2cProt = 1;
    u8EER_F = 1;
    return;
  }
  else if(u8ACCurrOver >= (25 * 10))        //��ΪERPϵ�й���E��,4HP�İ���Ҫ����������,���Ⲧ����ѡ�����Ͷ��ջ�Ӳ��
  {
    u8ACCurrOver = (25 * 10);
  }
  else
  {
    ;
  }

  //=======================�ڶ�֡========================
  for (i = 0; i < 2; i++)
  {
    ///Sequ_RD(U16_MAIN_EEP_START2_ADDR, EEP_SYS_PARA_LEN2, &u8ID_CODE2); ///ԭ���� //���ز�������510
    if(20 != u8ID_CODE2)
    {
      unEEWrtFlgs.bits.I2cProt = 1;
      return;
    }
    //s_delay(1000);      // 0.1ms,add
    if (0 == unEEWrtFlgs.bits.I2cProt)
    {
      break;
    }
  }
  BytesToMultiWords(&u8PlatHold0H, &u16PlatHold0, &u8RetOilTim2L);

  //=======================����֡========================
  u8ChkSum = 0;
  for (i = 0; i < 2; i++)
  {
    ///Sequ_RD(U16_MAIN_EEP_START3_ADDR, EEP_SYS_PARA_LEN3, &u8ID_CODE3); ///ԭ���� //���ز�������510
    if(30 != u8ID_CODE3)
    {
      unEEWrtFlgs.bits.I2cProt = 1;
      return;
    }
    //s_delay(1000);      // 0.1ms
    if (0 == unEEWrtFlgs.bits.I2cProt)
    {
      break;
    }
  }

  if (1 == unEEWrtFlgs.bits.I2cProt)
  {
    u8EER_F = 1;
  }
}

