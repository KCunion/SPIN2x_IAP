#include "uart.h" 
#include "interrupt_tab.h"

//UART1�жϴ������
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
//��tNVIC_TABLE���г�ʼ�������жϴ������ָ�븳ֵ
//�������ļ�����������Reset_Handler�е��ã�����λ����main����ǰ
void Interrupt_Init(void)
{
    tNVIC_TABLE.pUART1_Handler = UART1_Processing;
}
