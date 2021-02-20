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
unsigned char u8ErrHoldTim;   //所有故障解除后30s恢复正常机器状态
unsigned char u8T5TempErrTim; //排气温度传感器

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
unsigned char u8CurrArea;    //电流保护区间

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
unsigned char u8CompDly;          //压缩机在滞后继电器的动作时间
unsigned short u16CompSpd;      //频率升降速度 100ms--
unsigned short u16CompRunTim;     //压缩机运行时间
unsigned short u16CompStopTim;    //压缩机保护时间

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
unsigned char u8PowerOnFlg;     //主继电器标志
unsigned char u8ErrStop;        //压机停机原因
unsigned char u8CompStopTim;

//frequency
unsigned char u8MinRunFreq;
unsigned char u8MinForceFreq;   //强制跑的频率
unsigned short u16MinFreqChangeT;

unsigned char u8MaxFreqLim;     //最大允许运行频率
unsigned char u8MaxFreqArea;    //最大频率区间

unsigned char u8FreqSet;        //目标频率等级F0-F25
unsigned char u8RunFreqGrade;   //当前频率运行等级 通过当前运行频率,转换回等级F0-F25
unsigned char u8FreqAreaT3Min;  //区间持续3分钟,3min计算一次温差
unsigned char u8F1RunT60Min;    //j区间,以F1运行一个小时(和自清洗计时共用)

unsigned char u8T4MinFreqLim;
unsigned char u8LowFreqRateNum;

unsigned char u8EnergyFreq;      //能需计算出来要求跑的频率
unsigned char u8TargetFreq;      //外机目标频率
unsigned char u8LimitFreq;       //外机限频
unsigned char u8LastTrgFreq;
unsigned char u8RunCtrFreq;      //实际输出的控制频率
unsigned char u8RealRunFreq;     //实际跑的频率
unsigned char u8RealRunInterger; //压缩机实际运行频率

//debug theta company
unsigned char u8DgTrgHz;
unsigned char u8DgTrgHzDot;
unsigned short u16DgInFanSpd;
unsigned char u8DgFWValve;
unsigned short u16DgTrgStepPls;  //调试板设定开度
unsigned char u8DgDefrost;
unsigned char u8DgCanLimit;

unsigned short u16ROLowFreqTim;   //低频运转时间计时
unsigned short u16ROFreqPlatTim;  //回油平台运行时间计时

//4way
unsigned char u8ValveDlyTim;      //四通阀延时关闭
unsigned char u8T2ErrCompRunTim;  //T2故障压机运行时间

//electric heat
unsigned char u8HeatTime;         //电加热带开关计时
unsigned char u8TrgHeatDuty;      //预热占空比
unsigned char u8RunHeatDuty;      //运行占空比

//out fan
unsigned char u8StartFanDly;      //风机延时启动
unsigned char u8StopFanDly;       //外风机延时30秒关闭
unsigned char u8ACFanSwhDly;
unsigned char u8DCFanArea;
unsigned char u8Tim3Min;          //特定工况,T1 ?T4 超出范围3分钟,则退出
unsigned char u8TrgFanOK;
unsigned char u8FanSta;

//dc fan
unsigned char u8TrgFan;           //0:风机停 1:高风 2:中风 3:低风 5:制冷额定 6:制热额定 7:调试板风速 8:最小制冷 9:最大制冷 10:中间制冷
unsigned char u8TrgFanBak;
unsigned char u8LowCoolTim;       //低温制冷退出计时

unsigned short u16TrgFanSpd311;   //发给311的风速
unsigned short u16CurFanSpd311;
//unsigned short u16TrgFanSpd;    //外机直驱DC电机风速,已转换成定时器的触发值
unsigned short u16CurFanSpd0547;  //电机转一圈经历过时间值,按12脉冲计算,以100ms为计时单位
unsigned short u16TrgPulseWidth;
unsigned char u8FanSpdFBPulse;
unsigned short u16DCFanSpdCnt;
unsigned char u8DCFanErrTim;
unsigned short u16CurFanSpeed;

//PmvCtrl
unsigned short u16DeltaPulse;     //目标开度与当前开度的差值
unsigned short u16TrgPulse;       //目标开度
unsigned short u16CurrPulse;      //当前开度
unsigned char u8StepTim;          //励磁速度计时器,每ms计时加1
unsigned char u8PulseCnt;         //步进四相8拍计数
unsigned char u8StepExcitTim;     //结束相,停止相,折返相0.5s励磁计时器,每10ms计时加1
unsigned char u8PmvRegulateTim;

unsigned char u8InSetMode;
unsigned char u8InSetModeBuf;
unsigned char u8InSetModeBufPre;
unsigned char u8InSetTemp;       //设定温度
unsigned char u8InFanMode;       //室内设定风速
unsigned char u8OutRunMode;      //室外运行模式

//user set
unsigned char u8T1_TsArea;       //T1-TS温差区间
unsigned char u8T1_TsAreaH;      //T1-TS制热原区间
unsigned char u8T1_TsAreaPre;    //前一个T1-TS温差区间

//defrost
unsigned char u8DefrostExit;
unsigned short u16QuitDefrostTim;
unsigned short u16InDefrostTim;   // 压缩机累计运行时间
unsigned char u8DefrostDly3Min;   // 小于I3的连续3分钟计时
unsigned char u8TempT3Min;        // T3最小值
unsigned char u8T3MinDly;         // T3比较抗干扰时间

//Device
unsigned char u8DispFlashTim;

signed short s16IPM_k2;
signed short s16IPM_b2;
signed short s16IPM_k1;
signed short s16IPM_b1;

