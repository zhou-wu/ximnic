#ifndef __MISCFUNC_H__
#define __MISCFUNC_H__

extern void DelaySoft1(unsigned short u16Cnt);
extern void DelaySoft2(unsigned char cnt);
extern void DgLedInit(void);
extern void BytesToMultiWords(unsigned char *readdata, unsigned short *StartAdress, unsigned char *endadress);
extern unsigned short BytesToSingleWord(unsigned char *readdata);
extern unsigned char CommChkSum(unsigned char *ptParaAddr, unsigned char u8ParaNum);
extern void DgLedBlink(void);

extern unsigned char u8DgLedBlinkTim;

extern unsigned short u16DgTmp0;
extern unsigned short u16DgTmp1;
extern unsigned short u16DgTmp2;
extern unsigned short u16DgTmp3;
extern unsigned short u16DgTmp4;
extern unsigned short u16DgTmp5;
extern unsigned short u16DgTmp6;
extern unsigned short u16DgTmp7;
extern unsigned short u16DgTmp8;
extern unsigned short u16DgTmp9;
extern unsigned short u16DgTmp10;

extern unsigned char u8MacroTmp;

#endif

