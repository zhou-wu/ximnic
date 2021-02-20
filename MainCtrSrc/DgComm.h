#ifndef __DGCOMM_H__
#define __DGCOMM_H__

#define DBGCOMM_DATA_PORT_IN    u8MacroTmp=0 ///(PORTB.PDR.BIT.B3=0)
#define DBGCOMM_DATA_PORT       u8MacroTmp   ///PORTB.PIDR.BIT.B3
#define DBGCOMM_DATA_PORT_OUT   u8MacroTmp=0 ///(PORTB.PDR.BIT.B3=1)

#define DBGCOMM_DATA_H          u8MacroTmp=0 ///(PORTB.PODR.BIT.B3=1)
#define DBGCOMM_DATA_L          u8MacroTmp=0 ///(PORTB.PODR.BIT.B3=0)
#define DBGCOMM_CLK_PORT_IN     u8MacroTmp=0 ///(PORTB.PDR.BIT.B4=0)
#define DBGCOMM_CLK_PORT_MODE   u8MacroTmp=0 ///(PORTB.PMR.BIT.B4=1)
#define DBGCOMM_CLK_PORT        PORTB.PIDR.BIT.B4

#define U8_DBG_LEN 40               //DBG_BUFF_LENGTH

//Same as Drv, do not change:
//1-100: main error, 101-130: comp error, 131-160: fan error, 161-171: pfc error, others: reserve
//comp or fan: 1-30
#define PIN_VOLT_2_OVER_CURR          (1)         /* High impedance stop for over current*/
#define IPM_DECT_2_FO                 (2)         /* High impedance stop for over current*/
#define FREQ_DECT_2_LOSS_SPD          (3)         /* loss speed stop*/
#define BEMF_DECT_2_ZERO_SPD          (4)         /* zero speed stop*/
#define PEAK_RMS_2_OVER_CURR          (5)         /* Inverter instant over currrent */
#define RMS_30S_2_OVER_LOAD           (6)         /* motor overload */
#define PIN_START_2_CURR_ERR          (7)         /* inverter circuit error */
#define LOW_CURR_2_LOSS_PHS           (8)         /* Loss phase */
#define VDC_LOW_2_VOLT_DROP           (9)         /* Field weakening over input */
#define DLT_THETA_2_LOSS_STEP         (10)        /* loss step */
#define START_UP_MISS                 (11)        /* start up error */
//pfc:61-71
#define VDC_DECT_2_LOW_VOLT           (61)        /* voltage too low */
#define VDC_DECT_2_OVER_VOLT          (62)        /* voltage too high */
#define STOP_REASON_INSTANT_VOLTSTOP1 (63)        /* instant voltage stop 1 */
#define STOP_REASON_INSTANT_VOLTSTOP2 (64)        /* instant voltage stop 2 */
#define STOP_REASON_INSTANT_VOLTDROP  (65)        /* instant voltage drop */
#define HC_STOP_CONV_OVR_CURR_SW      (66)        /* software PFC over current */
#define PFC_RMS_2_OVER_CURR           (67)        /* PFC rms current over range */
#define PFC_CMP_2_OVER_CURR           (68)        /* PFC hardware over current */
#define PFC_START_2_CURR_ERR          (69)        /* PFC current sampling circuit error according to AD value  */
#define STOP_REASON_CNV_CURERR_PF     (70)        /* PFC current sampling circuit error accroding to PF */
#define PFC_RMS_2_OVER_LOAD           (71)        /* PFC over load */

#define STOP_IECTEST_ERR              (205)       /* IEC TEST ERR stop*/
#define DRIVE_STATUS_RUN              (2)
#define FM_STATUS_RUN                 (2)

union UNION_DGCOMMCTR_FLAGS
{
  unsigned char u8Byte;
  struct
  {

    unsigned char DgOutFreq      : 1;
    unsigned char DgOutFan       : 1;
    unsigned char DgOutPmv       : 1;
    unsigned char DgOutFw        : 1;
    unsigned char DgInFreq       : 1;
    unsigned char DgInFan        : 1;
    unsigned char DgInPmv        : 1;
    unsigned char DgInFw         : 1;
  } bits;
} ;
extern union UNION_DGCOMMCTR_FLAGS unDgCommCtr;

union UNION_DGCOMM_FLAGS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char DgSendHold      : 1;
    unsigned char CancelFreLim    : 1;
    unsigned char CancelInSet     : 1;
    unsigned char RecDebugBag1    : 1;
    unsigned char RecDebugBag3    : 1;
    unsigned char flg5            : 1;
    unsigned char DgDatRec        : 1;
    unsigned char DgCommSta       : 1;      //1--send, 0--receive
  } bits;
} ;
extern union UNION_DGCOMM_FLAGS unDgCommFlgs;

union UNION_DGSET_FLAGS
{
  unsigned char u8Byte;
  struct
  {
    unsigned char NoSendErr   : 1;
    unsigned char NoVoltLim   : 1;
    unsigned char NoCurrLim   : 1;
    unsigned char NoIn50Dbg   : 1;
    unsigned char no4     : 1;
    unsigned char no5     : 1;
    unsigned char no6     : 1;
    unsigned char no7     : 1;
  } bits;
} ;
extern union UNION_DGSET_FLAGS unDgSetFlgs;

extern void DgCommInit(void);
extern void DgCommSrv(void);
extern void r_icu_irq3_interrupt_Srv(void);

extern unsigned char u8DgCommRst1s;
extern unsigned char u8DgCommRstTim;
extern unsigned char u8ZGInCtrTrgHzOut;
extern unsigned short u16Dg2Drv;

#endif

