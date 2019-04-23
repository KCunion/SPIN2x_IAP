#include "mm32spin2x_it.h"
#include "interrupt_tab.h"
//中断服务函数，跳转到实际中断处理程序
void WWDG_IRQHandler(void)
{
    tNVIC_TABLE.pWWDG_Handler();
}    
void PVD_IRQHandler(void)
{
    tNVIC_TABLE.pPVD_Handler();
}
void PWM_IRQHandler(void)
{
    tNVIC_TABLE.pPWM_Handler();
}
void FLASH_IRQHandler(void)
{
    tNVIC_TABLE.pFLASH_Handler();
}
void RCC_IRQHandler(void)
{
    tNVIC_TABLE.pRCC_Handler();
}
void EXTI0_1_IRQHandler(void)
{
    tNVIC_TABLE.pEXTI0_1_Handler();
}
void EXTI2_3_IRQHandler(void)
{
    tNVIC_TABLE.pEXTI2_3_Handler();
}
void EXTI4_15_IRQHandler(void)
{
    tNVIC_TABLE.pEXTI4_15_Handler();
}
void HWDIV_IRQHandler(void)
{
    tNVIC_TABLE.pHWDIV_Handler();
}
void DMA1_Channel1_IRQHandler(void)
{
    tNVIC_TABLE.pDMA1_Channel1_Handler();
}
void DMA1_Channel2_3_IRQHandler(void)
{
    tNVIC_TABLE.pDMA1_Channel2_3_Handler();
}
void DMA1_Channel4_5_IRQHandler(void)
{
    tNVIC_TABLE.pDMA1_Channel4_5_Handler();
}
void ADC1_IRQHandler(void)
{
    tNVIC_TABLE.pADC1_Handler();
}
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    tNVIC_TABLE.pTIM1_BRK_UP_TRG_COM_Handler();
}
void TIM1_CC_IRQHandler(void)
{
    tNVIC_TABLE.pTIM1_CC_Handler();
}
void TIM2_IRQHandler(void)
{
    tNVIC_TABLE.pTIM2_Handler();
}
void TIM3_IRQHandler(void)
{
    tNVIC_TABLE.pTIM3_Handler();
}
void TIM8_BRK_UP_TRG_COM_IRQHandler(void)
{
    tNVIC_TABLE.pTIM8_BRK_UP_TRG_COM_Handler();
}
void TIM8_CC_IRQHandler(void)
{
    tNVIC_TABLE.pTIM8_CC_Handler();
}
void TIM14_IRQHandler(void)
{
    tNVIC_TABLE.pTIM14_Handler();
}
void ADC2_IRQHandler(void)
{
    tNVIC_TABLE.pADC2_Handler();
}
void TIM16_IRQHandler(void)
{
    tNVIC_TABLE.pTIM16_Handler();
}
void TIM17_IRQHandler(void)
{
    tNVIC_TABLE.pTIM17_Handler();
}
void I2C1_IRQHandler(void)
{
    tNVIC_TABLE.pI2C1_Handler();
}
void COMP1_2_3_4_5_IRQHandler(void)
{
    tNVIC_TABLE.pCOMP1_2_3_4_5_Handler();
}
void SPI1_IRQHandler(void)
{
    tNVIC_TABLE.pSPI1_Handler();
}
void SPI2_IRQHandler(void)
{
    tNVIC_TABLE.pSPI2_Handler();
}
void UART1_IRQHandler(void)
{
    tNVIC_TABLE.pUART1_Handler();
}
void UART2_IRQHandler(void)
{
    tNVIC_TABLE.pUART2_Handler();
}
