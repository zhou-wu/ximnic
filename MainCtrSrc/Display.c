#include "UsrDef.h"
#include "Display.h"
#include "MiscFunc.h"

//慢闪(1.5s闪)为待机,长亮为开机运行,快闪(0.25s闪)为系统保护故障,超快闪(0.1s闪)为驱动故障。
void DisplayPro(void)
{
  LED_PORT_OUT;
  //PM1 &= 0xfb;

  if (fgCompErrSta)
  {
    if (u8DispFlashTim == 0)
    {
      u8DispFlashTim = 10;//压缩机故障超快闪
      LED_FLASH;
    }
  }
  else if (unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unTempErrFlgs.bits.TipmErr)
  {
    if (u8DispFlashTim == 0)
    {
      u8DispFlashTim = 25;//系统功能故障快闪
      LED_FLASH;
    }
  }
  else
  {
    if (u8CompSta == COMP_STOP)
    {
      if (u8DispFlashTim == 0)
      {
        u8DispFlashTim = 150;   //1.5s on, 1.5s off//待机,慢闪
        LED_FLASH;
      }
    }
    else
    {
      LED_ON;//运行,常亮
    }
  }
}

