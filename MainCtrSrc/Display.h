#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define LED_PORT_OUT      u8MacroTmp=0 ///(PORT2.PDR.BIT.B4=1)
#define LED_ON            u8MacroTmp=0 ///(PORT2.PODR.BIT.B4=0)
#define LED_OFF           u8MacroTmp=0 ///(PORT2.PODR.BIT.B4=1)
#define LED_FLASH         u8MacroTmp=0 ///(PORT2.PODR.BIT.B4=~PORT2.PODR.BIT.B4)

extern void DisplayPro(void);

#endif

