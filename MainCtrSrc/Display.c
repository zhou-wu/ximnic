#include "UsrDef.h"
#include "Display.h"
#include "MiscFunc.h"

//����(1.5s��)Ϊ����,����Ϊ��������,����(0.25s��)Ϊϵͳ��������,������(0.1s��)Ϊ�������ϡ�
void DisplayPro(void)
{
  LED_PORT_OUT;
  //PM1 &= 0xfb;

  if (fgCompErrSta)
  {
    if (u8DispFlashTim == 0)
    {
      u8DispFlashTim = 10;//ѹ�������ϳ�����
      LED_FLASH;
    }
  }
  else if (unSysErrFlgs.u8Byte || unSysErrFlgs2.u8Byte || unTempErrFlgs.bits.TipmErr)
  {
    if (u8DispFlashTim == 0)
    {
      u8DispFlashTim = 25;//ϵͳ���ܹ��Ͽ���
      LED_FLASH;
    }
  }
  else
  {
    if (u8CompSta == COMP_STOP)
    {
      if (u8DispFlashTim == 0)
      {
        u8DispFlashTim = 150;   //1.5s on, 1.5s off//����,����
        LED_FLASH;
      }
    }
    else
    {
      LED_ON;//����,����
    }
  }
}

