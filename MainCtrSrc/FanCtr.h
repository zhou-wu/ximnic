#ifndef __FANCTR_H__
#define __FANCTR_H__

#define FanOFF    0
#define FanH      1
#define FanM      2
#define FanL      3
#define FanSL     4   //��΢��
#define FanSSL    5   //����΢��
#define FanBug    6   //��Ƶ����
#define FanHMIN   7   //������С����
#define FanHMID   8   //�����м����
#define FanHRAT   9   //���ȶ����
#define FanHMAX   10  //����������
#define FanH5     11
#define FanH6     12
#define FanH7     13
#define FanCMIN   14  //������С����
#define FanCMID   15  //�����м����
#define FanCRAT   16  //��������
#define FanCMAX   17  //����������
#define FanC5     18
#define FanC6     19
#define FanC7     20
#define FanRate   21

#define FREQLIM_FAN             (unFreqLimFlgs.bits.T2TempLLim || unFreqLimFlgs.bits.T2TempHLim || unFreqLimFlgs.bits.T3TempLim || unFreqLimFlgs.bits.T5TempLim ||unFreqLimFlgs.bits.CurrLim || unFreqLimFlgs.bits.VoltLim || unFreqLimFlgs.bits.IpmTempLim)
#define DC_FAN_DBG_MAX_SPD      1200        //debug set max speed 1500rpm
#define DC_FAN_DBG_MIN_SPD      200         //debug set min speed 320rpm  // modify  2015-04-07����Ҫ����ͷ��ת��150RPM

extern void FanInit(void);
extern void FanCtr(void);

extern unsigned char u8DownSpdTim;
extern unsigned short u16DgFanSpd;
extern unsigned short u16FanFBSpd;
extern unsigned short u16TrgFanSpd;

#endif


