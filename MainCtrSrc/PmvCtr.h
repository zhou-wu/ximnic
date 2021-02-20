#ifndef __PMV_CTR_H__
#define __PMV_CTR_H__

#define PMV_PORT_OUT      u8MacroTmp=0 ///do{PORT2.PDR.BIT.B2=1;PORT2.PDR.BIT.B3=1;PORTD.PDR.BIT.B6=1;PORT0.PDR.BIT.B2=1;}while(0)
#define PMVA_A            u8MacroTmp  ///PORT2.PODR.BIT.B2
#define PMVA_B            u8MacroTmp  ///PORT2.PODR.BIT.B3
#define PMVA_C            u8MacroTmp  ///PORT0.PODR.BIT.B2
#define PMVA_D            u8MacroTmp  ///PORTD.PODR.BIT.B6

#define STEP_PPS            32      //ÿ32ms,���һ������(���������ٶ�Ϊ31.25pps)
#define RESET_PULSE         560//520//�ϵ緧��㶨λ,�����ת������
#define STEP_START_EXCIT    50      //��ǰ��ʼ������ʱ��
#define DEFAULT_PULSE       304     //�ϵ縴λ���Ĭ�Ͽ���
#define CLOSE_CLR_PULSE     160     //������������
#define PMV_REG             120     //��������
#define MAX_PULSE           480     //�������������
#define MIN_PULSE           80      //������С������
#define STEP_STOP_EXCIT     50      //��ǰ����������ʱ��

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


