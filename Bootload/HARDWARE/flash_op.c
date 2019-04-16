#include "flash_op.h"
#include "delay.h"
#include "uart.h"

uint16_t Flash_ReadHalfWord(uint32_t wFlashAddr)
{
    return *(__IO uint16_t *)wFlashAddr;
}

void Flash_Read(uint32_t ReadAddr, uint16_t *phwBuffer, uint16_t NumToRead)
{
    uint16_t i;
    if (NULL == phwBuffer) {
        return;
    }
    for (i = 0; i < NumToRead; i++) {
        phwBuffer[i] = Flash_ReadHalfWord(ReadAddr);    //读取2个字节.
        ReadAddr += 2;                                  //偏移2个字节.
    }
}

void Flash_Write_NoCheck(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber)
{
    uint16_t i;
    if (NULL == phwBuffer) {
        return;
    }
    for (i = 0; i < hwNumber; i++) {
        FLASH_ProgramHalfWord(wWriteAddr, phwBuffer[i]);
        wWriteAddr += 2;                                    //地址增加2.
    }
    
}

bool Flash_Write_Check(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber)
{
    uint16_t i;
    if (NULL == phwBuffer) {
        return false;
    }
    for (i = 0; i < hwNumber; i++) {
        FLASH_ProgramHalfWord(wWriteAddr, phwBuffer[i]);
        if (Flash_ReadHalfWord(wWriteAddr) != phwBuffer[i]) {
            return false;
        }
        wWriteAddr += 2;                                    //地址增加2.
    }
    return true;
}

FLASH_Status Flash_ProgramOptionHalfWord(uint32_t wAddress, uint16_t hwData)
{
    FLASH_Status tStatus = FLASH_COMPLETE;
    tStatus = FLASH_WaitForLastOperation(0x00000FFF);
    if (tStatus == FLASH_COMPLETE) {
        /* Authorize the small information block programming */
        FLASH->OPTKEYR = 0x45670123;
        FLASH->OPTKEYR = 0xCDEF89AB;
        if (0xffff != *(u16 *)(wAddress)) {
            return FLASH_ERROR_PG;
        }
        FLASH->CR &= 0x00001FEF;
        /* Enables the Option Bytes Programming operation */
        FLASH->CR |= 0x00000010;
        *(__IO uint16_t *)(wAddress) = hwData;
        /* Wait for last operation to be completed */
        tStatus = FLASH_WaitForLastOperation(0x0000000F);
        if (tStatus != FLASH_COMPLETE) {
            if (tStatus != FLASH_BUSY) {
                    /* if the program operation is completed, disable the OPTPG Bit */
                    FLASH->CR &= 0x00001FEF;
                    /* Enables the Option Bytes Programming operation */
            }
        }
    }
    /* Return the protection operation Status */
    return tStatus;
}

void Flash_ReadOut_Protection(void)
{
    FLASH_Unlock();
    Flash_ProgramOptionHalfWord(0x1FFE0000, 0x7F80);
    Flash_ProgramOptionHalfWord(0x1FFE0002, 0xFF00);
    FLASH_Lock();
}
