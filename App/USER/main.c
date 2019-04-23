#include "interrupt_tab.h"
#include "delay.h"
#include "sys.h"
#include "led.h"

//define�жϴ�����ָ���б������0X20000000
//��Interrupt_Init()�ж�tNVIC_TABLE�ṹ����г�ʼ��
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
//TIM�жϴ������
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
//��tNVIC_TABLE���г�ʼ�������жϴ������ָ�븳ֵ
//��main����ǰ����
void Interrupt_Init(void)
{
    tNVIC_TABLE.pHardFault_Handler = HardFault_Processing;
    tNVIC_TABLE.pTIM1_BRK_UP_TRG_COM_Handler = TIM1_Processing;
}
