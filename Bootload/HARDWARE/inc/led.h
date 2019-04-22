#ifndef __LED_H
#define __LED_H	 
#include "HAL_device.h"
#include "HAL_rcc.h"
#include "HAL_gpio.h"

#define LED4_ON()       do{ GPIO_ResetBits(GPIOA,GPIO_Pin_15);}while(0)                    // PA15
#define LED4_OFF()      do{ GPIO_SetBits(GPIOA,GPIO_Pin_15);}while(0)                      // PA15

#define LED3_ON()       do{ GPIO_ResetBits(GPIOB,GPIO_Pin_3);}while(0)                     // PB3
#define LED3_OFF()      do{ GPIO_SetBits(GPIOB,GPIO_Pin_3);}while(0)                       // PB3

#define LED2_ON()       do{ GPIO_ResetBits(GPIOB,GPIO_Pin_4);}while(0)                     // PB4
#define LED2_OFF()      do{ GPIO_SetBits(GPIOB,GPIO_Pin_4);}while(0)                       // PB4

#define LED1_ON()       do{ GPIO_ResetBits(GPIOB,GPIO_Pin_5);}while(0)                     // PB5
#define LED1_OFF()      do{ GPIO_SetBits(GPIOB,GPIO_Pin_5);}while(0)                       // PB5

extern void Led_Init(void);
extern void Breath_Led(void);

#endif
