#ifndef __TEST_H__
#define __TEST_H__

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    System Clock Control Register (SCKCR)
*/
/* Peripheral Module Clock D (PCLKD) */
#define _00000000_CGC_PCLKD_DIV_1           (0x00000000UL) /* x1 */
#define _00000001_CGC_PCLKD_DIV_2           (0x00000001UL) /* x1/2 */
#define _00000002_CGC_PCLKD_DIV_4           (0x00000002UL) /* x1/4 */
#define _00000003_CGC_PCLKD_DIV_8           (0x00000003UL) /* x1/8 */
#define _00000004_CGC_PCLKD_DIV_16          (0x00000004UL) /* x1/16 */
#define _00000005_CGC_PCLKD_DIV_32          (0x00000005UL) /* x1/32 */
#define _00000006_CGC_PCLKD_DIV_64          (0x00000006UL) /* x1/64 */
/* Peripheral Module Clock B (PCLKB) */
#define _00000000_CGC_PCLKB_DIV_1           (0x00000000UL) /* x1 */
#define _00000100_CGC_PCLKB_DIV_2           (0x00000100UL) /* x1/2 */
#define _00000200_CGC_PCLKB_DIV_4           (0x00000200UL) /* x1/4 */
#define _00000300_CGC_PCLKB_DIV_8           (0x00000300UL) /* x1/8 */
#define _00000400_CGC_PCLKB_DIV_16          (0x00000400UL) /* x1/16 */
#define _00000500_CGC_PCLKB_DIV_32          (0x00000500UL) /* x1/32 */
#define _00000600_CGC_PCLKB_DIV_64          (0x00000600UL) /* x1/64 */
/* Peripheral Module Clock A (PCLKA) */
#define _00000000_CGC_PCLKA_DIV_1           (0x00000000UL) /* x1 */
#define _00001000_CGC_PCLKA_DIV_2           (0x00001000UL) /* x1/2 */
#define _00002000_CGC_PCLKA_DIV_4           (0x00002000UL) /* x1/4 */
#define _00003000_CGC_PCLKA_DIV_8           (0x00003000UL) /* x1/8 */
#define _00004000_CGC_PCLKA_DIV_16          (0x00004000UL) /* x1/16 */
#define _00005000_CGC_PCLKA_DIV_32          (0x00005000UL) /* x1/32 */
#define _00006000_CGC_PCLKA_DIV_64          (0x00006000UL) /* x1/64 */
/* System Clock (ICLK) */
#define _00000000_CGC_ICLK_DIV_1            (0x00000000UL) /* x1 */
#define _01000000_CGC_ICLK_DIV_2            (0x01000000UL) /* x1/2 */
#define _02000000_CGC_ICLK_DIV_4            (0x02000000UL) /* x1/4 */
#define _03000000_CGC_ICLK_DIV_8            (0x03000000UL) /* x1/8 */
#define _04000000_CGC_ICLK_DIV_16           (0x04000000UL) /* x1/16 */
#define _05000000_CGC_ICLK_DIV_32           (0x05000000UL) /* x1/32 */
#define _06000000_CGC_ICLK_DIV_64           (0x06000000UL) /* x1/64 */
/* System Clock (FCLK) */
#define _00000000_CGC_FCLK_DIV_1            (0x00000000UL) /* x1 */
#define _10000000_CGC_FCLK_DIV_2            (0x10000000UL) /* x1/2 */
#define _20000000_CGC_FCLK_DIV_4            (0x20000000UL) /* x1/4 */
#define _30000000_CGC_FCLK_DIV_8            (0x30000000UL) /* x1/8 */
#define _40000000_CGC_FCLK_DIV_16           (0x40000000UL) /* x1/16 */
#define _50000000_CGC_FCLK_DIV_32           (0x50000000UL) /* x1/32 */
#define _60000000_CGC_FCLK_DIV_64           (0x60000000UL) /* x1/64 */

/*
    High-Speed On-Chip Oscillator Control Register 2 (HOCOCR2)
*/
#define _00_CGC_HOCO_CLK_32                 (0x00U) /* HOCO clock source 32MHz */
#define _03_CGC_HOCO_CLK_64                 (0x03U) /* HOCO clock source 64MHz */

/*
    High-Speed On-Chip Oscillator Wait Control Register (HOCOWTCR)
*/
#define _05_CGC_HOCO_WAIT_CYCLE_142         (0x05U) /* HOCO Wait time = 142 cycles */
#define _06_CGC_HOCO_WAIT_CYCLE_270         (0x06U) /* HOCO Wait time = 270 cycles */

/*
    System Clock Control Register 3 (SCKCR3)
*/
#define _0000_CGC_CLOCKSOURCE_LOCO          (0x0000U) /* LOCO */
#define _0100_CGC_CLOCKSOURCE_HOCO          (0x0100U) /* HOCO */
#define _0200_CGC_CLOCKSOURCE_MAINCLK       (0x0200U) /* Main clock oscillator */
#define _0400_CGC_CLOCKSOURCE_PLL           (0x0400U) /* PLL circuit */

/*
    PLL Control Register (PLLCR)
*/
/* PLL Input Frequency Division Ratio Select (PLIDIV[1:0]) */
#define _0000_CGC_PLL_FREQ_DIV_1            (0x0000U) /* x1 */
#define _0001_CGC_PLL_FREQ_DIV_2            (0x0001U) /* x1/2 */
#define _0002_CGC_PLL_FREQ_DIV_4            (0x0002U) /* x1/4 */
/* PLL Clock Source Select (PLLSRCSEL) */
#define _0000_CGC_PLL_SOURCE_MAIN           (0x0000U) /* Main clock oscillator */
#define _0004_CGC_PLL_SOURCE_HOCO           (0x0004U) /* HOCO */
/* Frequency Multiplication Factor Select (STC[5:0]) */
#define _0700_CGC_PLL_FREQ_MUL_4_0          (0x0700U) /* x4 */
#define _0800_CGC_PLL_FREQ_MUL_4_5          (0x0800U) /* x4.5 */
#define _0900_CGC_PLL_FREQ_MUL_5_0          (0x0900U) /* x5 */
#define _0A00_CGC_PLL_FREQ_MUL_5_5          (0x0A00U) /* x5.5 */
#define _0B00_CGC_PLL_FREQ_MUL_6_0          (0x0B00U) /* x6 */
#define _0C00_CGC_PLL_FREQ_MUL_6_5          (0x0C00U) /* x6.5 */
#define _0D00_CGC_PLL_FREQ_MUL_7_0          (0x0D00U) /* x7 */
#define _0E00_CGC_PLL_FREQ_MUL_7_5          (0x0E00U) /* x7.5 */
#define _0F00_CGC_PLL_FREQ_MUL_8_0          (0x0F00U) /* x8 */
#define _1000_CGC_PLL_FREQ_MUL_8_5          (0x1000U) /* x8.5 */
#define _1100_CGC_PLL_FREQ_MUL_9_0          (0x1100U) /* x9 */
#define _1200_CGC_PLL_FREQ_MUL_9_5          (0x1200U) /* x9.5 */
#define _1300_CGC_PLL_FREQ_MUL_10_0         (0x1300U) /* x10 */
#define _1400_CGC_PLL_FREQ_MUL_10_5         (0x1400U) /* x10.5 */
#define _1500_CGC_PLL_FREQ_MUL_11_0         (0x1500U) /* x11 */
#define _1600_CGC_PLL_FREQ_MUL_11_5         (0x1600U) /* x11.5 */
#define _1700_CGC_PLL_FREQ_MUL_12_0         (0x1700U) /* x12 */
#define _1800_CGC_PLL_FREQ_MUL_12_5         (0x1800U) /* x12.5 */
#define _1900_CGC_PLL_FREQ_MUL_13_0         (0x1900U) /* x13 */
#define _1A00_CGC_PLL_FREQ_MUL_13_5         (0x1A00U) /* x13.5 */
#define _1B00_CGC_PLL_FREQ_MUL_14_0         (0x1B00U) /* x14 */
#define _1C00_CGC_PLL_FREQ_MUL_14_5         (0x1C00U) /* x14.5 */
#define _1D00_CGC_PLL_FREQ_MUL_15_0         (0x1D00U) /* x15 */
#define _1E00_CGC_PLL_FREQ_MUL_15_5         (0x1E00U) /* x15.5 */

/*
    Oscillation Stop Detection Control Register (OSTDCR)
*/
/* Oscillation Stop Detection Interrupt Enable (OSTDIE) */
#define _00_CGC_OSC_STOP_INT_DISABLE        (0x00U) /* Oscillation stop detection interrupt is disabled */
#define _01_CGC_OSC_STOP_INT_ENABLE         (0x01U) /* Oscillation stop detection interrupt is enabled */
/* Oscillation Stop Detection Function Enable (OSTDE) */
#define _00_CGC_OSC_STOP_DISABLE            (0x00U) /* Oscillation stop detection function is disabled */
#define _80_CGC_OSC_STOP_ENABLE             (0x80U) /* Oscillation stop detection function is enabled */

/*
    Main Clock Oscillator Wait Control Register (MOSCWTCR)
*/
/* Main Clock Oscillator Wait Time (MSTS[4:0]) */
#define _00_CGC_OSC_WAIT_CYCLE_2            (0x00U) /* Wait time = 2 cycles */
#define _01_CGC_OSC_WAIT_CYCLE_1024         (0x01U) /* Wait time = 1024 cycles */
#define _02_CGC_OSC_WAIT_CYCLE_2048         (0x02U) /* Wait time = 2048 cycles */
#define _03_CGC_OSC_WAIT_CYCLE_4096         (0x03U) /* Wait time = 4096 cycles */
#define _04_CGC_OSC_WAIT_CYCLE_8192         (0x04U) /* Wait time = 8192 cycles */
#define _05_CGC_OSC_WAIT_CYCLE_16384        (0x05U) /* Wait time = 16384 cycles */
#define _06_CGC_OSC_WAIT_CYCLE_32768        (0x06U) /* Wait time = 32768 cycles */
#define _07_CGC_OSC_WAIT_CYCLE_65536        (0x07U) /* Wait time = 65536 cycles */

/*
    Main Clock Oscillator Forced Oscillation Control Register (MOFCR)
*/
/* Main Oscillator Drive Capability Switch (MODRV21) */
#define _00_CGC_MAINOSC_UNDER10M            (0x00U) /* 1 MHz to lower than 10 MHz */
#define _20_CGC_MAINOSC_OVER10M             (0x20U) /* 10 MHz to 20 MHz */
/* Main Clock Oscillator Switch (MOSEL) */
#define _00_CGC_MAINOSC_RESONATOR           (0x00U) /* Resonator */
#define _40_CGC_MAINOSC_EXTERNAL            (0x40U) /* External oscillator input */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _00000000_CGC_SCKCR_RESERVED        (0x00000000UL) /* SCKCR Reserved b19 to b16 value */
#define _0065_CGC_STABLE_WAIT               (0x0065U) /* Wait for the stabilization time (~50us) */

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Port Direction Register (PDR)
*/
/* Pmn Direction Control (B7 - B0) */
#define _00_Pm0_MODE_NOT_USED   (0x00U) /* Pm0 not used */
#define _00_Pm0_MODE_INPUT      (0x00U) /* Pm0 as input */
#define _01_Pm0_MODE_OUTPUT     (0x01U) /* Pm0 as output */
#define _00_Pm1_MODE_NOT_USED   (0x00U) /* Pm1 not used */
#define _00_Pm1_MODE_INPUT      (0x00U) /* Pm1 as input */
#define _02_Pm1_MODE_OUTPUT     (0x02U) /* Pm1 as output */
#define _00_Pm2_MODE_NOT_USED   (0x00U) /* Pm2 not used */
#define _00_Pm2_MODE_INPUT      (0x00U) /* Pm2 as input */
#define _04_Pm2_MODE_OUTPUT     (0x04U) /* Pm2 as output */
#define _00_Pm3_MODE_NOT_USED   (0x00U) /* Pm3 not used */
#define _00_Pm3_MODE_INPUT      (0x00U) /* Pm3 as input */
#define _08_Pm3_MODE_OUTPUT     (0x08U) /* Pm3 as output */
#define _00_Pm4_MODE_NOT_USED   (0x00U) /* Pm4 not used */
#define _00_Pm4_MODE_INPUT      (0x00U) /* Pm4 as input */
#define _10_Pm4_MODE_OUTPUT     (0x10U) /* Pm4 as output */
#define _00_Pm5_MODE_NOT_USED   (0x00U) /* Pm5 not used */
#define _00_Pm5_MODE_INPUT      (0x00U) /* Pm5 as input */
#define _20_Pm5_MODE_OUTPUT     (0x20U) /* Pm5 as output */
#define _00_Pm6_MODE_NOT_USED   (0x00U) /* Pm6 not used */
#define _00_Pm6_MODE_INPUT      (0x00U) /* Pm6 as input */
#define _40_Pm6_MODE_OUTPUT     (0x40U) /* Pm6 as output */
#define _00_Pm7_MODE_NOT_USED   (0x00U) /* Pm7 not used */
#define _00_Pm7_MODE_INPUT      (0x00U) /* Pm7 as input */
#define _80_Pm7_MODE_OUTPUT     (0x80U) /* Pm7 as output */
#define _01_PDR1_DEFAULT        (0x01U) /* PDR1 default value */
#define _01_PDR2_DEFAULT        (0x01U) /* PDR2 default value */
#define _0C_PDR3_DEFAULT        (0x0CU) /* PDR3 default value */
#define _88_PDR4_DEFAULT        (0x88U) /* PDR4 default value */
#define _20_PDR5_DEFAULT        (0x20U) /* PDR5 default value */
#define _81_PDRB_DEFAULT        (0x81U) /* PDRB default value */
#define _07_PDRD_DEFAULT        (0x07U) /* PDRD default value */
#define _3B_PDRE_DEFAULT        (0x3BU) /* PDRE default value */

/*
    Port Output Data Register (PODR)
*/
/* Pmn Output Data Store (B7 - B0) */
#define _00_Pm0_OUTPUT_0        (0x00U) /* output low at B0 */
#define _01_Pm0_OUTPUT_1        (0x01U) /* output high at B0 */
#define _00_Pm1_OUTPUT_0        (0x00U) /* output low at B1 */
#define _02_Pm1_OUTPUT_1        (0x02U) /* output high at B1 */
#define _00_Pm2_OUTPUT_0        (0x00U) /* output low at B2 */
#define _04_Pm2_OUTPUT_1        (0x04U) /* output high at B2 */
#define _00_Pm3_OUTPUT_0        (0x00U) /* output low at B3 */
#define _08_Pm3_OUTPUT_1        (0x08U) /* output high at B3 */
#define _00_Pm4_OUTPUT_0        (0x00U) /* output low at B4 */
#define _10_Pm4_OUTPUT_1        (0x10U) /* output high at B4 */
#define _00_Pm5_OUTPUT_0        (0x00U) /* output low at B5 */
#define _20_Pm5_OUTPUT_1        (0x20U) /* output high at B5 */
#define _00_Pm6_OUTPUT_0        (0x00U) /* output low at B6 */
#define _40_Pm6_OUTPUT_1        (0x40U) /* output high at B6 */
#define _00_Pm7_OUTPUT_0        (0x00U) /* output low at B7 */
#define _80_Pm7_OUTPUT_1        (0x80U) /* output high at B7 */

/*
    Open Drain Control Register 0 (ODR0)
*/
/* Pmn Output Type Select (Pm0 to Pm3) */
#define _00_Pm0_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _01_Pm0_NCH_OPEN_DRAIN  (0x01U) /* NMOS open-drain output */
#define _00_Pm1_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _04_Pm1_NCH_OPEN_DRAIN  (0x04U) /* NMOS open-drain output */
#define _00_Pm2_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _10_Pm2_NCH_OPEN_DRAIN  (0x10U) /* NMOS open-drain output */
#define _00_Pm3_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _40_Pm3_NCH_OPEN_DRAIN  (0x40U) /* NMOS open-drain output */

/*
    Open Drain Control Register 1 (ODR1)
*/
/* Pmn Output Type Select (Pm4 to Pm7) */
#define _00_Pm4_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _01_Pm4_NCH_OPEN_DRAIN  (0x01U) /* NMOS open-drain output */
#define _00_Pm5_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _04_Pm5_NCH_OPEN_DRAIN  (0x04U) /* NMOS open-drain output */
#define _00_Pm6_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _10_Pm6_NCH_OPEN_DRAIN  (0x10U) /* NMOS open-drain output */
#define _00_Pm7_CMOS_OUTPUT     (0x00U) /* CMOS output */
#define _40_Pm7_NCH_OPEN_DRAIN  (0x40U) /* NMOS open-drain output */

/*
    Pull-Up Control Register (PCR)
*/
/* Pmn Input Pull-Up Resistor Control (B7 - B0) */
#define _00_Pm0_PULLUP_OFF      (0x00U) /* Pm0 pull-up resistor not connected */
#define _01_Pm0_PULLUP_ON       (0x01U) /* Pm0 pull-up resistor connected */
#define _00_Pm1_PULLUP_OFF      (0x00U) /* Pm1 pull-up resistor not connected */
#define _02_Pm1_PULLUP_ON       (0x02U) /* Pm1 pull-up resistor connected */
#define _00_Pm2_PULLUP_OFF      (0x00U) /* Pm2 Pull-up resistor not connected */
#define _04_Pm2_PULLUP_ON       (0x04U) /* Pm2 pull-up resistor connected */
#define _00_Pm3_PULLUP_OFF      (0x00U) /* Pm3 pull-up resistor not connected */
#define _08_Pm3_PULLUP_ON       (0x08U) /* Pm3 pull-up resistor connected */
#define _00_Pm4_PULLUP_OFF      (0x00U) /* Pm4 pull-up resistor not connected */
#define _10_Pm4_PULLUP_ON       (0x10U) /* Pm4 pull-up resistor connected */
#define _00_Pm5_PULLUP_OFF      (0x00U) /* Pm5 pull-up resistor not connected */
#define _20_Pm5_PULLUP_ON       (0x20U) /* Pm5 pull-up resistor connected */
#define _00_Pm6_PULLUP_OFF      (0x00U) /* Pm6 pull-up resistor not connected */
#define _40_Pm6_PULLUP_ON       (0x40U) /* Pm6 pull-up resistor connected */
#define _00_Pm7_PULLUP_OFF      (0x00U) /* Pm7 pull-up resistor not connected */
#define _80_Pm7_PULLUP_ON       (0x80U) /* Pm7 pull-up resistor connected */

/*
    Drive Capacity Control Register (DSCR)
*/
/* Pmn Drive Capacity Control (B7 - B0) */
#define _00_Pm0_HIDRV_OFF       (0x00U) /* Pm0 Normal drive output */
#define _01_Pm0_HIDRV_ON        (0x01U) /* Pm0 High-drive output */
#define _00_Pm1_HIDRV_OFF       (0x00U) /* Pm1 Normal drive output */
#define _02_Pm1_HIDRV_ON        (0x02U) /* Pm1 High-drive output */
#define _00_Pm2_HIDRV_OFF       (0x00U) /* Pm2 Normal drive output */
#define _04_Pm2_HIDRV_ON        (0x04U) /* Pm2 High-drive output */
#define _00_Pm3_HIDRV_OFF       (0x00U) /* Pm3 Normal drive output */
#define _08_Pm3_HIDRV_ON        (0x08U) /* Pm3 High-drive output */
#define _00_Pm4_HIDRV_OFF       (0x00U) /* Pm4 Normal drive output */
#define _10_Pm4_HIDRV_ON        (0x10U) /* Pm4 High-drive output */
#define _00_Pm5_HIDRV_OFF       (0x00U) /* Pm5 Normal drive output */
#define _20_Pm5_HIDRV_ON        (0x20U) /* Pm5 High-drive output */
#define _00_Pm6_HIDRV_OFF       (0x00U) /* Pm6 Normal drive output */
#define _40_Pm6_HIDRV_ON        (0x40U) /* Pm6 High-drive output */
#define _00_Pm7_HIDRV_OFF       (0x00U) /* Pm7 Normal drive output */
#define _80_Pm7_HIDRV_ON        (0x80U) /* Pm7 High-drive output */

extern void R_Systeminit(void);

#endif

