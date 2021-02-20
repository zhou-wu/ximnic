#ifndef __DEV_CTR_H__
#define __DEV_CTR_H__

#define U8_IPM_TEMP2VALUE(X)    ((X)+(X))

//for main control
#define ChassisPTC_PORT_OUT     u8MacroTmp=0 ///(PORTD.PDR.BIT.B4=1)
#define ChassisPTC_ON           u8MacroTmp=0 ///do{PORTD.PODR.BIT.B4=1;ChassisPTCSta=1;}while(0)
#define ChassisPTC_OFF          u8MacroTmp=0 ///do{PORTD.PODR.BIT.B4=0;ChassisPTCSta=0;}while(0)

//Power
#define POWER_PORT_OUT          ///(PORT0.PDR.BIT.B0=1)     ///需要重新移植
#define POWER_ON                ///(PORT0.PODR.BIT.B0=1)    ///需要重新移植
#define POWER_OFF               ///(PORT0.PODR.BIT.B0=0)    ///需要重新移植

//4 way
//#define FOUR_WAY_PORT_OUT       (PORT1.PDR.BIT.B1=1)
#define FOUR_WAY_PORT_OUT       u8MacroTmp=0 ///do{INTI_4WAY_PORT();}while(0)
//#define FOUR_WAY_ON           do{PORT1.PODR.BIT.B1=1;fgFWVSta=1;}while(0)
#define FOUR_WAY_ON             u8MacroTmp=0 ///do{OUT_4WAY_HIGH();fgFWVSta=1;}while(0)
//#define FOUR_WAY_OFF          do{PORT1.PODR.BIT.B1=0;fgFWVSta=0;}while(0)
#define FOUR_WAY_OFF            u8MacroTmp=0 ///do{OUT_4WAY_LOW();fgFWVSta=0;}while(0)

#define CompressPTC_ON          u8MacroTmp=0 ///(PORTD.PODR.BIT.B4=1)

//AC Fan control
#define AC_FAN_HI_ON            u8MacroTmp=0 ///(PORT9.PODR.BIT.B4=1)
#define AC_FAN_LOW_ON           u8MacroTmp=0 ///(PORT9.PODR.BIT.B5=1)

//DgLed
#define DgLedOn()                u8MacroTmp=0 ///OUT_LED_LOW()
#define DgLedOff()               u8MacroTmp=0 ///OUT_LED_HIGH()


///extern void MainCtrInit(void);
extern void MainCtrTaskPro(void);

extern unsigned char u8WaitPFCTim;
extern unsigned char u8InRateTestPre;

#endif


