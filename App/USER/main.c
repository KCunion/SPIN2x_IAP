#include "interrupt_tab.h"
#include "delay.h"
#include "sys.h"
#include "led.h"

//define中断处理函数指针列表，存放于0X20000000
//在Interrupt_Init()中对tNVIC_TABLE结构体进行初始化
struct NVIC_TABLE_t tNVIC_TABLE __attribute__((at(0X20000000)));

void Tim_Init(uint16_t hwPrescaler,uint16_t hwPeriod);

int main()
{
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
//TIM中断处理程序
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
//对tNVIC_TABLE进行初始化，将中断处理程序指针赋值
//进main函数前调用
void Interrupt_Init(void)
{
    tNVIC_TABLE.pHardFault_Handler = HardFault_Processing;
    tNVIC_TABLE.pTIM1_BRK_UP_TRG_COM_Handler = TIM1_Processing;
}
