#ifndef __INOUT_COMM_H__
#define __INOUT_COMM_H__

#define U8_COMM_BUF_SIZE          (45)
#define U8_COMM_CODE              (0xAA)
#define U8_COMM40_HEAD_IN2OUT     (0x58)//0b 0101 1000
#define U8_COMM40_HEAD_OUT2IN     (0x85)//0b 0101 1000
#define U8_COMM40_ADDR_INDOOR     (0xA0)//0b 1010 0000
#define U8_COMM40_ADDR_OUTDOOR    (0xA1)//0b 1010 0001
#define U8_COMM40_COMMAND_NORMAL  (0x01)//0b 0000 0001

union UNION_INIT_FLAGS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char ChgType    : 1;
    unsigned char InitTim    : 1;
    unsigned char flg2       : 1;
    unsigned char flg3       : 1;
    unsigned char flg4       : 1;
    unsigned char flg5       : 1;
    unsigned char flg6       : 1;
    unsigned char TempAD     : 1; //40字节协议与16字节协议温度值兼容

  } bits;
} ;
extern union UNION_INIT_FLAGS unInitFlgs;

extern void InOutCommInit(void);
extern void OutCommRec(void);
extern void OutCommSend(void);

//============== Leo ====================
extern void OutCommDec(unsigned char *pRxBuf);
extern void OutCommEnc(unsigned char *pTxBuf);
//extern void SendDelay(void);
//=======================================

extern unsigned char u8TimInitComm;
extern unsigned char u8InOutCommTxdTim;
extern unsigned char u8InDoorSetTim;
extern unsigned char u8InOutCommErrTim;
extern unsigned short u16ZGInCtrFanSpd;
extern unsigned char u8ZGInCtrFWValve;
extern unsigned char InOutComBuf[];
extern unsigned char u8InDoorSetTim;
extern unsigned char u8ACFanCtr;
extern unsigned short u16ZGInCtrTrgStepPulse;
extern unsigned char u8ZGPFCEnFlg;       //志高PFC使能标志 1:开PFC,0:关PFC

#endif

