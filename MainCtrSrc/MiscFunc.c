#include "MiscFunc.h"
#include "TimeTick.h"
#include "DevCtr.h"

unsigned char u8DgLedBlinkTim;

unsigned short u16DgTmp0;
unsigned short u16DgTmp1;
unsigned short u16DgTmp2;
unsigned short u16DgTmp3;
unsigned short u16DgTmp4;
unsigned short u16DgTmp5;
unsigned short u16DgTmp6;
unsigned short u16DgTmp7;
unsigned short u16DgTmp8;
unsigned short u16DgTmp9;
unsigned short u16DgTmp10;

unsigned char u8MacroTmp;

/**
  * @brief  :
  * @param  :readdata,8位变量 首地址
  * @param  :endadress,8位结束地址
  * @param  :StartAdress,16位变量首地址
  * @retval :
  */
void BytesToMultiWords(unsigned char *readdata, unsigned short *StartAdress, unsigned char *endadress)
{
  unsigned short u16EEReadTmp2 = 0;

  for(; readdata <= endadress; readdata++)
  {
    u16EEReadTmp2 =  *readdata;
    u16EEReadTmp2 <<= 8;
    readdata++;
    u16EEReadTmp2 = u16EEReadTmp2 +  *readdata;
    *StartAdress = u16EEReadTmp2;
    StartAdress++;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned short BytesToSingleWord(unsigned char *readdata)
{
  unsigned short u16EEReadTmp2 = 0;

  u16EEReadTmp2 =  *readdata;
  u16EEReadTmp2 <<= 8;
  readdata++;
  u16EEReadTmp2 = u16EEReadTmp2 +  *readdata;
  return(u16EEReadTmp2);
}


/**
  * @brief  :
  * @param  :
  * @retval :
  */
unsigned char CommChkSum(unsigned char *ptParaAddr, unsigned char u8ParaNum)
{
  unsigned char ucCheckSum;
  unsigned char ucLpCtrlTmp;

  for (ucLpCtrlTmp = 0, ucCheckSum = 0; ucLpCtrlTmp < u8ParaNum; ucLpCtrlTmp++, ptParaAddr += 1)
  {
    ucCheckSum += *ptParaAddr;
  }
  ucCheckSum ^= 0xff;
  ucCheckSum += 1;

  return (ucCheckSum);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DgLedInit(void)
{}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DelaySoft1(unsigned short u16Cnt)
{
  do
  {
    u16Cnt--;
  }
  while (u16Cnt != 0);
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DelaySoft2(unsigned char u8Cnt)
{
  unsigned short cnt1  = 0;
  unsigned char cnt2 = 0;

  for ( cnt2 = 0; cnt2 < u8Cnt; cnt2++ )
  {
    for ( cnt1 = 0; cnt1 < 0xFF00; cnt1++ )
    {
      ///WatchDogRst();
      ///__NOP();
      ///__NOP();
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void DgLedBlink(void)
{
  static unsigned char fgLedSta = 0;
  
  if ( u8DgLedBlinkTim < TIME10MS_300MS )
  {
    return;
  }

  u8DgLedBlinkTim = 0;
  
  if ( 0 == fgLedSta )
  {
    DgLedOff();
    fgLedSta = 1;
  }
  else
  {
    DgLedOn();
    fgLedSta = 0;
  }
}


