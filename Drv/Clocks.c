
//
// Clocks.c
//
#include "ICF_Symbols.h"
#include "Clocks.h"
#include "Hardware.h"

//
volatile uint32_t  systick_counter;

static RCC_ClocksTypeDef   stSystemClock = {0};
static uint32_t u32StartTime;
static uint32_t u32CycleTime;
volatile loop_timers_st stClocks;

static uint8_t    u8SysTicks; // counts 1mS systicks between overrun checks


void SystemInit(void) { } // replace the default system init.

/*************************************************************************
 *  FUNCTION NAME :
 *      delay_ms
 *
 *  FUNCTIONAL DESCRIPTION :
 *      delay_ the specified number of milliseconds
 *
 *  FORMAL INPUT PARAMETERS :
 *      ms - The number of milliseconds.
 *
 *      Note that this always ensures that the delay is from n to n+1
 *      milliseconds, synchronized to the master 1ms clock. When trying
 *      to do multiple 1ms delays, use 0 for all but the first call.
 ************************************************************************/
void delay_ms(clock_t ms)
{
  clock_t  t;

  t = clock();
  while(clock() - t < ms + 1) /* wait */
    ;
}

// Vhecks the saved start time on the SysTick timer against the current time 
// and the cycle length to see if the previous cycle overran. If so, the overrun
// cycle number is recorded, the overrun counter is incremented.
//
static void CheckForTimerOverrun(void)
{
  STATIC uint32_t u32Temp;

  stClocks.bCycleOvrFlag = false;

  // Calculate how long the previous cycle took.  The SysTick timer counts down
  // from its max, so start time should always be a higher number.  If this is
  // not the case, we definitely have an overrun.
  u32Temp = u32GetTick(); // get current time first, in case it rolls over
  u32StartTime += CLOCKS_PER_SYSTICK * (uint32_t)(u8SysTicks);  // adjust for 1mS SysTick
  u32CycleTime = (u32StartTime - u32Temp) + stClocks.u32OvrCycleTime;

  if (u32CycleTime > CLOCKS_PER_SYSTICK * SYSTICKS_PER_LOOP)
  {
    stClocks.u16NmbrOfOverruns++;
    stClocks.u32OverrunCycleNmbr = stClocks.u32CycleNumber;
    ClearCycleTimeFlag();
  }

  if (u32CycleTime < stClocks.u32MinCycleTime)
  { // if we were faster than before, record it.
    stClocks.u32MinCycleTime = u32CycleTime;
  }
  if (u32CycleTime > stClocks.u32MaxCycleTime)
  { // if we were slower than before, record it.
    stClocks.u32MaxCycleTime = u32CycleTime;
  }
}

//
//
void ClearCycleTimeFlag(void)
{
  stClocks.bCycleTimeFlag = false;
  u32StartTime = u32GetTick();
  u8SysTicks = 0;
  // Increment the cycle number
  stClocks.u32CycleNumber++;

  stClocks.u32OvrCycleTime = 0;
}

//
//
void StartLoopOvrDetection(void)
{
  stClocks.bCycleOvrFlag = true;
}

//
//
//
void InitClocks(void)
{
  systick_counter = 0;

//  // Before doing anything, the chip needs to be told that the vector table was
//  // moved to accommodate the Application Header
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH,
//      ICFEDIT_IntVec_Start - (uint8_t *) FLASH_START_ADDRESS);

//  //start the System Clocks from a known state
//  RCC_DeInit();
//
//  //Disable the external crystal oscillator so the pins can be used as GPIO
//  RCC_HSEConfig(RCC_HSE_OFF);
//  
//  //Configure the PLL for speeding up when we need to.  The output frequency
//  //cannot be > 72MHz.  8MHz HSI/2 x 16 => SYSCLK = 64MHz.
//  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
//
//  // Enable the PLL
//  RCC_PLLCmd(ENABLE);
//
//  // Enable the flash prefetch buffer. This can only be done with SYSCLK < 24MHz.
//  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//  // Set wait-states for flash accesses:
//  // Zero wait states, if 0 < SYSCLK <= 24 MHz
//  // One wait state, if 24 MHz < SYSCLK <= 48 MHz
//  // Two wait states, if 48 MHz < SYSCLK <= 72 MHz
//  FLASH_SetLatency(FLASH_Latency_2);
//
//  // Set the (high speed) APB2 clock (PCLK2) = SYSCLK.
//  // APB2 can run up to 72MHz.
//  RCC_PCLK2Config(RCC_HCLK_Div1);
//
//  // Set the (low speed) APB1 clock (PCLK1) = SYSCLK/2.
//  // APB1 can only run up to 36MHz.
//  RCC_PCLK1Config(RCC_HCLK_Div2);
//
//  // Wait until the PLL is ready.
//  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    DEBUG_NOP();
//
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//  // Wait for the switch over to complete
//  while (RCC_GetSYSCLKSource() != 0x08)
//    DEBUG_NOP();
//
  // We should now be running at 64.0MHz. Calculate and save the resultant
  // system clock frequencies for reference by other modules.
  RCC_GetClocksFreq(&stSystemClock);
  
  //Initialize the device variables
  stClocks.u32MaxCycleTime  = 0x00000000;
  stClocks.u32MinCycleTime  = 0xFFFFFFFF;
  stClocks.u16NmbrOfOverruns  = 0;
  stClocks.u32CycleNumber = 0;
  stClocks.bCycleTimeFlag = false;
  stClocks.bCycleOvrFlag  = false;

  //Initialize the local variables
  u32StartTime = 0x00000000;
  u32CycleTime = 0x00000000;

  // Configure the SysTick Timer for Main Loop Timing.  At 72.0MHz, HCLK is
  //qneeded to get to 10.0ms.  If using the /8 the math will be off.
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  // Place N-1 in the SysTick Load register to set the interrupt rate.
  SysTick->LOAD = CLOCKS_PER_SYSTICK - 1;
  
  // The SysTick timer should be disabled and not have the interrupt on by
  // default from the reset condition.  Enabling will cause the VAL register to
  // get populated with the value in the LOAD register.
  StartSysTick();
  
}

//
//
//
void Sleep(void)
{
   __WFI();
}

//
//
//
void StartSysTick(void)
{
  //Enable the SysTick interrupt and start the clock.  The act of enabling
  //also takes care of the initial load to the value in SysTick->LOAD.
   SysTick->CTRL |= (SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT);
}

//
//
//
void SysTick_Handler(void)
{
  systick_counter++;
  u8SysTicks ++;

  //Clear the Interrupt Count Flag
  SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG;

  if (stClocks.bCycleOvrFlag == true)
  {
    stClocks.u32OvrCycleTime += CLOCKS_PER_SYSTICK;
  }
  if (systick_counter % SYSTICKS_PER_LOOP == 0)
  {
    //Set the Cycle Time Flag
    stClocks.bCycleTimeFlag = true;
    __SEV();
  }
}

//
//  returns the current value of the SysTick Timer (counter)
//
uint32_t u32GetTick(void)
{
  return (SysTick->VAL);
}

uint32_t u32GetSysTick(void)
{
  return (systick_counter);
}

void WaitForCycleTime(void)
{
  CheckForTimerOverrun();

  while (stClocks.bCycleTimeFlag == false)
  {
    Sleep();
    __no_operation();
    
  }

  ClearCycleTimeFlag();
  StartLoopOvrDetection();
}
