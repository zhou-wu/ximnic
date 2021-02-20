#include "UsrDef.h"
#include "ParameterPro.h"
#include "InOutComm.h"
#include "DgComm.h"
#include "SelfChk.h"
#include "CompCtr.h"
#include "DevCtr.h"
#include "FanCtr.h"
#include "TempSample.h"
#include "TimeTick.h"
#include "MiscFunc.h"

static unsigned char u8TimTick1ms;
static unsigned char u8TimTick10ms;
static unsigned char u8TimTick100ms;
//static unsigned char u8TimTick1s;
//static unsigned char u8TimTick1Hour;

static unsigned char u8Tim2SCnt;
static unsigned char u8Tim5SCnt;
static unsigned char u8Tim10SCnt;
static unsigned char u8Tim15SCnt;
static unsigned char u8Timer1s;

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void TimTickInit(void)
{
  u8TimTick1ms = 0;
  u8TimTick10ms = 0;
  u8TimTick100ms = 0;
  //u8TimTick1s = 0;
  //u8TimTick1Hour = 0;
}

/**
  * @brief  :
  * @param  :
  * @retval :
  */
void TimTickPro(void)
{
  u8TimTick1ms++;

  if (u8StepTim != 0xFF)
  {
    u8StepTim++;
  }
  
  if (u8EEWrtTim != 0xFF)
  {
    u8EEWrtTim++;
  }

  if (u8TimInitComm < 0xFF)
  {
    u8TimInitComm += 1;
  }

  /* 10ms */
  if ( u8TimTick1ms >= 10 )
  {
    u8TimTick1ms = 0;
    u8TimTick10ms++;
#if 0
    if (u8DispTim)
    {
      u8DispTim--;
    }
    if (u8ErrDispTim)
    {
      u8ErrDispTim--;
    }
#endif
    if (u8AdcTim != 0xFF)
    {
      u8AdcTim++;
    }
#if 0
    if (u8FiltTim != 0xFF)
    {
      u8FiltTim++;
    }
#endif
#if 0
    if (u8Initfancam != 0xFF)
    {
      u8Initfancam++;
    }
#endif
    if (u8DispFlashTim)
    {
      u8DispFlashTim--;
    }
    if (u8StepExcitTim)
    {
      u8StepExcitTim--;
    }
    if (u8InOutCommTxdTim) 
    { 
      u8InOutCommTxdTim--;
    }
    if(u8DcVoltDly < 0xFF)
    {
      u8DcVoltDly++;
    }
    if(u8PfcErrDly < 0xFF)
    {
      u8PfcErrDly++;
    }
    if(u8CompDly < 0xFF)
    {
      u8CompDly++;
    }
    if(u8StartFanDly < 0xFF)
    {
      u8StartFanDly++;
    }
    if ( u8DgLedBlinkTim < 0xFF )
    {
      u8DgLedBlinkTim++;
    }
  }

  /* 100ms */
  if ( u8TimTick10ms >= 10 )
  {
    u8TimTick10ms = 0;
    u8TimTick100ms++;
#if 0
    if (stTP.u16SlowDwTim)
    {
      stTP.u16SlowDwTim--;
    }
    if (stTP.u8FastDwTim)
    {
      stTP.u8FastDwTim--;
    }
#endif
    if (u8DgCommRst1s != 0xFF)
    {
      u8DgCommRst1s++;
    }
#if 0
    if (u8FanDlyTim != 0xFF)    // 12041603 风机、四通阀分开切换功能
    {
      u8FanDlyTim++;
    }
#endif
#if 0
    if (u8IPMProtTim != 0xFF)
    {
      u8IPMProtTim++;
    }
#endif
    if (u8PreHeatStopTim)
    {
      u8PreHeatStopTim--;
    }
#if 0
    u8FreqDispTim++;
    if (u8FreqDispTim >= 20)
    {
      u8FreqDispTim = 0;
      if (unDispFlags4.stBit.FreqDisp)
      {
        unDispFlags4.stBit.FreqDisp = 0;
      }
      else
      {
        unDispFlags4.stBit.FreqDisp = 1;
      }
    }
#endif
    if(u8PowerOnDly < 0xFF)
    {
      u8PowerOnDly++;
    }
    if(u16CompSpd > 0)
    {
      u16CompSpd--;
    }
    if(u16ROFreqPlatTim > 0)
    {
      u16ROFreqPlatTim--;
    }
    if(u8LowPressTim < 0xFF)
    {
      u8LowPressTim++;
    }
    if(u8HighPressTim < 0xFF)
    {
      u8HighPressTim++;
    }
    if(u8TimPowerOn < 0xFF)
    {
      u8TimPowerOn++;
    }
    if(u8T5Prot5s < 0xFF)
    {
      u8T5Prot5s++;
    }
    if(u8T2Prot5s < 0xFF)
    {
      u8T2Prot5s++;
    }
    if(u8T3Prot5s < 0xFF)
    {
      u8T3Prot5s++;
    }
    if(u8CurrProt2s < 0xFF)
    {
      u8CurrProt2s++;
    }
    if(u8ACFanSwhDly > 0)
    {
      u8ACFanSwhDly--;
    }

    if ( u8SelfChkRYTim < 0xFF )
    {
      u8SelfChkRYTim++;
    }
  }

  /* 1s */
  if ( u8TimTick100ms >= 10 )
  {
    u8TimTick100ms = 0;
    
    u8Tim2SCnt++;
    u8Tim5SCnt++;
    u8Tim10SCnt++;
    u8Tim15SCnt++;
    u8Timer1s++;

    //1s timer base
#if 0
    if (u8FanAdjTim != 0xFF)
    {
      u8FanAdjTim++;
    }
#endif
    if(u8InDoorSetTim != 0xFF)
    {
      u8InDoorSetTim++;
    }
#if 0
    if (u8DownSpdTim != 0xFF)
    {
      u8DownSpdTim++;
    }
#endif
#if 0
    if (u8ODOutFan10sTim != 0xFF)
    {
      u8ODOutFan10sTim++;
    }
#endif
    if (u8EEWrtValidTim != 0xFF)
    {
      u8EEWrtValidTim++;
    }
    if(u8Force25HZTim != 0xFF)
    {
      if(COMP_RUN == u8CompSta )
      {
        u8Force25HZTim++;
      }
    }
#if 0
    if (u8PmvInit30s != 0xFF)
    {
      u8PmvInit30s++;
    }
    if (u8PMVT2min != 0xFF)
    {
      u8PMVT2min++;
    }
#endif
    if(u8LowCoolTim)
    {
      u8LowCoolTim--;
    }
    if (u8DownSpdTim != 0xFF)
    {
      u8DownSpdTim++;
    }
    if (u8InOutCommErrTim != 0xFF)
    {
      u8InOutCommErrTim++;
      if ((u8InOutCommErrTim == 90) || (u8InOutCommErrTim == 60) || (u8InOutCommErrTim == 30))
      {
        InOutCommInit();
      }
    }
    
    if(SeerTestNo != u8InRateTest)
    {
      if((C_ModeCool == u8InSetMode) || (C_ModeDry == u8InSetMode) || (C_ModeForce == u8InSetMode))
      {
        switch(u8InRateTest)
        {
          case 1://SeerTestMin
            if (unSeltFlgs7.bits.SeerDefreostA_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 2://SeerTestMid
            if (unSeltFlgs7.bits.SeerDefreostB_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 3://SeerTestRat
            if (unSeltFlgs7.bits.SeerDefreostC_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 4://SeerTestMax
            if (unSeltFlgs7.bits.SeerDefreostD_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 5://SeerTest5
            if (unSeltFlgs7.bits.SeerDefreostE_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 6://SeerTest6
            if (unSeltFlgs7.bits.SeerDefreostF_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 7://SeerTest7
            if (unSeltFlgs7.bits.SeerDefreostG_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          default://SeerTestNO
            DefrostCtr();
            break;
        }
      }
      else
      {
        switch(u8InRateTest)
        {
          case 1://SeerTestMin
            if (unSeltFlgs8.bits.SeerDefreostA_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 2://SeerTestMid
            if (unSeltFlgs8.bits.SeerDefreostB_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 3://SeerTestRat
            if (unSeltFlgs8.bits.SeerDefreostC_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 4://SeerTestMax
            if (unSeltFlgs8.bits.SeerDefreostD_OFF == 0)
            {
              //DefrostCtr();
            }
            SeerDefrostCtr();
            break;

          case 5://SeerTest5
            if (unSeltFlgs8.bits.SeerDefreostE_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 6://SeerTest6
            if (unSeltFlgs8.bits.SeerDefreostF_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          case 7://SeerTest7
            if (unSeltFlgs8.bits.SeerDefreostG_OFF == 0)
            {
              DefrostCtr();
            }
            break;

          default://SeerTestNo
            DefrostCtr();
            break;
        }
      }
    }
    else
    {
      DefrostCtr();          //化霜进入条件,1s调用
    }
    if(u8LowCoolTim > 0)
    {
      u8LowCoolTim--;
    }
    if(u16ROLowFreqTim > 0)
    {
      u16ROLowFreqTim--;
    }
    if(u16QuitDefrostTim > 0)
    {
      u16QuitDefrostTim--;
    }
    if(u8ValveDlyTim > 0)
    {
      u8ValveDlyTim--;
    }
    if(u8T2Lim180s < 0xFF)
    {
      u8T2Lim180s++;
    }
    if(u8T2Freq60s < 0xFF)
    {
      u8T2Freq60s++;
    }
    if(u8T3Freq60s < 0xFF)
    {
      u8T3Freq60s++;
    }
    if(u8T5Freq120s < 0xFF)
    {
      u8T5Freq120s++;
    }
    if(u8FreqAreaT3Min < 0xFF)
    {
      u8FreqAreaT3Min++;
    }
    if(u8StopFanDly < 0xFF)
    {
      u8StopFanDly++;
    }
    if(u16CompStopTim < 0xFF)
    {
      u16CompStopTim++;
    }
    if(u8Tim3Min < 0xFF)
    {
      u8Tim3Min++;
    }
    if(u8DgCommRstTim < 0xFF)
    {
      u8DgCommRstTim++;
    }
    if(u8ErrHoldTim < 0xFF)
    {
      u8ErrHoldTim++;
    }
    if(u8VoltLimPeriod < 0xFF)
    {
      u8VoltLimPeriod++;
    }
    if(u8T5TempErrTim < 0xFF)
    {
      u8T5TempErrTim++;
    }
    if(u8DCFanErrTim < 0xFF)
    {
      u8DCFanErrTim++;
    }
    if (u8VoltStopTim < 0xFF)
    {
      u8VoltStopTim++;
    }
    if(u8VoltLimitTim < 0xFF)
    {
      u8VoltLimitTim++;
    }
    if(u8DefrostExit < 0xFF)
    {
      u8DefrostExit++;
    }
    if(u8PmvRegulateTim < 0xFF)
    {
      u8PmvRegulateTim++;
    }
    if(u16CompRunTim < 0xFFFF)
    {
      u16CompRunTim++;
    }
    if(u16MinFreqChangeT < 0xFFFF)
    {
      u16MinFreqChangeT++;
    }
    if(u8TempErrHold < 0xFF)         //温度传感器恢复时间
    {
      u8TempErrHold++;
    }
    if(u8CompStopTim != 0xFF)
    {
      u8CompStopTim++;
    }
    if ( u8WaitPFCTim > 0 )
    {
      u8WaitPFCTim--;
    }

    
  }

}





