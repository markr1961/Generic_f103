//
#include  "GPIO_Init.h"
#include  "Pin_Defs.h"
#include  "Pin_Defs.c" // const structs
//
//
void InitGPIO(const io_init_table_st *pIOInitTable, size_t tNumTableEntries)
{
   GPIO_InitTypeDef    stPortConfig;

   for (int i = 0; i < tNumTableEntries; i++)
   {
      // Enable clock for the port peripheral.
      RCC_APB2PeriphClockCmd(pIOInitTable[i].RCC_APB2Periph, ENABLE);

      // And configure port pin(s).
      GPIO_StructInit(&stPortConfig);
      stPortConfig.GPIO_Mode  = pIOInitTable[i].GPIO_Mode;
      stPortConfig.GPIO_Pin   = pIOInitTable[i].GPIO_Pins;
      stPortConfig.GPIO_Speed = pIOInitTable[i].GPIO_Speed;
      GPIO_Init(pIOInitTable[i].GPIOx, &stPortConfig);

      if (pIOInitTable[i].enLockConfig == GPIO_PIN_CONFIG_LOCKED)
      {
      // Prevent the port pin configuration(s) from being inadvertently
      // changed.
         GPIO_PinLockConfig(pIOInitTable[i].GPIOx, pIOInitTable[i].GPIO_Pins);
      }
   }
}
