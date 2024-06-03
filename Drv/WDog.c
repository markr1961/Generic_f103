
//
#include "Hardware.h"

#define WDTIMEOUT(A) ((uint16_t)A/3.2) // in mSec

void InitWatchdog(void)
{
  //Configure the Debug Interface so that it stops the Watchdog
#ifdef DEBUG
  DBGMCU_Config(DBGMCU_IWDG_STOP, ENABLE);
#endif

  // Activate the LSI
  RCC_LSICmd(ENABLE);

  // Enable write access to IWDG_PR and IWDG_RLR registers
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  //IWDG counter clock: 40KHz(LSI) +25% -50% (30-60KHz)
  //IWDG_SetPrescaler(IWDG_Prescaler_4);    // 0.1mS/Tick
    IWDG_SetPrescaler(IWDG_Prescaler_128);  // 3.2mS/tick

  //Set Reload Counter for 7.5 Cycles worth of time
  // IWDG_SetReload(WDTIMEOUT(750));      //in milliseconds
  IWDG_SetReload(0xFFF);

  //Reload the IWDG Counter
  IWDG_ReloadCounter();

  //Enable the Watchdog
  IWDG_Enable();
}

void PatWatchdog(void)
{
  IWDG_ReloadCounter();
}
