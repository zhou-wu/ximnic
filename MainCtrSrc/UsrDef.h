#ifndef __USRDEF_H__
#define __USRDEF_H__

#define COMP_STOP     0
#define COMP_RUN      1
 
#define SeerTestNo    0x00
#define SeerTestMin   0x01
#define SeerTestMid   0x02
#define SeerTestRat   0x03
#define SeerTestMax   0x04
#define SeerTest5     0x05
#define SeerTest6     0x06
#define SeerTest7     0x07

#define C_ModeOff         0
#define C_ModeCool        1
#define C_ModeHeat        2
#define C_ModeFan         3
#define C_ModeDry         4
//#define                 5
#define C_ModeForce       6
#define C_ModeDefrost     7
#define C_ModeForceHeat   8

//indoor fan:注意内风机的宏与电流环通信的格式
#define C_InFanTurbo  7
#define C_InFanSH     6
#define C_InFanH      5
#define C_InFanM      4
#define C_InFanMM     3
#define C_InFanL      2
#define C_InFanSL     1     //静音
#define C_InFanOff    0

#define   MINUTE_MACRO(x)   (60*x)              //以s为计时单位的分钟

typedef struct BIT_STRU
{
  unsigned char no0: 1;
  unsigned char no1: 1;
  unsigned char no2: 1;
  unsigned char no3: 1;
  unsigned char no4: 1;
  unsigned char no5: 1;
  unsigned char no6: 1;
  unsigned char no7: 1;
} BitsTypeDef;

typedef union UNION_STRU
{
  unsigned char u8Byte;
  BitsTypeDef bits;
}ByteTypeDef;

union UNION_TEMPERR_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char T1Err     : 1;    //T1
    unsigned char T2Err     : 1;    //T2
    unsigned char T3Err     : 1;    //TcTemp,T3, Leo
    unsigned char T4Err     : 1;    //ToTemp,T4, Leo
    unsigned char T5Err     : 1;    //TdTemp,TP, Leo
    unsigned char TsErr     : 1;    //TS 回气传感器
    unsigned char TipmErr   : 1;    //T6 IPM
    unsigned char T2bErr    : 1;    //T2B
  } bits;
} ;
extern union UNION_TEMPERR_FLAG unTempErrFlgs;

extern ByteTypeDef unFixErrFlgs;
#define u8FixErrFlgs    unFixErrFlgs.u8Byte
#define fgTpProtFix     unFixErrFlgs.bits.no0
#define fgCompErrFix    unFixErrFlgs.bits.no1
#define fgFmErrFix      unFixErrFlgs.bits.no3
#define fgSysOvCurrFix  unFixErrFlgs.bits.no5

extern ByteTypeDef unFixSetFlgs;
#define fgCompErrSet    unFixSetFlgs.bits.no0
#define fgTpProtSet     unFixSetFlgs.bits.no1
#define fgFmErrSet      unFixSetFlgs.bits.no2
#define fgSysOvCurrSet  unFixSetFlgs.bits.no4

extern ByteTypeDef unOutdoorChk;
#define fgMachChk        unOutdoorChk.bits.no0
#define fgPCBAChk        unOutdoorChk.bits.no1
#define fgShortTim       unOutdoorChk.bits.no2
#define fgNoPlat         unOutdoorChk.bits.no3

union UNION_COMPERR_FLAG1
{
  unsigned char u8Byte;
  struct
  {
    unsigned char FoErr       : 1;
    unsigned char PhsLossErr  : 1;
    unsigned char SpeedErr    : 1;
    unsigned char LoVoltErr   : 1;
    unsigned char FanCurrent  : 1;
    unsigned char FanPhsLoss  : 1;
    unsigned char FanZer0     : 1;
    unsigned char PfcErr      : 1;
  } bits;
} ;
extern union UNION_COMPERR_FLAG1 unCompErrFlgs1;

union UNION_COMPERR_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char OvVoltErr   : 1;
    unsigned char ZeroSpdErr  : 1;
    unsigned char VoltDrop    : 1;
    unsigned char MCEErr      : 1;
    unsigned char CpOvCurrErr : 1;
    unsigned char E2Err       : 1;
    unsigned char StartErr    : 1;
    unsigned char FanErr      : 1;    //FanErr
  } bits;
} ;
extern union UNION_COMPERR_FLAG2 unCompErrFlgs2;

union UNION_COMPRUN_FLAG1
{
  unsigned char u8Byte;
  struct
  {
    unsigned char no0          : 1;
    unsigned char no1          : 1;
    unsigned char no2          : 1;
    unsigned char no3          : 1;
    unsigned char no4          : 1;
    unsigned char no5          : 1;
    unsigned char no6          : 1;
    unsigned char CompErrSta   : 1;
  } bits;
} ;
extern union UNION_COMPRUN_FLAG1 unCompRunFlgs1;
#define fgCompErrSta    unCompRunFlgs1.bits.CompErrSta

union UNION_COMPRUN_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char ErrT2Lim      : 1;  //T2故障限频标志
    unsigned char DefrostIn     : 1;  //1 频率已经降到F3
    unsigned char NoPlat        : 1;  //1:取消平台
    unsigned char CompForce7Min : 1;  //原CompForce7Min - 压缩机强制运行7分钟
    unsigned char FirstInHeat   : 1;  //第一次进入制热
    unsigned char FirstInCool   : 1;  //第一次进入制冷
    unsigned char DefrostExit   : 1;  //化霜退出
    unsigned char RetOil        : 1;
  } bits;
} ;
extern union UNION_COMPRUN_FLAG2 unCompRunFlgs2;

union UNION_OUTDUST_FLAG01
{
  unsigned char u8Byte;
  struct
  {
    unsigned char OutDuSta      : 1;
    unsigned char ODOutFanOk    : 1;
    unsigned char ODOutFanF1    : 1;
    unsigned char ODOutFanF2    : 1;
    unsigned char OutDustEn     : 1;
    unsigned char ODStartedF    : 1;
    unsigned char no6           : 1;
    unsigned char no7           : 1;
  } bits;
};
extern union UNION_OUTDUST_FLAG01 unOutDustFlgs01;

union UNION_FREQLIMIT_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char VoltLim     : 1; //电压限频
    unsigned char CurrLim     : 1; //电流限频
    unsigned char T5TempLim   : 1; //排气限频,TdTempLim, Leo
    unsigned char T3TempLim   : 1; //冷凝器温度限频 TcTempLim, Leo
    unsigned char T2TempLLim  : 1; //蒸发器低温限频 TeTempCLim, Leo
    unsigned char T2TempHLim  : 1; //蒸发器高温限频 TeTempHLim, Leo
    unsigned char IpmTempLim  : 1; //模块高温限频, TmTempLim, Leo
    unsigned char no7         : 1;
  } bits;
} ;
extern union UNION_FREQLIMIT_FLAG unFreqLimFlgs;

extern ByteTypeDef unTempProtFlgs;
#define ucTempProtFlgs      unTempProtFlgs.u8Byte
#define fgDryT1LowPro       unTempProtFlgs.bits.no0  //抽湿 室温低温保护
#define fgCoolT4HighPro     unTempProtFlgs.bits.no1
#define fgHeatT4HighPro     unTempProtFlgs.bits.no2
#define fgT1TSStop          unTempProtFlgs.bits.no3  //达温停机
#define fgT4Freq0Stop       unTempProtFlgs.bits.no4
#define fProtHeatAZone      unTempProtFlgs.bits.no5

union UNION_IN_STATUS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char FastTest    : 1;
    unsigned char PowerRun    : 1;    //强制运行
    unsigned char SleepRun    : 1;    //睡眠模式
    unsigned char RuiFan      : 1;    //室外低温保护
    unsigned char RationRun   : 1;    //额定测试
    unsigned char SelfChk     : 1;    //自检
    unsigned char no6         : 1;
    unsigned char no7         : 1;
  } bits;
} ;
extern union UNION_IN_STATUS unInSta;

union UNION_OUT_STATUS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char Comp        : 1;
    unsigned char FWValve     : 1;  //四通阀
    unsigned char HighFan     : 1;
    unsigned char MidFan      : 1;
    unsigned char LowFan      : 1;
    unsigned char AirFanH     : 1;
    unsigned char AirFanL     : 1;
    unsigned char no7         : 1;
  } bits;
} ;
extern union UNION_OUT_STATUS unOutSta;

union UNION_DEVICE_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char DefrostFirst    : 1;      //强制化霜,第一次启动压缩机,把时间清零
    unsigned char DefrostOnce     : 1;      //接收强制化霜,只接收1次
    unsigned char FreRunDisable   : 1;      //DR 检测T4
    unsigned char Fre05           : 1;      //1: 0.5hz
    unsigned char OneNight        : 1;      //1: 1晚1度电
    unsigned char Defrost7DW      : 1;      //1: 7度以下进入化霜
    unsigned char Defrost7UP      : 1;      //1: 7度以上进入化霜
    unsigned char LowCoolFanEn    : 1;
  } bits;
} ;
extern union UNION_DEVICE_FLAG unDevFlgs;

union UNION_SYSERR_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char OverCurrErr   : 1;
    unsigned char DrvCommErr    : 1;
    unsigned char no2           : 1;
    unsigned char I2cErr        : 1;
    unsigned char TempErr       : 1;
    unsigned char ACVoltLowErr  : 1;
    unsigned char DCFanSpdErr   : 1;
    unsigned char InOutCommErr  : 1;
  } bits;
} ;
extern union UNION_SYSERR_FLAG unSysErrFlgs;

extern ByteTypeDef unSysErrFlgs1;
#define fgIPMProt    unSysErrFlgs1.bits.no5
#define fgLowPress   unSysErrFlgs1.bits.no6
#define fgHighPress  unSysErrFlgs1.bits.no7

union UNION_SYSERR_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char ProtLP    : 1;
    unsigned char ProtHP    : 1;
    unsigned char PfcErr    : 1;
    unsigned char no3       : 1;
    unsigned char no4       : 1;
    unsigned char no5       : 1;
    unsigned char no6       : 1;
    unsigned char no7       : 1;
  } bits;
} ;
extern union UNION_SYSERR_FLAG2 unSysErrFlgs2;

union UNION_SELT_FLAG4
{
  unsigned char u8Byte;
  struct
  {
    unsigned char CTestACFanA   : 1;    //制冷测试模式A (最小制冷)  0:低风档 1:高风档
    unsigned char CTestACFanB   : 1;    //制冷测试模式B (中间制冷)  0:低风档 1:高风档
    unsigned char CTestACFanC   : 1;    //制冷测试模式C (额定制冷)  0:低风档 1:高风档
    unsigned char CTestACFanD   : 1;    //制冷测试模式D (最大制冷)  0:低风档 1:高风档
    unsigned char CTestACFanE   : 1;    //制冷测试模式E       0:低风档 1:高风档
    unsigned char CTestACFanF   : 1;    //制冷测试模式F 0:低风档 1:高风档
    unsigned char CTestACFanG   : 1;    //制冷测试模式G       0:低风档 1:高风档
    unsigned char no7           : 1;
  } bits;  //OutACFanCtrl
} ;
extern union UNION_SELT_FLAG4 unSeltFlgs4;

union UNION_SELT_FLAG5
{
  unsigned char u8Byte;
  struct
  {
    unsigned char HTestACFanA   : 1;    //制热测试模式A (最小制热)  0:低风档 1:高风档
    unsigned char HTestACFanB   : 1;    //制热测试模式B (中间制热)  0:低风档 1:高风档
    unsigned char HTestACFanC   : 1;    //制热测试模式C (额定制热)  0:低风档 1:高风档
    unsigned char HTestACFanD   : 1;    //制热测试模式D (最小制热)  0:低风档 1:高风档
    unsigned char HTestACFanE   : 1;    //制热测试模式E       0:低风档 1:高风档
    unsigned char HTestACFanF   : 1;    //制热测试模式F 0:低风档 1:高风档
    unsigned char HTestACFanG   : 1;    //制热测试模式G       0:低风档 1:高风档
    unsigned char no7           : 1;
  } bits;  //OutACFanCtrl
} ;
extern union UNION_SELT_FLAG5 unSeltFlgs5;

union UNION_SELT_FLAG1
{
  unsigned char u8Byte;
  struct
  {
    unsigned char FanSelt       : 1;    //0--AC fan,1--DC fan
    unsigned char ACFanSelt     : 1;
    unsigned char VLimSelt      : 1;
    unsigned char ACVoltSelt    : 1;
    unsigned char LCoolSelt     : 1;    //低温制冷功能选择
    unsigned char no5           : 1;    //
    unsigned char RetOilSelt    : 1;    //回油功能选择
    unsigned char PMVSelt       : 1;
  } bits;  //OutFanCtrl
} ;
extern union UNION_SELT_FLAG1 unSeltFlgs1;

union UNION_SELT_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char PmvCfgDir   : 1;    // 0: 膨胀阀正向     1:膨胀阀反向
    unsigned char PfcErrSel   : 1;    // 0: 允许判断PFC故障
    unsigned char FreMinRun   : 1;    // 0: 允许高低频运行功能
    unsigned char CompProTime : 1;    // 0: 制热3分钟保护  1:四分钟保护
    unsigned char TorqueOFF   : 1;    // 能力测试转矩控制,为1表示关闭,为0表示允许转矩补偿开
    unsigned char ValveDir    : 1;    // 0: 四通阀正向       1: 四通阀反向
    unsigned char CoolOnly    : 1;    // 0: 单冷            1: 冷暖
    unsigned char DcFanDrv    : 1;    // 0: 主芯片控制       1: 311驱动直流风机
  } bits;
} ;
extern union UNION_SELT_FLAG2 unSeltFlgs2;

union UNION_SELT_FLAG3
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerPFCA_OFF    : 1;  //  制冷测试模式A (最小制冷)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCB_OFF    : 1;  //  制冷测试模式B (中间制冷)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCC_OFF    : 1;  //  制冷测试模式C (额定制冷)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCD_OFF    : 1;  //  制冷测试模式D (最大制冷)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCE_OFF    : 1;  //  制冷测试模式E 0:打开PFC 1:关闭PFC
    unsigned char SeerPFCF_OFF    : 1;  //  制冷测试模式F 0:打开PFC 1:关闭PFC
    unsigned char SeerPFCG_OFF    : 1;  //  制冷测试模式G 0:打开PFC 1:关闭PFC
    unsigned char no7             : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG3 unSeltFlgs3;

union UNION_SELT_FLAG6
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerPFCA_OFF    : 1;  //  制热测试模式A (最小制热)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCB_OFF    : 1;  //  制热测试模式B (中间制热)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCC_OFF    : 1;  //  制热测试模式C (额定制热)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCD_OFF    : 1;  //  制热测试模式D (最大制热)  0:打开PFC 1:关闭PFC
    unsigned char SeerPFCE_OFF    : 1;  //  制热测试模式E 0:打开PFC 1:关闭PFC
    unsigned char SeerPFCF_OFF    : 1;  //  制热测试模式F 0:打开PFC 1:关闭PFC
    unsigned char SeerPFCG_OFF    : 1;  //  制热测试模式G 0:打开PFC 1:关闭PFC
    unsigned char no7             : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG6 unSeltFlgs6;

union UNION_SELT_FLAG7
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerDefreostA_OFF   : 1;  //  制冷测试模式A (最小制冷)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostB_OFF   : 1;  //  制冷测试模式B (中间制冷)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostC_OFF   : 1;  //  制冷测试模式C (额定制冷)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostD_OFF   : 1;  //  制冷测试模式D (最大制冷)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostE_OFF   : 1;  //  制冷测试模式E 0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostF_OFF   : 1;  //  制冷测试模式F 0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostG_OFF   : 1;  //  制冷测试模式G 0:使能除霜  1:禁止除霜
    unsigned char no7                 : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG7 unSeltFlgs7;

union UNION_SELT_FLAG8
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerDefreostA_OFF   : 1;  //  制热测试模式A (最小制热)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostB_OFF   : 1;  //  制热测试模式B (中间制热)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostC_OFF   : 1;  //  制热测试模式C (额定制热)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostD_OFF   : 1;  //  制热测试模式D (最大制热)  0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostE_OFF   : 1;  //  制热测试模式E 0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostF_OFF   : 1;  //  制热测试模式F 0:使能除霜  1:禁止除霜
    unsigned char SeerDefreostG_OFF   : 1;  //  制热测试模式G 0:使能除霜  1:禁止除霜
    unsigned char no7                 : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG8 unSeltFlgs8;

union UNION_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char OutFanCtr     : 1;
    unsigned char CompErrClr    : 1;
    unsigned char I2cProt       : 1;
    unsigned char DCFanOn       : 1;
    unsigned char FanSpdAdj     : 1;
    unsigned char DCFanCtr      : 1;
    unsigned char PTCCtr        : 1;
    unsigned char FWVCtr        : 1;
  } bits;
} ;
extern union UNION_FLAG unFlgs;

union UNION_FLAG1
{
  unsigned char u8Byte;
  struct
  {
    unsigned char AirFanHCtr    : 1;
    unsigned char AirFanLCtr    : 1;
    unsigned char Plat0RunOk    : 1;
    unsigned char ChassisHeat   : 1;
    unsigned char PmvRunDir     : 1;
    unsigned char PlatRun       : 1;
    unsigned char Plat2RunOk    : 1;
    unsigned char Plat1RunOk    : 1;
  } bits;
} ;
extern union UNION_FLAG1 unFlgs1;

union UNION_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char LowPressEnable  : 1;
    unsigned char NoLockMode      : 1;      // 1: 暂时取消小板锁定功能 化霜及化霜结束30S内
    unsigned char ParaFinish      : 1;      // 1: 根据拨码成功读取机型参数标志
    unsigned char Defrost         : 1;      // 1: 化霜标志
    unsigned char DCVlowPRO       : 1;      // 1: 直流电压过低
    unsigned char PTCCtrlBak      : 1;
    unsigned char FWVCtrlBak      : 1;
    unsigned char PmvReset        : 1;
  } bits;
} ;
extern union UNION_FLAG2 unFlgs2;

extern ByteTypeDef unFlgs3;
#define u8Flgs3       unFlgs3.u8Byte
#define fgFWVSta      unFlgs3.bits.no0
#define ChassisPTCSta unFlgs3.bits.no1

extern ByteTypeDef unInErr1;
#define u8InErr1      unInErr1.u8Byte
#define fgErrInFan    unInErr1.bits.no0 //内机风机失速故障
#define fgErrInEE     unInErr1.bits.no1 //室内EE故障

extern ByteTypeDef unInErr2;
#define u8InErr2       unInErr2.u8Byte
#define fgErrT3        unInErr2.bits.no0 //T3故障
#define fgErrT2        unInErr2.bits.no1 //T2传感器故障//T2B传感器故障
#define fgErrT2b       unInErr2.bits.no1 //T2传感器故障//T2B传感器故障
#define fgErrT1        unInErr2.bits.no2 //T1传感器故障
#define fgErrZero      unInErr2.bits.no3 //内机过零
#define fgInOutCommErr unInErr2.bits.no4 //室内外机通信故障
#define fgWaterFull    unInErr2.bits.no5 //水满保护
#define fgProtInT2H    unInErr2.bits.no6 //蒸发器高温保护
#define fgProtInT2L    unInErr2.bits.no7 //蒸发器防冻结保护

extern ByteTypeDef unRunTime;
#define fgT10ms     unRunTime.bits.no0
#define fgT20ms     unRunTime.bits.no1
#define fgT50ms     unRunTime.bits.no2
#define fgT100ms    unRunTime.bits.no3
#define fgT200ms    unRunTime.bits.no4
#define fgT500ms    unRunTime.bits.no5
#define fgT1sec     unRunTime.bits.no6
#define fgT1min     unRunTime.bits.no7

extern ByteTypeDef unFreqProt;                   //故障保护停机标志
#define fgProtT5        unFreqProt.bits.no0      //排气温度保护
#define fgProtHeatT2    unFreqProt.bits.no1      //蒸发器T2高温保护
#define fgProtCoolT2    unFreqProt.bits.no2      //蒸发器T2防冻结保护
#define fgProtCoolT3    unFreqProt.bits.no3      //冷凝器高温保护标志,压缩机停止
#define fgProtT1        unFreqProt.bits.no4      //抽湿,室温低温保护
#define fgErrT4Low      unFreqProt.bits.no5      //室外低温保护
#define fgCompFault     unFreqProt.bits.no6      //冷媒检测保护
#define fgProtErr       unFreqProt.bits.no7      //其他故障或则保护,要求停压缩机的

extern ByteTypeDef unLimStop;                    //限频停机
#define fgCurrentStop     unLimStop.bits.no0     //限流限频停机
#define fgCoolT2Stop      unLimStop.bits.no1     //制冷T2限频停机
#define fgHeatT2Stop      unLimStop.bits.no2     //制热T2限频停机
#define fgT1TsStop        unLimStop.bits.no3     //达温停机
#define fgT4LimStop       unLimStop.bits.no4     //T4限频停机
#define fgDefrostStop     unLimStop.bits.no5     //化霜停机
#define fgModeChangeStop  unLimStop.bits.no6     //模式切换停机
//#define fgDefrostStop   unLimStop.bits.no7     //化霜停机

extern unsigned char u8TempT1;
extern unsigned char u8TempT2;
extern unsigned char u8TempT2b;
extern unsigned char u8TempT3;
extern unsigned char u8TempT4;
extern unsigned char u8TempT5;
extern unsigned char u8TempIpm;
extern unsigned char u8T1TempAd;   //T1 AD
extern unsigned char u8T2TempAd;   //T2 AD
extern unsigned char u8T3TempAd;   //T3 AD
extern unsigned char u8T4TempAd;   //T4 AD
extern unsigned char u8T5TempAd;   //TP AD
extern unsigned char u8IpmTempAd;  //IPM AD
extern unsigned char u8T2bTempAd;  //T2B AD
extern unsigned char u8IpmTempk1k2Ad;//IPM k1k2 AD

extern unsigned char u8T2Freq60s;
extern unsigned char u8T2Prot5s;
extern unsigned char u8T3Prot5s;
extern unsigned char u8T3Freq60s;
extern unsigned char u8TempErrHold;
extern unsigned char u8ErrHoldTim;   //所有故障解除后30s恢复正常机器状态
extern unsigned char u8T5TempErrTim; //排气温度传感器

//votage
extern unsigned char u8DcVoltAd;
extern unsigned char u8AcVoltAd;
extern unsigned char u8VoltLimitTim;
extern unsigned char u8VoltLimPeriod;
extern unsigned char u8VoltLimArea;
extern unsigned char u8VoltStopTim;
extern unsigned char u8DcVoltDly;

//current
extern unsigned char u8AcCurrAd;
extern unsigned char u8CurrArea;    //电流保护区间

 //press switch
extern unsigned char u8LowPressTim;
extern unsigned char u8HighPressTim;

//IPM Temperatrue
extern unsigned char u8IpmHighProtTim;
extern unsigned char u8IpmQuitProcTim;

//power
extern unsigned char u8TimPowerOn;
extern unsigned char u8PowerOnDly;
extern unsigned char u8PfcErrDly;

//compressor
extern unsigned char u8CompSta;
extern unsigned char u8CompDly;          //压缩机在滞后继电器的动作时间
extern unsigned short u16CompSpd;        //频率升降速度 100ms--
extern unsigned short u16CompRunTim;     //压缩机运行时间
extern unsigned short u16CompStopTim;    //压缩机保护时间

extern unsigned char u8Force25HZTim;
extern unsigned char u8PreHeatStopTim;
extern unsigned char u8RunCtrFreqDot;
extern unsigned char u8RealRunDot;
extern unsigned char u8PFCRunFlg;
extern unsigned char u8CompErrCnt, u8FmErrCnt;
extern unsigned short u16CompErrCntTim, u16FmErrCntTim;
extern unsigned char u8CompStopTim;
extern unsigned char *pU8TabChkAddr;
extern unsigned short *pU16TabChkAddr;
extern unsigned char u8PowerOnFlg;     //主继电器标志
extern unsigned char u8ErrStop;        //压机停机原因
extern unsigned char u8CompStopTim;

//frequency
extern unsigned char u8MinRunFreq;
extern unsigned char u8MinForceFreq;   //强制跑的频率
extern unsigned short u16MinFreqChangeT;

extern unsigned char u8MaxFreqLim;     //最大允许运行频率
extern unsigned char u8MaxFreqArea;    //最大频率区间

extern unsigned char u8FreqSet;        //目标频率等级F0-F25
extern unsigned char u8RunFreqGrade;   //当前频率运行等级 通过当前运行频率,转换回等级F0-F25
extern unsigned char u8FreqAreaT3Min;  //区间持续3分钟,3min计算一次温差
extern unsigned char u8F1RunT60Min;    //j区间,以F1运行一个小时(和自清洗计时共用)

extern unsigned char u8T4MinFreqLim;
extern unsigned char u8LowFreqRateNum;

extern unsigned char u8EnergyFreq;      //能需计算出来要求跑的频率
extern unsigned char u8TargetFreq;      //外机目标频率
extern unsigned char u8LimitFreq;       //外机限频
extern unsigned char u8LastTrgFreq;
extern unsigned char u8RunCtrFreq;      //实际输出的控制频率
extern unsigned char u8RealRunFreq;     //实际跑的频率
extern unsigned char u8RealRunInterger; //压缩机实际运行频率

//debug theta company
extern unsigned char u8DgTrgHz;
extern unsigned char u8DgTrgHzDot;
extern unsigned short u16DgInFanSpd;
extern unsigned char u8DgFWValve;
extern unsigned short u16DgTrgStepPls;   //调试板设定开度
extern unsigned char u8DgDefrost;
extern unsigned char u8DgCanLimit;

extern unsigned short u16ROLowFreqTim;   //低频运转时间计时
extern unsigned short u16ROFreqPlatTim;  //回油平台运行时间计时

//4way
extern unsigned char u8ValveDlyTim;      //四通阀延时关闭
extern unsigned char u8T2ErrCompRunTim;  //T2故障压机运行时间

//electric heat
extern unsigned char u8HeatTime;         //电加热带开关计时
extern unsigned char u8TrgHeatDuty;      //预热占空比
extern unsigned char u8RunHeatDuty;      //运行占空比

//out fan
extern unsigned char u8StartFanDly;      //风机延时启动
extern unsigned char u8StopFanDly;       //外风机延时30秒关闭
extern unsigned char u8ACFanSwhDly;
extern unsigned char u8DCFanArea;
extern unsigned char u8Tim3Min;          //特定工况,T1 ?T4 超出范围3分钟,则退出
extern unsigned char u8TrgFanOK;
extern unsigned char u8FanSta;

//dc fan
extern unsigned char u8TrgFan;           //0:风机停 1:高风 2:中风 3:低风 5:制冷额定 6:制热额定 7:调试板风速 8:最小制冷 9:最大制冷 10:中间制冷
extern unsigned char u8TrgFanBak;
extern unsigned char u8LowCoolTim;       //低温制冷退出计时

extern unsigned short u16TrgFanSpd311;   //发给311的风速
extern unsigned short u16CurFanSpd311;

//unsigned short u16TrgFanSpd;    //外机直驱DC电机风速,已转换成定时器的触发值
extern unsigned short u16CurFanSpd0547;  //电机转一圈经历过时间值,按12脉冲计算,以100ms为计时单位
extern unsigned short u16TrgPulseWidth;
extern unsigned char u8FanSpdFBPulse;
extern unsigned short u16DCFanSpdCnt;
extern unsigned char u8DCFanErrTim;
extern unsigned short u16CurFanSpeed;

//PmvCtrl
extern unsigned short u16DeltaPulse;     //目标开度与当前开度的差值
extern unsigned short u16TrgPulse;       //目标开度
extern unsigned short u16CurrPulse;      //当前开度
extern unsigned char u8StepTim;          //励磁速度计时器,每ms计时加1
extern unsigned char u8PulseCnt;         //步进四相8拍计数
extern unsigned char u8StepExcitTim;     //结束相,停止相,折返相0.5s励磁计时器,每10ms计时加1
extern unsigned char u8PmvRegulateTim;

extern unsigned char u8InSetMode;
extern unsigned char u8InSetModeBuf;
extern unsigned char u8InSetModeBufPre;
extern unsigned char u8InSetTemp;       //设定温度
extern unsigned char u8InFanMode;       //室内设定风速
extern unsigned char u8OutRunMode;      //室外运行模式

//user set
extern unsigned char u8T1_TsArea;       //T1-TS温差区间
extern unsigned char u8T1_TsAreaH;      //T1-TS制热原区间
extern unsigned char u8T1_TsAreaPre;    //前一个T1-TS温差区间

//defrost
extern unsigned char u8DefrostExit;
extern unsigned short u16QuitDefrostTim;
extern unsigned short u16InDefrostTim;   // 压缩机累计运行时间
extern unsigned char u8DefrostDly3Min;   // 小于I3的连续3分钟计时
extern unsigned char u8TempT3Min;        // T3最小值
extern unsigned char u8T3MinDly;         // T3比较抗干扰时间

//Device
extern unsigned char u8DispFlashTim;

extern signed short s16IPM_k2;
extern signed short s16IPM_b2;
extern signed short s16IPM_k1;
extern signed short s16IPM_b1;

#endif

