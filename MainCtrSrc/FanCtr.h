#ifndef __FANCTR_H__
#define __FANCTR_H__

#define FanOFF    0
#define FanH      1
#define FanM      2
#define FanL      3
#define FanSL     4   //超微风
#define FanSSL    5   //超超微风
#define FanBug    6   //锁频风速
#define FanHMIN   7   //制热最小风速
#define FanHMID   8   //制热中间风速
#define FanHRAT   9   //制热额定风速
#define FanHMAX   10  //制热最大风速
#define FanH5     11
#define FanH6     12
#define FanH7     13
#define FanCMIN   14  //制冷最小风速
#define FanCMID   15  //制冷中间风速
#define FanCRAT   16  //制冷额定风速
#define FanCMAX   17  //制冷最大风速
#define FanC5     18
#define FanC6     19
#define FanC7     20
#define FanRate   21

#define FREQLIM_FAN             (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim || unFreqLimFlgs.bits.T3TempLim || unFreqLimFlgs.bits.T5TempLim ||unFreqLimFlgs.bits.CurrLim || unFreqLimFlgs.bits.VoltLim || unFreqLimFlgs.bits.IpmTempLim)
#define DC_FAN_DBG_MAX_SPD      1200        //debug set max speed 1500rpm
#define DC_FAN_DBG_MIN_SPD      200         //debug set min speed 320rpm  // modify  2015-04-07海外要求最低风机转速150RPM

extern void FanInit(void);
extern void FanCtr(void);

extern unsigned char u8DownSpdTim;
extern unsigned short u16DgFanSpd;
extern unsigned short u16FanFBSpd;
extern unsigned short u16TrgFanSpd;

#endif


