#include "UsrDef.h"
#include "InOutComm.h"
#include "DgComm.h"
#include "DevCtr.h"
#include "ParameterPro.h"
#include "PmvCtr.h"
#include "FanCtr.h"
#include "DataTransFunc2Drv.h"
#include "Signal.h"
#include "TempSample.h"
#include "Signal.h"
#include "MiscFunc.h"

#define INCOMM_TXD_PORT_MODE    (PORTB.PMR.BIT.B5=1)

static union
{
  unsigned char u8Byte;
  struct
  {
    unsigned char RDatRec : 1;
    unsigned char no1   : 1;
    unsigned char no2   : 1;
    unsigned char no3   : 1;
    unsigned char no4   : 1;
    unsigned char no5   : 1;
    unsigned char no6   : 1;
    unsigned char no7   : 1;
  } bits;
} unOutCommRxdFlgs;

static union
{
  unsigned char u8Byte;
  struct
  {
    unsigned char TDelay : 1;
    unsigned char no1   : 1;
    unsigned char no2   : 1;
    unsigned char no3   : 1;
    unsigned char no4   : 1;
    unsigned char no5   : 1;
    unsigned char no6   : 1;
    unsigned char no7   : 1;
  } bits;
} unOutCommTxdFlgs;

union UNION_INIT_FLAGS unInitFlgs;
unsigned char u8TimInitComm;
unsigned char u8InOutCommTxdTim;
unsigned char u8InDoorSetTim;
unsigned char u8InOutCommErrTim;
unsigned short u16ZGInCtrFanSpd;
unsigned char u8ZGInCtrFWValve;
unsigned char InOutComBuf[U8_COMM_BUF_SIZE];
unsigned char u8InDoorSetTim;
unsigned char u8ACFanCtr;
unsigned short u16ZGInCtrTrgStepPulse;
unsigned char u8Recving;
unsigned char u8InOutCommBufCnt;
unsigned char InOutCommBuf[U8_COMM_BUF_SIZE];
unsigned char u8SendOrder1;
unsigned char u8SendOrder2;
unsigned char u8SendOrder3;
unsigned char u8InInst;
unsigned char u8PowerTypeNew;
unsigned char u8ZGPFCEnFlg;       //־��PFCʹ�ܱ�־ 1:��PFC,0:��PFC
unsigned char u8OutCommTxdTim;
unsigned char u8InOutCmdLen;      ///u8INCOMMDATALEN

static void SendDelay(void);
static void OutComRecZG(void);
static void OutComRecZG40(void);
static void RecDecode40(void);
static void RecDecode(void);
static void OutComSend(void);
static void OutComSend40(void);
static void F_Uart5_SendEnabe(void);

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void InOutCommInit(void)
{
  unsigned char i = 0;
  
  for(i = 0; i < U8_COMM_BUF_SIZE; i++)
  {
    InOutComBuf[i] = 0;
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommRec(void)
{
  ///INCOMM_RXD_PORT_IN; // #define INCOMM_RXD_PORT_IN      (PORTB.PDR.BIT.B6 = 0)
  if(u8InOutCommErrTim >= 20)
  {
    unPmvFastDirFlgs.bits.Fistincommok = 1;
    unPmvFastDirFlgs.bits.InDoorPmvRstOk = 0;
  }

  if ( unInSta.bits.SelfChk )
  {
    if ( u8InOutCommErrTim >= 7 )
    {
      if (0 == unDgSetFlgs.bits.NoSendErr)
      {
        unSysErrFlgs.bits.InOutCommErr = 1;
      }
    }
  }
  else
  {
    if (u8InOutCommErrTim >= 60)
    {
      //1min error
      if (0 == unDgSetFlgs.bits.NoSendErr)
      {
        //С��ͨ���޸�
        unSysErrFlgs.bits.InOutCommErr = 1;
      }
    }
  }

  if (unInitFlgs.bits.InitTim)
  {
    unInitFlgs.bits.InitTim = 0;
    u8TimInitComm = 0;
  }
  //���ӵ����յ�һ�������ֽں�,����һ��ʱ��û�н��յ���һ���ֽ�,�����³�ʼ��ͨ��
  if(u8Recving) // // 2012-06-01
  {
    if(u8InOutCommBufCnt >= 1)
    {
      if(u8TimInitComm > 30) // 30MS
      {
        u8TimInitComm = 0;
        u8InOutCommBufCnt = 0;
        InOutCommInit();
      }
    }
    else
    {
      u8TimInitComm = 0;
    }
  }
  else
  {
    u8TimInitComm = 0;
  }

  if (unOutCommRxdFlgs.bits.RDatRec)
  {
    unOutCommRxdFlgs.bits.RDatRec = 0;
    u8Recving = 0;
    switch(InOutCommBuf[0])
    {
      case U8_COMM_CODE:
        {
          unPmvFastDirFlgs.bits.Fistincommok = 1;
          RecDecode();
        }
        break;
      case U8_COMM40_HEAD_IN2OUT:
        {
          unPmvFastDirFlgs.bits.Fistincommok = 1;
          RecDecode40();
        }
        break;
      default:
        {
          if (u8InOutCommErrTim >= 60)
          {
            //indoor and outdoor communication error for 2min
            unSysErrFlgs.bits.InOutCommErr = 1;
          }
        }
        break;
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void RecDecode(void)
{
  if ((InOutCommBuf[15] != 0x55) && (InOutCommBuf[15] != 0xA5))
  {
    //Data error.
    //DI();
    InOutCommInit();
    //EI();
  }
  /*  else if(u8InInst==1)
  {
    if (InOutCommBuf[15] != 0xA5)
      { //Data error.
        //DI();
        InOutCommInit();
        //EI();
      }
    }
  */
  else if (InOutCommBuf[14] != CommChkSum(&InOutCommBuf[1], 13))
  {
    //Check sum error.
    u8InOutCommBufCnt = 0;
    //DI();
    InOutCommInit();
    //EI();
  }
  else
  {
    // Data is ok!
    OutComRecZG();
    u8SendOrder1 = 0xAA;
    u8SendOrder2 = 0x00;
    u8SendOrder3 = 0x00;
    SendDelay();
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void RecDecode40(void)
{
  if (InOutCommBuf[39] != CommChkSum(&InOutCommBuf[0], 39))
  {
    //Check sum error.
    u8InOutCommBufCnt = 0;
    //DI();
    InOutCommInit();
    //EI();
  }
  else
  {
    // Data is ok!
    OutComRecZG40();
    u8SendOrder1 = U8_COMM40_HEAD_OUT2IN;
    SendDelay();
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComRecZG40(void)
{
  ByteTypeDef undata_tmp;

  u8InInst = InOutCommBuf[3];             //������ͨ������

  /// ����ģʽ
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    undata_tmp.u8Byte = 0;
    // = InOutCommBuf[4];                         //�ڻ�����/�ں�
    u8PowerTypeNew = InOutCommBuf[5];             //��ǰ���ڻ�����

    undata_tmp.u8Byte = InOutCommBuf[25];         //����ģʽ״̬������
    unInSta.bits.FastTest = undata_tmp.bits.no0;  //���
    unInSta.bits.SelfChk = undata_tmp.bits.no1;
    unInSta.bits.RationRun = undata_tmp.bits.no7; //������������//�����
    if (unInSta.bits.RationRun)                   //������������//�����
    {
      //unSeltFlgs3.u8Byte = undata_tmp.bits.no6; //PFC����ʹ��,��Э�鶨���෴ //1:�����������ж�
      u8ZGPFCEnFlg = !undata_tmp.bits.no6;        //PFC����ʹ��,��Э�鶨���෴ //1: �����������ж�
      // = InOutCommBuf[26];                      //��������ģʽ�������
      u8InRateTest = InOutCommBuf[27];            //��������ģʽ���Ե����

      if ( ( u8InRateTest != u8InRateTestPre ) || 
           ( u8InSetModeBufPre != u8InSetModeBuf ))
      {
        u8WaitPFCTim = 150;
        u8InRateTestPre = u8InRateTest;
        u8InSetModeBufPre = u8InSetModeBuf;
      }
    }
    else
    {
      ///unSeltFlgs3.u8Byte = 0;
      ///unSeltFlgs6.u8Byte = 0;
      
      u8ZGPFCEnFlg = 1;
      u8InRateTest = SeerTestNo;
      u8InRateTestPre = u8InRateTest;
      u8WaitPFCTim = 0;
    }
    
    undata_tmp.u8Byte = InOutCommBuf[6];       //�ڻ��趨ģʽ
    u8InSetModeBuf = undata_tmp.u8Byte & 0x0F; //&0b00001111;
    ///u8InSetModeBufPre = u8InSetModeBuf;
    if ( u8WaitPFCTim > 0 )
    {
      u8InSetMode = C_ModeOff;
    }
    else
    {
      u8InSetMode = u8InSetModeBuf;
    }
     
    // = undata_tmp.bits.no4;           //�ڻ��趨ģʽ,����
    // = undata_tmp.bits.no5;           //�ڻ��趨ģʽ,˯��
    // = undata_tmp.bits.no6;           //�ڻ��趨ģʽ,����
    // = undata_tmp.bits.no7;           //�ڻ��趨ģʽ,ǿ��

    undata_tmp.u8Byte = InOutCommBuf[7];//�ڻ���������
    // = undata_tmp.bits.no0;           //ǿ������ý
    // = undata_tmp.bits.no1;           //ǿ�ƻ�˪
    // = undata_tmp.bits.no2;           //��������
    // = undata_tmp.bits.no3;           //��������
    // = undata_tmp.bits.no3;           //ѹ��Ԥ��
    // = InOutCommBuf[8];               //Ԥ��
    unInitFlgs.bits.TempAD = 0;
    //LH K082 12-18K_1.0.0a_190930
    u8InSetTemp = InOutCommBuf[9] >> 1; //�û��趨�¶�,�趨�¶�*2
    u8TempT1 = InOutCommBuf[10];        //���ڻ����¶�ֵT1,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    u8TempT2 = InOutCommBuf[11];        //�����������в��¶�T2,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    u8TempT2b = InOutCommBuf[12];       //���������������¶�T2B,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    // = InOutCommBuf[13];              //����ģʽ�¶Ȳ���,(�¶�+30)*2,Ĭ�Ϸ�60,�ֱ���0.5��,ƫ��-30��
    // = InOutCommBuf[14];              //����ģʽ�¶Ȳ���,(�¶�+30)*2,Ĭ�Ϸ�60,�ֱ���0.5��,ƫ��-30��
    // = InOutCommBuf[15];              //����ʪ��
    // = InOutCommBuf[16];              //�����¶�

    u8InFanMode = InOutCommBuf[17];     //�ڷ�����е�λ
    // = InOutCommBuf[18];              //�ڷ��ʵ��ת��,ʵ��ת��/10 rpm,���������00H

    u8InErr1 = 0x00;
    u8InErr2 = 0x00;
    undata_tmp.u8Byte = InOutCommBuf[19];  //���ڱ���״̬1
    fgProtInT2L = undata_tmp.bits.no0;     //���������±���
    //fgProtInT2H = undata_tmp.bits.no1;   //���������±���
    // = undata_tmp.bits.no2;              //���ڷ����

    undata_tmp.u8Byte = InOutCommBuf[20];  //���ڱ���״̬2
    //
    undata_tmp.u8Byte = InOutCommBuf[21];  //���ڹ���ͣ��״̬1
    fgErrT1 = undata_tmp.bits.no0;         //T1����������
    fgErrT2 = undata_tmp.bits.no1;         //T2����������
    fgErrT2b = undata_tmp.bits.no2;        //T2B����������
    //fgInCommErr = undata_tmp.bits.no3;   //�������ͨ�Ź���
    //fgWaterFull = undata_tmp.bits.no4;   //ˮ������
    fgErrInEE = undata_tmp.bits.no5;       //����EE����
    fgErrInFan = undata_tmp.bits.no6;      //�ڻ����ʧ�ٹ���
    //fgErrZero = undata_tmp.bits.no7;     //�����źŹ���

    undata_tmp.u8Byte = InOutCommBuf[22];  //���ڹ���ͣ��״̬2

    // = InOutCommBuf[23];                 //Ԥ��
    // = InOutCommBuf[24];                 //�ڻ�����״̬

    if((InOutCommBuf[28] != 0) && (u8ZGInCtrTrgHzOut != InOutCommBuf[28])) //��Ƶ����ѹ����Ŀ��Ƶ��
    {
      u8ZGInCtrTrgHzOut = InOutCommBuf[28];
      unDgCommCtr.bits.DgOutFreq = 0;
    }
    if ((InOutCommBuf[29] != 0) && (u16ZGInCtrFanSpd != ((unsigned short)InOutCommBuf[29] * 10)) ) //��Ƶ��������Ŀ��ת��
    {
      u16ZGInCtrFanSpd = (unsigned short)InOutCommBuf[29] * 10;
      unDgCommCtr.bits.DgOutFan = 0;
    }
    if ((InOutCommBuf[30] >= 12) && (InOutCommBuf[30] <= 129))             //��Ƶ���Ե������ͷ�����
    {
      if (u16ZGInCtrTrgStepPulse != ((unsigned short)InOutCommBuf[30] << 2) )
      {
        u16ZGInCtrTrgStepPulse = (unsigned short)InOutCommBuf[30] << 2;
        unDgCommCtr.bits.DgOutPmv = 0;
      }
    }
    undata_tmp.u8Byte = InOutCommBuf[31];       //��Ƶ�����������״̬��
    if (u8ZGInCtrFWValve != undata_tmp.bits.no0)//��ͨ��
    {
      u8ZGInCtrFWValve = undata_tmp.bits.no0;
      unDgCommCtr.bits.DgOutFw = 0;
    }
    // = undata_tmp.bits.no1;        //��ŷ�SV1
    // = undata_tmp.bits.no2;        //��ŷ�SV2
    // = undata_tmp.bits.no3;        //���̵����
    // = undata_tmp.bits.no4;        //������ȴ�
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComRecZG(void)
{
  unsigned char data_tmp;
  // unsigned char u8TempT1HeatCmps;
  u8InInst = InOutCommBuf[2];               //������ͨ������

  if(u8InInst == 0)
  {
    //00����
    u8PowerTypeNew = InOutCommBuf[3];       //��ǰ���ڻ�����
    u8InSetMode = InOutCommBuf[4];
    //u8ZGInCtrPowerMod= InOutCommBuf[5];   //ZG��������ͨ��Э��,���ڲ���״̬

    data_tmp = InOutCommBuf[6];             //�������,�������ڷ絲�������¶Ȳ���ѡ��
    data_tmp >>= 4;
    u8InFanMode = data_tmp & 0x07;

    u8InErr1 = InOutCommBuf[7];
    if(u8InErr1 & 0x01)   //���ڷ��ʧ��
    {
      fgErrInFan = 1;
    }
    else
    {
      fgErrInFan = 0;
    }

    if(u8InErr1 & 0x02)   //����Eeprom����
    {
      fgErrInEE = 1;
    }
    else
    {
      fgErrInEE = 0;
    }
    //Byte8:���ڻ�����״̬2
    u8InErr2 = InOutCommBuf[8];
    if(u8InErr2 & 0x01)   //T3����
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x02)   //T2��T2B����
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x04)   //T1����
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x10)   //������ͨ�Ź���
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x20)   //ˮ������
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x40)   //���������±���
    {

    }
    else
    {

    }
    if(u8InErr2 & 0x80)   //���������±���
    {

    }
    else
    {

    }
    unInitFlgs.bits.TempAD = 1;//40�ֽ�Э����16�ֽ�Э���¶�ֵ����
    u8InSetTemp = InOutCommBuf[9];
    //= InOutCommBuf[10];     //���ڲ���״̬
    u8T1TempAd = InOutCommBuf[11];

    if(C_ModeHeat == u8InSetMode)
    {
      //����T2
      u8T2TempAd = InOutCommBuf[12];
    }
    else
    {
      //����T2B
      u8T2bTempAd = InOutCommBuf[12];
    }

    data_tmp = InOutCommBuf[13];
    if(data_tmp & 0x01)   //���
    {
      unInSta.bits.FastTest = 1;
    }

  }
  else
  {
    //01����
    data_tmp = InOutCommBuf[5];
    if(data_tmp & 0x04)   //PFC����ʹ��
    {
      u8ZGPFCEnFlg = 1;   //1:�����������ж�
    }
    else
    {
      u8ZGPFCEnFlg = 0;   //0:��Ŀ��Ƶ��С�ڲ������PFCƵ��ʱ��PFC
    }

    //Byte6:�����״̬����
    data_tmp = InOutCommBuf[6];
    unInitFlgs.bits.TempAD = 1;     //40�ֽ�Э����16�ֽ�Э���¶�ֵ����
    if(C_ModeHeat == u8InSetMode)   //T2��T2B��������
    {
      //����T2B
      u8T2bTempAd = InOutCommBuf[12];
    }
    else
    {
      //����T2
      u8T2TempAd = InOutCommBuf[12];
    }
  }
  /*
  if (u8InTrgFreq)
  { // Cancel pre-heater
    u8TrgHeatDuty = 0;
  }
  */
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommSend(void)
{
  ///INCOMM_TXD_PORT_OUT; #define INCOMM_TXD_PORT_OUT     (PORTB.PDR.BIT.B5 = 1)
  if (u8OutCommTxdTim == 0)
  {
    if (unOutCommTxdFlgs.bits.TDelay)
    {
      unOutCommTxdFlgs.bits.TDelay = 0;
      switch(u8SendOrder1)
      {
        case U8_COMM_CODE:
          {
            OutComSend();
            F_Uart5_SendEnabe();
          }
          break;
        case U8_COMM40_HEAD_OUT2IN:
          {
            OutComSend40();
            F_Uart5_SendEnabe();
          }
          break;
        default:
          {
            u8InOutCommBufCnt = 0;
            InOutCommInit();
          }
          break;
      }
    }
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComSend(void)
{
  unsigned char i = 0;
  
  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    InOutCommBuf[i] = 0;
  }
  
  //ZG
  InOutCommBuf[0] = 0xAA; //Flow begin.
  InOutCommBuf[1] = 0;  //outdoor address

  if(u8InInst == 0)
  {
    //00����,�ظ�������һ֡����
    InOutCommBuf[2] = 0;
    InOutCommBuf[3] = u8PowerTypeNew;   //�������ڷ��͸�����Ļ��ͻش�
    InOutCommBuf[4] = u8RealRunInterger;//ѹ����ʵ��Ƶ�� (311Ϊu8RunCtrFreq)

    /*
    if (unInStatus.bits.FastTest)
    {
      u8CompStopTim = U8_COMP_STOP_TIM;
      u8FreqStopTim = U8_COMP_STOP_TIM;
    }
    */
    InOutCommBuf[5] = 0;//��ǰ���������״̬1
    InOutCommBuf[6] = 0;//��ǰ���������״̬2


    if(unTempErrFlgs.bits.T3Err)
    {
      //�������¶ȴ���������T3
      InOutCommBuf[5] |= 0x01;
    }
    if(unTempErrFlgs.bits.T5Err)
    {
      //�����¶ȴ���������T5
      InOutCommBuf[5] |= 0x02;
    }
    if(unSysErrFlgs.bits.DrvCommErr)
    {
      //��IPMģ��ͨ�Ź���
      InOutCommBuf[5] |= 0x04;
    }
    if(unCompErrFlgs1.bits.FoErr)
    {
      //ģ�鱣��
      InOutCommBuf[5] |= 0x10;
    }
    if((unCompErrFlgs1.bits.LoVoltErr) || (unCompErrFlgs1.bits.PfcErr) || (unSysErrFlgs.bits.ACVoltLowErr) || (unCompErrFlgs2.bits.OvVoltErr))
    {
      //�����ѹ��Ƿѹ����
      InOutCommBuf[5] |= 0x20;
    }
    if(unTempErrFlgs.bits.T4Err)
    {
      //�����¶ȴ���������T4
      InOutCommBuf[5] |= 0x40;
    }
    if(unSysErrFlgs.bits.OverCurrErr)
    {
      //��������
      InOutCommBuf[5] |= 0x80;
    }


    if((unCompErrFlgs2.bits.CpOvCurrErr) || (unCompErrFlgs2.bits.MCEErr)
        || (unCompErrFlgs1.bits.PhsLossErr) || (unCompErrFlgs2.bits.VoltDrop) || (unCompErrFlgs1.bits.SpeedErr)
        || (unCompErrFlgs2.bits.ZeroSpdErr))
    {
      //ѹ������������
      InOutCommBuf[6] |= 0x01;
    }
    if (fgTpProtFix)
    {
      //�����¶ȹ���
      InOutCommBuf[6] |= 0x02;
    }
    if(unSysErrFlgs2.bits.ProtHP)
    {
      //��ѹ����
      InOutCommBuf[6] |= 0x10;
    }
    if(unSysErrFlgs2.bits.ProtLP)
    {
      //��ѹ����
      InOutCommBuf[6] |= 0x20;
    }
    if (unSysErrFlgs.bits.DCFanSpdErr)
    {
      //ֱ��������ʧ��
      InOutCommBuf[6] |= 0x40;
    }
    if ((unSysErrFlgs.bits.I2cErr) || (unCompErrFlgs2.bits.E2Err))
    {
      //E������
      InOutCommBuf[6] |= 0x08;
    }

    //������ڻ����̶�ֵ,�����ڻ����س���
    //InOutCommBuf[7] = u8CondMidTempAd;
    InOutCommBuf[7] = TEMP_MACRO(25);//u8TempT3;//ʵ���¶�*2+60
    //InOutCommBuf[8] = u8OutdoorTempAd;
    InOutCommBuf[8] = TEMP_MACRO(25);//u8TempT4;//ʵ���¶�*2+60
    //InOutCommBuf[9] = u8DiscPipeTempAd;
    InOutCommBuf[9] = 50;//u8TempTP;//ʵ���¶�
    InOutCommBuf[10] = 0;//u8AcCurrentRMS_T10 / 5;  //u8AcCurrentRMS_T10Ϊ�������*10 //һ�β���� ����ֵ = ʵ��ֵ*2
    //InOutCommBuf[11] = u8HalfDcVoltRMS;   //dc volt
    InOutCommBuf[11] = 110;             //AC��ѹ ͨ˳�ܷ���û�н�����ѹ��� //һ�β��ѹ ����ֵ = ʵ��ֵ/2
    InOutCommBuf[12] = u8InSetMode;       //mode��ǰ�������תģʽ
    if(unCompRunFlgs2.bits.RetOil)
    {
      //����
      InOutCommBuf[13] |= 0x02;
    }
  }
  else if(1 == u8InInst)
  {
    InOutCommBuf[2] = 1;
    InOutCommBuf[3] = TabAD2IpmTemp[u8TempIpm] - 50;            //IPMģ��ʵ���¶�*2-50
    InOutCommBuf[4] = 0;                //���β����
    InOutCommBuf[5] = u8HalfDcVoltFil >> 1;       //ֱ��ĸ�ߵ�ѹ ����ֵ = ʵ��ֵ/4
    InOutCommBuf[6] = (unsigned char)(u16CurrPulse >> 3);  //�������ͷ���ǰ����

    u16CurFanSpeed = ((unsigned short)Drv2Main[4] << 8) + (unsigned short)Drv2Main[3];

    InOutCommBuf[7] = u16CurFanSpeed / 10;       //ֱ�������ǰת��
    InOutCommBuf[8] = 0;

    if(u8PowerOnFlg == 1)
    {
      InOutCommBuf[8] |= 0x01; //�������̵���
    }
    if(unFlgs.bits.FWVCtr)
    {
      InOutCommBuf[8] |= 0x02; //��ͨ��
    }
    else
    {
      InOutCommBuf[8] &= 0xFD;
    }
    if(u8TrgFan == FanH)
    {
      InOutCommBuf[8] |= 0x04; //�߷�
    }
    else if(u8TrgFan == FanL)
    {
      InOutCommBuf[8] |= 0x10; //�ͷ�
    }
    else
    {
      InOutCommBuf[8] &= 0xE7;
    }

    InOutCommBuf[9] = 0;

    if(unSysErrFlgs.bits.InOutCommErr)
    {
      //�������ͨ�Ź���
      InOutCommBuf[9] |= 0x01; //������ͨ�Ź���
    }
    if(unSysErrFlgs.bits.DCFanSpdErr)
    {
      InOutCommBuf[9] |= 0x02;  //ֱ���������
    }
    //if(fgIPMProt)
    //{
    //  InOutCommBuf[9]|= 0x04; //IPMģ����±���
    //}
    //if()
    //{//IPMģ������Ҳ��ɻָ�
    //  InOutCommBuf[9] |= 0x08;
    //}
    if(fgCompErrFix)
    {
      InOutCommBuf[9] |= 0x10;  //���������Ҳ��ɻָ�
    }

    //if()
    //{//���ڳ�˪��
    //  InOutCommBuf[10] |= 0x01;
    //}

    //if()
    //{//���ڸ�λ��
    //  InOutCommBuf[10] |= 0x02;
    //}

  }
  InOutCommBuf[14] = CommChkSum(&InOutCommBuf[1], 13);
  InOutCommBuf[15] = 0x55;  // EOF.

  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 16;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutComSend40(void)
{
  unsigned char i = 0;
  ByteTypeDef undata_tmp;

  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    InOutCommBuf[i] = 0;
  }
  
  InOutCommBuf[0] = U8_COMM40_HEAD_OUT2IN;  //Flow begin.
  InOutCommBuf[1] = U8_COMM40_ADDR_OUTDOOR; //outdoor address
  InOutCommBuf[2] = U8_COMM40_ADDR_INDOOR;
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    //����ģʽ
    InOutCommBuf[3] = U8_COMM40_COMMAND_NORMAL;   //������ͨ������
    InOutCommBuf[4] = u8PowerTypeNew;             //�������ڷ��͸�����Ļ��ͻش� //�������/����
    //InOutCommBuf[5] = ;                         //ԭʼ������
    //InOutCommBuf[6] = ;                         //T4��������
    //InOutCommBuf[7] = ;                         //T2/T2B��������

    InOutCommBuf[8] = u8TempT3;           //�������в�����T3,(�¶�+30)*2
    InOutCommBuf[9] = u8TempT4;           //���⻷���¶�T4,(�¶�+30)*2
    InOutCommBuf[10] = u8TempT5;          //ѹ���������¶�T5,ʵ���¶�ֵ,��ƫ��
    InOutCommBuf[11] = TabAD2IpmTemp[u8TempIpm];  //IPM�¶�T6,ʵ���¶�ֵ,��ƫ��
    //InOutCommBuf[12] = ;          //Ԥ���¶�T7
    //InOutCommBuf[13] = ;          //Ԥ���¶�T8
    //InOutCommBuf[14] = ;          //T2/T2Bƽ���¶�,(�¶�+30)*2

    if (u8InSetMode <= C_ModeDry)
    {
      InOutCommBuf[15] = u8InSetMode;       //mode��ǰ�������תģʽ
    }
    else if (u8InSetMode == C_ModeForce)
    {
      InOutCommBuf[15] = C_ModeCool;
      InOutCommBuf[15] |= 0X80; //0b10000000;
    }
    else if (unFlgs2.bits.Defrost)
    {
      InOutCommBuf[15] = C_ModeHeat;
    }
    else
    {
      InOutCommBuf[15] = u8InSetMode;
    }

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unFlgs2.bits.Defrost;  //���ڳ�˪��
    undata_tmp.bits.no1 = unCompRunFlgs2.bits.RetOil;  //����
    InOutCommBuf[16] = undata_tmp.u8Byte;       //���������״̬

    InOutCommBuf[17] = u8EnergyFreq;            //��ǰѹ����Ŀ��Ƶ��
    InOutCommBuf[18] = u8RealRunInterger;       //ѹ����ʵ��Ƶ��
    InOutCommBuf[19] = u16FanFBSpd / 10;        //��ǰ����ʵ��ת��,ʵ��ת��/10
    InOutCommBuf[20] = (unsigned char)(u16CurrPulse >> 2);   //��ǰ�������ͷ�����,ʵ�ʿ���/4

    //InOutCommBuf[21] = u8AcVoltageAd;         //һ�β��ѹ,ʵ�ʵ�ѹ/2,
    InOutCommBuf[22] = u8AcCurrRMS_T10 / 5;     //һ�β����,ʵ�ʵ���*2,�ֱ���0.5A
    InOutCommBuf[23] = u8HalfDcVoltFil >> 1;    //���β��ѹ,ʵ�ʵ�ѹ/4,
    //InOutCommBuf[24] = ;    //���β����,ʵ�ʵ���*4,�ֱ���0.25A

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unFreqLimFlgs.bits.CurrLim;    //һ�β�/���β������Ƶ
    undata_tmp.bits.no1 = unFreqLimFlgs.bits.VoltLim;  //һ�β�/���β��ѹ��Ƶ
    undata_tmp.bits.no2 = unFreqLimFlgs.bits.T2TempHLim | unFreqLimFlgs.bits.T2TempLLim;   //��������/������Ƶ
    undata_tmp.bits.no3 = unFreqLimFlgs.bits.T3TempLim;//T3������������Ƶ
    //undata_tmp.bits.no4 = ;    //T4�⻷���¶���Ƶ
    undata_tmp.bits.no5 = unFreqLimFlgs.bits.T5TempLim;//T5ѹ���������¶���Ƶ
    undata_tmp.bits.no6 = unFreqLimFlgs.bits.IpmTempLim;    //T6IPMģ���¶���Ƶ
    //undata_tmp.bits.no7 = ;//��ѹѹ����Ƶ
    InOutCommBuf[25] = undata_tmp.u8Byte; //�������Ƶ״̬1

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;    //�ڷ���ͷ���Ƶ
    //undata_tmp.bits.no1 = ;    //�ڷ���з���Ƶ
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[26] = undata_tmp.u8Byte; //�������Ƶ״̬2

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unTempErrFlgs.bits.T3Err;    //T3�������¶ȴ���������
    undata_tmp.bits.no1 = unTempErrFlgs.bits.T4Err;    //T4�⻷������������
    undata_tmp.bits.no2 = unTempErrFlgs.bits.T5Err;    //T5ѹ������������������
    undata_tmp.bits.no3 = unTempErrFlgs.bits.TipmErr;  //T6IPMģ���¶ȴ���������
    undata_tmp.bits.no4 = ((unSysErrFlgs.bits.I2cErr) | (unCompErrFlgs2.bits.E2Err));  //����EEPROM����
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[27] = undata_tmp.u8Byte; //���������״̬1

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unSysErrFlgs.bits.OverCurrErr;   //һ�β�/���β������������
    undata_tmp.bits.no1 = ( (unSysErrFlgs.bits.ACVoltLowErr) | (unCompErrFlgs2.bits.OvVoltErr) );    //һ�β�/���β��ѹ��ѹǷ/ѹ��Ƶ
    if((fgProtCoolT2) | (fgProtHeatT2))
    {
      undata_tmp.bits.no2 = 1; //��������/���±���
    }
    else
    {
      undata_tmp.bits.no2 = 0; //��������/���±���
    }
    undata_tmp.bits.no3 = fgProtCoolT3;  //T3���������±���
    undata_tmp.bits.no4 = fgProtT5;      //T5ѹ���������¶ȹ��߱���
    undata_tmp.bits.no5 = fgIPMProt;     //T6IPMģ���¶ȹ��߱���
    undata_tmp.bits.no6 = unSysErrFlgs2.bits.ProtHP;  //��ѹѹ������
    undata_tmp.bits.no7 = unSysErrFlgs2.bits.ProtLP;  //��ѹѹ������
    InOutCommBuf[28] = undata_tmp.u8Byte; //���������״̬2

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no0 = unSysErrFlgs.bits.DCFanSpdErr;   //ֱ�����ʧ��
    //undata_tmp.bits.no1 = ;    //��̨�籣��
    undata_tmp.bits.no2 = unSysErrFlgs.bits.InOutCommErr; //����ͨѶ����
    undata_tmp.bits.no3 = unSysErrFlgs.bits.DrvCommErr;  //������DSPͨѶ����
    ///undata_tmp.bits.no4 = unCompErrFlgs1.bits.FoErr; //IPMģ�鱣��
    undata_tmp.bits.no4 = (unCompErrFlgs1.u8Byte||unCompErrFlgs2.u8Byte)?1:0; /// Leo
    //undata_tmp.bits.no5 = ;  //���򱣻�
    //undata_tmp.bits.no6 = ;  //ģʽ��ͻ
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[29] = undata_tmp.u8Byte; //���������״̬3

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;
    //undata_tmp.bits.no1 = ;
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[30] = undata_tmp.u8Byte; //���������״̬4

    undata_tmp.u8Byte = 0x00;
    //undata_tmp.bits.no0 = ;
    //undata_tmp.bits.no1 = ;
    //undata_tmp.bits.no2 = ;
    //undata_tmp.bits.no3 = ;
    //undata_tmp.bits.no4 = ;
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[30] = undata_tmp.u8Byte; //����ģʽ����״̬��

    undata_tmp.u8Byte = 0x00;
    undata_tmp.bits.no7 = unDgCommCtr.bits.DgInFan;
    InOutCommBuf[31] = undata_tmp.u8Byte; //��Ƶ�����ڷ��ת��

    undata_tmp.u8Byte = 0x00;
    if (u8TrgFan == FanL)
    {
      undata_tmp.bits.no0 = 1;   //��ͷ�
      undata_tmp.bits.no1 = 0;   //��߷�
    }
    else if (u8TrgFan == FanH)
    {
      undata_tmp.bits.no0 = 0;   //��ͷ�
      undata_tmp.bits.no1 = 1;   //��߷�
    }
    else
    {
      undata_tmp.bits.no0 = 0;   //��ͷ�
      undata_tmp.bits.no1 = 0;   //��߷�
    }
    //undata_tmp.bits.no0 = ;    //��ͷ�
    //undata_tmp.bits.no1 = ;    //��߷�
    undata_tmp.bits.no2 = unFlgs.bits.FWVCtr; //��ͨ��
    //undata_tmp.bits.no3 = ;    //���̼���
    undata_tmp.bits.no4 = unFlgs.bits.PTCCtr; //�������
    //undata_tmp.bits.no5 = ;    //sv1��ŷ�
    //undata_tmp.bits.no6 = ;    //sv2��ŷ�
    //undata_tmp.bits.no7 = ;
    InOutCommBuf[32] = u16DgInFanSpd / 10;
    InOutCommBuf[33] = undata_tmp.u8Byte; //����������״̬,�����̼�
  }
  InOutCommBuf[39] = CommChkSum(&InOutCommBuf[0], 39);
  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 40;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void SendDelay(void)
{
  u8InOutCommErrTim = 0;
  unSysErrFlgs.bits.InOutCommErr = 0;
  unOutCommRxdFlgs.u8Byte = 0;
  unOutCommTxdFlgs.u8Byte = 0;
  u8InOutCommTxdTim = 5;            //Delay 50ms to send.
  unOutCommTxdFlgs.bits.TDelay = 1;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void F_Uart5_SendEnabe(void)
{ 
#if 0  ///ԭ����
  INCOMM_TXD_PORT_MODE;
  SCI5.SCR.BIT.TIE = 1;
  SCI5.SCR.BIT.TE  = 1;     // enable tx
  SCI5.SCR.BIT.TEIE = 1;
  SCI5.SCR.BIT.RE = 0;      //disable recieving
#endif
}

//============================== Leo =================================

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommDec(unsigned char *pRxBuf)
{
  ByteTypeDef unTmp;
  unTmp.u8Byte = 0;

  ///DgLedBlink();

  u8InInst = pRxBuf[3];                           //������ͨ������

  /// ����ģʽ
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    unTmp.u8Byte = 0;
    // = pRxBuf[4];                               //�ڻ�����/�ں�
    u8PowerTypeNew = pRxBuf[5];                   //��ǰ���ڻ�����

    unTmp.u8Byte = pRxBuf[25];                    //����ģʽ״̬������
    unInSta.bits.FastTest = unTmp.bits.no0;       //���
    unInSta.bits.SelfChk = unTmp.bits.no1;
    unInSta.bits.RationRun = unTmp.bits.no7;      //������������//�����
    if (unInSta.bits.RationRun)                   //������������//�����
    {
      //unSeltFlgs3.u8Byte = undata_tmp.bits.no6; //PFC����ʹ��,��Э�鶨���෴ //1:�����������ж�
      u8ZGPFCEnFlg = !unTmp.bits.no6;             //PFC����ʹ��,��Э�鶨���෴ //1: �����������ж�
      // = pRxBuf[26];                            //��������ģʽ�������
      u8InRateTest = pRxBuf[27];                  //��������ģʽ���Ե����

      if ( ( u8InRateTest != u8InRateTestPre ) || 
           ( u8InSetModeBufPre != u8InSetModeBuf ))
      {
        u8WaitPFCTim = 150;
        u8InRateTestPre = u8InRateTest;
        u8InSetModeBufPre = u8InSetModeBuf;
      }
    }
    else
    {
      ///unSeltFls3.u8Byte = 0;
      ///unSeltFls6.u8Byte = 0;
      
      u8ZGPFCEnFlg = 1;
      u8InRateTest = SeerTestNo;
      u8InRateTestPre = u8InRateTest;
      u8WaitPFCTim = 0;
    }
    
    unTmp.u8Byte = pRxBuf[6];             //�ڻ��趨ģʽ
    u8InSetModeBuf = unTmp.u8Byte & 0x0F; //&0b00001111;
    ///u8InSetModeBufPre = u8InSetModeBuf;
    if ( u8WaitPFCTim > 0 )
    {
      u8InSetMode = C_ModeOff;
    }
    else
    {
      u8InSetMode = u8InSetModeBuf;
    }
     
    // = unTmp.bits.no4;           //�ڻ��趨ģʽ,����
    // = unTmp.bits.no5;           //�ڻ��趨ģʽ,˯��
    // = unTmp.bits.no6;           //�ڻ��趨ģʽ,����
    // = unTmp.bits.no7;           //�ڻ��趨ģʽ,ǿ��

    unTmp.u8Byte = pRxBuf[7];//�ڻ���������
    // = unTmp.bits.no0;           //ǿ������ý
    // = unTmp.bits.no1;           //ǿ�ƻ�˪
    // = unTmp.bits.no2;           //��������
    // = unTmp.bits.no3;           //��������
    // = unTmp.bits.no3;           //ѹ��Ԥ��
    // = pRxBuf[8];                //Ԥ��
    unInitFlgs.bits.TempAD = 0;
    u8InSetTemp = pRxBuf[9] >> 1;  //�û��趨�¶�,�趨�¶�*2
    u8TempT1 = pRxBuf[10];         //���ڻ����¶�ֵT1,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    u8TempT2 = pRxBuf[11];         //�����������в��¶�T2,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    u8TempT2b = pRxBuf[12];        //���������������¶�T2B,(�¶�+30)*2,�ֱ���0.5��,ƫ��-30��
    // = pRxBuf[13];               //����ģʽ�¶Ȳ���,(�¶�+30)*2,Ĭ�Ϸ�60,�ֱ���0.5��,ƫ��-30��
    // = pRxBuf[14];               //����ģʽ�¶Ȳ���,(�¶�+30)*2,Ĭ�Ϸ�60,�ֱ���0.5��,ƫ��-30��
    // = pRxBuf[15];               //����ʪ��
    // = pRxBuf[16];               //�����¶�

    u8InFanMode = pRxBuf[17];      //�ڷ�����е�λ
    // = pRxBuf[18];               //�ڷ��ʵ��ת��,ʵ��ת��/10 rpm,���������00H

    u8InErr1 = 0x00;
    u8InErr2 = 0x00;
    unTmp.u8Byte = pRxBuf[19];        //���ڱ���״̬1
    fgProtInT2L = unTmp.bits.no0;     //���������±���
    //fgProtInT2H = unTmp.bits.no1;   //���������±���
    // = unTmp.bits.no2;              //���ڷ����

    unTmp.u8Byte = pRxBuf[20];        //���ڱ���״̬2
    //
    unTmp.u8Byte = pRxBuf[21];        //���ڹ���ͣ��״̬1
    fgErrT1 = unTmp.bits.no0;         //T1����������
    fgErrT2 = unTmp.bits.no1;         //T2����������
    fgErrT2b = unTmp.bits.no2;        //T2B����������
    //fgInCommErr = unTmp.bits.no3;   //�������ͨ�Ź���
    //fgWaterFull = unTmp.bits.no4;   //ˮ������
    fgErrInEE = unTmp.bits.no5;       //����EE����
    fgErrInFan = unTmp.bits.no6;      //�ڻ����ʧ�ٹ���
    //fgErrZero = unTmp.bits.no7;     //�����źŹ���

    unTmp.u8Byte = pRxBuf[22];        //���ڹ���ͣ��״̬2

    // = pRxBuf[23];                  //Ԥ��
    // = pRxBuf[24];                  //�ڻ�����״̬

    if((pRxBuf[28] != 0) && (u8ZGInCtrTrgHzOut != pRxBuf[28])) //��Ƶ����ѹ����Ŀ��Ƶ��
    {
      u8ZGInCtrTrgHzOut = pRxBuf[28];
      unDgCommCtr.bits.DgOutFreq = 0;
    }
    if ((pRxBuf[29] != 0) && (u16ZGInCtrFanSpd != ((unsigned short)pRxBuf[29] * 10)) ) //��Ƶ��������Ŀ��ת��
    {
      u16ZGInCtrFanSpd = (unsigned short)pRxBuf[29] * 10;
      unDgCommCtr.bits.DgOutFan = 0;
    }
    if ((pRxBuf[30] >= 12) && (pRxBuf[30] <= 129))             //��Ƶ���Ե������ͷ�����
    {
      if (u16ZGInCtrTrgStepPulse != ((unsigned short)pRxBuf[30] << 2) )
      {
        u16ZGInCtrTrgStepPulse = (unsigned short)pRxBuf[30] << 2;
        unDgCommCtr.bits.DgOutPmv = 0;
      }
    }
    unTmp.u8Byte = pRxBuf[31];             //��Ƶ�����������״̬��
    if (u8ZGInCtrFWValve != unTmp.bits.no0)//��ͨ��
    {
      u8ZGInCtrFWValve = unTmp.bits.no0;
      unDgCommCtr.bits.DgOutFw = 0;
    }
    // = unTmp.bits.no1;                   //��ŷ�SV1
    // = unTmp.bits.no2;                   //��ŷ�SV2
    // = unTmp.bits.no3;                   //���̵����
    // = unTmp.bits.no4;                   //������ȴ�
  }
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void OutCommEnc(unsigned char *pTxBuf)
{
  unsigned char i = 0;
  unsigned short u16Tmp = 0;
  
  ByteTypeDef unTmp;

  unTmp.u8Byte = 0;
  
  for( i = 0; i < U8_COMM_BUF_SIZE; i++ )
  {
    pTxBuf[i] = 0;
  }
  
  pTxBuf[0] = U8_COMM40_HEAD_OUT2IN;  //Flow begin.
  pTxBuf[1] = U8_COMM40_ADDR_OUTDOOR; //outdoor address
  pTxBuf[2] = U8_COMM40_ADDR_INDOOR;
  if(u8InInst == U8_COMM40_COMMAND_NORMAL)
  {
    //����ģʽ
    pTxBuf[3] = U8_COMM40_COMMAND_NORMAL;   //������ͨ������
    pTxBuf[4] = u8PowerTypeNew;             //�������ڷ��͸�����Ļ��ͻش� //�������/����
    //pTxBuf[5] = ;                         //ԭʼ������
    //pTxBuf[6] = ;                         //T4��������
    //pTxBuf[7] = ;                         //T2/T2B��������

    pTxBuf[8] = u8TempT3;                   //�������в�����T3,(�¶�+30)*2
    pTxBuf[9] = u8TempT4;                   //���⻷���¶�T4,(�¶�+30)*2
    pTxBuf[10] = u8TempT5;                  //ѹ���������¶�T5,ʵ���¶�ֵ,��ƫ��
    pTxBuf[11] = TabAD2IpmTemp[u8TempIpm];  //IPM�¶�T6,ʵ���¶�ֵ,��ƫ��
    //pTxBuf[12] = ;                        //Ԥ���¶�T7
    //pTxBuf[13] = ;                        //Ԥ���¶�T8
    //pTxBuf[14] = ;                        //T2/T2Bƽ���¶�,(�¶�+30)*2

    if (u8InSetMode <= C_ModeDry)
    {
      pTxBuf[15] = u8InSetMode;             //mode��ǰ�������תģʽ
    }
    else if (u8InSetMode == C_ModeForce)
    {
      pTxBuf[15] = C_ModeCool;
      pTxBuf[15] |= 0X80; //0b10000000;
    }
    else if (unFlgs2.bits.Defrost)
    {
      pTxBuf[15] = C_ModeHeat;
    }
    else
    {
      pTxBuf[15] = u8InSetMode;
    }

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unFlgs2.bits.Defrost;             //���ڳ�˪��
    unTmp.bits.no1 = unCompRunFlgs2.bits.RetOil;       //����
    pTxBuf[16] = unTmp.u8Byte;                         //���������״̬

    pTxBuf[17] = u8EnergyFreq;                         //��ǰѹ����Ŀ��Ƶ��
    pTxBuf[18] = u8RealRunInterger;                    //ѹ����ʵ��Ƶ��
    u16Tmp = u16FanFBSpd;
    u16Tmp /= 10;
    pTxBuf[19] = (unsigned char)u16Tmp;                //��ǰ����ʵ��ת��,ʵ��ת��/10
    pTxBuf[20] = (unsigned char)(u16CurrPulse >> 2);   //��ǰ�������ͷ�����,ʵ�ʿ���/4

    //pTxBuf[21] = u8AcVoltageAd;                      //һ�β��ѹ,ʵ�ʵ�ѹ/2,
    pTxBuf[22] = u8AcCurrRMS_T10 / 5;                  //һ�β����,ʵ�ʵ���*2,�ֱ���0.5A
    pTxBuf[23] = u8HalfDcVoltFil >> 1;                 //���β��ѹ,ʵ�ʵ�ѹ/4,
    //pTxBuf[24] = ;    //���β����,ʵ�ʵ���*4,�ֱ���0.25A

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unFreqLimFlgs.bits.CurrLim;       //һ�β�/���β������Ƶ
    unTmp.bits.no1 = unFreqLimFlgs.bits.VoltLim;       //һ�β�/���β��ѹ��Ƶ
    unTmp.bits.no2 = unFreqLimFlgs.bits.T2TempHLim | unFreqLimFlgs.bits.T2TempLLim;   //��������/������Ƶ
    unTmp.bits.no3 = unFreqLimFlgs.bits.T3TempLim;     //T3������������Ƶ
    //unTmp.bits.no4 = ;                               //T4�⻷���¶���Ƶ
    unTmp.bits.no5 = unFreqLimFlgs.bits.T5TempLim;     //T5ѹ���������¶���Ƶ
    unTmp.bits.no6 = unFreqLimFlgs.bits.IpmTempLim;    //T6IPMģ���¶���Ƶ
    //undata_tmp.bits.no7 = ;                          //��ѹѹ����Ƶ
    pTxBuf[25] = unTmp.u8Byte;                         //�������Ƶ״̬1

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;                               //�ڷ���ͷ���Ƶ
    //unTmp.bits.no1 = ;                               //�ڷ���з���Ƶ
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[26] = unTmp.u8Byte;                         //�������Ƶ״̬2

    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unTempErrFlgs.bits.T3Err;         //T3�������¶ȴ���������
    unTmp.bits.no1 = unTempErrFlgs.bits.T4Err;         //T4�⻷������������
    unTmp.bits.no2 = unTempErrFlgs.bits.T5Err;         //T5ѹ������������������
    unTmp.bits.no3 = unTempErrFlgs.bits.TipmErr;       //T6IPMģ���¶ȴ���������
    unTmp.bits.no4 = ((unSysErrFlgs.bits.I2cErr) | (unCompErrFlgs2.bits.E2Err));  //����EEPROM����
    //undata_tmp.bits.no5 = ;
    //undata_tmp.bits.no6 = ;
    //undata_tmp.bits.no7 = ;
    pTxBuf[27] = unTmp.u8Byte; //���������״̬1
    u16DgTmp0 = unTmp.u8Byte;  ///
      
    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unSysErrFlgs.bits.OverCurrErr;    //һ�β�/���β������������
    unTmp.bits.no1 = ( (unSysErrFlgs.bits.ACVoltLowErr) | (unCompErrFlgs2.bits.OvVoltErr) );    //һ�β�/���β��ѹ��ѹǷ/ѹ��Ƶ
    if((fgProtCoolT2) | (fgProtHeatT2))
    {
      unTmp.bits.no2 = 1;                             //��������/���±���
    }
    else
    {
      unTmp.bits.no2 = 0;                             //��������/���±���
    }
    unTmp.bits.no3 = fgProtCoolT3;                    //T3���������±���
    unTmp.bits.no4 = fgProtT5;                        //T5ѹ���������¶ȹ��߱���
    unTmp.bits.no5 = fgIPMProt;                       //T6IPMģ���¶ȹ��߱���
    unTmp.bits.no6 = unSysErrFlgs2.bits.ProtHP;       //��ѹѹ������
    unTmp.bits.no7 = unSysErrFlgs2.bits.ProtLP;       //��ѹѹ������
    pTxBuf[28] = unTmp.u8Byte;                        //���������״̬2
    u16DgTmp1 = unTmp.u8Byte;  ///
    
    unTmp.u8Byte = 0x00;
    unTmp.bits.no0 = unSysErrFlgs.bits.DCFanSpdErr;   //ֱ�����ʧ��
    //unTmp.bits.no1 = ;                              //��̨�籣��
    unTmp.bits.no2 = unSysErrFlgs.bits.InOutCommErr;  //����ͨѶ����
    unTmp.bits.no3 = unSysErrFlgs.bits.DrvCommErr;    //������DSPͨѶ����
    ///unTmp.bits.no4 = unCompErrFlgs1.bits.FoErr;    //IPMģ�鱣��
    unTmp.bits.no4 = (unCompErrFlgs1.u8Byte||unCompErrFlgs2.u8Byte)?1:0; /// Leo
    //unTmp.bits.no5 = ;                              //���򱣻�
    //unTmp.bits.no6 = ;                              //ģʽ��ͻ
    //unTmp.bits.no7 = ;
    pTxBuf[29] = unTmp.u8Byte;                        //���������״̬3
    u16DgTmp2 = unTmp.u8Byte;  ///

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;
    //unTmp.bits.no1 = ;
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[30] = unTmp.u8Byte; //���������״̬4

    unTmp.u8Byte = 0x00;
    //unTmp.bits.no0 = ;
    //unTmp.bits.no1 = ;
    //unTmp.bits.no2 = ;
    //unTmp.bits.no3 = ;
    //unTmp.bits.no4 = ;
    //unTmp.bits.no5 = ;
    //unTmp.bits.no6 = ;
    //unTmp.bits.no7 = ;
    pTxBuf[30] = unTmp.u8Byte; //����ģʽ����״̬��

    unTmp.u8Byte = 0x00;
    unTmp.bits.no7 = unDgCommCtr.bits.DgInFan;
    pTxBuf[31] = unTmp.u8Byte; //��Ƶ�����ڷ��ת��

    unTmp.u8Byte = 0x00;
    if (u8TrgFan == FanL)
    {
      unTmp.bits.no0 = 1;     //��ͷ�
      unTmp.bits.no1 = 0;     //��߷�
    }
    else if (u8TrgFan == FanH)
    {
      unTmp.bits.no0 = 0;     //��ͷ�
      unTmp.bits.no1 = 1;     //��߷�
    }
    else
    {
      unTmp.bits.no0 = 0;   //��ͷ�
      unTmp.bits.no1 = 0;   //��߷�
    }
    //unTmp.bits.no0 = ;    //��ͷ�
    //unTmp.bits.no1 = ;    //��߷�
    unTmp.bits.no2 = unFlgs.bits.FWVCtr; //��ͨ��
    //unTmp.bits.no3 = ;    //���̼���
    unTmp.bits.no4 = unFlgs.bits.PTCCtr; //�������
    //unTmp.bits.no5 = ;    //sv1��ŷ�
    //unTmp.bits.no6 = ;    //sv2��ŷ�
    //unTmp.bits.no7 = ;
    pTxBuf[32] = u16DgInFanSpd / 10;
    pTxBuf[33] = unTmp.u8Byte; //����������״̬,�����̼�
  }
  pTxBuf[39] = CommChkSum(&pTxBuf[0], 39);
  u8InOutCommBufCnt = 0;
  u8InOutCmdLen = 40;
  SendDelay();
}



