#ifndef __LED_H
#define __LED_H	 
#include "HAL_device.h"

#define LED4_ON()       do{ GPIOA->BRR = 0x8000;}while(0)                    // PA15
#define LED4_OFF()      do{ GPIOA->BSRR = 0x8000;}while(0)                      // PA15

#define LED3_ON()       do{ GPIOB->BRR = 0x0008;}while(0)                     // PB3
#define LED3_OFF()      do{ GPIOB->BSRR = 0x0008;}while(0)                       // PB3

#define LED2_ON()       do{ GPIOB->BRR = 0x0010;}while(0)                     // PB4
#define LED2_OFF()      do{ GPIOB->BSRR = 0x0010;}while(0)                       // PB4

#define LED1_ON()       do{ GPIOB->BRR = 0x0020;}while(0)                     // PB5
#define LED1_OFF()      do{ GPIOB->BSRR = 0x0020;}while(0)                       // PB5

extern void Led_Init(void);
extern void Breath_Led(void);

#endif
