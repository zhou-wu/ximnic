/*==============================================================================
            Copyright(C) 1997-2019.  Sinowealth Tech. Co., Ltd.
-------------------------------------------------------------------------------------------------------------------------------------------------
Project No.   : SH32F205_Motor_Model
FileName      : speed process.c
Description :  compressor motor control function
Notice           :
                    1: 
    
Author         : shawn.wan (email: shawn.wan@sinowealth.com)
Date             : 2016/12/8
Version       : V0.0

Function List :
             1: 
             2: 
             3: 
             4: 
             5: 

RevisionHistory:
Rev#         Date              Author        Comments(Function+Date)
-----+-----------------------+----------------+-------------------------------------------------------
0.0       2016/12/8          shawn.wan       Just build the function
0.1
==============================================================================*/

#include "sh32f205_device.h"


/*-----------------------------------------------------------------------------*/
/* define varibles */
cmotor_speed  g_CMwr;

unsigned char u8Timer_SpeedCtrl = 20;
unsigned char u8Timer_SpeedHold = 25;
	
/*-----------------------------------------------------------------------------*/
/* the gain table of speed PI */
#ifdef TEST_MOTOR
uint16 const SpeedKTable[FREQ_SEG_MAX][3] =
{
/* Seg    Freq   GainKp   GainKi  */
/* 0  */{   5,   200,     200 },
/* 1  */{   7,   200,     200 },
/* 2  */{  10,   200,    200 },
/* 3  */{  15,   200,    200 },
/* 4  */{  21,    200,     200 },
/* 5  */{  25,    200,     200 },
/* 6  */{  30,   200,     200 },
/* 7  */{  40,    200,     200 },
/* 8  */{  60,    200,     200 },
/* 9  */{  80,    200,     200 },
/* 10 */{ 100,    200,     200 },
/* 11 */{ 110,    150,     150 },
/* 12 */{ 115,    150,     150 },
};	 
#endif	
#ifdef RBS98D_FJ
unsigned int const SpeedKTable[FREQ_SEG_MAX][3] =
{
/* Seg    Freq   GainKp   GainKi  */
/* 0  */{   5,   100,     100 },
/* 1  */{   7,   150,     150 },
/* 2  */{  10,   200,    200 },
/* 3  */{  15,   300,    300 },
/* 4  */{  21,    500,     500 },
/* 5  */{  25,    600,     600 },
/* 6  */{  30,    800,     800 },
/* 7  */{  40,    800,     800 },
/* 8  */{  60,    1000,    800 },
/* 9  */{  80,    900,     900 },
/* 10 */{ 100,    600,     600 },
/* 11 */{ 110,    500,     420 },
/* 12 */{ 115,    400,     390 },
};
#endif		
#ifdef TEST_FAN
uint16 const SpeedKTable[FREQ_SEG_MAX][3] =
{
/* Seg    Freq   GainKp   GainKi  */
/* 0  */{   5,   100,     100 },
/* 1  */{   7,   150,     150 },
/* 2  */{  10,   200,    200 },
/* 3  */{  15,   300,    300 },
/* 4  */{  21,    500,     500 },
/* 5  */{  25,    600,     600 },
/* 6  */{  30,    800,     800 },
/* 7  */{  40,    800,     800 },
/* 8  */{  60,    1000,    800 },
/* 9  */{  80,    800,     600 },
/* 10 */{ 100,    600,     400 },
/* 11 */{ 110,    400,     320 },
/* 12 */{ 115,    300,     190 },
};
#endif

#ifdef WHP05600AEK
unsigned int const SpeedKTable[FREQ_SEG_MAX][3] =
{
/* Seg    Freq   GainKp   GainKi  */
/* 0  */{   5,   100,     100 },
/* 1  */{   7,   150,     150 },
/* 2  */{  10,   200,    200 },
/* 3  */{  15,   300,    300 },
/* 4  */{  21,    500,     500 },
/* 5  */{  25,    600,     600 },
/* 6  */{  30,    800,     800 },
/* 7  */{  40,    800,     800 },
/* 8  */{  60,    1000,    800 },
/* 9  */{  80,    900,     900 },
/* 10 */{ 100,    600,     600 },
/* 11 */{ 110,    500,     420 },
/* 12 */{ 115,    400,     390 },
};
#endif	
/*=====================================================================================================
																
            Name:        MLP_GetSpeedGain( in main loop)   
            Function:    Control the gains (Ki and Kp) of speed PI 
			
            Input:                
            Return:      g_CMwr.KpObject
                         g_CMwr.KiObject										
            update:      2014/9/7								
																
=====================================================================================================*/
void MLP_GetSpeedGain(void)
{
    _iq TempReg;
    
    uint16 CurFreq;
    uint8 i;
    
    /* calculate current frequency */
    
    TempReg = ((g_CMwr.RefFreq / g_CMBody.u8PF) + IQ15(0.5)) >> 15;
    CurFreq = (uint16) TempReg;
    
    /* get the kp and ki of speed PI from speed ratio data table */
    if ( CurFreq <= SpeedKTable[0][0] )
    {
    	i = 0;
    }
    else if ( CurFreq >= SpeedKTable[FREQ_SEG_MAX-1][0] )
    {
        i = FREQ_SEG_MAX - 1;
    }
    else
    {
        for (i = 0; i < FREQ_SEG_MAX; i++)
        {
            if ( CurFreq >= SpeedKTable[i][0] && CurFreq < SpeedKTable[i+1][0] )
            {
                break;
            }
        }
    }
        
    /* get Kp gain */
    TempReg = (_iq24) SpeedKTable[i][1] << 15;
    TempReg = (_iq24) IQ15mpy(TempReg, IQ24(0.002));		//test motor 0.0002
    TempReg = IQ24mpy(TempReg, IQ24(SPEED_PI_KP));
    g_CMwr.KpObject = TempReg;
    
    /* get Ki gain */
    TempReg = (_iq24) SpeedKTable[i][2] << 15;
    TempReg = (_iq24) IQ15mpy(TempReg, IQ24(0.002));		//test motor 0.0002
    TempReg = IQ24mpy(TempReg, IQ24(SPEED_PI_KI));
    g_CMwr.KiObject = TempReg;
    
//    g_CMwr.KpObject = IQ20mpy(g_CMwr.KpObject,IQ20mpy(IQ20(g_stParameter.u16VCAsrKp1),IQ20(0.1)));
//    g_CMwr.KiObject = IQ20mpy(g_CMwr.KiObject,IQ20mpy(IQ20(g_stParameter.u16VCAsrKi1),IQ20(0.01)));
    
}
/*=====================================================================================================
																
			Name:		TM_SpeedEstLPF ( in 1ms )   
			Function:	Speed through Low pass filter          
			Return:												
			date:		2014/6/6								
																
=====================================================================================================*/
void TM_SpeedEstLPF(void)
{
	_iq TempReg;
	
	/* vdc to Low Pass Filter */
	TempReg = g_CMwr.Est - g_CMwr.EstSteady;
	TempReg = IQ15mpy(TempReg, g_CMwr.EstLPF_K);
	g_CMwr.EstSteady += TempReg;
	
}
/*=====================================================================================================
																
            Name:        SpeedUpdate( in timer0 interrupt)   
            Function:    Update speed 
			
            Input:       g_CMwr.RefCurrent, g_CMwr.Offset                
            Return:      g_CMwr.Ref, g_CMwr.RefFreq, g_CMCtrl.ThetaComp								
            update:      2014/9/7								
																
=====================================================================================================*/
void SpeedUpdate(void)
{
    _iq TempReg,TempReg1;
    
    TempReg = IQ15(g_stFreqControl.u16FreqRunning)*g_CMBody.u8PF/10;
	
		if (g_CMwr.RefFreq != TempReg)
    {
        /* set flag for speed update */
        
        /* calculate the frequency (Hz and Rad) */        
        g_CMwr.RefFreq = TempReg;                                   
        g_CMwr.u8Update = TRUE;
 
    }
    else
    {
        g_CMwr.u8Update = FALSE;
    }
		
    g_CMwr.Ref = IQ15mpy(g_CMwr.RefFreq, IQ15_2PI); 
		
    /* calculate the compensation theta */
		
		TempReg1 = MLIB_Slope(g_CMwr.RefFreq,IQ15(60),IQ15(100),IQ15(0.5),IQ15(0.5));
        
    TempReg = IQ15mpy(g_CMwr.RefFreq, IQ15(1.273239));			/* control compensation */
    TempReg = IQ15mpy(TempReg,TempReg1);

//    TempReg = IQ20mpy(TempReg,g_stParaMeter.iqVCAsrKp2);
          
    g_CMCtrl.ThetaComp = TempReg;
    
   
}	
/*=============================================================================
																
			Name:		TM_SpeedPI (in 1ms interrupt)   
			Function:	Speed PI procedure          
                        										
			Input:		g_CMwr.Ref
                        g_CMwr.EstSteady
			Return:		g_CMwr.iqOut
			Update:		2014/6/5								
																
=============================================================================*/
void TM_SpeedPI(void)
{
    _iq sp_P;
    _iq sp_Reg;
    
    /* get kp, ki */
    g_CMwr.Kp = MLIB_gAdd(g_CMwr.Kp, g_CMwr.KpObject, g_CMwr.KpInc, g_CMwr.KpInc);
    g_CMwr.Ki = MLIB_gAdd(g_CMwr.Ki, g_CMwr.KiObject, g_CMwr.KiInc, g_CMwr.KiInc);
    
    TM_SpeedEstLPF();

    g_CMwr.EstError = g_CMwr.Ref - g_CMwr.EstSteady;
   
    /* PI control */
    if (SystemMode == MODE_CLOSE_LOOP)
    {
        /* calculate the P of PI */
        sp_P = (_iq20) IQ19mpy(g_CMwr.EstError, g_CMwr.Kp);
					
        /* calculate the I of PI */
        sp_Reg = (_iq20) IQ19mpy(g_CMwr.EstError, g_CMwr.Ki);
        sp_Reg += g_CMwr.PI_I;
			
				g_CMwr.PI_I = MLIB_Sat(sp_Reg, 0, (g_CMCtrl.iqMax + IQ20(0)));
        
        /* calculate the output of PI */
        sp_Reg = sp_P + g_CMwr.PI_I;
				
				g_CMwr.iqOut = MLIB_Sat(sp_Reg, 0, g_CMCtrl.iqMax);
			
    }
    else
    {
        g_CMwr.EstError = 0;
        g_CMwr.iqOut = 0;
        g_CMwr.PI_I = 0;
    }
}	

