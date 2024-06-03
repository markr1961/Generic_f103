## A generic Bluepill project with the usual xxF103C6/C8

[git check out](https://github.com/markr1961/Generic_f103.git)  

## Build Environment
created from an IAR template.  
IAR v7.80.4  
Expects ST Std Periph lib one directory up from project.  

### includes
$PROJ_DIR$\Inc  
$PROJ_DIR$\..\STM32F10x_StdPeriph_Lib\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x  
$PROJ_DIR$\..\STM32F10x_StdPeriph_Lib\Libraries\STM32F10x_StdPeriph_Driver\inc  

### Pr-prcessor defines
STM32F10X_LD  
USE_STDPERIPH_DRIVER  
__ICCARM__  

## open issues
Which pins are SO08 SPI flash attached to?  

# To Do
- setup clocks to run CPU, SysTick, AHB, APB2 at max and APB1 at max. Without an external xTAL, the max is 64 and 32MHz respectively.  
- LED  
- USART  
- SPI  
- support W25Qxx SPI flash @ max clock. 75MHz?  
- RTC  
