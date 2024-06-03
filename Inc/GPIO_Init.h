
#ifndef     _GPIO_INIT_H
#define     _GPIO_INIT_H

#include    <stdlib.h>
#include    <stm32f10x.h>

// Format of the STM32F103 GPIO port initialization table structure (io_init_table_st).
// RCC_APB2Periph - value for RCC_APB2PeriphClockCmd() to ENABLE this port's clock.
//    Parameters for the 'GPIO_InitTypeDef' structure passed to GPIO_Init()
//       GPIOx       - address of the port to initialize.
//       GPIO_Mode   - the port mode for the pins (floating/pull-up/pull-down).
//       GPIO_Pins   - the port pins to configure.
//       GPIO_Speed  - port pin(s) speed
//       lock_config - see 'lock_config_t'

typedef enum
{
  GPIO_PIN_CONFIG_CHANGABLE,
  GPIO_PIN_CONFIG_LOCKED
}  lock_config_en;

typedef  struct
{
  uint32_t          RCC_APB2Periph;
  GPIO_TypeDef      *GPIOx;
  GPIOMode_TypeDef  GPIO_Mode;
  uint16_t          GPIO_Pins;
  GPIOSpeed_TypeDef GPIO_Speed;
  lock_config_en    enLockConfig;
}  io_init_table_st;

//
// global functions
//
void InitGPIO(const io_init_table_st *pIOInitTable, size_t tNumTableEntries);

#endif
