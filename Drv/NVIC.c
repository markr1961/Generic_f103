#include <stm32f10x.h>  // required for "NVIC_reset"
#include "NVIC.h"
#include "ICF_Symbols.h"
#include "Hardware.h"  // DEBUG_BREAK()

typedef struct
{
  uint8_t             u8Flag;   // For RCC_GetFlagStatus()
  reset_event_code_en enReason;
}  reset_st;

void __exit(int arg)
{
  DEBUG_BREAK();

  // shutting down, so prevent any interrupts that might affect it
  __disable_interrupt();
  // Software Reset
  NVIC_SystemReset();
  
  (void)arg;
}

void InitNVIC(void)
{
  NVIC_InitTypeDef stNVICCfg;

  // Configure the NVIC for 16 levels of preemption priority
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  // Configure NVIC for SysTick interrupts.
  stNVICCfg.NVIC_IRQChannel = (uint8_t) SysTick_IRQn;
  stNVICCfg.NVIC_IRQChannelCmd = ENABLE;
  stNVICCfg.NVIC_IRQChannelPreemptionPriority = 1;  //Highest Priority
  NVIC_Init(&stNVICCfg);

  // Enable division by zero trap.
  SCB->CCR |= SCB_CCR_DIV_0_TRP;
}

// eof NVIC.c
