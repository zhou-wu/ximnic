#ifndef __PMV_CTR_H__
#define __PMV_CTR_H__

#define PMV_PORT_OUT      u8MacroTmp=0 ///do{PORT2.PDR.BIT.B2=1;PORT2.PDR.BIT.B3=1;PORTD.PDR.BIT.B6=1;PORT0.PDR.BIT.B2=1;}while(0)
#define PMVA_A            u8MacroTmp  ///PORT2.PODR.BIT.B2
#define PMVA_B            u8MacroTmp  ///PORT2.PODR.BIT.B3
#define PMVA_C            u8MacroTmp  ///PORT0.PODR.BIT.B2
#define PMVA_D            u8MacroTmp  ///PORTD.PODR.BIT.B6

#define STEP_PPS            32      //每32ms,输出一个脉冲(换算励磁速度为31.25pps)
#define RESET_PULSE         560//520//上电阀零点定位,反向回转脉冲数
#define STEP_START_EXCIT    50      //当前起始相励磁时间
#define DEFAULT_PULSE       304     //上电复位后的默认开度
#define CLOSE_CLR_PULSE     160     //阀闭清零脉冲
#define PMV_REG             120     //调节周期
#define MAX_PULSE           480     //阀开最大脉冲数
#define MIN_PULSE           80      //阀闭最小脉冲数
#define STEP_STOP_EXCIT     50      //当前结束相励磁时间

#define U8_PMV_COOL_T5_ZONE_LEN   9
#define U8_PMV_HEAT_T5_ZONE_LEN   9

union UNION_PMVFASTDIR_FLAG
{
  unsigned char u8Byte;
  struct
  {
    unsigned char PmvRstOk        : 1;
    unsigned char InDoorPmvRstOk  : 1;
    unsigned char PmvRstFast      : 1;
    unsigned char FistCompStar    : 1;
    unsigned char FistPMVStar     : 1;
    unsigned char Fistincommok    : 1;
    unsigned char no6             : 1;
    unsigned char no7             : 1;
  } bits;
};
extern union UNION_PMVFASTDIR_FLAG unPmvFastDirFlgs;


extern void PmvInit(void);
extern void PmvCtr(void);
extern void StepDrv(unsigned char u8PortLevel);
  
extern unsigned char u8TrgDiscPipeTemp;

#endif


