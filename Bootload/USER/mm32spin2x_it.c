#include "mm32spin2x_it.h"
/*******************************************************************************
* @name   : UART1_IRQHandler
* @brief  : UART1 interrupt handler 
* @param  : void
* @retval : void
*******************************************************************************/
void UART1_IRQHandler(void)
{
    uint8_t chResByte;
    if (UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) {
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        chResByte = UART_ReceiveData(UART1);
        if (g_hwUartRxCnt < UART_REC_LEN) {
            g_chUartRxBuf[g_hwUartRxCnt] = chResByte;
            g_hwUartRxCnt++;
        }
    }
}
