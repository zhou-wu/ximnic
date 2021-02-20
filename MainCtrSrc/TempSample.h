#ifndef __TEMP_SAMPLE_H__
#define __TEMP_SAMPLE_H__

#define  TEMP_OFFSET       (60)                      //AD表偏移量,旧表为30,新表为60.目的是将温度值的负数区拉至正数处理
#define  TEMP_MACRO(x)     (2*(x)+TEMP_OFFSET)       //x为实际温度值
#define  TEMP_TO_VAL(x)    ((x-TEMP_OFFSET)>>1)
#define  dpad2_status()    (S12AD2.ADCSR.BIT.ADST)   /* ADA operation status */
#define  SAMPLE_BUFF_NUM   18
#define  ADCSAMPLE_FILTER  4//8//6

extern void TempSampleInit(void);
extern void TempSampleSrv(void);

extern unsigned char u8AdcTim;
extern unsigned char const TabAD2IpmTemp[];

#endif

