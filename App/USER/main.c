#include "interrupt_tab.h"
//#include "delay.h"
#include "sys.h"
#include "led.h"

//define�жϴ�����ָ���б������0X20000000
//��Interrupt_Init()�ж�tNVIC_TABLE�ṹ����г�ʼ��
struct NVIC_TABLE_t tNVIC_TABLE __attribute__((at(0X20000000)));

void Tim_Init(uint16_t hwPrescaler,uint16_t hwPeriod);

static __IO uint32_t TimingDelay;

extern u32 SystemCoreClock;
/*******************************************************************************
**������Ϣ ��delay_init()                 
**�������� ��systick��ʱ������ʼ��
**������� ����
**������� ����
*******************************************************************************/
void delay_init()
{
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick�ж����ȼ�����
}
/*******************************************************************************
**������Ϣ ��delay_ms(__IO uint32_t nTime)                     
**�������� ������Ӧ�õ�����ʱ��ʹ��systick
**������� ��nTime����ʱ
**������� ����
*******************************************************************************/
void delay_ms(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;
    
    while(TimingDelay != 0);
}

int main()
{
    Led_Init();
    delay_init();
    Tim_Init(4799,5000);
    while (1) {
//        Breath_Led();
        LED2_TOGGLE();
        delay_ms(500);
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


void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}

/*******************************************************************************
**������Ϣ ��SysTick_Handler(void)                    
**�������� ��������жϺ�����Systick���еݼ�
**������� ����
**������� ����
*******************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

//��tNVIC_TABLE���г�ʼ�������жϴ������ָ�븳ֵ
//��main����ǰ����
void Interrupt_Init(void)
{
   tNVIC_TABLE.pTIM1_BRK_UP_TRG_COM_Handler = TIM1_Processing;
}
