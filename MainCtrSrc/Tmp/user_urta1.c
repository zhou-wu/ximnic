
/****************************************************************************
* Copyright (C),    Technology Co., Ltd.
* File Name          : user_pro.c
* Author             : StiveHe
* Date First Issued  : 2011-10-28
* Description        : High level system control module
********************************************************************************
* History:
* 2011-3-05        ver. 1.0
********************************************************************************
*
*******************************************************************************/

/* Includes ******************************************************/
//#pragma	 	ioreg												/* */
/*=============================================================*/

/*=============================================================*/
/* 				 				   */
/*=============================================================*/
#include  "vals_typedef.h"
#include "user_urta1.h"
#include "user_pro.h"
#include "user_Proteor.h"
#include "user_iic.h"
#include "user_DCFan.h"
#include "user_EXVCtrl.h"
/* 20171226  IDU_Fan Begin Tina */
#include "user_simurta.h"
/* 20171226  IDU_Fan End   Tina */
#include    "user_1T5.h"
#include    "SCI.h"

extern  enum
{STARTUP_TYPE1, STARTUP_TYPE2, STARTUP_TYPE3} StartUpType;

extern	unsigned char VAC_RMS_AVG;
extern	unsigned char Ref_R410A;
unsigned short com1_cnt_comtimeout;
URLT1_STATUS  IDU_Urlt_Status;
unsigned char comMonitorCnt;//为了解决商检通信问题增加
unsigned char ChkCommSel;	//0: 检测中，1检测结束
unsigned char TestCommSel; //0: 1 stop bit, 1 2stop bits; 目前设置状态
extern unsigned char AcSensorErrCnt;
unsigned char customercode1;
/*
unsigned char UARXREG;		//UA1RX
unsigned char UATXREG;		//UA1TX
unsigned char UARXIF;		//UA1RIF
unsigned char UATXIF;		//UA1TIF
unsigned char UAERIF;		//UA1REIF
unsigned char UASTREG;		//UA1STR
unsigned char UATSF;		//UA1TSF
unsigned char UAPWR;		//UA1PWR
unsigned char UARXE;		//UA1RXE
unsigned char UATXE;		//UA1TXE
*/


//UN_TX_DATA    tclTxData;
//UN_TX_INFO    tclTxInfo;
//UN_RX_DATA    tclRxData;
//unsigned char urltMode;//0: IDU , 1: OTHER CONTROLLER testdata;
void user_com_uarterr_check(void);
/*******************************************************
*		UART1初始化
*******************************************************/
void user_uart1_init(unsigned char StopBitSel)
{
  int i;

  /**Serial Control register setting **/
  SCI5.SCR.BYTE = M_B_TO_I8(0, 0, 0, 0, 0, 0, 0, 0); //SCSCR_2
  /***********************| | | | | | | |
  			| | | | | | | |---CKE Clock Enable 00:SCK is ignored,01:internal clock,SCK for output,
  			| | | | | | | |------10:External clock,SCK pin used for clock input
  			| | | | | | | |------11:External clock,SCK pin used for clock input clock synchronous mode
  			| | | | | |-----TEIE Transmit end interrupt enable 0:disable,1:enable
  			| | | | |------MPIE Mutiprocessor interrupt enable 0:
  			| | | |-------RE Receive enable 0:disable,1:enable
  			| | |--------TE Transmit Enable 0: disble, 1:enable
  			| |---------RIE Recieve interrupt enable,0:disble,1:enable
  			|----------TIE Transmit interrupt enable 0:disble,1:enable
  ***********************/

  /** Serial mode register setting,Pф= 40MHZ **/
  if(1 == StopBitSel)
  {
#if (1==comm_Mode)
    SCI5.SMR.BYTE = M_B_TO_I8(0, 0, 1, 1, 0, 0, 1, 0); //2400bps
#else
    SCI5.SMR.BYTE = M_B_TO_I8(0, 0, 1, 0, 1, 0, 1, 0); //600bps
#endif
  }
  else
  {
#if (1==XIEYI_SELECT)
    SCI5.SMR.BYTE = M_B_TO_I8(0, 0, 1, 1, 0, 0, 1, 0); //2400bps
#else
    SCI5.SMR.BYTE = M_B_TO_I8(0, 0, 1, 0, 0, 0, 1, 0); //600bps
#endif
  }
  /*                  | | | | | | | | ----clock select 00: Pф,01:Pф/4,10:Pф/16,11:Pф/64
  					| | | | | |-----mutiprocessor mode 0: muti mode disabled,1: muti mode enable
  					| | | | |------stop bit length 0:1bit, 1: 2bit
  					| | | |-------parity mode 0: Even, 1: Odd
  					| | |--------parity Enable 0:not checked, 1: checked
  					| |---------Character Length 0:8bit , 1: 7bit
  					|----------Communication Mode 0: Asynchronous mode 1:Clock synchronous
    */

  SCI5.SCMR.BYTE = M_B_TO_I8(1, 1, 1, 1, 0, 0, 1, 0);
  /* 				 | | | | | | | |--Reserved  always be 0
  				 | | | | | | |---Reserved  always be 1
  				 | | | | | |----Reserved  always be 0
  				 | | | | |-----DIR Data Transfer Direction, 0: LSB, 1: MSB
  				 | | | |------Reserved
  */
  SCI5.SEMR.BYTE = 0x00;
  /*Bit Rate Register */
#if (1==XIEYI_SELECT)
  SCI5.BRR = 64;//1200 //129-1;//+1;	//600bps
#else
  SCI5.BRR = 128;//600 //129-1;//+1;	//600bps
#endif
  /*
  	N = Pф/(64 * BPS * 2^(2n-1)) - 1
  */
  i = SCI5.SSR.BYTE;
  SCI5.SSR.BYTE = 0;

  ICU.IPR[IPR_SCI5_ERI5].BIT.IPR = 10;		// interrupt priority

  ICU.IER[IER_SCI5_ERI5].BIT.IEN6 = 1;
  ICU.IER[IER_SCI5_RXI5].BIT.IEN7 = 1;
  ICU.IER[IER_SCI5_TXI5].BIT.IEN0 = 1;
  ICU.IER[IER_SCI5_TEI5].BIT.IEN1 = 1;

  //Set the PFC for the external pins to be used(SCK,TXD,RXD)
  MPC.PB5PFS.BIT.PSEL = 10;//TXD5
  PORTB.PMR.BIT.B5 = 1;
  PORTB.PDR.BIT.B5 = 1;
  PORTB.PODR.BIT.B5 = 1;
  PORTB.PCR.BIT.B5 = 1;		// PIN28	PB5 DIG OUT	TXD2_UP28		TX

  MPC.PB6PFS.BIT.PSEL = 10;//RXD5
  PORTB.PMR.BIT.B6 = 1;
  PORTB.PDR.BIT.B6 = 0;

  //Set TE and RE bits of SCSCR to 1 Set the RIE,TIE,TEIE,and MPIE bits in SCSCR
  /* Recieve Interrupt Enable (RIE = 1 ) */
  /* Transmission/Reception Enable (TE = 1, RE = 1) */
  //SCSCR_0 = 0b10110000;//0b00110100;
  SCI5.SCR.BYTE = M_B_TO_I8(0, 0, 0, 0, 0, 0, 0, 0);
  /*          	| | | | | | | |---CKE Clock Enable 00:SCK is ignored,01:internal clock,SCK for output,
  			| | | | | | | |------10:External clock,SCK pin used for clock input
  			| | | | | | | |------11:External clock,SCK pin used for clock input clock synchronous mode
  			| | | | | |-----TEIE Transmit end interrupt enable 0:disable,1:enable
  			| | | | |------MPIE Mutiprocessor interrupt enable 0:
  			| | | |-------RE Receive enable 0:disable,1:enable
  			| | |--------TE Transmit Enable 0: disble, 1:enable
  			| |---------RIE Recieve interrupt enable,0:disble,1:enable
  			|----------TIE Transmit interrupt enable 0:disble,1:enable
  */

  /*	Set interrupt priority od SCI_1 other than 0 in order to enbale interrupt. */
  //INTC.IPRL.BIT._SCI1 = 0x02;

  /*Serial Port Register*/
  //SCSPTR_2 = M_B_TO_I8(0,0,0,0,0,0,0,1);
  /* 				 | | | | | | | |--SPB0DT Serial port break data, control the TXD pins together with the TE bit bi SCSCR
  				 | | | | | | |---Reserved
  				 | | | | | |----SPB1DT Clock port data in serial port
  				 | | | | |-----SPB1/O Clock port input/Output in serial port
  				 | | | | |		0:don't output the SPB1DT bit to SCK pin
  				 | | | | |		1:Output the SPB1DT bit to SCK pin
  				 | | | |------Reserved
  				 |---------EIO Error interrupt only,0: disable,1:enable
  */
}


/*=============================================================*/
/* 		COM弶婜壔											   */
/*=============================================================*/
void user_com_init(void)
{
  //user_com_reset();
  //	user_uart1_init();
  //	COMMODE = UART;
  //	COM1TIMEOUT_RESET = 1000;
  user_com_bufclr(IDU_Urlt_Status.BufData, cn_BUFLEN);
  //user_com_recv(IDU_Urlt_Status.BufData, cn_BUFLEN);
  IDU_Urlt_Status.cnt_comrx = 0;
  IDU_Urlt_Status.cnt_comtx = 0;
  //IDU_Urlt_Status.Flag.bit_f. = 0;
  IDU_Urlt_Status.state = COM_POWERUP;
  IDU_Urlt_Status.timeCnt = 0;
  IDU_Urlt_Status.errCnt = 0;
  IDU_Urlt_Status.p_txbuf = IDU_Urlt_Status.BufData;
  //tclTxData.txData.faultCode1.all = 0;
  //tclTxData.txData.faultCode2.all = 0;
  //tclTxData.txData.faultCode3.all = 0;
  //tclTxData.txData.faultCode4.all = 0;
  //tclTxData.txData.levHigh = 0;
  //tclTxData.txData.levLow = 0;
  //tclTxData.txData.outMode = 0;
  //tclTxData.txData.outState = 0;
  comMonitorCnt = 0;
  Clr_1T5Flag();
}
/*=============================================================*/

/*=============================================================*/
/* 		儊僀儞儖乕僾										   */
/*=============================================================*/
void user_com_main(void)
{

  switch(IDU_Urlt_Status.state)
  {
    case COM_POWERUP:
      if(IDU_Urlt_Status.timeCnt > 1000)//410)
      {
        IDU_Urlt_Status.state = COM_RX;
        IDU_Urlt_Status.timeCnt = 0;
        //user_com_hw_reset();
        user_com_bufclr(IDU_Urlt_Status.BufData, cn_BUFLEN);
        IDU_Urlt_Status.cnt_comrx = 0;
        IDU_Urlt_Status.cnt_comtx = 0;
        IDU_Urlt_Status.urltsending = 0;
        IDU_Urlt_Status.CMD = 0x01;
        TestCommSel = 0;//TestCommSel = 1;
        user_uart1_init(1);
        CLR_UAERIF;
        CLR_UARXIF;
        SET_UARXE;
      }
      break;
    case COM_TX:
      //数据写如发送缓存
      if(!IDU_Urlt_Status.urltsending)
      {
        IDU_Urlt_Status.cnt_comtx = 40;
        IDU_Urlt_Status.BufData[0] = 0x85;

        IDU_Urlt_Status.BufData[1] = 0xA1;
        IDU_Urlt_Status.BufData[2] = 0xA0;

        IDU_Urlt_Status.BufData[3] = 0x01;
        IDU_Urlt_Status.BufData[4] = 0;
        IDU_Urlt_Status.BufData[5] = 0;
        IDU_Urlt_Status.BufData[6] = 0;
        IDU_Urlt_Status.BufData[7] = fgCtrl.freqGradeOut;
        IDU_Urlt_Status.BufData[8] = (unsigned char)sensorAD.outCoilMidTempAD;
        IDU_Urlt_Status.BufData[9] = (unsigned char)sensorAD.outdoorTempAD;
        IDU_Urlt_Status.BufData[10] = (unsigned char)(sensorAD.DischargeTempAD / 2);
        IDU_Urlt_Status.BufData[11] = 0;     //IPM温度   F
        IDU_Urlt_Status.BufData[12] = 0;
        IDU_Urlt_Status.BufData[13] = 0;
        IDU_Urlt_Status.BufData[14] = 0;
        switch(acState.basicRunMode)
        {
          case   WMS_OFF:
            IDU_Urlt_Status.BufData[15] = 0;
            break;
          case   WMS_REFRIGERATING:
            IDU_Urlt_Status.BufData[15] = 1;
            break;
          case   WMS_HEATING:
            IDU_Urlt_Status.BufData[15] = 2;
            break;
          case   WMS_DEHUMIDIFIERING:
            IDU_Urlt_Status.BufData[15] = 4;
            break;
          default:
            break;
        }
        if (tclOutState.bit.deforRun)IDU_Urlt_Status.BufData[16] = 0x01;
        if (hlFreqPro.lFreProFlg)IDU_Urlt_Status.BufData[16] = 0x02;
        IDU_Urlt_Status.BufData[17] = (signed short)(Compress.TargetFreq / 2);
        IDU_Urlt_Status.BufData[18] = freqCtrl.fFdb;
        IDU_Urlt_Status.BufData[18] /= 2;
        IDU_Urlt_Status.BufData[19] = (unsigned char)(Compress.TargetOutFanSpd / 10);
        IDU_Urlt_Status.BufData[20] = (unsigned char)(eevCtrl.nowOE / 4);
        IDU_Urlt_Status.BufData[21] = (unsigned char)(VAC_RMS_AVG / 1.37);
        IDU_Urlt_Status.BufData[22] = (unsigned char)(IAC_RMS_AVG / 4);
        IDU_Urlt_Status.BufData[23] = (unsigned char)((VAC_RMS_AVG / 1.37) * 2);
        IDU_Urlt_Status.BufData[24] = (unsigned char)(IAC_RMS_AVG / 2);
        if(!iComOverLoad.state)IDU_Urlt_Status.BufData[25] = 0x01;
        if(fgCtrl.AC_VoltFreMax < fgCtrl.freqGradeOut)IDU_Urlt_Status.BufData[25] = 0x02;
        if((!(sysFreqPro.fFallPro & 0x05)) || (!(sysFreqPro.fHoldPro & 0x05)))IDU_Urlt_Status.BufData[25] = 0x04;
        if((!(sysFreqPro.fFallPro & 0x02)) || (!(sysFreqPro.fHoldPro & 0x02)))IDU_Urlt_Status.BufData[25] = 0x08;
        if((!(sysFreqPro.fFallPro & 0x08)) || (!(sysFreqPro.fHoldPro & 0x08)))IDU_Urlt_Status.BufData[25] = 0x20;
        if((inFanLim.limEnable) && (((inCmd.cmd == WMS_HEATING) && (fgCtrl.inFanFreGMax = eepParam.member.HeatinFanLim_L))
                                    || ((inCmd.cmd == WMS_REFRIGERATING) && (fgCtrl.inFanFreGMax = eepParam.member.CoolinFanLim_L))))IDU_Urlt_Status.BufData[26] = 0x01;
        if((inFanLim.limEnable) && (((inCmd.cmd == WMS_HEATING) && (fgCtrl.inFanFreGMax = eepParam.member.HeatinFanLim_M))
                                    || ((inCmd.cmd == WMS_REFRIGERATING) && (fgCtrl.inFanFreGMax = eepParam.member.CoolinFanLim_M))))IDU_Urlt_Status.BufData[26] = 0x02;
        if(spySysParam.faultCode1.bit.outCoilMidSensor)IDU_Urlt_Status.BufData[27] = 0x01;
        if(spySysParam.faultCode1.bit.outTempSensor)IDU_Urlt_Status.BufData[27] = 0x02;
        if(spySysParam.faultCode1.bit.discharSensor)IDU_Urlt_Status.BufData[27] = 0x04;
        if(spySysParam.faultCode2.bit.eeprom)IDU_Urlt_Status.BufData[27] = 0x10;

        if(spySysParam.protCode1.bit.iacOverCur)IDU_Urlt_Status.BufData[28] = 0x01;
        if(spySysParam.faultCode1.bit.vACErr)IDU_Urlt_Status.BufData[28] = 0x02;
        if(spySysParam.protCode2.bit.inCoilFreeze || spySysParam.protCode2.bit.inCoilOverTemp)IDU_Urlt_Status.BufData[28] = 0x04;
        if(spySysParam.protCode2.bit.outCoilTemp)IDU_Urlt_Status.BufData[28] = 0x08;
        if(spySysParam.protCode1.bit.discharTemp)IDU_Urlt_Status.BufData[28] = 0x10;

        if(FANErrorCode || FanCommuniLost)IDU_Urlt_Status.BufData[29] = 0x01;
        if(spySysParam.faultCode2.bit.communicate)IDU_Urlt_Status.BufData[29] = 0x04;
        if(Protect.protect.bit.IPMFault || spySysParam.protCode2.bit.ipmErr)IDU_Urlt_Status.BufData[29] = 0x10;

        IDU_Urlt_Status.BufData[30] = 0;
        IDU_Urlt_Status.BufData[31] = 0;     //(商检标志：预留）F
        IDU_Urlt_Status.BufData[32] = 0;
        IDU_Urlt_Status.BufData[33] = 0;

        IDU_Urlt_Status.BufData[34] = 0;
        IDU_Urlt_Status.BufData[35] = 0;
        IDU_Urlt_Status.BufData[36] = 0;
        IDU_Urlt_Status.BufData[37] = 0;



        IDU_Urlt_Status.BufData[IDU_Urlt_Status.cnt_comtx - 1] = user_com_send_csum_calc((IDU_Urlt_Status.BufData), IDU_Urlt_Status.cnt_comtx - 1);
        IDU_Urlt_Status.urltsending = 1;
        IDU_Urlt_Status.timeCnt = 0;
        IDU_Urlt_Status.state = COM_RXWAIT;
        //UA1TXE = 1;
        CLR_UARXE;
        IDU_Urlt_Status.p_txbuf = IDU_Urlt_Status.BufData;
        //UATXREG = *IDU_Urlt_Status.p_txbuf++;
        //IDU_Urlt_Status.cnt_comtx--;
        SET_UATXE;
      }
      break;

    case COM_RESET:
      //连续三分钟不能通讯，报通讯故障，并且停机
      IDU_Urlt_Status.state = COM_POWERUP;

      IDU_Urlt_Status.timeCnt = 0;
      break;

    case COM_RXWAIT:
      if(IDU_Urlt_Status.cnt_comtx > 0)
      {
        IDU_Urlt_Status.timeCnt = 0;
      }
      if(IDU_Urlt_Status.timeCnt >= 40)
      {
        //发送超时,发送故障
        IDU_Urlt_Status.state = COM_RX;//COM_RESET;
        IDU_Urlt_Status.cnt_comrx = 0;
        IDU_Urlt_Status.timeCnt = 0;
        CLR_UAERIF;
        CLR_UARXIF;
        SET_UARXE;

        //IDU_Urlt_Status.tx.state = SCI_IDLE;
      }
      break;

    case COM_RX:
      //若主机发送数据后500ms内接收不到从机数据或数据错误时主机再次发送数据


      if(IDU_Urlt_Status.timeCnt > 1000)
      {
        IDU_Urlt_Status.errCnt++;
        if(MER_TEST == inCmd.testCmd)
        {
          comMonitorCnt = 2;
        }

        if(IDU_Urlt_Status.errCnt >= 110)//120 )
        {
          IDU_Urlt_Status.errCnt = 0;
          IDU_Urlt_Status.state = COM_RESET;
          if(!Ref_R410A)
          {
            spySysParam.faultCode2.bit.communicate = 1;
            Protect.protect.bit.CommuFault = 1;

          }

        }
        else
        {
          IDU_Urlt_Status.cnt_comrx = 0;
          IDU_Urlt_Status.state = COM_RXWAIT;
        }
        IDU_Urlt_Status.timeCnt = 0;

      }
      else
      {

        if(IDU_Urlt_Status.cnt_comrx == 1)
        {
          if(0x58 != IDU_Urlt_Status.BufData[0])
          {
            IDU_Urlt_Status.cnt_comrx = 0;
          }
        }
        if(IDU_Urlt_Status.cnt_comrx >= 2)
        {
          if(0xA0 != IDU_Urlt_Status.BufData[1])
          {
            IDU_Urlt_Status.cnt_comrx = 0;
          }
        }
        if(IDU_Urlt_Status.cnt_comrx == 3)
        {
          if(0xA0 != IDU_Urlt_Status.BufData[2])
          {
            //IDU_Urlt_Status.cnt_comrx = 0;
          }
        }

        if(IDU_Urlt_Status.cnt_comrx >= 40)		// 庴怣姰椆
        {
          if(IDU_Urlt_Status.BufData[39] == user_com_send_csum_calc((IDU_Urlt_Status.BufData), 39))
          {
            IDU_Urlt_Status.errCnt = 0;
            spySysParam.faultCode2.bit.communicate = 0;
            Protect.protect.bit.CommuFault = 0;
            GetIndoorParam_Tcl();
            comMonitorCnt = 25;
            IDU_Urlt_Status.state = COM_TXWAIT;
            IDU_Urlt_Status.timeCnt = 0;

          }


        }

      }
      //user_com_rx_uart();
      break;

    case COM_TXWAIT:
      //主机接收并处理完数据后，等待20ms后再发送数据
      if(IDU_Urlt_Status.timeCnt > 40)
      {
        CLR_UARXE;
        IDU_Urlt_Status.state = COM_TX;
        IDU_Urlt_Status.cnt_comrx = 0;
        IDU_Urlt_Status.timeCnt = 0;
      }
      break;

    case COM_FAULT:
    default:
      //若主机连续3min 接收不到从机数据或数据错误，则报通信故障。
      // sysFault.code = FAULT_COMMUN;
      //            Protect.protect.bit.CommuFault = 1;
      IDU_Urlt_Status.state = COM_POWERUP;
      break;
  }
}
/*=============================================================*/
/*=============================================================*/
/* 															   */
/*=============================================================*/
u1 user_com_driver(u1 mode)
{
  u1 val = 0;

  if(COM_RX == mode)
  {
    user_com_rx_uart();
  }
  //else if((COM_TX == mode)&&IDU_Urlt_Status.urltsending)
  //{
  //	val = user_com_tx_uart();
  //}
  user_com_uarterr_check();
  return(val);
}
/*=============================================================*/

/*=============================================================*/
/* 		UART捠怣僄儔乕僠僃僢僋								   */
/*=============================================================*/
//u1 user_com_uarterr_check(void)
void	user_com_uarterr_check(void)
{
  /*
  u1 val = 0;
  if(0!=UAERIF)
  {
  	u1 dummy = UARXREG;
  	UASTREG = 0x00;
  	UAERIF = 0;
  	val = 1;
  }
  return(val);
  */


}
/*=============================================================*/
/*=============================================================*/
/*=============================================================*/
/* 		UART数据发送										   */
/*=============================================================*/
//u1 user_com_tx_uart(void)
void user_com_tx_uart(void)
{
  //if(0!=IDU_Urlt_Status.cnt_comtx)
  if(1 < IDU_Urlt_Status.cnt_comtx)
  {
    UATXREG = *IDU_Urlt_Status.p_txbuf++;
    IDU_Urlt_Status.cnt_comtx--;
  }
  else if(1 == IDU_Urlt_Status.cnt_comtx)
  {
    UATXREG = *IDU_Urlt_Status.p_txbuf++;
    IDU_Urlt_Status.cnt_comtx--;

    //IDU_Urlt_Status.state = COM_TXWAIT;
    //IDU_Urlt_Status.timeCnt = SCI0_TX_WAIT_TIM-4;
    //IDU_Urlt_Status.urltsending = 0;

    //CLR_UATXE;
    //SET_UARXE;
    SCI5.SCR.BYTE |= 0x04;
    SCI5.SCR.BYTE = SCI5.SCR.BYTE & (~0x80);
  }

}
/*=============================================================*/

/*=============================================================*/
/* 		UART数据接收程序中断处理							   */
/*=============================================================*/
//u1	user_com_rx_uart(void)
void user_com_rx_uart(void)
{
  IDU_Urlt_Status.timeCnt = 0;
#if (0==XIEYI_SELECT)
  IDU_Urlt_Status.BufData[IDU_Urlt_Status.cnt_comrx++] = UARXREG;
  if(IDU_Urlt_Status.cnt_comrx >= 40)
  {
    CLR_UARXE;
    //SET_UATXE;
  }
#else
  b_Master_Rxd_Buffer[IDU_Urlt_Status.cnt_comrx] = UARXREG;
  if(b_Master_Rxd_Buffer[0] == ( b_Master_Command_Byte | 0x80 ))
  {
    IDU_Urlt_Status.cnt_comrx ++;
    b_Master_Rxd_Counter ++;
  }
  else
  {
    IDU_Urlt_Status.cnt_comrx = 0;
    b_Master_Rxd_Counter = 0;
  }
  if(IDU_Urlt_Status.cnt_comrx >= 40)//(IDU_Urlt_Status.DataLength+6)
  {
    CLR_UARXE;
    //SET_UATXE;
  }
#endif
}

//the shift register is void
void	user_com_te_uart(void)
{
  if(!IDU_Urlt_Status.cnt_comtx)
  {
    SCI5.SCR.BYTE &= (~0x04);//SCSCR_2
    //IDU_Urlt_Status.state = COM_TXWAIT;
    IDU_Urlt_Status.timeCnt = SCI0_TX_WAIT_TIM - 4;
    IDU_Urlt_Status.urltsending = 0;
    CLR_UATXE;
  }
}
/*=============================================================*/

/*=============================================================*/
/* 															   */
/*=============================================================*/
void user_com_send(u1 *buf, u1 len)
{
  //IDU_Urlt_Status.p_txbuf = IDU_Urlt_Status.BufData;
  //IDU_Urlt_Status.cnt_comtx = len;


}

/*=============================================================*/

/*=============================================================*/
/* 		憲怣僠僃僢僋僒儉寁嶼								   */
/*=============================================================*/
u1 user_com_send_csum_calc(u1 *buf, u1 len)
{
  u1 csum = 0;
  u2 i;
  for(i = 0; i < len; i++)
  {
    csum += buf[i];
  }
  csum = (csum ^ 0xFF) + 1;
  return(csum);
}
u1 user_com_send_csum_calc1(u1 *buf, u1 len)
{
  u1 csum = 0;
  u2 i;
  for(i = 0; i < len; i++)
  {
    csum += buf[i];
  }
  return(~csum);
}

/*=============================================================*/
/* 		捠怣僶僢僼傽僋儕傾									   */
/*=============================================================*/
void user_com_bufclr(u1 *buf, u1 len)
{
  u2 i;
  for(i = 0; i < len; i++)
  {
    buf[i] = 0;
  }
}
/*=============================================================*/

/*=============================================================*/
/* 		捠怣1ms僐乕儖娭悢									   */
/*=============================================================*/
//u1 user_com_proc_1ms(void)
void	user_com_proc_1ms(void)
{
  if(IDU_Urlt_Status.timeCnt < 0xffff)
  {
    IDU_Urlt_Status.timeCnt++;
  }
  //return(0);
}

/*=============================================================*/
/* 		urlt1 reset     									   */
/*=============================================================*/
void user_com_reset(void)
{
  /*
  	UARXE	= 0;		// RX disable
  	UATXE	= 0;		// TX disable
  	UAPWR	= 0;		// OFF
  	UASTREG = 0x00;		// 僄儔乕僼儔僌僋儕傾
  	UARXIF	= 0;
  	UATXIF	= 0;
  	UAERIF	= 0;
  	UAPWR	= 1;		// ON
  	UARXE	= 1;		// RX enable
  	UATXE	= 1;		// TX enable
  */


}

