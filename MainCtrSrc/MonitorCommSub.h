#ifndef __MONITORCOMMSUB_H__
#define __MONITORCOMMSUB_H__

#include "UsrDef.h"

#define CTR_FROM_PC     1
#define CTR_FROM_NOPC   0


///#define SimBuf   simUartStatus.simuart.tx.simBuf       ///

extern ByteTypeDef unSimurtCtr0;
#define fgCompCtrSrc    unSimurtCtr0.bits.no7
#define fgOutFanCtrSrc  unSimurtCtr0.bits.no6
#define fg4WayCtrSrc    unSimurtCtr0.bits.no5
#define fgEleHeatCtrSrc unSimurtCtr0.bits.no4
#define fgExVCtrSrc     unSimurtCtr0.bits.no3
//#define               unSimurtCtr0.bits.no2
#define fgForceCompOn   unSimurtCtr0.bits.no1
//#define               unSimurtCtr0.bits.no0

extern ByteTypeDef unPC2Ctr0;
#define fgPC2_4Way      unPC2Ctr0.bits.no7
#define fgPC2EleHeat    unPC2Ctr0.bits.no6
//#define     unPC2Ctr0.bits.no5
//#define     unPC2Ctr0.bits.no4
//#define     unPC2Ctr0.bits.no3
//#define     unPC2Ctr0.bits.no2
//#define     unPC2Ctr0.bits.no1
//#define     unPC2Ctr0.bits.no0

extern unsigned char u8PC2CompHz;
extern unsigned char u8PC2FanSpd;


extern void SimurtDec(unsigned char *pBuf);
extern void SimurtEnc(unsigned char *pBuf );

#endif

