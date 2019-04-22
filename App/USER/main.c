#include "interrupt_tab.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "stdbool.h"
#define APPLICATION_ADDRESS         ((uint32_t)0x08010000)

void Tim_Init(uint16_t hwPrescaler,uint16_t hwPeriod);
void TIM1_Processing(void);
struct NVIC_TABLE_t tNVIC_TABLE __attribute__((at(0X20000000)));

//bool serial_out(uint8_t chByte)
//{
//    if ((UART1 ->CSR & ((uint16_t)0x0001)) != 0) {
//        UART1 ->TDR = (chByte & (uint16_t)0x00FF);      
//        return true;
//    }
//    return false;
//}

int main()
{
//    uint8_t *pchTimAddr;
//    uint32_t wTimAddr = *(__IO uint32_t *)0X20000074;
//    pchTimAddr = (uint8_t *)&wTimAddr;
//    while(!serial_out(*pchTimAddr));
//    while(!serial_out(*(pchTimAddr+1)));
//    while(!serial_out(*(pchTimAddr+2)));
//    while(!serial_out(*(pchTimAddr+3)));
    Led_Init();
    Tim_Init(4799,5000);
    while (1) {
        Breath_Led();
    }
}
void Tim_Init(uint16_t hwPrescaler,uint16_t hwPeriod)
{
    do {
        NVIC_InitTypeDef NVIC_InitStructure;

        NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);	
    } while (0);

    RCC ->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1 ->PSC = hwPrescaler;
    TIM1 ->ARR = hwPeriod;
    TIM1 ->DIER = TIM_DIER_UIE;
    TIM1 ->CR1 |= TIM_CR1_CEN;
    TIM1 ->SR = (uint32_t)~TIM_SR_UIF;
}

void TIM1_Processing(void)
{
    if (TIM1 ->SR&TIM_SR_UIF) {
        TIM1 ->SR = (uint32_t)~TIM_SR_UIF;
        LED4_TOGGLE();
    }
}

void HardFault_Processing(void)
{
    while(1);
}

void Interrupt_Init(void)
{
    uint32_t i = 0;     
    for(i = 0; i < 16; i++) {
        *((uint32_t*)(0x20000000 + (i << 2))) = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i << 2));
    }
    
    tNVIC_TABLE.pHardFault_Handler = HardFault_Processing;
    tNVIC_TABLE.pTIM1_BRK_UP_TRG_COM_Handler = TIM1_Processing;
    
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->CFGR |= 0x03;
}
