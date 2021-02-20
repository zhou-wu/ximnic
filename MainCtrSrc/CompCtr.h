#ifndef __COMP_CTR_H__
#define __COMP_CTR_H__

#define   STOP_DELAY        50
#define   COMP_MAXFREQTAP   25

/* 驱动响应滞后主控, Leo */
#define   PLAT0_DRV_LAG_MAIN  100
#define   PLAT1_DRV_LAG_MAIN  0
#define   PLAT2_DRV_LAG_MAIN  0

extern void DefrostCtr(void);
extern void SeerDefrostCtr(void);
extern unsigned char AreaCompare(unsigned char u8AD, const unsigned char *TempTab, unsigned char Area);
extern void CompInit(void);
extern void CompCtr(void);
extern unsigned char LimitFreq(unsigned char freq);
extern unsigned char GetFreqGrade(unsigned char freq);

extern unsigned short u16CompAccel;
extern unsigned short u16CompDecel;
extern unsigned char u8SeerFreq;

#endif

