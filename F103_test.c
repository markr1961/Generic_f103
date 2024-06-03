
//
// header files
//
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Clocks.h"
#include "WDog.h"
#include "Hardware.h"

uint32_t last_systick;

void main(void)
{

  InitClocks();

  InitHardware();

  StartSysTick();

  PatWatchdog();
  
  last_systick = systick_counter; 
  ClearCycleTimeFlag();
  StartLoopOvrDetection();

  while(true)
  {
    uint32_t currentSysTick = u32GetSysTick();
    if (currentSysTick > last_systick + HEART_BEAT_RATE)
    {
      last_systick = currentSysTick;

      // toggle LED:
      if(GPIO_ReadOutputDataBit(LED))
        // clear
        GPIO_ResetBits(LED);
      else // set
        GPIO_SetBits(LED);

    }
    
    PatWatchdog();
    
//    GPIO_ON;
    WaitForCycleTime();
//    GPIO_OFF;
  }

} // end main()

// eof F103_Test.c
