/****************************************************************************
* Copyright (C),  Co., Ltd.

* �� �� ���� user_simurta.h

* ���ݼ����� ģ�⴮��Э������ļ�
             2.1 �����ʣ�500bps.
             2.2 ֡��ʽ
            (1) 1λ��ʼλ��8λ����λ��1λֹͣλ���һ֡���ݡ�
            (2) ��ʼλΪ�͵�ƽ���߼�0������������λΪ�ߣ��߼�1����ͣ��߼�0����ƽ��ֹͣλΪ�ߵ�ƽ���߼�1����

*
* �ļ���ʷ��
* �汾��    ����            ����        ˵��
* v01       2009-11-14      StiveHe
*****************************************************************/


#ifndef __USER_SIMURTA_H_
#define __USER_SIMURTA_H_

#include "user_pro.h"


#define _CHK_DRIVEPARA_ 0

#define SET_UA0RXE	(SCSCR_0 |= 0x50)//	UA1RXE
#define CLR_UA0RXE	(SCSCR_0 &= (~0x50))
#define SET_UA0TXE	(SCSCR_0 |= 0xA0)//UA1TXE
#define CLR_UA0TXE   (SCSCR_0 &= (~0xA0))




typedef enum
{SIM_READREADY, SIM_READSTAR, SIM_READING, SIM_READOVER} SIMUART_RXSTATE;

#define SIM_SENDREADY   SIM_READREADY
#define SIM_SENDSTAR    SIM_READSTAR
#define SIM_SENDING     SIM_READING
#define SIM_SENDOVER    SIM_READOVER

//ͨѶ״̬
typedef enum
{SIMCOM_POWERUP, SIMCOM_RESET, SIMCOM_TX, SIMCOM_TXWAIT, SIMCOM_RX, SIMCOM_RXWAIT, SIMCOM_COMCHECK, SIMCOM_FAULT} SIMCOMMUTE_STATE;


typedef union
{
  unsigned char  all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  byteOver: 1;
    unsigned char  frameOver: 1;
    unsigned char  rev: 6;
#else
    unsigned char  rev: 6;
    unsigned char  frameOver: 1;
    unsigned char  byteOver: 1;
#endif
  } bit;

} SIMFLAG;
typedef struct
{
  SIMUART_RXSTATE       state;             //λ���ͽ���״̬
  unsigned char                  busLevel;          //λ�˿ڵ�ƽ��
  unsigned char                  Tcnt;              //λ���ͽ��ռ�ʱ��
  unsigned char                  dataOut;              //��������
  unsigned char                  bitNum;            //���ڷ��͵�����λ
  unsigned char                  checkCnt;          //У�������
  unsigned char                  datatemp;
  SIMFLAG                 Flag;
  unsigned char                  cnt;
  unsigned char                  simBuf[12];
} SIM_UART;

typedef union
{
  SIM_UART    rx;
  SIM_UART    tx;
} UN_SIM_UART;
#define ENABLE_FILTER   0

typedef struct
{
  SIMCOMMUTE_STATE     state;
  UN_SIM_UART          simuart;
  unsigned short       timeCnt;
  unsigned char               errCnt;
  unsigned char               comCheckOver;            //0 = �ϵ���ʱ��1 = �̼�ͨѶ�� 2 = ����ͨѶ
  unsigned char              comCheckCnt;
  unsigned char              TxEn;

  // 20170401 India start Tina
  unsigned char nErrRestCnt;
  // 20170401 India end Tina
} SIM_UART_STATUS;

typedef union
{
  signed int  s4_data;
  unsigned char  u1_buf[4];
} S4_UNION;

typedef union
{
  unsigned char   all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  compressor: 1;
    unsigned char  outfan: 1;
    unsigned char  fourway: 1;
    unsigned char  heater: 1;
    unsigned char  exv: 1;
    /* 20171226  IDU_Fan Begin Tina */
    /* Tina
    unsigned char  T1T5Flag:1;
    Tina */
    unsigned char	IduFan: 1;
    /* 20171226  IDU_Fan End   Tina */
    unsigned char  startupTest: 1;
    unsigned char  shortflg: 1;
#else
    unsigned char  shortflg: 1;
    unsigned char  startupTest: 1;
    /* 20171226  IDU_Fan Begin Tina */
    /* Tina
    unsigned char  T1T5Flag:1;
    Tina */
    unsigned char	IduFan: 1;
    /* 20171226  IDU_Fan End   Tina */
    unsigned char  exv: 1;
    unsigned char  heater: 1;
    unsigned char  fourway: 1;
    unsigned char  outfan: 1;
    unsigned char  compressor: 1;

#endif
  } bit;
} PC_FLAG;

typedef union
{
  unsigned char   all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  fourway: 1;
    unsigned char  heater: 1;
    unsigned char  revs: 1;
    unsigned char  exv_test_product: 1;
    unsigned char  exv_a: 1;
    unsigned char  exv_b: 1;
    unsigned char  exv_c: 1;
    unsigned char  exv_d: 1;
    //unsigned char  revs:6;
#else
    //unsigned char  revs:6;
    unsigned char  exv_d: 1;
    unsigned char  exv_c: 1;
    unsigned char  exv_b: 1;
    unsigned char  exv_a: 1;
    unsigned char  exv_test_product: 1;
    unsigned char  revs: 1;
    unsigned char  heater: 1;
    unsigned char  fourway: 1;
#endif
  } bit;
} PC_COMMAND_FLAG;

typedef struct
{
  PC_FLAG pc_flag;
  PC_COMMAND_FLAG pc_command_flag;
  unsigned char      testFourWayValev;
  unsigned char      testFanSpeed;
  unsigned char      testCompTarg;
  unsigned char      testExv;
  /* 20171226  IDU_Fan Begin Tina */
  unsigned char      testFanSpeed_IDU;
  /* 20171226  IDU_Fan End   Tina */

  // 20161224 self-check start Tina
  unsigned short  nCkDlyTim;		// �Լ��ȶ��жϵȴ�ʱ�� F
  unsigned short	SC_CompTarg;
  unsigned short	SC_FanSpeed;
  unsigned short  nPWMStandard;		// �Լ��� F
  unsigned short  nT4Standard;		// �Լ�T4 F
  unsigned short  nT3Standard;		// �Լ�T3 F
  unsigned short  nTpStandard;		// �Լ�TP F
  // 20161224 self-check end Tina
} PC_COMMAND;

typedef union
{
  unsigned char   all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  high_fan: 1;
    unsigned char  mid_fan: 1;
    unsigned char  low_fan: 1;
    unsigned char  fourway: 1;
    unsigned char  heater: 1;
    unsigned char  revs: 3;
#else
    unsigned char  revs: 3;
    unsigned char  heater: 1;
    unsigned char  fourway: 1;
    unsigned char  low_fan: 1;
    unsigned char  mid_fan: 1;
    unsigned char  high_fan: 1;
#endif
  } bit;
} TEST_COMMAND_FLAG1;
typedef union
{
  unsigned char   all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  EXV_A: 1;
    unsigned char  EXV_B: 1;
    unsigned char  EXV_C: 1;
    unsigned char  EXV_D: 1;
    unsigned char  revs: 4;
#else
    unsigned char  revs: 4;
    unsigned char  EXV_D: 1;
    unsigned char  EXV_C: 1;
    unsigned char  EXV_B: 1;
    unsigned char  EXV_A: 1;
#endif
  } bit;
} TEST_COMMAND_FLAG2;
typedef struct
{
  TEST_COMMAND_FLAG1  test_command_flag1;
  TEST_COMMAND_FLAG2  test_command_flag2;
} TEST_COMMAND;

//#define SET_TXBIT           (  P1.2 = 1)
//#define CLR_TXBIT           ( P1.2 = 0)

//#define READ_RXBIT          (P1.3)

#define COMMUN_ERR_DLY  60

extern  SIM_UART_STATUS simUartStatus;

extern  PC_COMMAND  pcSet;
extern  S4_UNION    s4_temp;//[0]--high, [3]--low
extern  void SimUart_Pro(void);

extern void CommunicateInit(void);
extern void Communicate_TCL(void);

extern  void InitSimUart(void);
extern  void SimUartReadByte(void);
extern  void SimUartTxByte(void);
extern  void SimUartRxFrame(void);
extern  void SimUartTxFrame(void);
unsigned short CalcCheckSum_TCL(unsigned short *pData, unsigned short len);

extern  void    SimUartTeFrame(void);

#endif

