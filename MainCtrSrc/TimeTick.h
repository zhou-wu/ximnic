#ifndef __TIMETICK_H__
#define __TIMETICK_H__

//10ms timer base
#define TIME10MS_300MS      30
#define TIME10MS_400MS      40
#define TIME10MS_1S5        150
#define TIME10MS_2S         200
#define TIME10MS_3S         300
#define TIME10MS_10S        1000

//100ms timer base
#define TIME100MS_1S5       15
#define TIME100MS_2S        20
#define TIME100MS_2S5       25
#define TIME100MS_3S        30

//1s timer base
#define TIME1S_150S         150

//2s timer base
#define TIME2S_30MIN        900

//5s timer base
#define TIME5S_60MIN        720
#define TIME5S_5MIN         60

//10s timer base
#define TIME10S_90S         9
#define TIME10S_5MIN        30
#define TIME10S_7MIN        42
#define TIME10S_10MIN       60
#define TIME10S_12MIN       72
#define TIME10S_15MIN       90
#define TIME10S_20MIN       120
#define TIME10S_30MIN       180
#define TIME10S_40MIN       240
#define TIME10S_45MIN       270
#define TIME10S_55MIN       330
#define TIME10S_70MIN       420
#define TIME10S_95MIN       570
#define TIME10S_105MIN      630
#define TIME10S_120MIN      720

//1min timer base
#define TIME1M_100MIN       100

#define ERR_RST_TIM         30              //×î¶Ì30s»Ö¸´Ê±¼ä
#define FST_RUN_DELAY       3

extern void TimTickInit(void);
extern void TimTickPro(void);
#endif


