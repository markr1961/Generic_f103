//
#ifndef _CLOCKS_H
#define _CLOCKS_H

#include <stdint.h>
#include <stdbool.h>

//
// global constants
//
#define CLOCK_FREQUENCY       8000000

#define CLOCKS_PER_SYSTICK    (CLOCK_FREQUENCY / 1000)

#define SYSTICKS_PER_LOOP     10

#define DELAY_US_MULTIPLIER   (CLOCK_FREQUENCY / 3500000)  // magic number

//
// clock timer type def
//
typedef struct
{
  uint32_t u32MaxCycleTime;
  uint32_t u32MinCycleTime;
  uint32_t u32OvrCycleTime;
  uint32_t u32CycleNumber;
  uint32_t u32OverrunCycleNmbr;
  uint16_t u16NmbrOfOverruns;
  bool bCycleOvrFlag;
  bool bCycleTimeFlag;
} loop_timers_st;

extern volatile uint32_t    systick_counter;

//
// exported functions
//
//  void CheckForTimerOverrun(void);
void ClearCycleTimeFlag(void);
void InitClocks(void);
//void Sleep(void);
void StartSysTick(void);
uint32_t u32GetTick(void);
uint32_t u32GetSysTick(void);
void WaitForCycleTime(void);
void StartLoopOvrDetection(void);
#endif

// eof Clocks.h
