#include "UsrDef.h"
#include "MonitorCommSub.h"
#include "FanCtr.h"
#include "TempSample.h"
#include "DevCtr.h"
#include "MiscFunc.h"
#include "DataTransFunc2Drv.h"
#include "Signal.h"

#if 0
enum InFanStatus
{
  FanOff=0,    //indoor fan off
  FanSlow,     //indoor fan low speed
  FanLow,      //indoor fan low speed
  FanMid,      //indoor fan middle speed
  FanHigh,     //indoor fan high speed
  FanSuperHigh,
  FanAuto,     //indoor fan auto speed
  FanTurbo
};
#endif

signed long Uc_Comp_Sendbuff[20];
signed long Uc_Fan_Sendbuff[20];
ByteTypeDef unSimurtCtr0;
ByteTypeDef unPC2Ctr0;
unsigned char u8PC2CompHz;
unsigned char u8PC2FanSpd;

static void SimurtEnc0x01(unsigned char *pBuf);
static void SimurtEnc0x02(unsigned char *pBuf);
static void SimurtEnc0x03(unsigned char *pBuf);
static void SimurtEnc0x04(unsigned char *pBuf);
static void SimurtEnc0x05(unsigned char *pBuf);
static void SimurtEnc0x06(unsigned char *pBuf);
static void SimurtEnc0x07(unsigned char *pBuf);
static void SimurtEnc0x08(unsigned char *pBuf);
static void SimurtEnc0x09(unsigned char *pBuf);
static void SimurtEnc0x0A(unsigned char *pBuf);
static void SimurtEnc0x0B(unsigned char *pBuf);
static void SimurtEnc0x71(unsigned char *pBuf);

static void SimurtDec0x11(unsigned char *pBuf);
static void SimurtDec0x20(unsigned char *pBuf);
static void SimurtDec0x21(unsigned char *pBuf);
static void SimurtDec0xE0(unsigned char *pBuf);
static void SimurtDec0xE1(unsigned char *pBuf);
static void SimurtDec0xF1(unsigned char *pBuf);
static void SimurtDec0xD1(unsigned char *pBuf);
static void SimurtDec0x25(unsigned char *pBuf);
static void SimurtDec0x50(unsigned char *pBuf);
static void SimurtDec0x51(unsigned char *pBuf);
static void SimurtDec0x52(unsigned char *pBuf);
static void SimurtDec0x53(unsigned char *pBuf);
static void SimurtDec0x55(unsigned char *pBuf);

static ByteTypeDef unTmp;
unsigned char u8CmdCode;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec(unsigned char *pBuf)
{
  u8CmdCode = pBuf[1];
  switch(u8CmdCode)
  {
    case 0x11:
      SimurtDec0x11(pBuf);
      break;
    case 0x20:
      SimurtDec0x20(pBuf);
      break;
    case 0x21:
      SimurtDec0x21(pBuf);
      break;
    case 0xE0:
      SimurtDec0xE0(pBuf);
      break;
    case 0xE1:
      SimurtDec0xE1(pBuf);
      break;
    case 0xF1:
      SimurtDec0xF1(pBuf);
      break;
    case 0xD1:
      SimurtDec0xD1(pBuf);
      break;
    case 0x25:
      SimurtDec0x25(pBuf);
      break;
    case 0x50:
      SimurtDec0x50(pBuf);
      break;
    case 0x51:
      SimurtDec0x51(pBuf);
      break;
    case 0x52:
      SimurtDec0x52(pBuf);
      break;
    case 0x53:
      SimurtDec0x53(pBuf);
      break;
    case 0x55:
      SimurtDec0x55(pBuf);
      break;
    default:
      break;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc(unsigned char *pBuf )
{
  switch(pBuf[1])
  {
    case 0x01:
      SimurtEnc0x01(pBuf);
      break;
    case 0x02:
      SimurtEnc0x02(pBuf);
      break;
    case 0x03:
      SimurtEnc0x03(pBuf);
      break;
    case 0x04:
      SimurtEnc0x04(pBuf);
      break;
    case 0x05:
      SimurtEnc0x05(pBuf);
      break;
    case 0x06:
      SimurtEnc0x06(pBuf);
      break;
    case 0x07:
      SimurtEnc0x07(pBuf);
      break;
    case 0x08:
      SimurtEnc0x08(pBuf);
      break;
    case 0x09:
      SimurtEnc0x09(pBuf);
      break;
    case 0x0A:
      SimurtEnc0x0A(pBuf);
      break;
    case 0x0B:
      SimurtEnc0x0B(pBuf);
      break;
    case 0x71:
      SimurtEnc0x71(pBuf);
      break;
    default:
      break;
  }

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x01(unsigned char *pBuf)
{
  /* Byte 1 */
  /* Byte 2 */
  switch(u8InSetMode)
  {
    case C_ModeOff:
      pBuf[2] = 0x00;
      break;
    case C_ModeCool:
      pBuf[2] = 0x01;
      break;
    case C_ModeHeat:
      pBuf[2] = 0x02;
      break;
    case C_ModeFan:
      pBuf[2] = 0x03;
      break;
    case C_ModeDry:
      pBuf[2] = 0x04;
      break;
    default:
      break;
  }
  /*5:AUTO;6:²âÊÔ;7:Ô¤ÈÈ Ã»ÓÐ¼æÈÝ*/
  /* Byte 3 */
  pBuf[3] = 0;
  /* Byte 4 */
  switch(u8InFanMode)
  {
    case C_InFanOff:
      pBuf[4] = 0x00;
      break;
    case C_InFanH:
      pBuf[4] = 0x01;
      break;
    case C_InFanM:
      pBuf[4] = 0x02;
      break;
    case C_InFanMM:
    case C_InFanL:
      pBuf[4] = 0x03;
      break;
    case C_InFanSH:
    case C_InFanTurbo:
      pBuf[4] = 0x04;
      break;
    default:
      pBuf[4] = 0x00;
      break;
  }

  /*Byte 5*/
  pBuf[5] = TEMP_MACRO(u8InSetTemp);
  /*Byte 6*/
  pBuf[6] = u8TempT1 /*TEMP_MACRO(25)*/ ;
  /*Byte 7*/
  pBuf[7] = u8TempT2;
  /*Byte 8*/
  pBuf[8] = 0x00;
  /*Byte 9*/
  unTmp.u8Byte = 0;
  unTmp.bits.no0 = fgErrInFan;
  unTmp.bits.no1 = 0;
  unTmp.bits.no2 = fgErrInEE;
  unTmp.bits.no3 = 0;
  unTmp.bits.no4 = fgErrT1;
  unTmp.bits.no5 = (fgErrT2||fgErrT2b)?1:0;
  unTmp.bits.no6 = 0;
  unTmp.bits.no7 = fgProtInT2L;
  pBuf[9] = unTmp.u8Byte;
  /*Byte 10*/
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x02(unsigned char *pBuf)
{
  unsigned char u8Tmp = 0;
  unsigned short u16Tmp = 0;
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
#if 0
  switch(u8OutRunMode)
  {
    
  }
#endif
  pBuf[2] = 0x00;
  /* Byte 3 */
  pBuf[3] = 0x00;
  /* Byte 4 */
  switch(u8TrgFan)
  {
    case FanOFF:
      pBuf[4] = 0x00;
      break;
    case FanH:
      pBuf[4] = 0x01;
      break;
    case FanM:
      pBuf[4] = 0x02;
      break;
    case FanL:
      pBuf[4] = 0x03;
      break;
    default:
      break;
  }
  u16Tmp = u16TrgFanSpd;
  u16Tmp /= 10;
  pBuf[4] = (unsigned char)u16Tmp;
  /* Byte 5 */
  pBuf[5] = u8RunFreqGrade;
  /* Byte 6 */
  u8Tmp = u8TargetFreq;
  u8Tmp *= 2;
  pBuf[6] = u8Tmp;
  /* Byte 7 */
  u8Tmp = u8RealRunFreq;
  u8Tmp *= 2;
  pBuf[7] = u8Tmp;
  /* Byte 8 */
  pBuf[8] = (unsigned char)(u16CurrPulse>>2);
  /* Byte 9 */
  unTmp.bits.no0 = fgFWVSta;
  unTmp.bits.no1 = ChassisPTCSta;
  pBuf[9] = unTmp.u8Byte;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x03(unsigned char *pBuf)
{
  unsigned char u8Tmp = 0;
  unTmp.u8Byte = 0;
  
  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = u8DcVoltAd;
  /* Byte 3 */
  pBuf[3] = u8AcCurrRMS_T10;
  /* Byte 4 */
  pBuf[4] = u8TempT4;
  /* Byte 5 */
  pBuf[5] = /*TEMP_MACRO(25)*/u8TempT3;
  /* Byte 6 */
  u8Tmp = u8TempT5;
  u8Tmp *= 2;
  pBuf[6] = u8Tmp;
  /* Byte 7 */
  pBuf[7] = 0x00;
  /* Byte 8 */
  pBuf[8] = u8AcVoltAd;
  /* Byte 9 */
  pBuf[9] = unTmp.u8Byte;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x04(unsigned char *pBuf)
{
  unsigned short u16Tmp = 0;
  
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
  unTmp.bits.no3 = fgInOutCommErr;
  pBuf[2] = unTmp.u8Byte;
  /* Byte 3 */
  unTmp.u8Byte = 0;
  unTmp.bits.no5 = fgProtT5;
  pBuf[3] = unTmp.u8Byte;
  /* Byte 4 */
  unTmp.u8Byte = 0;
  unTmp.bits.no0 = unTempErrFlgs.bits.T3Err;
  unTmp.bits.no1 = unTempErrFlgs.bits.T5Err;
  unTmp.bits.no2 = unTempErrFlgs.bits.T4Err;
  unTmp.bits.no3 = 0;
  unTmp.bits.no4 = 0;
  unTmp.bits.no5 = unSysErrFlgs.bits.DCFanSpdErr;
  unTmp.bits.no6 = 0;
  unTmp.bits.no7 = unSysErrFlgs.bits.I2cErr;
  pBuf[4] = unTmp.u8Byte;
  /* Byte 5 */
  pBuf[5] = u8TempT3;
  /* Byte 6 */
  pBuf[6] = u8LytranCompErrCode;
  /* Byte 7 */
  u16Tmp = u16FanFBSpd;
  u16Tmp /= 10;
  pBuf[7] = (unsigned char)u16Tmp;
  /* Byte 8 */
  pBuf[8] = 0x00;
  /* Byte 9 */
  pBuf[9] = 0x00;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x05(unsigned char *pBuf)
{
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = unTmp.u8Byte;
  /* Byte 3 */
  unTmp.u8Byte = 0;
  pBuf[3] = unTmp.u8Byte;
  /* Byte 4 */
  unTmp.u8Byte = 0;
  pBuf[4] = unTmp.u8Byte;
  /* Byte 5 */
  unTmp.u8Byte = 0;
  pBuf[5] = unTmp.u8Byte;
  /* Byte 6 */
  pBuf[6] = 0x00;
  /* Byte 7 */
  pBuf[7] = 0x00;
  /* Byte 8 */
  pBuf[8] = 0x00;
  /* Byte 9 */
  pBuf[9] = 0x00;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x06(unsigned char *pBuf)
{
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = unTmp.u8Byte;
  /* Byte 3 */
  unTmp.u8Byte = 0;
  pBuf[3] = unTmp.u8Byte;
  /* Byte 4 */
  unTmp.u8Byte = 0;
  pBuf[4] = unTmp.u8Byte;
  /* Byte 5 */
  unTmp.u8Byte = 0;
  pBuf[5] = unTmp.u8Byte;
  /* Byte 6 */
  pBuf[6] = 0x00;
  /* Byte 7 */
  pBuf[7] = 0x00;
  /* Byte 8 */
  pBuf[8] = 0x00;
  /* Byte 9 */
  pBuf[9] = 0x00;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x07(unsigned char *pBuf)
{
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = unTmp.u8Byte;
  /* Byte 3 */
  unTmp.u8Byte = 0;
  pBuf[3] = unTmp.u8Byte;
  /* Byte 4 */
  unTmp.u8Byte = 0;
  pBuf[4] = unTmp.u8Byte;
  /* Byte 5 */
  unTmp.u8Byte = 0;
  pBuf[5] = unTmp.u8Byte;
  /* Byte 6 */
  pBuf[6] = 0x00;
  /* Byte 7 */
  pBuf[7] = 0x00;
  /* Byte 8 */
  pBuf[8] = 0x00;
  /* Byte 9 */
  pBuf[9] = 0x00;
  /* Byte 10 */
} 

/**
  * @brief  :
  * @param  :
  * @retval :
  */
extern unsigned short u16T3_AN207Min,u16T3_AN207Max,u16T3_UsrAd_AN207;
extern unsigned short u16Tipm_AN210Min,u16Tipm_AN210Max,u16Tipm_UsrAd_AN210;
extern unsigned long u32T3_AdSum,u32T4_AdSum,u32T5_AdSum,u32Tipm_AdSum;
void SimurtEnc0x08(unsigned char *pBuf)
{
  //unTmp.u8Byte = 0;
  
  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = (unsigned char)(u32T3_AdSum>>24);
  /* Byte 3 */
  pBuf[3] = (unsigned char)(u32T3_AdSum>>16);
  /* Byte 4 */
  pBuf[4] = (unsigned char)(u32T3_AdSum>>8);
  /* Byte 5 */
  pBuf[5] = (unsigned char)(u32T3_AdSum);
  /* Byte 6 */
  pBuf[6] = (unsigned char)(u16T3_AN207Min>>8);
  /* Byte 7 */
  pBuf[7] = (unsigned char)(u16T3_AN207Min);
  /* Byte 8 */
  pBuf[8] = (unsigned char)(u16T3_AN207Max>>8);
  /* Byte 9 */
  pBuf[9] = (unsigned char)(u16T3_AN207Max);
  /* Byte 10 */
} 

/**
  * @brief  :
  * @param  :
  * @retval :
  */
extern unsigned char u8TempT1;
extern unsigned char u8TempT2;
extern unsigned char u8TempT2b;
extern unsigned char u8TempT3;
extern unsigned char u8TempT4;
extern unsigned char u8TempT5;
extern unsigned char u8TempIpm;
extern unsigned char u8T1TempAd;   //T1 AD
extern unsigned char u8T2TempAd;   //T2 AD
extern unsigned char u8T3TempAd;   //T3 AD
extern unsigned char u8T4TempAd;   //T4 AD
extern unsigned char u8T5TempAd;   //TP AD
extern unsigned char u8IpmTempAd;  //IPM AD
extern unsigned char u8T2bTempAd;  //T2B AD
extern unsigned char u8IpmTempk1k2Ad;   //IPM k1k2 AD;
void SimurtEnc0x09(unsigned char *pBuf)
{
  //unTmp.u8Byte = 0;
  
  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = u8InSetMode;
  /* Byte 3 */
  pBuf[3] = u8FreqSet;
  /* Byte 4 */
  pBuf[4] = u8RunFreqGrade;
  /* Byte 5 */
  pBuf[5] = u8EnergyFreq;
  /* Byte 6 */
  pBuf[6] = u8TargetFreq;
  /* Byte 7 */
  pBuf[7] = u8RunCtrFreq;
  /* Byte 8 */
  pBuf[8] = u8RealRunFreq;
  /* Byte 9 */
  pBuf[9] = u8LimitFreq;
  /* Byte 10 */
} 

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x0A(unsigned char *pBuf)
{
  //unTmp.u8Byte = 0;
  
  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = (fgProtErr)?1:0;
  /* Byte 3 */
  pBuf[3] = (unSysErrFlgs.bits.I2cErr)?1:0;
  /* Byte 4 */
  pBuf[4] = (fgProtT5)?1:0;
  /* Byte 5 */
  pBuf[5] = unFreqLimFlgs.u8Byte;
  /* Byte 6 */
  pBuf[6] = unLimStop.u8Byte;
  /* Byte 7 */
  pBuf[7] = unFreqProt.u8Byte;
  /* Byte 8 */
  pBuf[8] = u8DcVoltAd;
  /* Byte 9 */
  pBuf[9] = 0;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
extern signed long Uc_Comp_Sendbuff[];
extern signed long Uc_Fan_Sendbuff[];
void SimurtEnc0x0B(unsigned char *pBuf)
{
  unsigned short u16Tmp = 0;

  u16Tmp = (unsigned short)Uc_Fan_Sendbuff[1];
  
  //unTmp.u8Byte = 0;
  
  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = unInErr1.u8Byte;
  /* Byte 3 */
  pBuf[3] = unSysErrFlgs.u8Byte;
  /* Byte 4 */
  pBuf[4] = unSysErrFlgs1.u8Byte;
  /* Byte 5 */
  pBuf[5] = unSysErrFlgs2.u8Byte;
  /* Byte 6 */
  pBuf[6] = unCompRunFlgs1.u8Byte;
  /* Byte 7 */
  pBuf[7] = (unsigned char)(u16Tmp>>8);
  /* Byte 8 */
  pBuf[8] = (unsigned char)(u16Tmp);
  /* Byte 9 */
  pBuf[9] = Uc_Comp_Sendbuff[1];
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtEnc0x71(unsigned char *pBuf)
{
  unTmp.u8Byte = 0;

  /* Byte 1 */
  /* Byte 2 */
  pBuf[2] = unTmp.u8Byte;
  /* Byte 3 */
  unTmp.u8Byte = 0;
  pBuf[3] = unTmp.u8Byte;
  /* Byte 4 */
  unTmp.u8Byte = 0;
  pBuf[4] = unTmp.u8Byte;
  /* Byte 5 */
  unTmp.u8Byte = 0;
  pBuf[5] = unTmp.u8Byte;
  /* Byte 6 */
  pBuf[6] = 0x00;
  /* Byte 7 */
  pBuf[7] = 0x00;
  /* Byte 8 */
  pBuf[8] = 0x00;
  /* Byte 9 */
  pBuf[9] = 0x00;
  /* Byte 10 */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x11(unsigned char *pBuf)
{
  //DgLedOn();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x20(unsigned char *pBuf)
{
  //DgLedOn();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x21(unsigned char *pBuf)
{
  //DgLedOn();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x25(unsigned char *pBuf)
{
  ByteTypeDef unTmp;
  unTmp.u8Byte = 0;

  unTmp.u8Byte = pBuf[2];

  fgCompCtrSrc    = unTmp.bits.no7;
  fgOutFanCtrSrc  = unTmp.bits.no6;
  fg4WayCtrSrc    = unTmp.bits.no5;
  fgEleHeatCtrSrc = unTmp.bits.no4;
  fgExVCtrSrc     = unTmp.bits.no3;
  //
  fgForceCompOn   = unTmp.bits.no1;
  //

  u8PC2CompHz = pBuf[3];
  u8PC2CompHz /=2;
  u8PC2FanSpd = pBuf[4];

  unTmp.u8Byte = pBuf[5];
  fgPC2_4Way = unTmp.bits.no7;
  fgPC2EleHeat = unTmp.bits.no6; 
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0xE0(unsigned char *pBuf)
{
  //DgLedOn();
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0xE1(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0xF1(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0xD1(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x50(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x51(unsigned char *pBuf)
{

}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x52(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x53(unsigned char *pBuf)
{

}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SimurtDec0x55(unsigned char *pBuf)
{

}

