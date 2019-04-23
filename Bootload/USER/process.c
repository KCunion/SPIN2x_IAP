#include "process.h"

void HardFault_Processing(void)
{

}

void TIM1_Processing(void)
{

}
//UART1中断处理程序
void UART1_Processing(void)
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
//对tNVIC_TABLE进行初始化，将中断处理程序指针赋值
//进main函数前调用
void Interrupt_Init(void)
{
    tNVIC_TABLE.pUART1_Handler = UART1_Processing;
}
