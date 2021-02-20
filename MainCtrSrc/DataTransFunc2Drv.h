#ifndef __DATATRANSFUNC2DRV_H__
#define __DATATRANSFUNC2DRV_H__

#define U8_COMP_FLUX  0
#define U8_FAN_FLUX   1
#define U8_ID_CURR    2
#define U8_IQ_CURR    3
#define U8_COMP_CURR  4
#define U8_PFC_CURR   5
#define U8_FAN_CURR   6
#define U8_TORQ_ANGL_L  7
#define U8_TORQ_ANGL_H  8
#define U8_TORQ_MAG     9

extern void fb_bldc64_err(void);
///extern void DataMainDrvCommInit(unsigned char *pMain2DrvBuf, unsigned char *pDrv2MainBuf);
extern void Main2DrvLytran(signed long *p2CmpDrvBuf, signed long *p2FanDrvBuf);
extern void DrvLytran2Main(signed long *pCompDrv2Buf, signed long *pFanDrv2Buf);

extern unsigned char u8PFCEnFlg, u8ErrCnt;
extern unsigned char u8MtrErrCode,
                     u8FMErrCode,
                     u8MtrFBSta,
                     u8FMFBSta,
                     u8ErrCode,
                     u8CompErrClr,
                     u8FanErrClr,
                     u8Timer1msfg;
extern unsigned char DrvEepPara[], /* *pMain2Drv, *pDrv2Main */ Main2Drv[], Drv2Main[];
extern unsigned char Drv2SysBuff[];
extern unsigned char u8LytranCompErrCode;
extern unsigned char u8LytranFanErrCode;

#endif

