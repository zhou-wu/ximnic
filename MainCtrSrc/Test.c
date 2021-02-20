#include "Test.h"

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function initializes the clock generator.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_Create(void)
{
#if 0 ///Ô­À´ÓÐ
    unsigned long sckcr_dummy;
    unsigned short w_count;

//#if 0
    /* Set main clock control registers */
    SYSTEM.MOFCR.BYTE = _00_CGC_MAINOSC_RESONATOR | _20_CGC_MAINOSC_OVER10M;
    SYSTEM.MOSCWTCR.BYTE = _04_CGC_OSC_WAIT_CYCLE_8192;

    /* Set main clock operation */
    SYSTEM.MOSCCR.BIT.MOSTP = 0U;

    /* Wait for main clock oscillator wait counter overflow */
    while (1U != SYSTEM.OSCOVFSR.BIT.MOOVF);

    /* Change to high-speed operating mode */
    if (0U != SYSTEM.OPCCR.BIT.OPCM)
    {
      SYSTEM.OPCCR.BIT.OPCM = 0U;
      
      while (0U != SYSTEM.OPCCR.BIT.OPCMTSF);
    }

    /* Set clock source */
    SYSTEM.SCKCR3.WORD = _0200_CGC_CLOCKSOURCE_MAINCLK;

    while (_0200_CGC_CLOCKSOURCE_MAINCLK != SYSTEM.SCKCR3.WORD);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0065_CGC_STABLE_WAIT; w_count++)
    {
        ;//__nop();
    }
//#endif

    /* Set system clock */
    sckcr_dummy = _00000000_CGC_PCLKD_DIV_1 | _00000000_CGC_PCLKB_DIV_1 | _00000000_CGC_PCLKA_DIV_1 | 
                  _00000000_CGC_ICLK_DIV_1 | _00000000_CGC_FCLK_DIV_1 | _00000000_CGC_SCKCR_RESERVED;
    SYSTEM.SCKCR.LONG = sckcr_dummy;

    while (SYSTEM.SCKCR.LONG != sckcr_dummy);

    /* Wait for the stabilization time */
    for (w_count = 0U; w_count < _0065_CGC_STABLE_WAIT; w_count++)
    {
        ;//nop();
    }

    /* Set LOCO */
    SYSTEM.LOCOCR.BIT.LCSTP = 0U;
#endif
}

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
#if 0
  PORT0.PODR.BYTE = _01_Pm0_OUTPUT_1;
  PORT0.DSCR.BYTE |= _00_Pm0_HIDRV_OFF;
  PORT0.PDR.BYTE = _01_Pm0_MODE_OUTPUT;
#endif
}

/***********************************************************************************************************************
* Function Name: R_Systeminit
* Description  : This function initializes every macro.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Systeminit(void)
{
  /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
  //SYSTEM.PRCR.WORD = 0xA50FU;

  /* Enable writing to MPC pin function control registers */
  //MPC.PWPR.BIT.B0WI = 0U;
  //MPC.PWPR.BIT.PFSWE = 1U;

#if 0
  /* Initialize non-existent pins */
  PORT1.PDR.BYTE = 0x01U;
  PORT2.PDR.BYTE = 0x01U;
  PORT3.PDR.BYTE = 0x0CU;
  PORT4.PDR.BYTE = 0x88U;
  PORT5.PDR.BYTE = 0x20U;
  PORT6.PDR.BYTE = 0x3FU;
  PORT8.PDR.BYTE = 0x07U;
  PORTA.PDR.BYTE = 0x3FU;
  PORTB.PDR.BYTE = 0x81U;
  PORTD.PDR.BYTE = 0x07U;
  PORTE.PDR.BYTE = 0x3BU;
#endif

  /* Set peripheral settings */
  //R_CGC_Create();
  R_PORT_Create();

  /* Disable writing to MPC pin function control registers */
  //MPC.PWPR.BIT.PFSWE = 0U;
  //MPC.PWPR.BIT.B0WI = 1U;

  /* Enable protection */
  //SYSTEM.PRCR.WORD = 0xA500U;
}

