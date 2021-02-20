#include "UsrDef.h"
#include "DevCtr.h"
#include "FanCtr.h"
#include "ParameterPro.h"
#include "PmvCtr.h"
#include "MiscFunc.h"

unsigned char u8SelfChkRYTim;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SelfChkPro(void)
{  
  static unsigned char u8StcTmp1 = 0;
  ///static unsigned char u8StcTmp2 = 0;
  static unsigned char u8StcTmp3 = 0;
  
  if (0 == unInSta.bits.SelfChk )
  {
    return;
  }

  if ( u8TimPowerOn < 20 )    // 100ms++,2s
  {
    return;
  }

  if ( u8SelfChkRYTim < 15 )  // 100ms++,1.5s
  {
    return;
  }

  u8SelfChkRYTim = 0;
  u8StcTmp1++;
  switch( u8StcTmp1 )
  {
    case 1:
       FOUR_WAY_ON;
       break;
    case 2:
       CompressPTC_ON;
       break;
    case 3:
       if ( 0 == unSeltFlgs1.bits.FanSelt )  // 交流风机
       {
         AC_FAN_LOW_ON;
       }
       else                                  // 直流风机
       {
         u16TrgFanSpd = u16FanHighSpd;
         u8StcTmp1 = 0xFE;
       }
       break;
    case 4:
       AC_FAN_HI_ON;
       break;
    default:
       u8StcTmp1 = 0xFE;
       break;
  }

  if ( u8StepTim > 150 )     /// 150ms
  {
    u8StepTim = 0;
    if ( u8StcTmp3 > 3 )
    {
      u8StcTmp3 = 0;
    }
    
    StepDrv((unsigned char)(1<<u8StcTmp3));
    u8StcTmp3++;
  }
}

