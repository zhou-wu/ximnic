/****************************************************************************
* Copyright (C),  Co., Ltd.

* 文 件 名： user_simurta.c

* 内容简述： IO口模拟SCI协议解析文件 002290  27.8
*
* 文件历史：
* 版本号        日期          作者              说明
*
****************************************************************************/
//#pragma ioreg
#include  "vals_typedef.h"
#include "user_Proteor.h"
#include "user_simurta.h"
#include "user_pro.h"
#include "user_iic.h"
#include "user_urta1.h"
#include "user_outpro.h"
#include "user_EXVCtrl.h"
#include "user_DCFan.h"
// 20161224 self-check start Tina
#include "user_PWMfan.h"
// 20161224 self-check end Tina

//extern  URLT1_STATUS  Urlt1Status;
//extern  signed int s4_fb_sys_read (unsigned char _u1_set_id);

extern unsigned char Flag_Plat_Freq1;
extern  unsigned char Plat_Freq1;


extern	unsigned char DcfanLowLimit;
extern  const	unsigned char  iniaddr[];
extern  unsigned char AVG_VBUS;
extern  unsigned char VAC_RMS_AVG;
extern  unsigned char IAC_RMS_AVG;
extern  Uchar8  disp_cnt;
extern  Uchar8  disp_ledState;
//extern  Uchar8  ResetCnt;
extern  unsigned int driveError;
extern  unsigned char   starDlyTimer;
extern  unsigned char gDeltaCnt;
extern  unsigned char SPD_REF;
extern  unsigned char u1_mode_system;
extern  unsigned char u1_com_system_ctrl;
//extern  unsigned char u1_flag_load_status;
extern  unsigned char TestDataDis;
extern  unsigned char FANCommCommand;
extern  unsigned char   Com_SDP;
//extern  unsigned char
//extern enum{
//    STARTUP_STOP,STARTUP_START,STARTUP_OVER,STARTUP_WAIT,STARTUP_WARM}StartUpMode;
extern  enum
{STARTUP_TYPE1, STARTUP_TYPE2, STARTUP_TYPE3} StartUpType;
extern  unsigned char CompStartupCnt;
extern SIM_UART_STATUS  Fan_simUartStatus;


//year,itemid, 0, 'O'--ODU,   D1.0  year  month   data
const unsigned char softVersion[8] = {170,   1,  0,   'O',    0x11,  18,   8,      18};



SIM_UART_STATUS simUartStatus;

//Uint16 comErrFlg = 0;
S4_UNION    s4_temp;
PC_COMMAND  pcSet;
TEST_COMMAND testCommand;
Uchar8  inforCnt;// = 0;
Uchar8  eepromversionSend;
unsigned char WrEEpromBuf[16];
enum {PC_NON, PC_DEBUG, PC_TEST} PCcommand;
unsigned char comprunTim;
void InitSimUart(void);
void SimUartReadByte(void);
void SimUartTxByte(void);
void SimUartRxFrame(void);
void SimUartTxFrame(void);
/*-----------------------------------------------------------------------------
* Function Name:InitSimUart()
* Description:
* Inputs:
* Returns:
* Comments:
-----------------------------------------------------------------------------*/
void InitSimUart(void)
{

  //配置串口发送:每个字节数据12位组成-- 起始位-1，数据位-8，偶校验-，停止位-1,
  //波特率500BPS
  // PM1.3  = 1;
  unsigned char i;
  /*Serial Control register setting */
  SCSCR_0 = M_B_TO_I8(0, 0, 0, 0, 0, 0, 0, 0); //0b00000000;//0b00110100;
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
  PBICR |= 0x02;
  /* Serial mode register setting,Pф= 40MHZ */
  SCSMR_0 = M_B_TO_I8(0, 0, 0, 0, 0, 0, 1, 0); //0b00101010;
  /*          	| | | | | | | |---clock select 00: Pф,01:Pф/4,10:Pф/16,11:Pф/64
  			| | | | | |-----mutiprocessor mode 0: muti mode disabled,1: muti mode enable
  			| | | | |------stop bit length 0:1bit, 1: 2bit
  			| | | |-------parity mode 0: Even, 1: Odd
  			| | |--------parity Enable 0:not checked, 1: checked
  			| |---------Character Length 0:8bit , 1: 7bit
  			|----------Communication Mode 0: Asynchronous mode 1:Clock synchronous
  */
  SCSCMR_0 = M_B_TO_I8(1, 1, 1, 1, 0, 0, 1, 0); //0b11110010;
  /* 				 | | | | | | | |--Reserved  always be 0
  				 | | | | | | |---Reserved  always be 1
  				 | | | | | |----Reserved  always be 0
  				 | | | | |-----DIR Data Transfer Direction, 0: LSB, 1: MSB
  				 | | | |------Reserved
  */
  SCSEMR_0 = 0x00;
  /*Bit Rate Register */
  SCBRR_0 = 64;	//1200bps
  /*
  	N = Pф/(64 * BPS * 2^(2n-1)) - 1
  */
  i = SCSSR_0;
  SCSSR_0 = 0;
  IER1A |= 0x0c0;
  IER1B |= 0x03;

  SCSCR_0 = 0x50;
  //Set the PFC for the external pins to be used(SCK,TXD,RXD)
  PFCMTU |= 0x02;
  PBDDR |= 0x04;
  PBDDR &= ~0x02;
  PBDR |= 0x06;

  //IPR80 = 7;
  //Set TE and RE bits of SCSCR to 1 Set the RIE,TIE,TEIE,and MPIE bits in SCSCR
  /* Recieve Interrupt Enable (RIE = 1 ) */
  /* Transmission/Reception Enable (TE = 1, RE = 1) */
  //SCSCR_0 = 0b10110000;//0b00110100;
  //SCSCR_0 = M_B_TO_I8(0,0,0,0,0,0,0,0);
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
  //SCSPTR_1 = M_B_TO_I8(0,0,0,0,0,0,0,1);
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


//===================================================TCL====================================================
/**************************************************************************
 * 函数名：CalcCheSun()
 * 功  能：计算数据校验和
 * 输  入：数组，计算数据长度
 * 输  出：返回校验和的低8位
 *************************************************************************/
Uchar8 CalcCheckSum(Uchar8 *pData, Uchar8 len)
{
  unsigned char i = 0, sum = 0;

  for (; i < len; i++)
  {
    sum += (unsigned char) pData[i];
  }

  sum = ~sum;
  return (sum & 0xFF);
}
void RelayTest(void)
{
  if(testCommand.test_command_flag1.bit.fourway)
  {
    OUT_4WAY_ON;
  }
  else
  {
    OUT_4WAY_OFF;
  }
  if(testCommand.test_command_flag1.bit.heater)
  {
    //OUT_HEATER_ON;
  }
  else
  {
    //OUT_HEATER_OFF;
  }
  if(testCommand.test_command_flag1.bit.high_fan)
  {
    OUT_FAN_HIGH;
  }
  else
  {

  }
  if(testCommand.test_command_flag1.bit.low_fan)
  {
    OUT_FAN_LOW;
  }
  else
  {

  }
  //IO_EXV_A = testCommand.test_command_flag2.bit.EXV_A;
  //IO_EXV_B = testCommand.test_command_flag2.bit.EXV_B;
  //IO_EXV_C = testCommand.test_command_flag2.bit.EXV_C;
  //IO_EXV_D = testCommand.test_command_flag2.bit.EXV_D;
}
/*-----------------------------------------------------------------------------
* Function Name:
* Description:
* Inputs:
* Returns:
* Comments:
-----------------------------------------------------------------------------*/
void CommunicateInit()
{
  // PAIORL &= 0x3fff;//PA14->RXD1,PA15->TXD1
  // PAIORL |= 0x8000;

  simUartStatus.state = SIMCOM_POWERUP;
  simUartStatus.timeCnt = 0;
  simUartStatus.TxEn = 0;
  inforCnt = 0;
  PCcommand = PC_NON;

  EuropeTest_PCset = 0;
  SH_pc_set(0);

  pcSet.pc_flag.all = 0;
  pcSet.pc_command_flag.all = 0;
  pcSet.testCompTarg = 0;
  pcSet.testExv = 0;
  pcSet.testFanSpeed = 0;
  testCommand.test_command_flag1.all = 0;
  testCommand.test_command_flag2.all = 0;
  //    pcSet.testFourWayValev = 0;

  // 20170317 India start Tina
  simUartStatus.nErrRestCnt = 10;
  // 20170317 India end Tina

  /* 20171226  IDU_Fan Begin Tina */
  pcSet.testFanSpeed_IDU = 0;
  /* 20171226  IDU_Fan End   Tina */
}


/*-----------------------------------------------------------------------------
* Function Name:
* Description:
* Inputs:
* Returns:
* Comments: 1ms模块 调用
-----------------------------------------------------------------------------*/
void Communicate_sim()
{
#if  _CHK_DRIVEPARA_ == 1
  Uchar8 i;
  //static Uint16 txNum = 0;

  signed int temp = 0;
#endif
  unsigned short temp;
  switch(simUartStatus.state)
  {
    case SIMCOM_POWERUP:
      //等待系统稳定后，再发送第一帧数据
      if(++simUartStatus.timeCnt > 1000)
      {
        simUartStatus.state = SIMCOM_TX;

        CLR_UA0TXE;
        SET_UA0RXE;
      }
      break;
    case SIMCOM_TX:
      //数据写如发送缓存
      //SimSciTx(SimTxBuf,TCL_BUFLEN);
      simUartStatus.simuart.tx.simBuf[0] = 0x55;
      if(!PCcommand)
      {
#if  _CHK_DRIVEPARA_ == 0
        if(inforCnt >= 9) //7)
        {
          //if(!strt_1t5.T1T5ComFlag || strt_1t5.freq1t5page >= 7)
          {
            inforCnt = 0;
          }
        }
        inforCnt++;
        switch(inforCnt)
        {
          case    1://
            //simUartStatus.simuart.tx.simBuf[0] = 0x55;
            simUartStatus.simuart.tx.simBuf[1] = 0x01;
            switch(inCmd.cmd)//mode
            {
              case    WMS_REFRIGERATING:
                simUartStatus.simuart.tx.simBuf[2] = 0x01;
                break;
              case    WMS_HEATING:
                simUartStatus.simuart.tx.simBuf[2] = 0x02;
                break;
              case    WMS_DEHUMIDIFIERING:
                simUartStatus.simuart.tx.simBuf[2] = 0x04;
                break;
              case    WMS_OFF:
              default:
                simUartStatus.simuart.tx.simBuf[2] = 0x00;
                break;
            }

            switch(acState.subRunMode)
            {
              case    SUB_POWER://强力
                simUartStatus.simuart.tx.simBuf[3] = 0x02;//
                break;
              case    SUB_TEST:
                switch (cfTemp.setTemp)
                {
                  case TEMPER(16):
                  //制热中频测试
                  case TEMPER(29):
                    //制冷中频测试
                    simUartStatus.simuart.tx.simBuf[3] = 0xe0;//
                    break;
                  case TEMPER(17):
                  //制热额定频率测试
                  case TEMPER(30):
                    //制冷额定频率测试
                    simUartStatus.simuart.tx.simBuf[3] = 0xa0;//
                    break;
                  case TEMPER(18):
                  //制热高频测试
                  case TEMPER(31):
                    //制冷高频测试
                    simUartStatus.simuart.tx.simBuf[3] = 0x60;//
                    break;
                  default:
                    break;
                }
                break;
              case    SUB_ECONO:
                simUartStatus.simuart.tx.simBuf[3] = 0x04;//
                break;
              case    NO_TEST://无测试
              default:
                simUartStatus.simuart.tx.simBuf[3] = 0x00;//
                break;
            }
            switch(inSet.inFanGrade)//没给上位机传强力和微风
            {
              case    1:
                simUartStatus.simuart.tx.simBuf[4] = 3;
                break;
              case    2:
                simUartStatus.simuart.tx.simBuf[4] = 2;
                break;
              case    3:
                simUartStatus.simuart.tx.simBuf[4] = 1;
                break;

              /* 20180606 FAN_Grade   Begin  Jone */
              case    4:
                simUartStatus.simuart.tx.simBuf[4] = 4;
                break;
              case    5:
                simUartStatus.simuart.tx.simBuf[4] = 5;
                break;
              /* 20180606 FAN_Grade   Begin  Jone */

              case    0:
              default:
                simUartStatus.simuart.tx.simBuf[4] = 0;
                break;
            }
            //simUartStatus.simuart.tx.simBuf[4] = inSet.inFanGrade;
            simUartStatus.simuart.tx.simBuf[5] = cfTemp.setTemp;
            simUartStatus.simuart.tx.simBuf[6] = cfTemp.roomTemp;
            simUartStatus.simuart.tx.simBuf[7] = cfTemp.inCoilMidTemp;
            simUartStatus.simuart.tx.simBuf[8] = 0x00;
            simUartStatus.simuart.tx.simBuf[9] = 0x00;

            //Bit0	风机失速
            //Bit1	风机过零错误
            //Bit2	EEPROM参数错误
            //Bit3	室内板与显示板通信保护
            //Bit4	室温传感故障
            //Bit5	盘管传感故障
            //
            //Bit7	蒸发器防冻结保护
            if(inSet.inProtCode1 & 0x20)//idu fan lose speed
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x01;
            }
            if(inSet.inProtCode2 & 0x20)//室内风机保护
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x02;
            }
            if(inSet.inProtCode1 & 0x10)//室内EEPROM故障
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x04;
            }
            if(inSet.inProtCode1 & 0x40)//室内室温传感器故障
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x10;
            }
            if(inSet.inProtCode1 & 0x80)//室内盘管传感器故障
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x20;
            }
            if(spySysParam.protCode2.bit.inCoilFreeze
                || spySysParam.protCode2.bit.inCoilOverTemp) //室内防冻/防过热保护
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x80;
            }
            break;
          case    2:
            simUartStatus.simuart.tx.simBuf[1] = 0x02;
            simUartStatus.simuart.tx.simBuf[2] = acState.wkState;
            simUartStatus.simuart.tx.simBuf[3] = defrostCtrl.state;
            if(eepParam.member.outFanType & 0x07)
            {
              if(pcSet.pc_flag.bit.outfan)
              {
                simUartStatus.simuart.tx.simBuf[4] = pcSet.testFanSpeed;
              }
              else
              {
                simUartStatus.simuart.tx.simBuf[4] = outFanCtrl.outSpdOld;//Compress.TargetOutFanSpd;
              }
              switch(simUartStatus.simuart.tx.simBuf[4])
              {
                case    OutFanOff:
                  simUartStatus.simuart.tx.simBuf[4] = 0;
                  break;
                case    OutFanHig:
                  simUartStatus.simuart.tx.simBuf[4] = 1;
                  break;
                case    OutFanMid:
                  simUartStatus.simuart.tx.simBuf[4] = 2;
                  break;
                case    OutFanLow:
                  simUartStatus.simuart.tx.simBuf[4] = 3;
                default:
                  //simUartStatus.simuart.tx.simBuf[4] = (unsigned char)(Compress.FactOutFanSpd/10);
                  break;
              }
            }
            else
            {
              if(pcSet.pc_flag.bit.outfan)
              {
                simUartStatus.simuart.tx.simBuf[4] = pcSet.testFanSpeed;
              }
              else
              {
                simUartStatus.simuart.tx.simBuf[4] = (unsigned char)(Compress.TargetOutFanSpd / 10);
              }
            }
            simUartStatus.simuart.tx.simBuf[5]  = fgCtrl.freqGradeOut;
            if(pcSet.pc_flag.bit.compressor)
            {
              simUartStatus.simuart.tx.simBuf[6] = pcSet.testCompTarg;
              Com_SDP = pcSet.testCompTarg;
            }
            else
            {
              simUartStatus.simuart.tx.simBuf[6] = (SPD_REF);//Compress.TargetFreq;
              Com_SDP = SPD_REF;
            }

#if (DEBUG_RUN==1)
            if(pcSet.pc_flag.bit.compressor)
              simUartStatus.simuart.tx.simBuf[7] = (pcSet.testCompTarg);
            else
              simUartStatus.simuart.tx.simBuf[7] = spySysParam.inCmd;
#else
            simUartStatus.simuart.tx.simBuf[7] = freqCtrl.fFdb;//freqCtrl.fFdb;
#endif
            if(eevCtrl.nowOE < 481)
            {
              simUartStatus.simuart.tx.simBuf[8] = (unsigned char)(eevCtrl.nowOE >> 1);
            }
            else
            {
              simUartStatus.simuart.tx.simBuf[8] = 240;
            }

            simUartStatus.simuart.tx.simBuf[9] = 0;
            if(Compress.Status.bit.WayValveOn)
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x01;
            }
            if(flag_prewarm)
            {
              simUartStatus.simuart.tx.simBuf[9] |= 0x02;
            }

            break;
          case    3:
            simUartStatus.simuart.tx.simBuf[1] = 0x03;
            simUartStatus.simuart.tx.simBuf[2] = AVG_VBUS;
            simUartStatus.simuart.tx.simBuf[3] = IAC_RMS_AVG;
            simUartStatus.simuart.tx.simBuf[4] = cfTemp.outdoorTemp;
            simUartStatus.simuart.tx.simBuf[5] = cfTemp.outCoilMidTemp;
            simUartStatus.simuart.tx.simBuf[6] = cfTemp.dischargeTemp;
            simUartStatus.simuart.tx.simBuf[7] = exv_eeprom.member.CompressType;

            // 20170328 IPM_PRO start Tina
            //simUartStatus.simuart.tx.simBuf[8] = cfTemp.suctionTemp;

            simUartStatus.simuart.tx.simBuf[8] = cfTemp.IPMTemp + 60;
            // 20170328 IPM_PRO end Tina
            simUartStatus.simuart.tx.simBuf[9] = VAC_RMS_AVG;
            break;
          case    4:
            simUartStatus.simuart.tx.simBuf[1] = 0x04;
            //bit0	IPM故障
            //bit1	传感器故障
            //bit2	顶部温度开关
            //bit3	室内外通信故障
            //bit4	母线电压过高过低
            //bit5
            //bit6	电流采样故障
            //bit7	模块温度过高

            simUartStatus.simuart.tx.simBuf[2] = 0;
            if(Protect.protect.bit.IPMFault || spySysParam.protCode2.bit.ipmErr)
              simUartStatus.simuart.tx.simBuf[2] |= 0x01;
            //if(Protect.protect.bit.SensorError)
            //{
            //    simUartStatus.simuart.tx.simBuf[2] |= 0x02;
            //}
            if(Protect.protect.bit.CompTopfailure)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x04;
            }
            if(Protect.protect.bit.CommuFault)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x08;
            }
            if(Protect.protect.bit.VBUSError || sysVBUSError)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x10;
            }
            if(Protect.protect.bit.PFC_CurSensorErr || Protect.protect.bit.PFC_Fo
                || Protect.protect.bit.PFC_OverLoad)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x20;
            }
            if(Protect.protect.bit.CurSensorErr || spySysParam.faultCode1.bit.curSensor)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x40;
            }
            if(Protect.protect.bit.IPMTempOver)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x80;
            }

            //bit2 bit1 bit0	压机驱动其他异常
            //bit3	 交流电压
            //bit4
            //bit5	排气温度高温保护
            //bit6	室外冷凝器高温保护
            //bit7	室外温度超范围故障
            simUartStatus.simuart.tx.simBuf[3] = Protect.protect.bit.CompProtect;
            if(Protect.protect.bit.DriveOthers || Protect.protect.bit.CurSensorErr
                || (sysFault.compProCnt >= (eepParam.member.e9_times))) //_CN_MAX_IPMFO_)))
            {
              simUartStatus.simuart.tx.simBuf[3] = 0x07;
            }
            if(spySysParam.faultCode1.bit.vACErr)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x08;
            }
            if(Protect.protect.bit.IACOver || spySysParam.protCode1.bit.iacOverCur)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x10;
            }
            if(spySysParam.protCode1.bit.discharTemp)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x20;
            }

            if(spySysParam.protCode2.bit.outCoilTemp)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x40;
            }
            if(spySysParam.protCode2.bit.outTemp)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x80;
            }
            //bit0	室外管温传感器故障
            //bit1	排气温度传感器故障
            //bit2	室外温度传感器故障
            //bit3	回气温度传感器故障
            //bit4	散热器温度传感器故障
            //bit5	室外风机故障
            //bit6	预留
            //bit7	室外EEPROM故障

            simUartStatus.simuart.tx.simBuf[4] = 0;
            if(spySysParam.faultCode1.bit.outCoilMidSensor)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x01;
            }
            if(spySysParam.faultCode1.bit.discharSensor)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x02;
            }
            if(spySysParam.faultCode1.bit.outTempSensor)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x04;
            }
            if(spySysParam.faultCode1.bit.suctionSensor)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x08;
            }
            //if(spySysParam.faultCode1.bit.)
            //{
            //}
            if(FanCommuniLost || FANErrorCode)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x20;
            }
            if(spySysParam.faultCode2.bit.eeprom)
            {
              simUartStatus.simuart.tx.simBuf[4] |= 0x80;
            }
            simUartStatus.simuart.tx.simBuf[5] = sysFault.code;//FAULT_PHASE;

            //if(Flag_Motor_Run)
            //{
            //    simUartStatus.simuart.tx.simBuf[6] = 1;
            //}else
            //
            simUartStatus.simuart.tx.simBuf[6] = (unsigned char)driveError;//T2_limit.Tp_Value;//shCtrl.sh_mothod;//fgCtrl.freqGradeMax;////disp_ledState;//disp_cnt;
            //
            simUartStatus.simuart.tx.simBuf[7] = (unsigned char)PhaseCnt;//(Gtype_Down_Fre>>1);//Tina//0;//ew_flag;//EuropeTest;//inSet.inEER;//shCtrl.Tp_first;//shCtrl.Tp_Area;//strt_1t5.T1T5ComFlag;//IAC_RMS_HALL;//(unsigned char)Compress.TargetFreqOffset;//T2_limit.Tp_target;//eevCtrl.state;//(unsigned char)Compress.TargetFreqOffset;//(driveError>>8);//comm_acc_32s;//
            //

            simUartStatus.simuart.tx.simBuf[8] = (unsigned char)(Compress.FactOutFanSpd / 10); //ew_downStatus;// Drv_protect.Status;//(unsigned char)Fan_simUartStatus.errCnt;//T2_limit.T2_base;//;//(driveError>>16);//comm_fr_r_32s;//IDU_Urlt_Status.errCnt;////u1_mode_system;//u1_com_system_ctrl;//defrostCtrl.state;//
            if(1 == dcOutFanDerict)
            {
              simUartStatus.simuart.tx.simBuf[8] |= 0x80;
            }


            // 20161224 self-check start Tina
            //simUartStatus.simuart.tx.simBuf[9] = rstFreq.grade;

            if (IsSelfCheckFlg())
            {
              simUartStatus.simuart.tx.simBuf[9] = (unsigned char)(str_pwmfan.PWMduyte * 10 >> 8);
            }
            else
            {
              simUartStatus.simuart.tx.simBuf[9] = rstFreq.grade;
            }
            // 20161224 self-check end Tina
            break;

          case    5:
            simUartStatus.simuart.tx.simBuf[1] = 0x05;
            simUartStatus.simuart.tx.simBuf[2] = 0;
            //保护标志位(
            //bit0-内盘管防冻;
            //bit1-外管防过热;
            //bit2-内管防过热;
            //bit3-排气;
            //bit4-电流;
            //bit5 电压 ;
            //bit6 PFC)
            //if(sysFreqPro.proNum & 0x08)
            //{
            //    simUartStatus.simuart.tx.simBuf[2] |= 0x03;
            //}else
            if(sysFreqPro.fFallPro & 0x08) //排气
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x02;
            }
            else if(sysFreqPro.fHoldPro & 0x08)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x01;
            }
            if(iComOverLoad.state == 2) //压机电流
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x08;
            }
            else if(iComOverLoad.state == 1)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x04;
            }

            if(sysFreqPro.fFallPro & 0x01)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x20;
            }
            else if(sysFreqPro.fHoldPro & 0x01)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x10;
            }

            if(sysFreqPro.fFallPro & 0x04)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x80;
            }
            else if(sysFreqPro.fHoldPro & 0x04)
            {
              simUartStatus.simuart.tx.simBuf[2] |= 0x40;
            }
            simUartStatus.simuart.tx.simBuf[3] = 0;
            if(sysFreqPro.fFallPro & 0x02)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x02;
            }
            else if(sysFreqPro.fHoldPro & 0x02)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x01;
            }
            if(sysFreqPro.fFallPro & 0x10)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x08;
            }
            else if(sysFreqPro.fHoldPro & 0x10)
            {
              simUartStatus.simuart.tx.simBuf[3] |= 0x04;
            }
            simUartStatus.simuart.tx.simBuf[4] = fgCtrl.AC_VoltFreMax;//(unsigned char)vacLimCurK;

            simUartStatus.simuart.tx.simBuf[5] = oTempCur.grade;
            simUartStatus.simuart.tx.simBuf[6] = fgCtrl.inFanFreGMax;
            simUartStatus.simuart.tx.simBuf[7] = fgCtrl.oTempFreGMax;
            //
            simUartStatus.simuart.tx.simBuf[8] = FANErrorCode;
            if(FanCommuniLost)
            {
              simUartStatus.simuart.tx.simBuf[8] |= 0x80;
            }
            //
            /* 20171226  IAC_Pro_PC Begin Tina */
            /* Tina
                      simUartStatus.simuart.tx.simBuf[9] = (signed char)shCtrl.TpAD_ch;//IDU_Urlt_Status.errCnt;//T2_limit.now_status;//DcfanLowLimit;//sysFreqPro.fFallFlg;//Urlt1Status.errCnt;//sensorAD.vAcAD;//acState.state;//eepParam.buf[1];//ResetCnt;
            Tina */
            simUartStatus.simuart.tx.simBuf[9] = GetoTempVacCur();
            /* 20171226  IAC_Pro_PC End   Tina */
            break;
          case    6:
            simUartStatus.simuart.tx.simBuf[1] = 6;
            simUartStatus.simuart.tx.simBuf[2] = (unsigned char)(defrostCtrl.timeCnt / 60);
            simUartStatus.simuart.tx.simBuf[3] = defrostCtrl.beginTemp;
            simUartStatus.simuart.tx.simBuf[4] = Avg_Is;

#if 1
            simUartStatus.simuart.tx.simBuf[5] = StartUpMode;
            simUartStatus.simuart.tx.simBuf[6] = StartUpType;
            simUartStatus.simuart.tx.simBuf[7] = CompStartupCnt;
            if(shCtrl.sh_mothod)
            {
              //Tp control
              if(shCtrl.Tp_tar > 194)
              {
                simUartStatus.simuart.tx.simBuf[8] =  255;
              }
              else
              {
                simUartStatus.simuart.tx.simBuf[8] = shCtrl.Tp_tar + 60;
              }

            }
            else
            {
              simUartStatus.simuart.tx.simBuf[8] = (S8)shCtrl.supHeat;
            }
#else
            simUartStatus.simuart.tx.simBuf[5] = (unsigned char)shCtrl.TpAD_tar;
            simUartStatus.simuart.tx.simBuf[6] = (unsigned char)(shCtrl.TpAD_tar >> 8);
            simUartStatus.simuart.tx.simBuf[7] = (unsigned char)sensorAD.DischargeTempAD;
            simUartStatus.simuart.tx.simBuf[8] = (unsigned char)(sensorAD.DischargeTempAD >> 8);
#endif

            /* 20171226  IDU_Fan Begin Tina */
            /* Tina
                      simUartStatus.simuart.tx.simBuf[9] = (unsigned char)(Compress.FactOutFanSpd/10);
            Tina */
            if (Protect.protect.bit.CommuFault) // 室内外通信故障 F
            {
              inSet.IduFanSpeed = 0;
            }
            simUartStatus.simuart.tx.simBuf[9] = inSet.IduFanSpeed;			// 内风机转速 F
            /* 20171226  IDU_Fan End   Tina */
            break;
          case    7:
            simUartStatus.simuart.tx.simBuf[1] = 7;
            simUartStatus.simuart.tx.simBuf[2] = softVersion[0];
            simUartStatus.simuart.tx.simBuf[3] = softVersion[1];
            simUartStatus.simuart.tx.simBuf[4] = softVersion[2];
            simUartStatus.simuart.tx.simBuf[5] = softVersion[3];
            simUartStatus.simuart.tx.simBuf[6] = softVersion[4];
            simUartStatus.simuart.tx.simBuf[7] = softVersion[5];
            simUartStatus.simuart.tx.simBuf[8] = softVersion[6];
            simUartStatus.simuart.tx.simBuf[9] = softVersion[7];

            break;
          case    8:
            simUartStatus.simuart.tx.simBuf[1] = 0x08;
            if(eepromversionSend == 0)
            {
              eepromversionSend = 1;
              simUartStatus.simuart.tx.simBuf[2] = 0;
              simUartStatus.simuart.tx.simBuf[2 + 1] = 0; //strt_1t5.freq1t5page;
              simUartStatus.simuart.tx.simBuf[3 + 1] = IDU_Urlt_Status.errCnt;
              simUartStatus.simuart.tx.simBuf[4 + 1] = TestCommSel;
              simUartStatus.simuart.tx.simBuf[5 + 1] = inSet.inTxMode; //
              simUartStatus.simuart.tx.simBuf[6 + 1] = IDU_Urlt_Status.CMD; //
              simUartStatus.simuart.tx.simBuf[7 + 1] = inSet.inShorten;
              simUartStatus.simuart.tx.simBuf[8 + 1] = inSet.inFunction;

            }
            else
            {
              eepromversionSend = 0;
              simUartStatus.simuart.tx.simBuf[2] = 1;
              simUartStatus.simuart.tx.simBuf[3] = Euro_eeprom.member.coustomcode[0];
              simUartStatus.simuart.tx.simBuf[4] = Euro_eeprom.member.coustomcode[1];
              simUartStatus.simuart.tx.simBuf[5] = Euro_eeprom.member.coustomcode[2];//
              simUartStatus.simuart.tx.simBuf[6] = Euro_eeprom.member.coustomcode[3];//
              simUartStatus.simuart.tx.simBuf[7] = Euro_eeprom.member.coustomcode[4];
              simUartStatus.simuart.tx.simBuf[8] = eepParam.member.EepVersion;
              simUartStatus.simuart.tx.simBuf[9] = inSet.inEER;
            }
            break;
          case    9:
            simUartStatus.simuart.tx.simBuf[1] = 0x09;
            simUartStatus.simuart.tx.simBuf[2] = shCtrl.sh_value_h;
            simUartStatus.simuart.tx.simBuf[3] = cfTemp.dischargeTemp;
            simUartStatus.simuart.tx.simBuf[4] = (signed char)shCtrl.Err;
            simUartStatus.simuart.tx.simBuf[5] = (signed char)shCtrl.LastErr;
            simUartStatus.simuart.tx.simBuf[6] = (signed char)(shCtrl.ajStepOut / 2);
            simUartStatus.simuart.tx.simBuf[7] = 0;
            simUartStatus.simuart.tx.simBuf[8] = 0;
            simUartStatus.simuart.tx.simBuf[9] = 0;
          // break;
          default:
            inforCnt = 0;
            break;
        }
#else

        simUartStatus.simuart.tx.simBuf[1] = inforCnt;
        i = inforCnt << 1;
        if(inforCnt >= 25)
        {
          inforCnt = 0;
          temp = s4_fb_sys_read (iniaddr[i]);
          simUartStatus.simuart.tx.simBuf[2] = (unsigned char)(temp >> 24);
          simUartStatus.simuart.tx.simBuf[3] = (unsigned char)(temp >> 16);
          simUartStatus.simuart.tx.simBuf[4] = (unsigned char)(temp >> 8);
          simUartStatus.simuart.tx.simBuf[5] = (unsigned char)(temp);

        }
        else
        {
          temp = s4_fb_sys_read (iniaddr[i]);
          simUartStatus.simuart.tx.simBuf[2] = (unsigned char)(temp > 24);
          simUartStatus.simuart.tx.simBuf[3] = (unsigned char)(temp >> 16);
          simUartStatus.simuart.tx.simBuf[4] = (unsigned char)(temp >> 8);
          simUartStatus.simuart.tx.simBuf[5] = (unsigned char)(temp);

          temp = s4_fb_sys_read (iniaddr[i + 1]);
          simUartStatus.simuart.tx.simBuf[6] = (unsigned char)(temp >> 24);
          simUartStatus.simuart.tx.simBuf[7] = (unsigned char)(temp >> 16);
          simUartStatus.simuart.tx.simBuf[8] = (unsigned char)(temp >> 8);
          simUartStatus.simuart.tx.simBuf[9] = (unsigned char)(temp);
          inforCnt++;
        }



#endif
      }
      else
      {
        //if(0xe1 == simUartStatus.simuart.tx.simBuf[1])
        {
          PCcommand = PC_NON;
        }
      }
      simUartStatus.simuart.tx.simBuf[10]
        = user_com_send_csum_calc(simUartStatus.simuart.tx.simBuf, 10) + 1; //

      //simUartStatus.simuart.tx.Flag.bit.byteOver = 1;
      simUartStatus.simuart.tx.state = SIM_SENDOVER;
      simUartStatus.state = SIMCOM_TXWAIT;
      simUartStatus.timeCnt = 0;
      simUartStatus.simuart.rx.cnt = 0;
      CLR_UA0RXE;
      SET_UA0TXE;
      SCTDR_0 = simUartStatus.simuart.tx.simBuf[0];
      break;
    case SIMCOM_RESET:

      simUartStatus.state = SIMCOM_RXWAIT;
      simUartStatus.simuart.rx.state = SIM_READREADY;
      simUartStatus.timeCnt = 0;
      //comErrFlg = 1;
      if (++simUartStatus.errCnt > COMMUN_ERR_DLY)
      {
        //通讯故障,报错
        simUartStatus.state = SIMCOM_FAULT;
        simUartStatus.errCnt = 0;
        CommunicateInit();
        simUartStatus.timeCnt = 900;
      }
      break;

    case SIMCOM_TXWAIT:
      if(++simUartStatus.timeCnt > 300)
      {
        //发送超时,发送故障
        simUartStatus.state = SIMCOM_TX;
      }
      else
      {

      }
      break;
    case SIMCOM_RX:
      //若主机发送数据后500ms内接收不到从机数据或数据错误时主机再次发送数据
      if(++simUartStatus.timeCnt > 300)
      {
        //重新发送数据
        simUartStatus.state = SIMCOM_RESET;
        simUartStatus.timeCnt = 0;
      }
      else
      {
        if(simUartStatus.simuart.rx.cnt == 1)
        {
          if(0xAA != simUartStatus.simuart.rx.simBuf[0])
          {
            simUartStatus.simuart.rx.cnt = 0;
          }
        }
        else
        {
          if(simUartStatus.simuart.rx.Flag.bit.frameOver)
          {
            simUartStatus.simuart.rx.Flag.bit.frameOver = 0;
            simUartStatus.state = SIMCOM_RXWAIT;
            simUartStatus.timeCnt = 0;
            temp = user_com_send_csum_calc(simUartStatus.simuart.rx.simBuf, 10) + 1;
            if(simUartStatus.simuart.rx.simBuf[10]
                == (unsigned char)temp)//(user_com_send_csum_calc(simUartStatus.simuart.rx.simBuf,10)+1))//)
            {
              switch(simUartStatus.simuart.rx.simBuf[1])
              {
                case    0x11://read the system running parameter
                  PCcommand = PC_NON;
                  break;
                case    0x20://read the compressor parameter
                  PCcommand = PC_DEBUG;
                  s4_temp.s4_data = s4_fb_sys_read (simUartStatus.simuart.rx.simBuf[2]);
                  simUartStatus.simuart.tx.simBuf[3] = s4_temp.u1_buf[0];
                  simUartStatus.simuart.tx.simBuf[4] = s4_temp.u1_buf[1];
                  simUartStatus.simuart.tx.simBuf[5] = s4_temp.u1_buf[2];
                  simUartStatus.simuart.tx.simBuf[6] = s4_temp.u1_buf[3];
                  simUartStatus.simuart.tx.simBuf[7] = 0;
                  simUartStatus.simuart.tx.simBuf[8] = 0;
                  simUartStatus.simuart.tx.simBuf[9] = 0;
                  break;
                case    0x21://set the compressor parameter
                  PCcommand = PC_DEBUG;
                  s4_temp.u1_buf[0] = simUartStatus.simuart.rx.simBuf[3];
                  s4_temp.u1_buf[1] = simUartStatus.simuart.rx.simBuf[4];
                  s4_temp.u1_buf[2] = simUartStatus.simuart.rx.simBuf[5];
                  s4_temp.u1_buf[3] = simUartStatus.simuart.rx.simBuf[6];

                  if(ID_GAIN_IROSS == simUartStatus.simuart.rx.simBuf[2])
                  {
                    if(s4_temp.u1_buf[3] == 0x05)
                    {
                      PC_LIJU_SET = 1;
                    }
                    else if(s4_temp.u1_buf[3] == 0x04)
                    {
                      PC_LIJU_SET = 2;
                    }
                    else
                    {
                      PC_LIJU_SET = 0;
                    }
                  }
                  else
                  {
                    PC_LIJU_SET = 0;
                  }


                  if(PC_LIJU_SET == 1)
                  {
                    drive_para_set(0x0A, ID_GAIN_IROSS); //M力矩切换
                  }
                  else if(PC_LIJU_SET == 2)
                  {
                    drive_para_set(0x08, ID_GAIN_IROSS); //无力矩切换
                  }
                  else
                  {
                    drive_para_set(0x00, ID_GAIN_IROSS); //E方控制
                  }



                  if(ID_PFC_ENABLE == simUartStatus.simuart.rx.simBuf[2])
                  {
                    unPFCLiju_SetFlag = 1;
                  }

                  drive_para_set(s4_temp.s4_data, simUartStatus.simuart.rx.simBuf[2]); //s4_temp.u1_buf[4] =
                  break;

                case    0x25://系统运行参数
                  PCcommand = PC_NON;
                  simUartStatus.errCnt = 0;
                  pcSet.pc_flag.all = simUartStatus.simuart.rx.simBuf[2];
                  pcSet.testCompTarg = simUartStatus.simuart.rx.simBuf[3];
                  /*
                  if(pcSet.pc_flag.bit.T1T5Flag )
                  {
                      if(Flag_Motor_Run&&(sysFault.code==0))
                      {
                          if((pcSet.testCompTarg>0))
                          {
                            if(comprunTim < 35)
                            {
                                pcSet.testCompTarg = Plat_Freq1;
                            }else{
                                  pcSet.testCompTarg *= 2;
                          }
                        }
                      }else{
                          pcSet.pc_flag.bit.compressor = 0;
                      }
                  }
                  */
                  pcSet.testFanSpeed = simUartStatus.simuart.rx.simBuf[4];
                  if(eepParam.member.outFanType & 0x07)//AC
                  {
                    if(pcSet.testFanSpeed > 3)
                    {
                      pcSet.testFanSpeed = 3;
                    }
                  }
                  else
                  {
                    switch(simUartStatus.simuart.rx.simBuf[4])
                    {
                      case    0:
                        pcSet.testFanSpeed = 0;
                        break;
                      case    1://high
                        pcSet.testFanSpeed = eepParam.member.coolDcFanSpd[10];
                        break;
                      case    2://mid
                        pcSet.testFanSpeed = eepParam.member.coolDcFanSpd[6];
                        break;
                      case    3:
                        pcSet.testFanSpeed = eepParam.member.coolDcFanSpd[4];
                        break;
                      default:
                        if(pcSet.testFanSpeed < 20)
                          pcSet.testFanSpeed = 20;
                        break;
                    }
                  }

                  // 20170317 India start Tina
                  // 如有外机故障，频率强控无效。强控频率时，风机默认高风 F
                  if (sysFault.code)
                  {
                    if ((spySysParam.faultCode1.all)
                        || (spySysParam.faultCode2.all & 0xFB)
                        || (spySysParam.protCode1.all)
                        || (spySysParam.protCode2.all)
                        || ((Protect.protect.all & 0xFFF7)))
                    {
                      pcSet.pc_flag.bit.compressor = 0; // 外机故障，强控无效 F
                      simUartStatus.nErrRestCnt = 10;
                    }
                    else
                    {
                      if ((Protect.protect.all & 0x0008) // 通信 F
                          || (spySysParam.faultCode2.all & 0x04) // 通信 F
                          || inSet.inProtCode1
                          || inSet.inProtCode2)
                      {
                        simUartStatus.nErrRestCnt = 0;
                      }
                      else
                      {
                        if (simUartStatus.nErrRestCnt)
                        {
                          pcSet.pc_flag.bit.compressor = 0; // 外机故障，强控无效 F
                        }
                      }
                    }
                  }
                  else
                  {
                    if (simUartStatus.nErrRestCnt)
                    {
                      simUartStatus.nErrRestCnt--;
                      pcSet.pc_flag.bit.compressor = 0; // 外机故障，强控无效 F
                    }
                  }

                  if (pcSet.pc_flag.bit.compressor)
                  {
                    if (!pcSet.pc_flag.bit.outfan)
                    {
                      pcSet.pc_flag.bit.outfan = 1;
                      if(eepParam.member.outFanType & 0x07)//AC
                      {
                        pcSet.testFanSpeed = 3;
                      }
                      else
                      {
                        pcSet.testFanSpeed = eepParam.member.coolDcFanSpd[10];
                      }
                    }
                  }
                  // 20170317 India end Tina

                  pcSet.pc_command_flag.all = simUartStatus.simuart.rx.simBuf[5];
                  pcSet.testExv = simUartStatus.simuart.rx.simBuf[6];
                  /* 20171226  IDU_Fan Begin Tina */
                  pcSet.testFanSpeed_IDU = simUartStatus.simuart.rx.simBuf[9];
                  /* 20171226  IDU_Fan End   Tina */
                  break;
                //case    0x30://设置系统在线测试参数(生产线测试使用)
                //    PCcommand = PC_DEBUG;
                //
                //    break;
                //case    0x31://查询系统在线测试状态(生产线测试使用)
                //    PCcommand = PC_DEBUG;
                //
                //    break;

                // 20161228 self-check start Tina
                case    0x55://查询系统在线测试状态(生产线测试使用) F
                  if (0x89 == simUartStatus.simuart.rx.simBuf[2])
                  {
                    PCcommand = PC_NON;
                    pcSet.nCkDlyTim = simUartStatus.simuart.rx.simBuf[3];

                    pcSet.SC_CompTarg = (unsigned short)simUartStatus.simuart.rx.simBuf[4] << 1; // 频率 F

                    if (pcSet.SC_CompTarg > FREQHZ(120))
                    {
                      pcSet.SC_CompTarg = FREQHZ(120);
                    }

                    Compress.Status.bit.rsvd1 = 1; // 自检 F

                    pcSet.SC_FanSpeed = simUartStatus.simuart.rx.simBuf[5]; // 风速 F

                    pcSet.nPWMStandard = simUartStatus.simuart.rx.simBuf[6];		// 自检风机 F

                    if (pcSet.SC_FanSpeed <= 30)
                    {
                      pcSet.SC_FanSpeed = 30;
                    }
                    else if (pcSet.SC_FanSpeed >= 90)
                    {
                      pcSet.SC_FanSpeed = 90;
                    }
                    else
                    {
                      pcSet.SC_FanSpeed = (pcSet.SC_FanSpeed + 5) / 10 * 10;
                    }

                    pcSet.nT4Standard = simUartStatus.simuart.rx.simBuf[7];		// 自检T4 F
                    pcSet.nT3Standard = simUartStatus.simuart.rx.simBuf[8];		// 自检T3 F
                    pcSet.nTpStandard = simUartStatus.simuart.rx.simBuf[9];		// 自检TP F
                  }

                  break;
                // 20161228 self-check end Tina

                case 0x60:
                  PCcommand = PC_TEST;
                  testCommand.test_command_flag1.all = simUartStatus.simuart.rx.simBuf[2];
                  testCommand.test_command_flag2.all = simUartStatus.simuart.rx.simBuf[3];
                  RelayTest();
                  break;
                case    0xe1:
                  PCcommand = PC_DEBUG;
                  if(2 == simUartStatus.simuart.rx.simBuf[2])
                  {
                    //over
                    //Page_Wr(0,(simUartStatus.simuart.rx.simBuf[3]<<2),eepParam.buf);
                    User_iic_inti();

                  }
                  else
                  {
                    simUartStatus.timeCnt = 280;
                    switch(simUartStatus.simuart.rx.simBuf[3] & 0x01)
                    {
                      case 0:
                        WrEEpromBuf[0] = simUartStatus.simuart.rx.simBuf[6];
                        WrEEpromBuf[1] = simUartStatus.simuart.rx.simBuf[7];
                        WrEEpromBuf[2] = simUartStatus.simuart.rx.simBuf[8];
                        WrEEpromBuf[3] = simUartStatus.simuart.rx.simBuf[9];
                        break;
                      case   1:
                        WrEEpromBuf[4] = simUartStatus.simuart.rx.simBuf[6];
                        WrEEpromBuf[5] = simUartStatus.simuart.rx.simBuf[7];
                        WrEEpromBuf[6] = simUartStatus.simuart.rx.simBuf[8];
                        WrEEpromBuf[7] = simUartStatus.simuart.rx.simBuf[9];
                        /*
                        break;
                        case  2:
                           WrEEpromBuf[8] = simUartStatus.simuart.rx.simBuf[6];
                        WrEEpromBuf[9] = simUartStatus.simuart.rx.simBuf[7];
                        WrEEpromBuf[10] = simUartStatus.simuart.rx.simBuf[8];
                        WrEEpromBuf[11] = simUartStatus.simuart.rx.simBuf[9];
                           break;
                        case    3:
                           WrEEpromBuf[12] = simUartStatus.simuart.rx.simBuf[6];
                        WrEEpromBuf[13] = simUartStatus.simuart.rx.simBuf[7];
                        WrEEpromBuf[14] = simUartStatus.simuart.rx.simBuf[8];
                        WrEEpromBuf[15] = simUartStatus.simuart.rx.simBuf[9];*/
                        temp = (unsigned short)simUartStatus.simuart.rx.simBuf[2];
                        temp <<= 8;
                        temp += ((unsigned short)(simUartStatus.simuart.rx.simBuf[3] & 0xfe));
                        temp <<= 2;
                        if(Page_Wr((temp), 8, WrEEpromBuf))
                        {
                          PCcommand = PC_NON;
                          simUartStatus.timeCnt = 0;
                        }
                        break;
                      default:

                        break;
                    }

                  }
                  break;
                case 0xe0:
                  PCcommand = PC_DEBUG;
                  if(2 == simUartStatus.simuart.rx.simBuf[2])
                  {
                    User_iic_inti();
                  }
                  else
                  {
                    if(0 == simUartStatus.simuart.rx.simBuf[2])
                    {
                      if(!simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0, 255, eepParam.buf);
                        //Rand_RD(255,(eepParam.buf+255));
                      }
                      else if(0x40 == simUartStatus.simuart.rx.simBuf[3])
                      {

                        Sequ_RD(0x100, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                      else if(0x80 == simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0x200, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                      else if(0xc0 == simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0x300, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                    }
                    else
                    {
                      if(!simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0x400, 255, eepParam.buf);
                        //Rand_RD(255,(eepParam.buf+255));
                      }
                      else if(0x40 == simUartStatus.simuart.rx.simBuf[3])
                      {

                        Sequ_RD(0x500, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                      else if(0x80 == simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0x600, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                      else if(0xc0 == simUartStatus.simuart.rx.simBuf[3])
                      {
                        Sequ_RD(0x700, 255, eepParam.buf);
                        //Rand_RD(256+255,(eepParam.buf+255));
                      }
                    }
                    temp = simUartStatus.simuart.rx.simBuf[3];
                    if(temp >= 0x40)
                    {
                      temp -= 0x40;
                    }
                    if(temp >= 0x40)
                    {
                      temp -= 0x40;
                    }
                    if(temp >= 0x40)
                    {
                      temp -= 0x40;
                    }
                    temp <<= 2;
                    simUartStatus.simuart.rx.simBuf[6] = eepParam.buf[temp];
                    simUartStatus.simuart.rx.simBuf[7] = eepParam.buf[temp + 1];
                    simUartStatus.simuart.rx.simBuf[8] = eepParam.buf[temp + 2];
                    simUartStatus.simuart.rx.simBuf[9] = eepParam.buf[temp + 3];
                    //simUartStatus.simuart.rx.simBuf[3]>>=2;
                    simUartStatus.timeCnt = 280;
                  }
                  break;
                case    0xf1:
                  PCcommand = PC_DEBUG;
                  s4_temp.u1_buf[0] = simUartStatus.simuart.rx.simBuf[3];
                  s4_temp.u1_buf[1] = simUartStatus.simuart.rx.simBuf[4];
                  s4_temp.u1_buf[2] = simUartStatus.simuart.rx.simBuf[5];
                  s4_temp.u1_buf[3] = simUartStatus.simuart.rx.simBuf[6];
                  FANCommCode = simUartStatus.simuart.rx.simBuf[2];
                  PCSetFanPar = 1;
                  break;
                case    0xf0:

                  PCSetFanPar = 2;
                  break;
                case	0xd1://电子膨胀阀调节方式设置　
                  SH_method_set(simUartStatus.simuart.rx.simBuf[2]);
                  SH_pc_set(simUartStatus.simuart.rx.simBuf[3] & 0x01);
                  EuropeTest_PCset = simUartStatus.simuart.rx.simBuf[3] & 0x02;
                  if(simUartStatus.simuart.rx.simBuf[3])
                  {
                    SH_value_h_set(simUartStatus.simuart.rx.simBuf[4]);
                    SH_value_l_set(simUartStatus.simuart.rx.simBuf[5]);
                  }
                  PCcommand = PC_DEBUG;
                  break;
                case 0x80:
                  break;
                default:
                  PCcommand = PC_NON;
                  break;
              }
            }
          }
        }
      }
      break;

    case SIMCOM_RXWAIT:
      //主机接收并处理完数据后，等待100ms后再发送数据
      if(++simUartStatus.timeCnt > 300)
      {
        simUartStatus.state = SIMCOM_TX;
      }
      break;

    case SIMCOM_FAULT:
    default:
      simUartStatus.state = SIMCOM_POWERUP;
      //若主机连续2min 接收不到从机数据或数据错误，则报通信故障。
      //sysFault.code = FAULT_COMMUN;
      //Protect.protect.bit.CommuFault = 1;
      //communParam.state = SIMCOM_POWERUP;
      break;
  }
}
//************************************模拟SCI部分*************************************************

/**************************************************************************
 * 函数名：SimUartReadWord()
 * 功  能：UART模拟数据接收 ，接收一个字节数据
 *         1位起始位（低电平）、8位数据位、1位校验位和2位停止位（高电平）组成一帧数据。
 *
 *
 * 输  入:
 * 输  出:
 * 说  明：1.TCL板，接收需要反相. 2.带校验，可设置停止位长度
 *************************************************************************/
void SimUartReadByte()
{

  /*
         switch ((Uchar8)simUartStatus.simuart.rx.state)
         {
             case SIM_READREADY://查找下降沿
                 //simUartStatus.simuart.rx.busLevelOld = simUartStatus.simuart.rx.busLevel;
                 //simUartStatus.simuart.rx.busLevel = READ_RXBIT;
                 if((READ_RXBIT==0)&&(simUartStatus.simuart.rx.busLevel == 1))
                 {
                     simUartStatus.simuart.rx.state = SIM_READSTAR;
                     simUartStatus.simuart.rx.bitNum = 0;
                     simUartStatus.simuart.rx.Tcnt = 0;
                     simUartStatus.simuart.rx.checkCnt = 0;
                     simUartStatus.simuart.rx.dataOut = 0;
                     simUartStatus.simuart.rx.datatemp = 0x00;
                 }
                 simUartStatus.simuart.rx.busLevel = READ_RXBIT;
                 break;
             case SIM_READSTAR://接收起始位
                 if (++simUartStatus.simuart.rx.Tcnt >= 2)//(SIM_BPS_CNT/2))
                 {
                     simUartStatus.simuart.rx.Tcnt = 0;
                     simUartStatus.simuart.rx.state = SIM_READING;
                 }
                 //等待一个bit周期起始位
                 break;

             case SIM_READING://读取数据
                 if (++simUartStatus.simuart.rx.Tcnt >= SIM_BPS_CNT)
                 {
                     simUartStatus.simuart.rx.Tcnt = 0;

  //                  //滤波，读5次总线，高电平大于3次，才确认高电平有效
                     #if ENABLE_FILTER
                     if (pRx->busLevelCnt >= 3)
                     {
                         pRx->busLevel = 1;
                         pRx->checkCnt++;
                     }
                     else
                     {
                         pRx->busLevel = 0;
                     }
                     pRx->busLevelCnt = 0;
                     #else
                     //不滤波
                     if (READ_RXBIT)
                     {
                         simUartStatus.simuart.rx.busLevel = 0x80;
                         simUartStatus.simuart.rx.checkCnt++;
                     }
                     else
                     {
                         simUartStatus.simuart.rx.busLevel = 0;
                     }
                     #endif
                     if(++simUartStatus.simuart.rx.bitNum<9)
                     {
                         simUartStatus.simuart.rx.dataOut >>= 1;
                         simUartStatus.simuart.rx.dataOut |= simUartStatus.simuart.rx.busLevel;
                         //simUartStatus.simuart.rx.busLevel = (simUartStatus.simuart.rx.busLevel << simUartStatus.simuart.rx.bitNum);
                         //simUartStatus.simuart.rx.datatemp |= simUartStatus.simuart.rx.busLevel;
                     }
                     if(simUartStatus.simuart.rx.bitNum>=9){//判断校验
                             simUartStatus.simuart.rx.state = SIM_READOVER;
                             //simUartStatus.simuart.rx.dataOut = simUartStatus.simuart.rx.datatemp;
                             simUartStatus.simuart.rx.Flag.bit.byteOver = 1;
                     }
                 }

                 break;
             case SIM_READOVER://接收结束位

             default:
                 simUartStatus.simuart.rx.state = SIM_READREADY;
                 simUartStatus.simuart.rx.busLevel = 0;
                 break;
         }
    */

}

void SimUartRxFrame(void)
{
  //   if (simUartStatus.simuart.rx.Flag.bit.byteOver)
  //   {
  //清接收完毕标志
  //      simUartStatus.simuart.rx.Flag.bit.byteOver = 0;
  // simUartStatus.simuart.rx.simBuf[simUartStatus.simuart.rx.cnt++] = simUartStatus.simuart.rx.dataOut;
  // ++simUartStatus.simuart.rx.cnt;
  simUartStatus.simuart.rx.simBuf[simUartStatus.simuart.rx.cnt++] = SCRDR_0;
  if(simUartStatus.simuart.rx.cnt >= 11)
  {
    simUartStatus.simuart.rx.Flag.bit.frameOver = 1;
    simUartStatus.simuart.rx.cnt = 0;
    CLR_UA0RXE;
    //SET_UA0TXE;
  }
}

void SimUartTxFrame(void)
{
  if(simUartStatus.simuart.tx.cnt < 10)
  {
    SCTDR_0 = simUartStatus.simuart.tx.simBuf[simUartStatus.simuart.tx.cnt++];
    //simUartStatus.simuart.tx.dataOut = simUartStatus.simuart.tx.simBuf[simUartStatus.simuart.tx.cnt++];
    //simUartStatus.TxEn = 1;
    //simUartStatus.simuart.tx.state = SIM_SENDREADY;
    //simUartStatus.simuart.tx.cnt++;
  }
  else if(simUartStatus.simuart.tx.cnt == 10)
  {
    SCTDR_0 = simUartStatus.simuart.tx.simBuf[simUartStatus.simuart.tx.cnt++];
    SCSCR_0 |= 0x04;
    SCSCR_0 = SCSCR_0 & (~0x80);
  }
}


/**************************************************************************
 * 函数名：SimUartTxByte()
 * 功  能：UART模拟总线，发送一个字节数据
 *         1位起始位（低电平）、8位数据位和1位停止位（高电平）组成一帧数据。
 *
 *
 * 输  入：
 * 输  出：
 *
 *************************************************************************/
void SimUartTxByte(void)
{
  /*
     if(!simUartStatus.TxEn)
     {
         return;
     }
     switch (simUartStatus.simuart.tx.state)
     {
         case SIM_SENDREADY://start bit
             //simUartStatus.
             simUartStatus.simuart.tx.state = SIM_SENDING;//SIM_SENDSTAR;
             simUartStatus.simuart.tx.bitNum = 0;
             //simUartStatus.simuart.rx.busLevel = 0;
             simUartStatus.simuart.tx.Tcnt = 0;
             simUartStatus.simuart.tx.checkCnt = 0;
             CLR_TXBIT;
             break;
         case    SIM_SENDSTAR:
             simUartStatus.simuart.tx.state = SIM_SENDING;
             break;
         case SIM_SENDING:
             if(++simUartStatus.simuart.tx.Tcnt>=SIM_BPS_CNT)
             {
                 simUartStatus.simuart.tx.Tcnt = 0;
                 if(++simUartStatus.simuart.tx.bitNum < 9)
                 {
                     if(simUartStatus.simuart.tx.dataOut & 0x01)
                     {
                         SET_TXBIT;
                         simUartStatus.simuart.tx.checkCnt++;
                     }else{
                         CLR_TXBIT;
                     }
                     simUartStatus.simuart.tx.dataOut>>=1;
                 }else{//check bit & stop bit
                     SET_TXBIT;
                     simUartStatus.simuart.tx.state = SIM_SENDOVER;
                 }
             }
             break;
         case SIM_SENDOVER:
             if(++simUartStatus.simuart.tx.Tcnt>=SIM_BPS_CNT){
                 simUartStatus.simuart.tx.Flag.bit.byteOver = 1;
                 simUartStatus.TxEn = 0;
                 simUartStatus.simuart.tx.state = SIM_SENDREADY;
             }
             break;
     }*/
}


void    SimUartTeFrame(void)
{
  if(simUartStatus.simuart.tx.cnt >= 11)
  {
    SCSCR_0 &= (~0x04);
    simUartStatus.simuart.rx.cnt = 0;
    simUartStatus.state = SIMCOM_RX;//SIMCOM_TXWAIT;
    simUartStatus.timeCnt = 0;
    //simUartStatus.simuart.rx.state = SIM_READREADY;
    CLR_UA0TXE;
    SET_UA0RXE;
  }
}


void SimUart_Pro(void)
{
  /*
     if(simUartStatus.state == SIMCOM_TXWAIT)
     {
         SimUartTxFrame();
         SimUartTxByte();
     }else if(simUartStatus.state == SIMCOM_RX){
         SimUartReadByte();
         SimUartRxFrame();
     }*/
}

