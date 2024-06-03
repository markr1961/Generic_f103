#include    "Hardware.h"

//
//  The STM32F1xx factory programmed flash size value. This indicates the total
//  size of flash memory in KB.
//  See section 23.1.1 of the STM32F100 Reference Manual.
#define F_SIZE_ADDRESS              ((__I uint16_t *) 0x1FFFF7E0)
#define F_SIZE                      (*F_SIZE_ADDRESS)


typedef struct
{
   uint8_t             u8Flag;   // For RCC_GetFlagStatus()
   reset_event_code_en enReason;
}  reset_st;

uint32_t deviceID, revisionID;
reset_event_code_en resetReason;

//
//
reset_event_code_en enGetResetReason(void)
{
   const reset_st stResetCause[] =
   {
      { //Independent Watchdog
         RCC_FLAG_IWDGRST,
         ARM32_WATCHDOG_RESET
      },
      { //Window Watchdog
         RCC_FLAG_WWDGRST,
         ARM32_WATCHDOG_RESET
      },
      { //Self (software)
         RCC_FLAG_SFTRST,
         ARM32_SOFTWARE_RESET
      },
      { //Power-on
         RCC_FLAG_PORRST,
         ARM32_POWER_ON_RESET
      },
      { //Reset switch etc
         RCC_FLAG_PINRST,
         ARM32_EXTERNAL_RESET
      },
      { //Low power
         RCC_FLAG_LPWRRST,
         ARM32_LOW_POWER_RESET
      },
   };
   reset_event_code_en enReason;

   enReason = ARM32_UNKNOWN_RESET;
   for (int i=0; i < sizeof(stResetCause)/sizeof(reset_st); i++)
   {
      if(RCC_GetFlagStatus(stResetCause[i].u8Flag) == SET)
      {
         enReason = stResetCause[i].enReason;
         break;
      }
   }
   RCC_ClearFlag();
   
   return(enReason);
}


void InitHardware(void)
{
  
  //RCC_AHBPeriphClockCmd(, ENABLE);
  DBGMCU_Config(DBGMCU_SLEEP, ENABLE);

  deviceID = DBGMCU_GetDEVID();
  revisionID = DBGMCU_GetREVID();
  resetReason = enGetResetReason();

  InitWatchdog();
  InitNVIC();
  
  InitGPIO(stHwGpioTable, HwGpioSize);  // LED
  // InitGPIO(stHwUsartTable, HwUsartSize); // USART

} // end InitHardware()

// end of Hardware.c
