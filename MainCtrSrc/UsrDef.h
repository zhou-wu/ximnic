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

//indoor fan:ע���ڷ���ĺ��������ͨ�ŵĸ�ʽ
#define C_InFanTurbo  7
#define C_InFanSH     6
#define C_InFanH      5
#define C_InFanM      4
#define C_InFanMM     3
#define C_InFanL      2
#define C_InFanSL     1     //����
#define C_InFanOff    0

#define   MINUTE_MACRO(x)   (60*x)              //��sΪ��ʱ��λ�ķ���

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
    unsigned char TsErr     : 1;    //TS ����������
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
    unsigned char ErrT2Lim      : 1;  //T2������Ƶ��־
    unsigned char DefrostIn     : 1;  //1 Ƶ���Ѿ�����F3
    unsigned char NoPlat        : 1;  //1:ȡ��ƽ̨
    unsigned char CompForce7Min : 1;  //ԭCompForce7Min - ѹ����ǿ������7����
    unsigned char FirstInHeat   : 1;  //��һ�ν�������
    unsigned char FirstInCool   : 1;  //��һ�ν�������
    unsigned char DefrostExit   : 1;  //��˪�˳�
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
    unsigned char VoltLim     : 1; //��ѹ��Ƶ
    unsigned char CurrLim     : 1; //������Ƶ
    unsigned char T5TempLim   : 1; //������Ƶ,TdTempLim, Leo
    unsigned char T3TempLim   : 1; //�������¶���Ƶ TcTempLim, Leo
    unsigned char T2TempLLim  : 1; //������������Ƶ TeTempCLim, Leo
    unsigned char T2TempHLim  : 1; //������������Ƶ TeTempHLim, Leo
    unsigned char IpmTempLim  : 1; //ģ�������Ƶ, TmTempLim, Leo
    unsigned char no7         : 1;
  } bits;
} ;
extern union UNION_FREQLIMIT_FLAG unFreqLimFlgs;

extern ByteTypeDef unTempProtFlgs;
#define ucTempProtFlgs      unTempProtFlgs.u8Byte
#define fgDryT1LowPro       unTempProtFlgs.bits.no0  //��ʪ ���µ��±���
#define fgCoolT4HighPro     unTempProtFlgs.bits.no1
#define fgHeatT4HighPro     unTempProtFlgs.bits.no2
#define fgT1TSStop          unTempProtFlgs.bits.no3  //����ͣ��
#define fgT4Freq0Stop       unTempProtFlgs.bits.no4
#define fProtHeatAZone      unTempProtFlgs.bits.no5

union UNION_IN_STATUS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char FastTest    : 1;
    unsigned char PowerRun    : 1;    //ǿ������
    unsigned char SleepRun    : 1;    //˯��ģʽ
    unsigned char RuiFan      : 1;    //������±���
    unsigned char RationRun   : 1;    //�����
    unsigned char SelfChk     : 1;    //�Լ�
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
    unsigned char FWValve     : 1;  //��ͨ��
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
    unsigned char DefrostFirst    : 1;      //ǿ�ƻ�˪,��һ������ѹ����,��ʱ������
    unsigned char DefrostOnce     : 1;      //����ǿ�ƻ�˪,ֻ����1��
    unsigned char FreRunDisable   : 1;      //DR ���T4
    unsigned char Fre05           : 1;      //1: 0.5hz
    unsigned char OneNight        : 1;      //1: 1��1�ȵ�
    unsigned char Defrost7DW      : 1;      //1: 7�����½��뻯˪
    unsigned char Defrost7UP      : 1;      //1: 7�����Ͻ��뻯˪
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
    unsigned char CTestACFanA   : 1;    //�������ģʽA (��С����)  0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanB   : 1;    //�������ģʽB (�м�����)  0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanC   : 1;    //�������ģʽC (�����)  0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanD   : 1;    //�������ģʽD (�������)  0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanE   : 1;    //�������ģʽE       0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanF   : 1;    //�������ģʽF 0:�ͷ絵 1:�߷絵
    unsigned char CTestACFanG   : 1;    //�������ģʽG       0:�ͷ絵 1:�߷絵
    unsigned char no7           : 1;
  } bits;  //OutACFanCtrl
} ;
extern union UNION_SELT_FLAG4 unSeltFlgs4;

union UNION_SELT_FLAG5
{
  unsigned char u8Byte;
  struct
  {
    unsigned char HTestACFanA   : 1;    //���Ȳ���ģʽA (��С����)  0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanB   : 1;    //���Ȳ���ģʽB (�м�����)  0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanC   : 1;    //���Ȳ���ģʽC (�����)  0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanD   : 1;    //���Ȳ���ģʽD (��С����)  0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanE   : 1;    //���Ȳ���ģʽE       0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanF   : 1;    //���Ȳ���ģʽF 0:�ͷ絵 1:�߷絵
    unsigned char HTestACFanG   : 1;    //���Ȳ���ģʽG       0:�ͷ絵 1:�߷絵
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
    unsigned char LCoolSelt     : 1;    //�������书��ѡ��
    unsigned char no5           : 1;    //
    unsigned char RetOilSelt    : 1;    //���͹���ѡ��
    unsigned char PMVSelt       : 1;
  } bits;  //OutFanCtrl
} ;
extern union UNION_SELT_FLAG1 unSeltFlgs1;

union UNION_SELT_FLAG2
{
  unsigned char u8Byte;
  struct
  {
    unsigned char PmvCfgDir   : 1;    // 0: ���ͷ�����     1:���ͷ�����
    unsigned char PfcErrSel   : 1;    // 0: �����ж�PFC����
    unsigned char FreMinRun   : 1;    // 0: ����ߵ�Ƶ���й���
    unsigned char CompProTime : 1;    // 0: ����3���ӱ���  1:�ķ��ӱ���
    unsigned char TorqueOFF   : 1;    // ��������ת�ؿ���,Ϊ1��ʾ�ر�,Ϊ0��ʾ����ת�ز�����
    unsigned char ValveDir    : 1;    // 0: ��ͨ������       1: ��ͨ������
    unsigned char CoolOnly    : 1;    // 0: ����            1: ��ů
    unsigned char DcFanDrv    : 1;    // 0: ��оƬ����       1: 311����ֱ�����
  } bits;
} ;
extern union UNION_SELT_FLAG2 unSeltFlgs2;

union UNION_SELT_FLAG3
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerPFCA_OFF    : 1;  //  �������ģʽA (��С����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCB_OFF    : 1;  //  �������ģʽB (�м�����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCC_OFF    : 1;  //  �������ģʽC (�����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCD_OFF    : 1;  //  �������ģʽD (�������)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCE_OFF    : 1;  //  �������ģʽE 0:��PFC 1:�ر�PFC
    unsigned char SeerPFCF_OFF    : 1;  //  �������ģʽF 0:��PFC 1:�ر�PFC
    unsigned char SeerPFCG_OFF    : 1;  //  �������ģʽG 0:��PFC 1:�ر�PFC
    unsigned char no7             : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG3 unSeltFlgs3;

union UNION_SELT_FLAG6
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerPFCA_OFF    : 1;  //  ���Ȳ���ģʽA (��С����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCB_OFF    : 1;  //  ���Ȳ���ģʽB (�м�����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCC_OFF    : 1;  //  ���Ȳ���ģʽC (�����)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCD_OFF    : 1;  //  ���Ȳ���ģʽD (�������)  0:��PFC 1:�ر�PFC
    unsigned char SeerPFCE_OFF    : 1;  //  ���Ȳ���ģʽE 0:��PFC 1:�ر�PFC
    unsigned char SeerPFCF_OFF    : 1;  //  ���Ȳ���ģʽF 0:��PFC 1:�ر�PFC
    unsigned char SeerPFCG_OFF    : 1;  //  ���Ȳ���ģʽG 0:��PFC 1:�ر�PFC
    unsigned char no7             : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG6 unSeltFlgs6;

union UNION_SELT_FLAG7
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerDefreostA_OFF   : 1;  //  �������ģʽA (��С����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostB_OFF   : 1;  //  �������ģʽB (�м�����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostC_OFF   : 1;  //  �������ģʽC (�����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostD_OFF   : 1;  //  �������ģʽD (�������)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostE_OFF   : 1;  //  �������ģʽE 0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostF_OFF   : 1;  //  �������ģʽF 0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostG_OFF   : 1;  //  �������ģʽG 0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char no7                 : 1;  //
  } bits;
} ;
extern union UNION_SELT_FLAG7 unSeltFlgs7;

union UNION_SELT_FLAG8
{
  unsigned char u8Byte;
  struct
  {
    unsigned char SeerDefreostA_OFF   : 1;  //  ���Ȳ���ģʽA (��С����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostB_OFF   : 1;  //  ���Ȳ���ģʽB (�м�����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostC_OFF   : 1;  //  ���Ȳ���ģʽC (�����)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostD_OFF   : 1;  //  ���Ȳ���ģʽD (�������)  0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostE_OFF   : 1;  //  ���Ȳ���ģʽE 0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostF_OFF   : 1;  //  ���Ȳ���ģʽF 0:ʹ�ܳ�˪  1:��ֹ��˪
    unsigned char SeerDefreostG_OFF   : 1;  //  ���Ȳ���ģʽG 0:ʹ�ܳ�˪  1:��ֹ��˪
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
    unsigned char NoLockMode      : 1;      // 1: ��ʱȡ��С���������� ��˪����˪����30S��
    unsigned char ParaFinish      : 1;      // 1: ���ݲ���ɹ���ȡ���Ͳ�����־
    unsigned char Defrost         : 1;      // 1: ��˪��־
    unsigned char DCVlowPRO       : 1;      // 1: ֱ����ѹ����
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
#define fgErrInFan    unInErr1.bits.no0 //�ڻ����ʧ�ٹ���
#define fgErrInEE     unInErr1.bits.no1 //����EE����

extern ByteTypeDef unInErr2;
#define u8InErr2       unInErr2.u8Byte
#define fgErrT3        unInErr2.bits.no0 //T3����
#define fgErrT2        unInErr2.bits.no1 //T2����������//T2B����������
#define fgErrT2b       unInErr2.bits.no1 //T2����������//T2B����������
#define fgErrT1        unInErr2.bits.no2 //T1����������
#define fgErrZero      unInErr2.bits.no3 //�ڻ�����
#define fgInOutCommErr unInErr2.bits.no4 //�������ͨ�Ź���
#define fgWaterFull    unInErr2.bits.no5 //ˮ������
#define fgProtInT2H    unInErr2.bits.no6 //���������±���
#define fgProtInT2L    unInErr2.bits.no7 //�����������ᱣ��

extern ByteTypeDef unRunTime;
#define fgT10ms     unRunTime.bits.no0
#define fgT20ms     unRunTime.bits.no1
#define fgT50ms     unRunTime.bits.no2
#define fgT100ms    unRunTime.bits.no3
#define fgT200ms    unRunTime.bits.no4
#define fgT500ms    unRunTime.bits.no5
#define fgT1sec     unRunTime.bits.no6
#define fgT1min     unRunTime.bits.no7

extern ByteTypeDef unFreqProt;                   //���ϱ���ͣ����־
#define fgProtT5        unFreqProt.bits.no0      //�����¶ȱ���
#define fgProtHeatT2    unFreqProt.bits.no1      //������T2���±���
#define fgProtCoolT2    unFreqProt.bits.no2      //������T2�����ᱣ��
#define fgProtCoolT3    unFreqProt.bits.no3      //���������±�����־,ѹ����ֹͣ
#define fgProtT1        unFreqProt.bits.no4      //��ʪ,���µ��±���
#define fgErrT4Low      unFreqProt.bits.no5      //������±���
#define fgCompFault     unFreqProt.bits.no6      //��ý��Ᵽ��
#define fgProtErr       unFreqProt.bits.no7      //�������ϻ��򱣻�,Ҫ��ͣѹ������

extern ByteTypeDef unLimStop;                    //��Ƶͣ��
#define fgCurrentStop     unLimStop.bits.no0     //������Ƶͣ��
#define fgCoolT2Stop      unLimStop.bits.no1     //����T2��Ƶͣ��
#define fgHeatT2Stop      unLimStop.bits.no2     //����T2��Ƶͣ��
#define fgT1TsStop        unLimStop.bits.no3     //����ͣ��
#define fgT4LimStop       unLimStop.bits.no4     //T4��Ƶͣ��
#define fgDefrostStop     unLimStop.bits.no5     //��˪ͣ��
#define fgModeChangeStop  unLimStop.bits.no6     //ģʽ�л�ͣ��
//#define fgDefrostStop   unLimStop.bits.no7     //��˪ͣ��

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
extern unsigned char u8ErrHoldTim;   //���й��Ͻ����30s�ָ���������״̬
extern unsigned char u8T5TempErrTim; //�����¶ȴ�����

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
extern unsigned char u8CurrArea;    //������������

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
extern unsigned char u8CompDly;          //ѹ�������ͺ�̵����Ķ���ʱ��
extern unsigned short u16CompSpd;        //Ƶ�������ٶ� 100ms--
extern unsigned short u16CompRunTim;     //ѹ��������ʱ��
extern unsigned short u16CompStopTim;    //ѹ��������ʱ��

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
extern unsigned char u8PowerOnFlg;     //���̵�����־
extern unsigned char u8ErrStop;        //ѹ��ͣ��ԭ��
extern unsigned char u8CompStopTim;

//frequency
extern unsigned char u8MinRunFreq;
extern unsigned char u8MinForceFreq;   //ǿ���ܵ�Ƶ��
extern unsigned short u16MinFreqChangeT;

extern unsigned char u8MaxFreqLim;     //�����������Ƶ��
extern unsigned char u8MaxFreqArea;    //���Ƶ������

extern unsigned char u8FreqSet;        //Ŀ��Ƶ�ʵȼ�F0-F25
extern unsigned char u8RunFreqGrade;   //��ǰƵ�����еȼ� ͨ����ǰ����Ƶ��,ת���صȼ�F0-F25
extern unsigned char u8FreqAreaT3Min;  //�������3����,3min����һ���²�
extern unsigned char u8F1RunT60Min;    //j����,��F1����һ��Сʱ(������ϴ��ʱ����)

extern unsigned char u8T4MinFreqLim;
extern unsigned char u8LowFreqRateNum;

extern unsigned char u8EnergyFreq;      //����������Ҫ���ܵ�Ƶ��
extern unsigned char u8TargetFreq;      //���Ŀ��Ƶ��
extern unsigned char u8LimitFreq;       //�����Ƶ
extern unsigned char u8LastTrgFreq;
extern unsigned char u8RunCtrFreq;      //ʵ������Ŀ���Ƶ��
extern unsigned char u8RealRunFreq;     //ʵ���ܵ�Ƶ��
extern unsigned char u8RealRunInterger; //ѹ����ʵ������Ƶ��

//debug theta company
extern unsigned char u8DgTrgHz;
extern unsigned char u8DgTrgHzDot;
extern unsigned short u16DgInFanSpd;
extern unsigned char u8DgFWValve;
extern unsigned short u16DgTrgStepPls;   //���԰��趨����
extern unsigned char u8DgDefrost;
extern unsigned char u8DgCanLimit;

extern unsigned short u16ROLowFreqTim;   //��Ƶ��תʱ���ʱ
extern unsigned short u16ROFreqPlatTim;  //����ƽ̨����ʱ���ʱ

//4way
extern unsigned char u8ValveDlyTim;      //��ͨ����ʱ�ر�
extern unsigned char u8T2ErrCompRunTim;  //T2����ѹ������ʱ��

//electric heat
extern unsigned char u8HeatTime;         //����ȴ����ؼ�ʱ
extern unsigned char u8TrgHeatDuty;      //Ԥ��ռ�ձ�
extern unsigned char u8RunHeatDuty;      //����ռ�ձ�

//out fan
extern unsigned char u8StartFanDly;      //�����ʱ����
extern unsigned char u8StopFanDly;       //������ʱ30��ر�
extern unsigned char u8ACFanSwhDly;
extern unsigned char u8DCFanArea;
extern unsigned char u8Tim3Min;          //�ض�����,T1 ?T4 ������Χ3����,���˳�
extern unsigned char u8TrgFanOK;
extern unsigned char u8FanSta;

//dc fan
extern unsigned char u8TrgFan;           //0:���ͣ 1:�߷� 2:�з� 3:�ͷ� 5:���� 6:���ȶ 7:���԰���� 8:��С���� 9:������� 10:�м�����
extern unsigned char u8TrgFanBak;
extern unsigned char u8LowCoolTim;       //���������˳���ʱ

extern unsigned short u16TrgFanSpd311;   //����311�ķ���
extern unsigned short u16CurFanSpd311;

//unsigned short u16TrgFanSpd;    //���ֱ��DC�������,��ת���ɶ�ʱ���Ĵ���ֵ
extern unsigned short u16CurFanSpd0547;  //���תһȦ������ʱ��ֵ,��12�������,��100msΪ��ʱ��λ
extern unsigned short u16TrgPulseWidth;
extern unsigned char u8FanSpdFBPulse;
extern unsigned short u16DCFanSpdCnt;
extern unsigned char u8DCFanErrTim;
extern unsigned short u16CurFanSpeed;

//PmvCtrl
extern unsigned short u16DeltaPulse;     //Ŀ�꿪���뵱ǰ���ȵĲ�ֵ
extern unsigned short u16TrgPulse;       //Ŀ�꿪��
extern unsigned short u16CurrPulse;      //��ǰ����
extern unsigned char u8StepTim;          //�����ٶȼ�ʱ��,ÿms��ʱ��1
extern unsigned char u8PulseCnt;         //��������8�ļ���
extern unsigned char u8StepExcitTim;     //������,ֹͣ��,�۷���0.5s���ż�ʱ��,ÿ10ms��ʱ��1
extern unsigned char u8PmvRegulateTim;

extern unsigned char u8InSetMode;
extern unsigned char u8InSetModeBuf;
extern unsigned char u8InSetModeBufPre;
extern unsigned char u8InSetTemp;       //�趨�¶�
extern unsigned char u8InFanMode;       //�����趨����
extern unsigned char u8OutRunMode;      //��������ģʽ

//user set
extern unsigned char u8T1_TsArea;       //T1-TS�²�����
extern unsigned char u8T1_TsAreaH;      //T1-TS����ԭ����
extern unsigned char u8T1_TsAreaPre;    //ǰһ��T1-TS�²�����

//defrost
extern unsigned char u8DefrostExit;
extern unsigned short u16QuitDefrostTim;
extern unsigned short u16InDefrostTim;   // ѹ�����ۼ�����ʱ��
extern unsigned char u8DefrostDly3Min;   // С��I3������3���Ӽ�ʱ
extern unsigned char u8TempT3Min;        // T3��Сֵ
extern unsigned char u8T3MinDly;         // T3�ȽϿ�����ʱ��

//Device
extern unsigned char u8DispFlashTim;

extern signed short s16IPM_k2;
extern signed short s16IPM_b2;
extern signed short s16IPM_k1;
extern signed short s16IPM_b1;

#endif

