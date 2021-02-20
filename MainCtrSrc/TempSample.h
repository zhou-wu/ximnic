#ifndef __TEMP_SAMPLE_H__
#define __TEMP_SAMPLE_H__

#define  TEMP_OFFSET       (60)                      //AD��ƫ����,�ɱ�Ϊ30,�±�Ϊ60.Ŀ���ǽ��¶�ֵ�ĸ�����������������
#define  TEMP_MACRO(x)     (2*(x)+TEMP_OFFSET)       //xΪʵ���¶�ֵ
#define  TEMP_TO_VAL(x)    ((x-TEMP_OFFSET)>>1)
#define  dpad2_status()    (S12AD2.ADCSR.BIT.ADST)   /* ADA operation status */
#define  SAMPLE_BUFF_NUM   18
#define  ADCSAMPLE_FILTER  4//8//6

extern void TempSampleInit(void);
extern void TempSampleSrv(void);

extern unsigned char u8AdcTim;
extern unsigned char const TabAD2IpmTemp[];

#endif

