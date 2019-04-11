#include "led.h"

#define ABS(__N)    ((__N) < 0 ? -(__N) : (__N))
#define _BV(__N)    ((uint32_t)1<<(__N))
#define TOP         (0x02FF)

void Led_Init(void)
{ 
    RCC->AHBENR |= 0x3 << 17;  //开启GPIOA,GPIOB时钟

	GPIOA->CRH &= 0x0fffffff;
    GPIOA->CRH |= GPIO_CRH_MODE15;         //PA15 推挽输出   	 
    GPIOA->ODR |= GPIO_ODR_ODR15;          //PA15 输出高
    
	GPIOB->CRL &= 0xff000fff;
    GPIOB->CRL |= GPIO_CRL_MODE3|GPIO_CRL_MODE4|GPIO_CRL_MODE5;      ////PB3,PB4,PB5推挽输出
    GPIOB->ODR |= GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5;          ////PB.3,PB4,PB5输出高 
    
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}


/*! \brief set the 16-level led gradation
 *! \param hwLevel gradation
 *! \return none
 */
static void Set_Led_Gradation(uint16_t hwLevel)
{
    static uint16_t s_hwCounter = 0;

    if (hwLevel >= s_hwCounter) {
        LED2_ON();
    } else {
        LED2_OFF();
    }

    s_hwCounter++;
    s_hwCounter &= TOP;
}
 
void Breath_Led(void)
{
    static uint16_t s_hwCounter = 0;
    static int16_t s_nGray = (TOP >> 1);
    
    s_hwCounter++;
    if (!(s_hwCounter & (_BV(10)-1))) {
        s_nGray++; 
        if (s_nGray == TOP) {
            s_nGray = 0;
        }
    }
    Set_Led_Gradation(ABS(s_nGray - (TOP >> 1)));
}
