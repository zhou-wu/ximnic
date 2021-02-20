#include "UsrDef.h"

ByteTypeDef unFixErrFlgs;
ByteTypeDef unFixSetFlgs;
ByteTypeDef unOutdoorChk;
union UNION_COMPERR_FLAG1 unCompErrFlgs1;
union UNION_COMPERR_FLAG2 unCompErrFlgs2;
union UNION_COMPRUN_FLAG1 unCompRunFlgs1;
union UNION_COMPRUN_FLAG2 unCompRunFlgs2;
union UNION_OUTDUST_FLAG01 unOutDustFlgs01;
union UNION_FREQLIMIT_FLAG unFreqLimFlgs;
ByteTypeDef unTempProtFlgs;
union UNION_IN_STATUS unInSta;
union UNION_OUT_STATUS unOutSta;
union UNION_DEVICE_FLAG unDevFlgs;
union UNION_SYSERR_FLAG unSysErrFlgs;
union UNION_TEMPERR_FLAG unTempErrFlgs;
ByteTypeDef unSysErrFlgs1;
union UNION_SYSERR_FLAG2 unSysErrFlgs2;
union UNION_SELT_FLAG4 unSeltFlgs4;
union UNION_SELT_FLAG5 unSeltFlgs5;
union UNION_SELT_FLAG1 unSeltFlgs1;
union UNION_SELT_FLAG2 unSeltFlgs2;
union UNION_SELT_FLAG3 unSeltFlgs3;
union UNION_SELT_FLAG6 unSeltFlgs6;
union UNION_SELT_FLAG7 unSeltFlgs7;
union UNION_SELT_FLAG8 unSeltFlgs8;
union UNION_FLAG unFlgs;
union UNION_FLAG1 unFlgs1;
union UNION_FLAG2 unFlgs2;
ByteTypeDef unFlgs3;
ByteTypeDef unInErr1;
ByteTypeDef unInErr2;
ByteTypeDef unRunTim;
ByteTypeDef unFreqProt;
ByteTypeDef unLimStop;
  
unsigned char u8TempT1;
unsigned char u8TempT2;
unsigned char u8TempT2b;
unsigned char u8TempT3;
unsigned char u8TempT4;
unsigned char u8TempT5;
unsigned char u8TempIpm;
unsigned char u8T1TempAd;   //T1 AD
unsigned char u8T2TempAd;   //T2 AD
unsigned char u8T3TempAd;   //T3 AD
unsigned char u8T4TempAd;   //T4 AD
unsigned char u8T5TempAd;   //TP AD
unsigned char u8IpmTempAd;  //IPM AD
unsigned char u8T2bTempAd;  //T2B AD
unsigned char u8IpmTempk1k2Ad;   //IPM k1k2 AD

unsigned char u8T2Freq60s;
unsigned char u8T2Prot5s;
unsigned char u8T3Prot5s;
unsigned char u8T3Freq60s;
unsigned char u8TempErrHold;
unsigned char u8ErrHoldTim;   //���й��Ͻ����30s�ָ���������״̬
unsigned char u8T5TempErrTim; //�����¶ȴ�����

//voltage
unsigned char u8DcVoltAd;
unsigned char u8AcVoltAd;
unsigned char u8VoltLimitTim;
unsigned char u8VoltLimPeriod;
unsigned char u8VoltLimArea;
unsigned char u8VoltStopTim;
unsigned char u8DcVoltDly;

//current
unsigned char u8AcCurrAd;
unsigned char u8CurrArea;    //������������

//press switch
unsigned char u8LowPressTim;
unsigned char u8HighPressTim;

//IPM Temperatrue
unsigned char u8IpmHighProtTim;
unsigned char u8IpmQuitProcTim;

//power
unsigned char u8TimPowerOn;
unsigned char u8PowerOnDly;
unsigned char u8PfcErrDly;

//compressor
unsigned char u8CompSta;
unsigned char u8CompDly;          //ѹ�������ͺ�̵����Ķ���ʱ��
unsigned short u16CompSpd;      //Ƶ�������ٶ� 100ms--
unsigned short u16CompRunTim;     //ѹ��������ʱ��
unsigned short u16CompStopTim;    //ѹ��������ʱ��

unsigned char u8Force25HZTim;
unsigned char u8PreHeatStopTim;
unsigned char u8RunCtrFreqDot;
unsigned char u8RealRunDot;
unsigned char u8PFCRunFlg;
unsigned char u8CompErrCnt, u8FmErrCnt;
unsigned short u16CompErrCntTim, u16FmErrCntTim;
unsigned char u8CompStopTim;
unsigned char *pU8TabChkAddr;
unsigned short *pU16TabChkAddr;
unsigned char u8PowerOnFlg;     //���̵�����־
unsigned char u8ErrStop;        //ѹ��ͣ��ԭ��
unsigned char u8CompStopTim;

//frequency
unsigned char u8MinRunFreq;
unsigned char u8MinForceFreq;   //ǿ���ܵ�Ƶ��
unsigned short u16MinFreqChangeT;

unsigned char u8MaxFreqLim;     //�����������Ƶ��
unsigned char u8MaxFreqArea;    //���Ƶ������

unsigned char u8FreqSet;        //Ŀ��Ƶ�ʵȼ�F0-F25
unsigned char u8RunFreqGrade;   //��ǰƵ�����еȼ� ͨ����ǰ����Ƶ��,ת���صȼ�F0-F25
unsigned char u8FreqAreaT3Min;  //�������3����,3min����һ���²�
unsigned char u8F1RunT60Min;    //j����,��F1����һ��Сʱ(������ϴ��ʱ����)

unsigned char u8T4MinFreqLim;
unsigned char u8LowFreqRateNum;

unsigned char u8EnergyFreq;      //����������Ҫ���ܵ�Ƶ��
unsigned char u8TargetFreq;      //���Ŀ��Ƶ��
unsigned char u8LimitFreq;       //�����Ƶ
unsigned char u8LastTrgFreq;
unsigned char u8RunCtrFreq;      //ʵ������Ŀ���Ƶ��
unsigned char u8RealRunFreq;     //ʵ���ܵ�Ƶ��
unsigned char u8RealRunInterger; //ѹ����ʵ������Ƶ��

//debug theta company
unsigned char u8DgTrgHz;
unsigned char u8DgTrgHzDot;
unsigned short u16DgInFanSpd;
unsigned char u8DgFWValve;
unsigned short u16DgTrgStepPls;  //���԰��趨����
unsigned char u8DgDefrost;
unsigned char u8DgCanLimit;

unsigned short u16ROLowFreqTim;   //��Ƶ��תʱ���ʱ
unsigned short u16ROFreqPlatTim;  //����ƽ̨����ʱ���ʱ

//4way
unsigned char u8ValveDlyTim;      //��ͨ����ʱ�ر�
unsigned char u8T2ErrCompRunTim;  //T2����ѹ������ʱ��

//electric heat
unsigned char u8HeatTime;         //����ȴ����ؼ�ʱ
unsigned char u8TrgHeatDuty;      //Ԥ��ռ�ձ�
unsigned char u8RunHeatDuty;      //����ռ�ձ�

//out fan
unsigned char u8StartFanDly;      //�����ʱ����
unsigned char u8StopFanDly;       //������ʱ30��ر�
unsigned char u8ACFanSwhDly;
unsigned char u8DCFanArea;
unsigned char u8Tim3Min;          //�ض�����,T1 ?T4 ������Χ3����,���˳�
unsigned char u8TrgFanOK;
unsigned char u8FanSta;

//dc fan
unsigned char u8TrgFan;           //0:���ͣ 1:�߷� 2:�з� 3:�ͷ� 5:���� 6:���ȶ 7:���԰���� 8:��С���� 9:������� 10:�м�����
unsigned char u8TrgFanBak;
unsigned char u8LowCoolTim;       //���������˳���ʱ

unsigned short u16TrgFanSpd311;   //����311�ķ���
unsigned short u16CurFanSpd311;
//unsigned short u16TrgFanSpd;    //���ֱ��DC�������,��ת���ɶ�ʱ���Ĵ���ֵ
unsigned short u16CurFanSpd0547;  //���תһȦ������ʱ��ֵ,��12�������,��100msΪ��ʱ��λ
unsigned short u16TrgPulseWidth;
unsigned char u8FanSpdFBPulse;
unsigned short u16DCFanSpdCnt;
unsigned char u8DCFanErrTim;
unsigned short u16CurFanSpeed;

//PmvCtrl
unsigned short u16DeltaPulse;     //Ŀ�꿪���뵱ǰ���ȵĲ�ֵ
unsigned short u16TrgPulse;       //Ŀ�꿪��
unsigned short u16CurrPulse;      //��ǰ����
unsigned char u8StepTim;          //�����ٶȼ�ʱ��,ÿms��ʱ��1
unsigned char u8PulseCnt;         //��������8�ļ���
unsigned char u8StepExcitTim;     //������,ֹͣ��,�۷���0.5s���ż�ʱ��,ÿ10ms��ʱ��1
unsigned char u8PmvRegulateTim;

unsigned char u8InSetMode;
unsigned char u8InSetModeBuf;
unsigned char u8InSetModeBufPre;
unsigned char u8InSetTemp;       //�趨�¶�
unsigned char u8InFanMode;       //�����趨����
unsigned char u8OutRunMode;      //��������ģʽ

//user set
unsigned char u8T1_TsArea;       //T1-TS�²�����
unsigned char u8T1_TsAreaH;      //T1-TS����ԭ����
unsigned char u8T1_TsAreaPre;    //ǰһ��T1-TS�²�����

//defrost
unsigned char u8DefrostExit;
unsigned short u16QuitDefrostTim;
unsigned short u16InDefrostTim;   // ѹ�����ۼ�����ʱ��
unsigned char u8DefrostDly3Min;   // С��I3������3���Ӽ�ʱ
unsigned char u8TempT3Min;        // T3��Сֵ
unsigned char u8T3MinDly;         // T3�ȽϿ�����ʱ��

//Device
unsigned char u8DispFlashTim;

signed short s16IPM_k2;
signed short s16IPM_b2;
signed short s16IPM_k1;
signed short s16IPM_b1;

