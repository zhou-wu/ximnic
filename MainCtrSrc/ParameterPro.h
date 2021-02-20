#ifndef __PARAMETER_PRO_H__
#define __PARAMETER_PRO_H__

#ifdef _PARA_C_
#define _PARA_EXT
#else
#define _PARA_EXT extern
#endif

#define EEP_SYS_PARA_LEN    0xFE  //254 //0x1fe
#define EEP_SYS_PARA_LEN1   0x7D  //125 
#define EEP_SYS_PARA_LEN2   0x45  //69 
#define EEP_SYS_PARA_LEN3   0x3B  //59 
#define U8_COOL_T4_LEN        12
#define U8_HEAT_T4_LEN        16

#define U16_MAIN_EEP_START1_ADDR     0x0000
#define U16_MAIN_EEP_START2_ADDR     0x007E
#define U16_MAIN_EEP_START3_ADDR     0x00C4
#define U16_MAIN_EEP_START_ADDR      0x0100          //0x0000
#define U16_DRV_EEP_START_ADDR       0x0200
#define U16_MAIN_EEP_START_ADDR_2   (0x0100 | 0x0400) //(0x0000 | 0x0400)
#define U16_DRV_EEP_START_ADDR_2    (0x0200 | 0x0400)

#define u8EepSysPara                  EeSysPara.EEPROM_SysPara

//=============================down is seer test===============================//
#define u8EESn                        EeSysPara.Elem.EE_SN_CODE_ADD
#define U8_COOL_MIN_RATI_FREQ         EeSysPara.Elem.COOL_TEST_FREQ_A
#define U8_COOL_MID_RATI_FREQ         EeSysPara.Elem.COOL_TEST_FREQ_B
#define U8_COOL_RATI_FREQ             EeSysPara.Elem.COOL_TEST_FREQ_C
#define U8_COOL_MAX_RATI_FREQ         EeSysPara.Elem.COOL_TEST_FREQ_D
#define U8_COOL_TEST_FREQ5            EeSysPara.Elem.COOL_TEST_FREQ_E
#define U8_COOL_TEST_FREQ6            EeSysPara.Elem.COOL_TEST_FREQ_F
#define U8_COOL_TEST_FREQ7            EeSysPara.Elem.COOL_TEST_FREQ_G

#define U8_HEAT_MIN_RATI_FREQ         EeSysPara.Elem.HEAT_TEST_FREQ_A    /// 010B
#define U8_HEAT_MID_RATI_FREQ         EeSysPara.Elem.HEAT_TEST_FREQ_B
#define U8_HEAT_RATI_FREQ             EeSysPara.Elem.HEAT_TEST_FREQ_C
#define U8_HEAT_MAX_RATI_FREQ         EeSysPara.Elem.HEAT_TEST_FREQ_D
#define U8_HEAT_TEST_FREQ5            EeSysPara.Elem.HEAT_TEST_FREQ_E
#define U8_HEAT_TEST_FREQ6            EeSysPara.Elem.HEAT_TEST_FREQ_F
#define U8_HEAT_TEST_FREQ7            EeSysPara.Elem.HEAT_TEST_FREQ_G

#define U8_COOL_MIN_RATI_SPDH         EeSysPara.Elem.COOL_TEST_SPD_AH
#define U8_COOL_MIN_RATI_SPDL         EeSysPara.Elem.COOL_TEST_SPD_AL
#define U8_COOL_MID_RATI_SPDH         EeSysPara.Elem.COOL_TEST_SPD_BH
#define U8_COOL_MID_RATI_SPDL         EeSysPara.Elem.COOL_TEST_SPD_BL
#define U8_COOL_RATI_SPDH             EeSysPara.Elem.COOL_TEST_SPD_CH
#define U8_COOL_RATI_SPDL             EeSysPara.Elem.COOL_TEST_SPD_CL
#define U8_COOL_MAX_RATI_SPDH         EeSysPara.Elem.COOL_TEST_SPD_DH
#define U8_COOL_MAX_RATI_SPDL         EeSysPara.Elem.COOL_TEST_SPD_DL
#define U8_COOL_TEST_SPD5H            EeSysPara.Elem.COOL_TEST_SPD_EH
#define U8_COOL_TEST_SPD5L            EeSysPara.Elem.COOL_TEST_SPD_EL
#define U8_COOL_TEST_SPD6H            EeSysPara.Elem.COOL_TEST_SPD_FH
#define U8_COOL_TEST_SPD6L            EeSysPara.Elem.COOL_TEST_SPD_FL
#define U8_COOL_TEST_SPD7H            EeSysPara.Elem.COOL_TEST_SPD_GH
#define U8_COOL_TEST_SPD7L            EeSysPara.Elem.COOL_TEST_SPD_GL

#define U8_HEAT_MIN_RATI_SPDH         EeSysPara.Elem.HEAT_TEST_SPD_AH
#define U8_HEAT_MIN_RATI_SPDL         EeSysPara.Elem.HEAT_TEST_SPD_AL
#define U8_HEAT_MID_RATI_SPDH         EeSysPara.Elem.HEAT_TEST_SPD_BH
#define U8_HEAT_MID_RATI_SPDL         EeSysPara.Elem.HEAT_TEST_SPD_BL
#define U8_HEAT_RATI_SPDH             EeSysPara.Elem.HEAT_TEST_SPD_CH
#define U8_HEAT_RATI_SPDL             EeSysPara.Elem.HEAT_TEST_SPD_CL
#define U8_HEAT_MAX_RATI_SPDH         EeSysPara.Elem.HEAT_TEST_SPD_DH
#define U8_HEAT_MAX_RATI_SPDL         EeSysPara.Elem.HEAT_TEST_SPD_DL
#define U8_HEAT_TEST_SPD5H            EeSysPara.Elem.HEAT_TEST_SPD_EH
#define U8_HEAT_TEST_SPD5L            EeSysPara.Elem.HEAT_TEST_SPD_EL
#define U8_HEAT_TEST_SPD6H            EeSysPara.Elem.HEAT_TEST_SPD_FH
#define U8_HEAT_TEST_SPD6L            EeSysPara.Elem.HEAT_TEST_SPD_FL
#define U8_HEAT_TEST_SPD7H            EeSysPara.Elem.HEAT_TEST_SPD_GH
#define U8_HEAT_TEST_SPD7L            EeSysPara.Elem.HEAT_TEST_SPD_GL

#define U8_COOL_MIN_RATI_PLSH         EeSysPara.Elem.COOL_TEST_TEMP_PLS_AH
#define U8_COOL_MIN_RATI_PLSL         EeSysPara.Elem.COOL_TEST_TEMP_PLS_AL
#define U8_COOL_MID_RATI_PLSH         EeSysPara.Elem.COOL_TEST_TEMP_PLS_BH
#define U8_COOL_MID_RATI_PLSL         EeSysPara.Elem.COOL_TEST_TEMP_PLS_BL
#define U8_COOL_RATI_PLSH             EeSysPara.Elem.COOL_TEST_TEMP_PLS_CH
#define U8_COOL_RATI_PLSL             EeSysPara.Elem.COOL_TEST_TEMP_PLS_CL
#define U8_COOL_MAX_RATI_PLSH         EeSysPara.Elem.COOL_TEST_TEMP_PLS_DH
#define U8_COOL_MAX_RATI_PLSL         EeSysPara.Elem.COOL_TEST_TEMP_PLS_DL
#define U8_COOL_TEST_PLS5H            EeSysPara.Elem.COOL_TEST_TEMP_PLS_EH
#define U8_COOL_TEST_PLS5L            EeSysPara.Elem.COOL_TEST_TEMP_PLS_EL
#define U8_COOL_TEST_PLS6H            EeSysPara.Elem.COOL_TEST_TEMP_PLS_FH
#define U8_COOL_TEST_PLS6L            EeSysPara.Elem.COOL_TEST_TEMP_PLS_FL
#define U8_COOL_TEST_PLS7H            EeSysPara.Elem.COOL_TEST_TEMP_PLS_GH
#define U8_COOL_TEST_PLS7L            EeSysPara.Elem.COOL_TEST_TEMP_PLS_GL

#define U8_HEAT_MIN_RATI_PLSH         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_AH
#define U8_HEAT_MIN_RATI_PLSL         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_AL
#define U8_HEAT_MID_RATI_PLSH         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_BH
#define U8_HEAT_MID_RATI_PLSL         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_BL
#define U8_HEAT_RATI_PLSH             EeSysPara.Elem.HEAT_TEST_TEMP_PLS_CH
#define U8_HEAT_RATI_PLSL             EeSysPara.Elem.HEAT_TEST_TEMP_PLS_CL
#define U8_HEAT_MAX_RATI_PLSH         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_DH
#define U8_HEAT_MAX_RATI_PLSL         EeSysPara.Elem.HEAT_TEST_TEMP_PLS_DL
#define U8_HEAT_TEST_PLS5H            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_EH
#define U8_HEAT_TEST_PLS5L            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_EL
#define U8_HEAT_TEST_PLS6H            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_FH
#define U8_HEAT_TEST_PLS6L            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_FL
#define U8_HEAT_TEST_PLS7H            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_GH
#define U8_HEAT_TEST_PLS7L            EeSysPara.Elem.HEAT_TEST_TEMP_PLS_GL

//=============================up is seer test===============================//
//#define u8SeltFlags1              EeSysPara.Elem.FG_SELT1
//#define u8SeltFlags2              EeSysPara.Elem.FG_SELT2
//#define u8SeltFlags3              EeSysPara.Elem.FG_SELT3
#define u8PlatFreq1EE               EeSysPara.Elem.PLAT_FREQ1_ADD
#define u8PlatFreq2EE               EeSysPara.Elem.PLAT_FREQ2_ADD
#define u8PlatFreq3EE               EeSysPara.Elem.PLAT_FREQ3_ADD
#define u8PlatFreq4EE               EeSysPara.Elem.PLAT_FREQ4_ADD
#define u8PlatFreq5EE               EeSysPara.Elem.PLAT_FREQ5_ADD
#define u8PlatHold1                 EeSysPara.Elem.PLAT_TIME1_ADD1
#define u8PlatHold2                 EeSysPara.Elem.PLAT_TIME2_ADD1
#define u8PlatHold3                 EeSysPara.Elem.PLAT_TIME3_ADD1
#define u8PlatHold4                 EeSysPara.Elem.PLAT_TIME4_ADD1
#define u8PlatHold5                 EeSysPara.Elem.PLAT_TIME5_ADD1
#define u8FreqUpSpdL_1              EeSysPara.Elem.FREQ_UP_SPD_L_1
#define u8FreqUpSpd1_2              EeSysPara.Elem.FREQ_UP_SPD1_2
#define u8FreqUpSpd2_3              EeSysPara.Elem.FREQ_UP_SPD2_3
#define u8FreqUpSpd3_4              EeSysPara.Elem.FREQ_UP_SPD3_4
#define u8FreqUpSpd4_5              EeSysPara.Elem.FREQ_UP_SPD4_5
#define u8FreqUpSpd5_H              EeSysPara.Elem.FREQ_UP_SPD5_H
#define u8FreqDwSpdChg_1            EeSysPara.Elem.FREQ_DW_SPD_CHG_1
#define u8FreqDwSpd1_2              EeSysPara.Elem.FREQ_DW_SPD1_2
#define u8FreqDwSpd2_3              EeSysPara.Elem.FREQ_DW_SPD2_3
#define u8FreqDwSpd3_4              EeSysPara.Elem.FREQ_DW_SPD3_4
#define u8FreqDwSpd4_5              EeSysPara.Elem.FREQ_DW_SPD4_5
#define u8FreqDwSpd5_H              EeSysPara.Elem.FREQ_DW_SPD5_H
#define U8_LMT_DW_SPD               EeSysPara.Elem.LMT_DW_SPD
#define u8CoolCurrRstDiff           EeSysPara.Elem.CoolCurrRstDiff

#define U8_T4_LimTemp               EeSysPara.Elem.T4_TEMP
#define U8_IPM_CHANGE_TEMP          EeSysPara.Elem.ICT
#define U8_IPM_CHANGE_TEMP_AD       EeSysPara.Elem.ICTAD
#define U8_IPM_MIN_TEMP_AD          EeSysPara.Elem.IMINTAD
#define U8_IPM_MAX_TEMP_AD          EeSysPara.Elem.IMAXTAD
#define EE_IPM_SLOW_UP_TEMP         EeSysPara.Elem.ISUT

#define U8_LOW_TEMP_DEFORST_EE      EeSysPara.Elem.PMV_LOW_TEMP_DEFORST_EE
#define U8_LH_DEFORST_OFF_INIT_EE   EeSysPara.Elem.PMV_LH_DEFORST_OFF_INIT_EE
#define U8_LH_QUIT_DEF_TEMP1        EeSysPara.Elem.LH_QUIT_DEF_TEMP1
#define U8_LH_QUIT_DEF_TEMP2        EeSysPara.Elem.LH_QUIT_DEF_TEMP2
#define U8_TEST_DEF_FREQ            EeSysPara.Elem.TEST_DEF_FREQ
#define QuitDefrostFreq             EeSysPara.Elem.QuitDefrostFreq_ADD
#define U8_TEST_PFC_OFF_FREQ        EeSysPara.Elem.TEST_PFC_OFF_FREQ

#define U8_ENTER_OIL_FREQ           EeSysPara.Elem.ENTER_OIL_FREQ
#define U8_OIL_FREQ                 EeSysPara.Elem.OilFreq
#define U8_FRC_COOL_FREQ            EeSysPara.Elem.FRC_COOL_FREQ

#define DEFROSTTIM                  EeSysPara.Elem.EE_DEFROSTTIM
#define LOWHEATT3                   EeSysPara.Elem.EE_LOWHEATT3
#define u8T3DefrostExit3            EeSysPara.Elem.EE_TCDE3
#define u8T3DefrostExit4            EeSysPara.Elem.EE_TCDE4
#define u8FreqDefrostTest1          EeSysPara.Elem.EE_SEERDEFFRE1
#define u8FreqDefrostTest2          EeSysPara.Elem.EE_SEERDEFFRE3

#define U8_LOW_DC_VOLT_EE           EeSysPara.Elem.LOW_DC_VOLT_EE
#define U8_HIGH_DC_VOLT_EE          EeSysPara.Elem.HIGH_DC_VOLT_EE

#define U8_T4LimtFre01              EeSysPara.Elem.T4HI_LMT_MIN_FREQ1
#define U8_T4LimtFre02              EeSysPara.Elem.T4HI_LMT_MIN_FREQ2
#define U8_DEF_START_FREQ           EeSysPara.Elem.DefrostStartFreq_ADD
#define U8_DEF_FREQ                 EeSysPara.Elem.DEF_FREQ
#define U8_COMPOFFPLAT              EeSysPara.Elem.STOP_FREQ

#define u8T4CoolMaxFreq             EeSysPara.Elem.T4_COOL_MAX_FREQ
#define u8T4HeatMaxFreq             EeSysPara.Elem.T4_HEAT_MAX_FREQ

#define EE_COOL_DC_FAN_SPD          EeSysPara.Elem.COOL_DC_FAN_SPD
#define EE_HEAT_DC_FAN_SPD          EeSysPara.Elem.HEAT_DC_FAN_SPD

#define U8_T4DIF                    EeSysPara.Elem.T4DIF
#define U8_T4CT                     EeSysPara.Elem.T4CT
#define U8_T4HT                     EeSysPara.Elem.T4HT

#define U8_LH_DEF_ENTER_T3          EeSysPara.Elem.LH_DEF_ENTER_T3
#define U8_LH_DEF_ENTER_TIM         EeSysPara.Elem.LH_DEF_ENTER_TIM

_PARA_EXT union
{
  unsigned char u8Byte;
  struct
  {

    unsigned char Rcv         : 1;
    unsigned char WrtDly      : 1;
    unsigned char ReadAckErr  : 1;
    unsigned char no3         : 1;
    unsigned char ByteCmp     : 1;
    unsigned char I2cProt     : 1;
    unsigned char no6         : 1;
    unsigned char no7         : 1;
  } bits;
} unEEWrtFlgs;


//=============================把EE参数放在一起=========================
//E方参数第一帧数据,按照参数表顺序排列,不能把位置掉转
//
_PARA_EXT unsigned char u8ID_CODE1;         //E方第一帧头码 00
_PARA_EXT unsigned char u8EESnCode;         //E方参数代号 01
_PARA_EXT unsigned char u8EEVerCode;        //E方参数版本号 02
_PARA_EXT unsigned char u8ACCurrOver;       //关机保护电流值 03
_PARA_EXT unsigned char u8VoltRst1;         //第一档电压限频恢复电压值 04
_PARA_EXT unsigned char u8VoltLim1;         //第一档电压限频限频电压值 05
_PARA_EXT unsigned char u8VoltRst2;         //第二档电压限频恢复电压值 06
_PARA_EXT unsigned char u8VoltLim2;         //第二档电压限频限频电压值 07
_PARA_EXT unsigned char u8VoltRst3;         //第三档电压限频恢复电压值 08
_PARA_EXT unsigned char u8VoltLim3;         //第三档电压限频限频电压值 09
_PARA_EXT unsigned char u8VoltLowRst;       //交流电压关机恢复电压值 0A
_PARA_EXT unsigned char u8VoltLowStop;      //交流电压关机保护电压值 0B
_PARA_EXT unsigned char u8VoltLimFreq1;     //第一档电压限频频率值 0C
_PARA_EXT unsigned char u8VoltLimFreq2;     //第二档电压限频频率值 0D
_PARA_EXT unsigned char u8VoltLimFreq3;     //第三档电压限频频率值 0E
_PARA_EXT unsigned char u8DcStop;           //直流母线电压小于该值,则关闭PTC并联继电器 0F
_PARA_EXT unsigned char u8DcRTS;            //直流母线电压大于该值,退出保护 10
_PARA_EXT unsigned char u8StartFreq;        //压缩机启动频率 11
_PARA_EXT unsigned char u8MinFreq;          //压缩机运行最小频率 12

_PARA_EXT unsigned char u8PlatFreq0;       //第0平台频率值 13
_PARA_EXT unsigned char u8PlatFreq1;       //第一平台频率值 14
_PARA_EXT unsigned char u8PlatFreq2;       //第二平台频率值 15

_PARA_EXT unsigned char u8FreqUpSpd0;      //最低频率~第0平台升频速率 16
_PARA_EXT unsigned char u8FreqUpSpd1;      //第0平台~第一平台升频速率 17
_PARA_EXT unsigned char u8FreqUpSpd2;      //第一平台~第二平台升频速率 18
_PARA_EXT unsigned char u8FreqUpSpd3;      //第二平台~最高频率升频速率 19

_PARA_EXT unsigned char u8FreqDwSpd0;      //最低频率~第0平台降频速率 1A
_PARA_EXT unsigned char u8FreqDwSpd1;      //第0平台~第一平台降频速率 1B
_PARA_EXT unsigned char u8FreqDwSpd2;      //第一平台~第二平台降频速率 1C
_PARA_EXT unsigned char u8FreqDwSpd3;      //第二平台~最高频率降频速率 1D

_PARA_EXT unsigned char TabRunFreq[26];     //频率表F0-F25 (ADDR:1E~37)

_PARA_EXT unsigned char u8T4MinCoolFreq1;        //制冷T4最小频率1 38
_PARA_EXT unsigned char u8T4MinCoolFreq2;        //制冷T4最小频率2 39
_PARA_EXT unsigned char u8T4MaxCoolFreq1;        //制冷T4限制最大运行频率1 3A
_PARA_EXT unsigned char u8T4MaxCoolFreq2;        //制冷T4限制最大运行频率2 3B
_PARA_EXT unsigned char u8T4MaxCoolFreq3;        //制冷T4限制最大运行频率3 3C
_PARA_EXT unsigned char u8T4MaxCoolFreq4;        //制冷T4限制最大运行频率4 3D
_PARA_EXT unsigned char u8T4MaxCoolFreq5;        //制冷T4限制最大运行频率5 3E
_PARA_EXT unsigned char u8T4MaxCoolFreq6;        //制冷T4限制最大运行频率6 3F
_PARA_EXT unsigned char u8T4MaxCoolFreq7;        //制冷T4限制最大运行频率7 40

_PARA_EXT unsigned char u8T4MidLimHeatFreq1;     //制热T4中风频率1 41
_PARA_EXT unsigned char u8T4LowLimHeatFreq2;     //制热T4低风频率2 42
_PARA_EXT unsigned char u8T4MaxHeatFreq1;        //制热T4限制最大运行频率1 43
_PARA_EXT unsigned char u8T4MaxHeatFreq2;        //制热T4限制最大运行频率2 44
_PARA_EXT unsigned char u8T4MaxHeatFreq3;        //制热T4限制最大运行频率3 45
_PARA_EXT unsigned char u8T4MaxHeatFreq4;        //制热T4限制最大运行频率4 46
_PARA_EXT unsigned char u8T4MaxHeatFreq5;        //制热T4限制最大运行频率5 47
_PARA_EXT unsigned char u8T4MaxHeatFreq6;        //制热T4限制最大运行频率6 48
_PARA_EXT unsigned char u8T4MaxHeatFreq7;        //制热T4限制最大运行频率7 49
_PARA_EXT unsigned char u8T4MaxHeatFreq8;        //制热T4限制最大运行频率8 4A
_PARA_EXT unsigned char u8T4MaxHeatFreq9;        //制热T4限制最大运行频率9 4B

_PARA_EXT unsigned char u8InEECurrentCoolExit;  //4C
_PARA_EXT unsigned char u8InEECurrentCool5I;    //4D
_PARA_EXT unsigned char u8InEECurrentCool4I;    //4E
_PARA_EXT unsigned char u8InEECurrentCool3I;    //4F
_PARA_EXT unsigned char u8InEECurrentCool2I;    //50
_PARA_EXT unsigned char u8InEECurrentCool1I;    //51
_PARA_EXT unsigned char u8InEECurrentCoolStop;  //52

_PARA_EXT unsigned char u8InEECurrentHeatExit;  //53
_PARA_EXT unsigned char u8InEECurrentHeat4I;    //54
_PARA_EXT unsigned char u8InEECurrentHeat3I;    //55
_PARA_EXT unsigned char u8InEECurrentHeat2I;    //56
_PARA_EXT unsigned char u8InEECurrentHeat1I;    //57
_PARA_EXT unsigned char u8InEECurrentHeatStop;  //58

_PARA_EXT unsigned char u8CoolUptemp1;   //电子膨胀阀制冷第一档上升温度 59
_PARA_EXT unsigned char u8CoolDntemp1;   //电子膨胀阀制冷第一档下降温度 5A
_PARA_EXT unsigned char u8CoolUptemp2;   //电子膨胀阀制冷第二档上升温度 5B
_PARA_EXT unsigned char u8CoolDntemp2;   //电子膨胀阀制冷第二档下降温度 5C
_PARA_EXT unsigned char u8CoolUptemp3;   //电子膨胀阀制冷第三档上升温度 5D
_PARA_EXT unsigned char u8CoolDntemp3;   //电子膨胀阀制冷第三档下降温度 5E
_PARA_EXT unsigned char u8CoolUptemp4;   //电子膨胀阀制冷第四档上升温度 5F
_PARA_EXT unsigned char u8CoolDntemp4;   //电子膨胀阀制冷第四档下降温度 60
_PARA_EXT unsigned char u8CoolUptemp5;   //电子膨胀阀制冷第五档上升温度 61
_PARA_EXT unsigned char u8CoolDntemp5;   //电子膨胀阀制冷第五档下降温度 62
_PARA_EXT unsigned char u8CoolUptemp6;   //电子膨胀阀制冷第六档上升温度 63
_PARA_EXT unsigned char u8CoolDntemp6;   //电子膨胀阀制冷第六档下降温度 64
_PARA_EXT unsigned char u8CoolUptemp7;   //电子膨胀阀制冷第七档上升温度 65
_PARA_EXT unsigned char u8CoolDntemp7;   //电子膨胀阀制冷第七档下降温度 66
_PARA_EXT unsigned char u8CoolUptemp8;   //电子膨胀阀制冷第八档上升温度 67
_PARA_EXT unsigned char u8CoolDntemp8;   //电子膨胀阀制冷第八档下降温度 68
_PARA_EXT unsigned char u8CoolUptemp9;   //电子膨胀阀制冷第九档上升温度 69
_PARA_EXT unsigned char u8CoolDntemp9;   //电子膨胀阀制冷第九档下降温度 6A

_PARA_EXT unsigned char u8HeatUptemp1;   //电子膨胀阀制热第一档上升温度 6B
_PARA_EXT unsigned char u8HeatDntemp1;   //电子膨胀阀制热第一档下降温度 6C
_PARA_EXT unsigned char u8HeatUptemp2;   //电子膨胀阀制热第二档上升温度 6D
_PARA_EXT unsigned char u8HeatDntemp2;   //电子膨胀阀制热第二档下降温度 6E
_PARA_EXT unsigned char u8HeatUptemp3;   //电子膨胀阀制热第三档上升温度 6F
_PARA_EXT unsigned char u8HeatDntemp3;   //电子膨胀阀制热第三档下降温度 70
_PARA_EXT unsigned char u8HeatUptemp4;   //电子膨胀阀制热第四档上升温度 71
_PARA_EXT unsigned char u8HeatDntemp4;   //电子膨胀阀制热第四档下降温度 72
_PARA_EXT unsigned char u8HeatUptemp5;   //电子膨胀阀制热第五档上升温度 73
_PARA_EXT unsigned char u8HeatDntemp5;   //电子膨胀阀制热第五档下降温度 74
_PARA_EXT unsigned char u8HeatUptemp6;   //电子膨胀阀制热第六档上升温度 75
_PARA_EXT unsigned char u8HeatDntemp6;   //电子膨胀阀制热第六档下降温度 76
_PARA_EXT unsigned char u8HeatUptemp7;   //电子膨胀阀制热第七档上升温度 77
_PARA_EXT unsigned char u8HeatDntemp7;   //电子膨胀阀制热第七档下降温度 78
_PARA_EXT unsigned char u8HeatUptemp8;   //电子膨胀阀制热第八档上升温度 79
_PARA_EXT unsigned char u8HeatDntemp8;   //电子膨胀阀制热第八档下降温度 7A
_PARA_EXT unsigned char u8HeatUptemp9;   //电子膨胀阀制热第九档上升温度 7B
_PARA_EXT unsigned char u8HeatDntemp9;   //电子膨胀阀制热第九档下降温度 7C
_PARA_EXT unsigned char u8I2cCheckSum1;  //校验和1    7D

_PARA_EXT unsigned char u8ID_CODE2;           //E方第二帧头码 7E
_PARA_EXT unsigned char u8PlatHold0H;         //第0平台停留时间值 7F
_PARA_EXT unsigned char u8PlatHold0L;         //80
_PARA_EXT unsigned char u8PlatHold1H;         //第一平台停留时间值 81
_PARA_EXT unsigned char u8PlatHold1L;         //82
_PARA_EXT unsigned char u8PlatHold2H;         //第2平台停留时间值 83
_PARA_EXT unsigned char u8PlatHold2L;         //84

_PARA_EXT unsigned char u8FanHighSpdH;        //直流风机高风转速值 85
_PARA_EXT unsigned char u8FanHighSpdL;        //86
_PARA_EXT unsigned char u8FanMidSpdH;         //直流风机中风转速值 87
_PARA_EXT unsigned char u8FanMidSpdL;         //88
_PARA_EXT unsigned char u8FanLowSpdH;         //直流风机低风转速值V 89
_PARA_EXT unsigned char u8FanLowSpdL;         //8A
_PARA_EXT unsigned char u8FanSLowSpdH;        //直流风机微风转速值 8B
_PARA_EXT unsigned char u8FanSLowSpdL;        //8C
_PARA_EXT unsigned char u8FanSSLowSpdH;       //直流风机超微风转速值 8D
_PARA_EXT unsigned char u8FanSSLowSpdL;       //8E

_PARA_EXT unsigned char u8CoolInitPulseH;     //电子膨胀阀制冷初始开度 8F
_PARA_EXT unsigned char u8CoolInitPulseL;     //90

_PARA_EXT unsigned char u8CoolPlat1PulseH;    //电子膨胀阀制冷第1档开度 91
_PARA_EXT unsigned char u8CoolPlat1PulseL;    //92
_PARA_EXT unsigned char u8CoolPlat2PulseH;    //电子膨胀阀制冷第2档开度 93
_PARA_EXT unsigned char u8CoolPlat2PulseL;    //94
_PARA_EXT unsigned char u8CoolPlat3PulseH;    //电子膨胀阀制冷第3档开度 95
_PARA_EXT unsigned char u8CoolPlat3PulseL;    //96
_PARA_EXT unsigned char u8CoolPlat4PulseH;    //电子膨胀阀制冷第4档开度 97
_PARA_EXT unsigned char u8CoolPlat4PulseL;    //98
_PARA_EXT unsigned char u8CoolPlat5PulseH;    //电子膨胀阀制冷第5档开度 99
_PARA_EXT unsigned char u8CoolPlat5PulseL;    //9A
_PARA_EXT unsigned char u8CoolPlat6PulseH;    //电子膨胀阀制冷第6档开度 9B
_PARA_EXT unsigned char u8CoolPlat6PulseL;    //9C
_PARA_EXT unsigned char u8CoolPlat7PulseH;    //电子膨胀阀制冷第7档开度 9D
_PARA_EXT unsigned char u8CoolPlat7PulseL;    //9E
_PARA_EXT unsigned char u8CoolPlat8PulseH;    //电子膨胀阀制冷第8档开度 9F
_PARA_EXT unsigned char u8CoolPlat8PulseL;    //A0
_PARA_EXT unsigned char u8CoolPlat9PulseH;    //电子膨胀阀制冷第9档开度 A1
_PARA_EXT unsigned char u8CoolPlat9PulseL;    //A2
_PARA_EXT unsigned char u8CoolPlat10PulseH;   //电子膨胀阀制冷第10档开度 A3
_PARA_EXT unsigned char u8CoolPlat10PulseL;   //A4

_PARA_EXT unsigned char u8HeatInitPulseH;     //电子膨胀阀制热初始开度 A5
_PARA_EXT unsigned char u8HeatInitPulseL;     //A6
_PARA_EXT unsigned char u8HeatPlat1PulseH;    //电子膨胀阀制热第一档开 A7
_PARA_EXT unsigned char u8HeatPlat1PulseL;    //A8
_PARA_EXT unsigned char u8HeatPlat2PulseH;    //电子膨胀阀制热第2档开 A9
_PARA_EXT unsigned char u8HeatPlat2PulseL;    //AA
_PARA_EXT unsigned char u8HeatPlat3PulseH;    //电子膨胀阀制热第3档开 AB
_PARA_EXT unsigned char u8HeatPlat3PulseL;    //AC
_PARA_EXT unsigned char u8HeatPlat4PulseH;    //电子膨胀阀制热第4档开 AD
_PARA_EXT unsigned char u8HeatPlat4PulseL;    //AE
_PARA_EXT unsigned char u8HeatPlat5PulseH;    //电子膨胀阀制热第一档开 AF
_PARA_EXT unsigned char u8HeatPlat5PulseL;    //B0
_PARA_EXT unsigned char u8HeatPlat6PulseH;    //电子膨胀阀制热第2档开 B1
_PARA_EXT unsigned char u8HeatPlat6PulseL;    //B2
_PARA_EXT unsigned char u8HeatPlat7PulseH;    //电子膨胀阀制热第3档开 B3
_PARA_EXT unsigned char u8HeatPlat7PulseL;    //B4
_PARA_EXT unsigned char u8HeatPlat8PulseH;    //电子膨胀阀制热第4档开 B5
_PARA_EXT unsigned char u8HeatPlat8PulseL;    //B6
_PARA_EXT unsigned char u8HeatPlat9PulseH;    //电子膨胀阀制热第一档开 B7
_PARA_EXT unsigned char u8HeatPlat9PulseL;    //B8
_PARA_EXT unsigned char u8HeatPlat10PulseH;   //电子膨胀阀制热第2档开 B9
_PARA_EXT unsigned char u8HeatPlat10PulseL;   //BA

_PARA_EXT unsigned char u8DefrostPulseH;      //电子膨胀阀制热化霜开度 BB
_PARA_EXT unsigned char u8DefrostPulseL;      //BC
_PARA_EXT unsigned char u8RecOilPulseH;       //电子膨胀阀回油开度 BD
_PARA_EXT unsigned char u8RecOilPulseL;       //BE
_PARA_EXT unsigned char u8RetOilTim1H;        //回油连续低频运转时间 BF
_PARA_EXT unsigned char u8RetOilTim1L;        //C0
_PARA_EXT unsigned char u8RetOilTim2H;        //回油平台运行时间 C1
_PARA_EXT unsigned char u8RetOilTim2L;        //C2
_PARA_EXT unsigned char u8I2cCheckSum2;       //校验和2 C3

//===================参数转换出来的16位参数======================
//按照参数表第二帧顺序排列,不能把位置掉转！！！
//读取E方的第二帧数据后,需将原数据转成16bit数据
//转换完成后的E方变量使用define定义成第三帧的E方参数变量以节省RAM资源
_PARA_EXT unsigned short u16PlatHold0;
_PARA_EXT unsigned short u16PlatHold1;
_PARA_EXT unsigned short u16PlatHold2;

_PARA_EXT unsigned short u16FanHighSpd;
_PARA_EXT unsigned short u16FanMidSpd;
_PARA_EXT unsigned short u16FanLowSpd;
_PARA_EXT unsigned short u16FanSLowSpd;
_PARA_EXT unsigned short u16FanSSLowSpd;

_PARA_EXT unsigned short u16CoolInitPulse;    //电子膨胀阀制冷初始开度
_PARA_EXT unsigned short u16CoolPlat1Pulse;   //电子膨胀阀制冷第一档开度
_PARA_EXT unsigned short u16CoolPlat2Pulse;   //电子膨胀阀制冷第二档开度
_PARA_EXT unsigned short u16CoolPlat3Pulse;   //电子膨胀阀制冷第三档开度
_PARA_EXT unsigned short u16CoolPlat4Pulse;   //电子膨胀阀制冷第四档开度
_PARA_EXT unsigned short u16CoolPlat5Pulse;   //电子膨胀阀制冷第五档开度
_PARA_EXT unsigned short u16CoolPlat6Pulse;   //电子膨胀阀制冷第六档开度
_PARA_EXT unsigned short u16CoolPlat7Pulse;   //电子膨胀阀制冷第七档开度
_PARA_EXT unsigned short u16CoolPlat8Pulse;   //电子膨胀阀制冷第八档开度
_PARA_EXT unsigned short u16CoolPlat9Pulse;   //电子膨胀阀制冷第九档开度
_PARA_EXT unsigned short u16CoolPlat10Pulse;  //电子膨胀阀制冷第十档开度

_PARA_EXT unsigned short u16HeatInitPulse;    //电子膨胀阀制热初始开度
_PARA_EXT unsigned short u16HeatPlat1Pulse;   //电子膨胀阀制热第一档开度
_PARA_EXT unsigned short u16HeatPlat2Pulse;   //电子膨胀阀制热第二档开度
_PARA_EXT unsigned short u16HeatPlat3Pulse;   //电子膨胀阀制热第三档开度
_PARA_EXT unsigned short u16HeatPlat4Pulse;   //电子膨胀阀制热第四档开度
_PARA_EXT unsigned short u16HeatPlat5Pulse;   //电子膨胀阀制热第五档开度
_PARA_EXT unsigned short u16HeatPlat6Pulse;   //电子膨胀阀制热第六档开度
_PARA_EXT unsigned short u16HeatPlat7Pulse;   //电子膨胀阀制热第七档开度
_PARA_EXT unsigned short u16HeatPlat8Pulse;   //电子膨胀阀制热第八档开度
_PARA_EXT unsigned short u16HeatPlat9Pulse;   //电子膨胀阀制热第九档开度
_PARA_EXT unsigned short u16HeatPlat10Pulse;  //电子膨胀阀制热第十档开度

_PARA_EXT unsigned short u16DefrostPulse;
_PARA_EXT unsigned short u16RecOilPulse;

_PARA_EXT unsigned short u16RetOilTim1;       //回油连续低频运转时间
_PARA_EXT unsigned short u16RetOilTim2;       //回油平台运行时间

//===================第三帧E方参数将使用define的方式定义====================
//借用第二帧的参数变量,节省RAM单元,在Eeprom.h定义第三帧数据
//宏名称要与第三帧数据对应,第二帧也最好一一对应,以防出错
//其它已经转换成16位变量的参数,可以随便使用,不过要注意初始化
#define  u8ID_CODE3         u8ID_CODE2            //头码3 C4

#define  u8T4LmtCoolTemp    u8PlatHold0H          //制冷T4限频温度点 C5
#define  u8T1F21LmtTemp     u8PlatHold0L          //允许高频温度点21 C6
#define  u8T1F22LmtTemp     u8PlatHold1H          //允许高频温度点22 C7
#define  u8T1F23LmtTemp     u8PlatHold1L          //允许高频温度点23 C8
#define  u8T1F24LmtTemp     u8PlatHold2H          //允许高频温度点24 C9
#define  u8T1F25LmtTemp     u8PlatHold2L          //允许高频温度点25 CA

#define  HEAT_RAT_PFC       u8FanHighSpdH         //室外电加热开启温度 CB
#define  u8T3HighLmtTemp    u8FanHighSpdL         //冷凝器高温限频温度 CC
#define  u8T2HighLmtTemp    u8FanMidSpdH          //蒸发器高温限频温度 CD

#define  u8T3DefrostIn1     u8FanMidSpdL          //除霜进入温度1 CE
#define  u8T3DefrostIn2     u8FanLowSpdH          //除霜进入温度2 CF
#define  u8T3DefrostIn3     u8FanLowSpdL          //除霜进入温度3 D0

#define  u8T3DefrostExit1   u8FanSLowSpdH         //除霜退出温度1 D1
#define  u8T3DefrostExit2   u8FanSLowSpdL         //除霜退出温度2 D2
#define  u8T3DefrostSUB1    u8FanSSLowSpdH        //化霜条件一T30减T3差值 D3
#define  u8T3DefrostSUB2    u8FanSSLowSpdL        //化霜条件二T30减T3差值 D4

#define  u8DefrostInT       u8CoolInitPulseH      //化霜进入时间 D5
#define  u8DefrostExitT     u8CoolInitPulseL      //化霜退出时间 D6
#define  u8DefrostInRateT   u8CoolPlat1PulseH     //化霜进入时间 能力测试 D7

#define  u8MinLimFreqCool   u8CoolPlat1PulseL     //允许运行最小频率 D8(制冷)
#define  u8MinLimFreqDry    u8CoolPlat2PulseH     //允许运行最小频率 D9(抽湿)
#define  u8MinLimFreqHeat   u8CoolPlat2PulseL     //允许运行最小频率 DA(制热)

#define  u8RetOilFreq1      u8CoolPlat3PulseH     //回油低频运转频率 DB
#define  u8RetOilFreq2      u8CoolPlat3PulseL     //回油平台频率 DC

#define  u8FreqCoolTestMin    u8CoolPlat4PulseH   //DD
#define  u8FreqCoolTestMax    u8CoolPlat4PulseL   //DE
#define  u8FreqHeatTestMin    u8CoolPlat5PulseH   //DF
#define  u8FreqHeatTestMax    u8CoolPlat5PulseL   //E0
#define  u8FreqDefrostTest    u8CoolPlat6PulseH   //E1

#define  u8StartFreCool1      u8CoolPlat6PulseL   //制冷模式压缩机起动频率档位1
#define  u8StartFreCool2      u8CoolPlat7PulseH   //制冷模式压缩机起动频率档位2
#define  u8StartFreCool3      u8CoolPlat7PulseL   //制冷模式压缩机起动频率档位3
#define  u8StartFreCool4      u8CoolPlat8PulseH   //制冷模式压缩机起动频率档位4
#define  u8StartFreCool5      u8CoolPlat8PulseL   //制冷模式压缩机起动频率档位5
#define  u8StartFreCool6      u8CoolPlat9PulseH   //制冷模式压缩机起动频率档位6
#define  u8StartFreCool7      u8CoolPlat9PulseL   //制冷模式压缩机起动频率档位7
#define  u8StartFreCool8      u8CoolPlat10PulseH  //制冷模式压缩机起动频率档位8
#define  u8StartFreCool9      u8CoolPlat10PulseL  //制冷模式压缩机起动频率档位9
#define  u8StartFreCool10     u8HeatInitPulseH    //制冷模式压缩机起动频率档位10

#define  u8StartFreHeat1      u8HeatInitPulseL    //制热模式压缩机起动频率档位1
#define  u8StartFreHeat2      u8HeatPlat1PulseH   //制热模式压缩机起动频率档位2
#define  u8StartFreHeat3      u8HeatPlat1PulseL   //制热模式压缩机起动频率档位3
#define  u8StartFreHeat4      u8HeatPlat2PulseH   //制热模式压缩机起动频率档位4
#define  u8StartFreHeat5      u8HeatPlat2PulseL   //制热模式压缩机起动频率档位5
#define  u8StartFreHeat6      u8HeatPlat3PulseH   //制热模式压缩机起动频率档位6
#define  u8StartFreHeat7      u8HeatPlat3PulseL   //制热模式压缩机起动频率档位7
#define  u8StartFreHeat8      u8HeatPlat4PulseH   //制热模式压缩机起动频率档位8
#define  u8StartFreHeat9      u8HeatPlat4PulseL   //制热模式压缩机起动频率档位9
#define  u8StartFreHeat10     u8HeatPlat5PulseH   //制热模式压缩机起动频率档位10

#define  u8FreDefrost         u8HeatPlat5PulseL   //化霜频率档位

//以下参数是E方表格里的预留项,后期需要扩展将u8InEEFx填具体的变量名即可
#define  COOL_TEST_AC_FAN   u8HeatPlat6PulseH
#define  HEAT_TEST_AC_FAN   u8HeatPlat6PulseL
#define  COOL_RAT_DEFROST   u8HeatPlat7PulseH
#define  HEAT_RAT_DEFROST   u8HeatPlat7PulseL
#define  u8SeltFlgs3        u8HeatPlat8PulseH
#define  u8SeltFlgs2        u8HeatPlat8PulseL
#define  u8SeltFlgs1        u8HeatPlat9PulseH
#define  HEAT_DLT_TEMP      u8HeatPlat9PulseL
#define  u8I2cCheckSum3     u8HeatPlat10PulseH

//第二帧的E方变量还有14个Byte未重新分配,可用作其他用途
#define  u8T5Prot5s      u8HeatPlat10PulseL  //TP保护5秒抗干扰
#define  u8T5Freq120s    u8DefrostPulseH     //TP限频,频率变化时间
#define  u8CurrProt2s    u8DefrostPulseL     //电流限频,频率变化时间
#define  u8T2Lim180s     u8RecOilPulseH      //T2 限频,频率变化时间
#define  u8InRateFreq    u8RecOilPulseL      //
#define  u8InRateOutFan  u8RetOilTim1H       // 
#define  u8InRatePMV     u8RetOilTim1L       //
#define  u8InRateTest    u8RetOilTim2H       //能力测试模式

_PARA_EXT unsigned char EEPROM_SysPara[EEP_SYS_PARA_LEN];
_PARA_EXT union
{
  unsigned char EEPROM_SysPara[EEP_SYS_PARA_LEN];
  struct
  {
    unsigned char EE_SN_CODE_ADD;
    unsigned char CUSTOMER_NUM[2];
    unsigned char VERSION;
    //---------------below is seer test-------------//
    unsigned char COOL_TEST_FREQ_A;     /// 0104
    unsigned char COOL_TEST_FREQ_B;     /// 0105
    unsigned char COOL_TEST_FREQ_C;
    unsigned char COOL_TEST_FREQ_D;
    unsigned char COOL_TEST_FREQ_E;
    unsigned char COOL_TEST_FREQ_F;
    unsigned char COOL_TEST_FREQ_G;
    unsigned char HEAT_TEST_FREQ_A;     /// 010B
    unsigned char HEAT_TEST_FREQ_B;
    unsigned char HEAT_TEST_FREQ_C;
    unsigned char HEAT_TEST_FREQ_D;
    unsigned char HEAT_TEST_FREQ_E;
    unsigned char HEAT_TEST_FREQ_F;
    unsigned char HEAT_TEST_FREQ_G;
    unsigned char COOL_TEST_SPD_AH;
    unsigned char COOL_TEST_SPD_AL;
    unsigned char COOL_TEST_SPD_BH;
    unsigned char COOL_TEST_SPD_BL;
    unsigned char COOL_TEST_SPD_CH;
    unsigned char COOL_TEST_SPD_CL;
    unsigned char COOL_TEST_SPD_DH;
    unsigned char COOL_TEST_SPD_DL;
    unsigned char COOL_TEST_SPD_EH;
    unsigned char COOL_TEST_SPD_EL;
    unsigned char COOL_TEST_SPD_FH;
    unsigned char COOL_TEST_SPD_FL;
    unsigned char COOL_TEST_SPD_GH;
    unsigned char COOL_TEST_SPD_GL;
    unsigned char HEAT_TEST_SPD_AH;
    unsigned char HEAT_TEST_SPD_AL;
    unsigned char HEAT_TEST_SPD_BH;
    unsigned char HEAT_TEST_SPD_BL;
    unsigned char HEAT_TEST_SPD_CH;
    unsigned char HEAT_TEST_SPD_CL;
    unsigned char HEAT_TEST_SPD_DH;
    unsigned char HEAT_TEST_SPD_DL;
    unsigned char HEAT_TEST_SPD_EH;
    unsigned char HEAT_TEST_SPD_EL;
    unsigned char HEAT_TEST_SPD_FH;
    unsigned char HEAT_TEST_SPD_FL;
    unsigned char HEAT_TEST_SPD_GH;
    unsigned char HEAT_TEST_SPD_GL;
    unsigned char COOL_TEST_TEMP_PLS_AH;
    unsigned char COOL_TEST_TEMP_PLS_AL;
    unsigned char COOL_TEST_TEMP_PLS_BH;
    unsigned char COOL_TEST_TEMP_PLS_BL;
    unsigned char COOL_TEST_TEMP_PLS_CH;
    unsigned char COOL_TEST_TEMP_PLS_CL;
    unsigned char COOL_TEST_TEMP_PLS_DH;
    unsigned char COOL_TEST_TEMP_PLS_DL;
    unsigned char COOL_TEST_TEMP_PLS_EH;
    unsigned char COOL_TEST_TEMP_PLS_EL;
    unsigned char COOL_TEST_TEMP_PLS_FH;
    unsigned char COOL_TEST_TEMP_PLS_FL;
    unsigned char COOL_TEST_TEMP_PLS_GH;
    unsigned char COOL_TEST_TEMP_PLS_GL;
    unsigned char HEAT_TEST_TEMP_PLS_AH;
    unsigned char HEAT_TEST_TEMP_PLS_AL;
    unsigned char HEAT_TEST_TEMP_PLS_BH;
    unsigned char HEAT_TEST_TEMP_PLS_BL;
    unsigned char HEAT_TEST_TEMP_PLS_CH;
    unsigned char HEAT_TEST_TEMP_PLS_CL;
    unsigned char HEAT_TEST_TEMP_PLS_DH;
    unsigned char HEAT_TEST_TEMP_PLS_DL;
    unsigned char HEAT_TEST_TEMP_PLS_EH;
    unsigned char HEAT_TEST_TEMP_PLS_EL;
    unsigned char HEAT_TEST_TEMP_PLS_FH;
    unsigned char HEAT_TEST_TEMP_PLS_FL;
    unsigned char HEAT_TEST_TEMP_PLS_GH;
    unsigned char HEAT_TEST_TEMP_PLS_GL;
    unsigned char COOL_TEST_MIN_PLS;
    unsigned char HEAT_TEST_MIN_PLS;
    unsigned char PMV_LOW_TEMP_DEFORST_EE;
    unsigned char PMV_LH_DEFORST_OFF_INIT_EE;
    unsigned char LH_QUIT_DEF_TEMP1;
    unsigned char LH_QUIT_DEF_TEMP2;
    unsigned char LH_DEF_ENTER_T3;
    unsigned char LH_DEF_ENTER_TIM;
    unsigned char DefrostStartFreq_ADD;
    unsigned char TEST_DEF_FREQ;
    unsigned char QuitDefrostFreq_ADD;
    unsigned char TEST_PFC_OFF_FREQ;
    //
    unsigned char EE_DEFROSTTIM;
    unsigned char EE_LOWHEATT3;
    unsigned char EE_TCDE3;
    unsigned char EE_TCDE4;
    unsigned char EE_SEERDEFFRE1;
    unsigned char EE_SEERDEFFRE3;
    //---------------up is seer test-------------//
    unsigned char DEF_FREQ;
    unsigned char STOP_FREQ;
    unsigned char FRC_COOL_FREQ;
    unsigned char COOL_DC_FAN_SPD[7];
    unsigned char HEAT_DC_FAN_SPD[7];
    unsigned char PLAT_FREQ1_ADD;
    unsigned char PLAT_FREQ2_ADD;
    unsigned char PLAT_FREQ3_ADD;
    unsigned char PLAT_FREQ4_ADD;
    unsigned char PLAT_FREQ5_ADD;
    unsigned char PLAT_TIME1_ADD1;
    unsigned char PLAT_TIME2_ADD1;
    unsigned char PLAT_TIME3_ADD1;
    unsigned char PLAT_TIME4_ADD1;
    unsigned char PLAT_TIME5_ADD1;
    unsigned char FREQ_UP_SPD_L_1;
    unsigned char FREQ_UP_SPD1_2;
    unsigned char FREQ_UP_SPD2_3;
    unsigned char FREQ_UP_SPD3_4;
    unsigned char FREQ_UP_SPD4_5;
    unsigned char FREQ_UP_SPD5_H;
    unsigned char FREQ_DW_SPD_CHG_1;
    unsigned char FREQ_DW_SPD1_2;
    unsigned char FREQ_DW_SPD2_3;
    unsigned char FREQ_DW_SPD3_4;
    unsigned char FREQ_DW_SPD4_5;
    unsigned char FREQ_DW_SPD5_H;
    unsigned char LMT_DW_SPD;
    unsigned char CoolCurrRstDiff;
    unsigned char CoolStopCurr;
    unsigned char HeatCurrRstDiff;
    unsigned char LMT_CURR_C1;
    unsigned char LMT_CURR_C2;
    unsigned char LMT_CURR_C3;
    unsigned char LMT_CURR_C4;
    unsigned char LMT_CURR_C5;
    unsigned char T4_TEMP;
    unsigned char T4HI_LMT_MIN_FREQ1;
    unsigned char T4HI_LMT_MIN_FREQ2; //0159
    unsigned char ICT;
    unsigned char ICTAD;
    unsigned char IMINTAD;
    unsigned char IMAXTAD;
    unsigned char ISUT;
    unsigned char LOW_DC_VOLT_EE;     //0164
    unsigned char HIGH_DC_VOLT_EE;    //0165
    unsigned char T4DIF;
    unsigned char T4CT[U8_COOL_T4_LEN - 1];
    unsigned char T4HT[U8_HEAT_T4_LEN - 1];
    unsigned char Reserved2[77];
  } Elem;
} EeSysPara;


//各机型参数在EEPROM的起始地址,通过拨码选择不同的机型
#define EEPROM_ADDR_ERP24K    (0*256)
#define EEPROM_ADDR_ERP36K    (1*256)
#define EEPROM_ADDR_ERP48K    (2*256)
#define EEPROM_ADDR_ERP55K    (3*256)

#define I2C_DEV_ADD_W          0xa0
#define I2C_DEV_ADD_R          0xa1

#define EEPROM_SN              40//2
#define U8_DRV_EEPROM_SN       100

#define DlyNoUse()      NOP()

/************************************************************************
 Exported Variables
*************************************************************************/
_PARA_EXT unsigned char u8I2cTmp;
_PARA_EXT unsigned char u8ChkSum;
_PARA_EXT unsigned char u8EEWrtValidTim;
_PARA_EXT unsigned char u8EEWrtTim;
_PARA_EXT unsigned short u16EEReadChkSumTmp;
_PARA_EXT unsigned char u8EER_F, u8EEReadChkSumTmp, u8ACK;
_PARA_EXT unsigned char u8MachType;
_PARA_EXT unsigned char u8I2cFlg;

/************************************************************************
 Exported Function Call Prototypes
*************************************************************************/
//_PARA_EXT void ReadMainParameter3(void);
//_PARA_EXT void ReadMainParameter(unsigned short Address);
//_PARA_EXT void ReadDrvParameter(unsigned short Address);
//_PARA_EXT void I2cDelay();
//_PARA_EXT void I2cSendStart();
//_PARA_EXT void I2cSendStop();
//_PARA_EXT void I2cSendByte(unsigned char u8I2cDataSend);
//_PARA_EXT void EEReadByte(unsigned short u16EEPROM_StartAddr, unsigned short u16EEPROM_Len, unsigned char *pU8RamTrgAddr);
//_PARA_EXT void s_delay(unsigned short val);
//_PARA_EXT unsigned char I2cRecvByte();
//_PARA_EXT unsigned char ReadI2c(unsigned short u16I2cDataAddr);
//_PARA_EXT unsigned char ReadI2cPar2(unsigned char u8I2cDataAddr);
//_PARA_EXT unsigned short eeprom_read(unsigned short adss);
_PARA_EXT unsigned short BytesToSingleWord(unsigned char *readdata);
_PARA_EXT void BytesToMultiWords(unsigned char *readdata, unsigned short *StartAdress, unsigned char *endadress);
//_PARA_EXT void I2cInit();
//_PARA_EXT void EEInit();
//_PARA_EXT void EepParaDeal();

extern void EeParamPro(void);

#endif

