#ifndef __FLASH_OP_H__
#define __FLASH_OP_H__
#include "sys.h"  
#include "stdbool.h"

//从指定地址读出半字
//wFlashAddr:读取地址
extern uint16_t Flash_ReadHalfWord(uint32_t wFlashAddr);
//不检查的写入
//wWriteAddr:起始地址
//phwBuffer:数据指针
//hwNumber:半字(16位)数
extern void Flash_Write_NoCheck(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber);
//检查写入是否成功，成功返回true,不成功返回false
//wWriteAddr:起始地址
//phwBuffer:数据指针
//hwNumber:半字(16位)数
extern bool Flash_Write_Check(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber);
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//phwBuffer:数据指针
//NumToWrite:半字(16位)数
extern void Flash_Read(uint32_t ReadAddr, uint16_t *phwBuffer, uint16_t NumToRead);
//FLASH 选项字节半字编程操作
//wAddress：选项字节地址
//hwData：要写入的数据
FLASH_Status Flash_ProgramOptionHalfWord(uint32_t wAddress, uint16_t hwData);
//使能整片读保护
extern void Flash_ReadOut_Protection(void);
#endif
