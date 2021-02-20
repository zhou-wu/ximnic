
/*
    user_urta1 head file
*/
#ifndef _USER_URTA1_H
#define _USER_URTA1_H

#include "user_pro.h"

#define cn_COMM_ALL 1

#define cn_BUFLEN   (40)

#define UARXREG		SCI5.RDR//UA1RX
#define UATXREG		SCI5.TDR//UA1TX
#define UARXIF		(SCI5.SSR.BYTE & 0x40) //RDRF //UA1RIF
#define CLR_UARXIF  (SCI5.SSR.BYTE=(SCI5.SSR.BYTE & (~0x40)))
#define UATXIF		(SCI5.SSR.BYTE & 0x80)//TDRE//UA1TIF
#define CLR_UATXIF	(SCI5.SSR.BYTE=(SCI5.SSR.BYTE & (~0x80)))

#define UAERIF		(SCI5.SSR.BYTE & 0x38)//UA1REIF 
#define CLR_UAERIF  (SCI5.SSR.BYTE &=(~0x38))
//#define UASTREG		SCSSR_2//UA1STR

#define SET_UARXE	(SCI5.SCR.BYTE |= 0x50)//	UA1RXE
#define CLR_UARXE	(SCI5.SCR.BYTE &= (~0x50))
#define SET_UATXE	(SCI5.SCR.BYTE |= 0xA0)//UA1TXE
#define CLR_UATXE   (SCI5.SCR.BYTE &= (~0xA0))

#define SCI0_TX_WAIT_TIM    430  //380
#define XIEYI_SELECT  0//自主协议：0创维协议：1

//typedef enum COM_STATUS{COM_RECIEVE,COM_SEND,COM_WAIT};
typedef enum {COM_POWERUP, COM_TX, COM_TXWAIT, COM_RX, COM_RXWAIT, COM_RESET, COM_FAULT} COM_STATUS;
typedef union
{
  unsigned char  all;
  struct
  {
#if _bit_from_high_ == 0
    unsigned char  bit_Rec: 1;             //when rx intterrupt then setting
    unsigned char  bit_Send: 1;            //
    unsigned char  bit_Sending: 1;
    //Uchar8  bit_PC:1;
    unsigned char  recv: 5;
#else
    unsigned char  recv: 5;
    //Uchar8  bit_PC:1;
    unsigned char  bit_Sending: 1;
    unsigned char  bit_Send: 1;
    unsigned char  bit_Rec: 1;
#endif
  } bit_f;
} URLT1_FLAG;

typedef struct //系统运行状态结构体
{
  //    URLT1_FLAG Flag;
  unsigned short timeCnt;
  unsigned char errCnt;
  unsigned char urltsending;
  COM_STATUS     state;//comflag;

  unsigned char  BufData[40];
  unsigned short  SendTim;
  unsigned short  RecTime;         //
  unsigned short  cnt_comrx;         //
  unsigned short  cnt_comtx;
  unsigned short  DataLength;
  unsigned char   *p_txbuf;
  unsigned char   CMD;
  //   unsigned char   DataDptr;
} URLT1_STATUS;


/*

*/
#define ILT_EXCOIL_LBIT      0        //室内蒸发器出口温度最低2位右移位个数 
#define ILT_ROOM_LBIT        2        //室内温度最低2位右移位个数 
#define ILT_COIL_LBIT        4        //室内蒸发器温度(中部)最低2位右移位个数 
#define ILT_LOWBIT_MSK       0x03     //室内温度低两位掩码 



//extern  UN_TX_DATA    tclTxData;
//extern  UN_TX_INFO    tclTxInfo;

extern  URLT1_STATUS  IDU_Urlt_Status;
extern	unsigned char TestCommSel;
/*
extern  unsigned char UARXREG;		//UA1RX
extern  unsigned char UATXREG;		//UA1TX
extern  unsigned char UARXIF;		//UA1RIF
extern  unsigned char UATXIF;		//UA1TIF
extern  unsigned char UAERIF;		//UA1REIF
extern  unsigned char UASTREG;		//UA1STR
extern  unsigned char UATSF;		//UA1TSF
extern  unsigned char UAPWR;		//UA1PWR
extern  unsigned char UARXE;		//UA1RXE
extern  unsigned char UATXE;		//UA1TXE

 user_com_main() 通信处理程序
 user_com_driver 通信数据发送接收处理程序
 user_com_tx_uart 数据发送程序
 user_com_rx_uart 数据接收程序
*/
unsigned char user_com_driver(unsigned char mode);
void user_com_tx_uart(void);
void user_com_rx_uart(void);
void	user_com_te_uart(void);
void user_com_send(unsigned char *buf, unsigned char len);
unsigned char user_com_recv_csum_calc(unsigned char *buf, unsigned char len);
unsigned char user_com_send_csum_calc(unsigned char *buf, unsigned char len);
unsigned char user_com_send_csum_calc1(unsigned char *buf, unsigned char len);
void user_com_bufclr(unsigned char *buf, unsigned char len);
void user_com_proc_1ms(void);
void user_com_reset(void);
void user_uart1_init(unsigned char StopBitSel);
#endif




