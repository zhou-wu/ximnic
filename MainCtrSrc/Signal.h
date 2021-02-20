#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define PROTECT_T5_HIGH     115
#define PROTECT_T5_LIMT     107
#define PROTECT_T5_KEEP     10
#define PROTECT_T5_RST      90

#define PROTECT_T2_COOL_LOW       TEMP_MACRO(0)
#define PROTECT_T2_COOL_RST       TEMP_MACRO(5)
#define PROTECT_T2_COOL_LIMT      TEMP_MACRO(4)
#define PROTECT_T2_COOL_LIMTRST   TEMP_MACRO(7)

#define PROTECT_T2_HEAT_HIGH      TEMP_MACRO(65)
#define PROTECT_T2_HEAT_DEC       TEMP_MACRO(55)

#define PROTECT_T23_HEAT_HIGH     TEMP_MACRO(65)

extern void SignalInit(void);
extern void SignalSrv(void);
extern void ProtTP(void);
extern void ProtT2Cool(void);
extern void ProtT2Heat(void);
extern void ProtT3Cool(void); ///

extern unsigned char u8HalfDcVoltFil;
extern unsigned char u8AcCurrRMS_T10;
extern unsigned char u8HalfDcVoltRMS;


#endif

