#include "uart.h" 

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    while((UART1->CSR&UART_IT_TXIEN)==0);
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}
#else
#pragma import(__use_no_semihosting)             
            
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;       

_sys_exit(int x) 
{ 
    x = x; 
} 

int fputc(int ch, FILE *f)
{      
    while((UART1->CSR&UART_IT_TXIEN)==0);
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}
#endif


uint8_t g_chUartRxBuf[UART_REC_LEN]   __attribute__((at(0X20000600)));    //接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20000600.
uint16_t g_hwUartRxCnt = 0;                                                   //接收到的Bytes


void Uart1_Init(uint32_t wBaudRrate)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    do {
        NVIC_InitTypeDef NVIC_InitStruct;

        NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStruct.NVIC_IRQChannelPriority = 2;
        NVIC_Init(& NVIC_InitStruct);
    } while(0);
    
    do {
        UART_InitTypeDef UART_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
        UART_InitStructure.UART_BaudRate = wBaudRrate;
        UART_InitStructure.UART_WordLength = UART_WordLength_8b;
        UART_InitStructure.UART_StopBits = UART_StopBits_1;
        UART_InitStructure.UART_Parity = UART_Parity_No;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
        UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
        UART_Init(UART1, &UART_InitStructure);
        
        UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
//        UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        UART_Cmd(UART1, ENABLE);
    } while(0);
    
    do {
        GPIO_InitTypeDef GPIO_InitStructure;
        //UART_TXInit
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //UART_RXInit
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    } while(0);
}

void Uart_PutChar (char chByte)
{
    while((UART1->CSR&UART_IT_TXIEN)==0);
    UART1->TDR = (chByte & (uint16_t)0x00FF);    
}

void Uart_PutBuff (uint8_t *pchBuff, uint32_t wLen)
{
    while (wLen--) {
        Uart_PutChar(*pchBuff);
        pchBuff++;
    }
}

void Send_Ware(void *pWareaddr, uint32_t wWwaresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};

    Uart_PutBuff(cmdf, sizeof(cmdf));
    Uart_PutBuff((uint8_t *)pWareaddr, wWwaresize);
    Uart_PutBuff(cmdr, sizeof(cmdr));
}
