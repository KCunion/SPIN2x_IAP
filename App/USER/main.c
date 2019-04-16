#include "delay.h"
#include "sys.h"
#include "led.h"

#define APPLICATION_ADDRESS     ((uint32_t)0x08010000)

int main()
{
//    uint32_t i = 0;     
//    for(i = 0; i < 48; i++) {
//        *((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i << 2));
//    }
//    RCC->APB2ENR |= 1;
//    SYSCFG->CFGR |= 0x03;
    Led_Init();

    while (1) {
        Breath_Led();
    }
}
