#include "delay.h"
#include "led.h"

#define USE_SYSTICK_DELAY 1

#if USE_SYSTICK_DELAY

extern uint32_t SystemCoreClock;

static uint32_t	s_wDelay_us = 0;							//us Time delay multiplier
static uint32_t	s_wDelay_ms = 0;							//ms Time delay multiplier
/**
* @brief  This function will be used to initialize SysTick.
* @param : None
* @retval : None
*/
void delay_init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);    //Select external clock HCLK
    s_wDelay_us = SystemCoreClock / 1000000;            //1/8 of the system clock
    //Represents the number of systick clocks required for each ms
    s_wDelay_ms = (uint16_t)s_wDelay_us * 1000;	
}
/**
* @brief  Use SysTick as the us delay function.
* @param nus: Setting the us delay time.
* @retval : None
*/
void delay_us(uint32_t wNus)
{
    uint32_t temp;
    SysTick ->LOAD = wNus * s_wDelay_us;         //Time loading
    SysTick ->VAL = 0x00;                       //Empty counter
    SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk;  //Start counting down
    do {
        temp = SysTick ->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));		//Waiting time to arrive
    SysTick ->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//Close counter
    SysTick ->VAL = 0X00;      					//Empty counter
}
/**
* @brief  Use SysTick as the us delay function.
* @param nms: Setting the ms delay time.
	Note the range of nms
	SysTick->LOAD is a 24-bit register, so the maximum delay is:
	nms<=0xffffff*1000/SYSCLK
	SYSCLK unit is Hz, nms is in ms
	For 48M  HCLK conditions, nms<=349
* @retval : None
*/
void delay_ms(uint32_t wNms)
{
    uint32_t temp;
    SysTick ->LOAD = (uint32_t)wNms * s_wDelay_ms;	//Time loading(SysTick->LOADΪ24bit)
    SysTick ->VAL = 0x00;						//Empty counter
    SysTick ->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//Start counting down  
    do {
        temp = SysTick ->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));		//Waiting time to arrive   
    SysTick ->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//Close counter
    SysTick ->VAL = 0X00;       				//Empty counter	  	    
}
#else

void delay_us(__IO uint32_t wNus)
{
    uint32_t i = 0;
    while (wNus --) {
        i = 10;
        while (i --);
    }
}

void delay_ms(__IO uint32_t wNms)
{
    uint32_t i = 0;
    while(wNms --) {
        i = 3100;
        while(i --);
    }
}
#endif

