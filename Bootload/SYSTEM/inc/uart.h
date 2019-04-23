#ifndef __UART_H
#define __UART_H
#include "HAL_conf.h"
#include "stdio.h"
#include "dtype.h"
		   
#define UART_REC_LEN            (10*1024)        //定义最大接收字节数 10K

extern uint8_t  g_chUartRxBuf[UART_REC_LEN];      //接收缓冲,最大UART_REC_LEN个字节.末字节为换行符
extern uint16_t g_hwUartRxCnt;                    //接收的字节数

/**
* @brief  This function will initialize the UART.
* @param wBaudRrate: Configuring UART baud rate.
	According to the data UartTX & UartRX
* @retval : None
*/
extern void Uart1_Init(uint32_t wBaudRrate);
/**
* @brief  This function will send a character.
* @param chByte: This character will be sent.
* @retval : None
*/
extern void Uart_PutChar(char chByte);
/**
* @brief  This function will send a string.
* @param pchBuff: The string of this address will be sent.
* @param wLen: The length of the string to be sent.
* @retval : None
*/
extern void Uart_PutBuff(uint8_t *buff, uint32_t len);
/**
* @brief  VCAN Uart serial port assistant interface function.
* @param pWareaddr: The array data address to be sent.
* @param wWwaresize: The size of the array data to be sent.
* @retval : None
*/
extern void Send_Ware(void *wareaddr, uint32_t waresize);

#endif
