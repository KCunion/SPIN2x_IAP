#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "flash_op.h"
#include "iap.h"

iapfun_t tJumpToApp;

void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize)
{
    uint8_t chEraseNumber = wAppSize / FLASH_SECTOR_SIZE;   //计算需要擦除页（实际页数 - 1）
    if (NULL != pchAppBuf) {
        FLASH_Unlock();                                         //解锁
        do {
            FLASH_ErasePage(wAppxAddr + chEraseNumber * FLASH_SECTOR_SIZE);     //擦除APP代码使用地址页，倒序擦除
            if (0 == chEraseNumber) {
                break;
            }
            chEraseNumber --;
        }while(1);
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);    //写入APP代码
        FLASH_Lock();                                           //上锁
    }
}

void Iap_LoadApp(uint32_t wAppxAddr)
{
    if (((*(__IO uint32_t *)wAppxAddr) & 0x2FFE0000) == 0x20000000) {       //检查栈顶地址是否合法.
        tJumpToApp = (iapfun_t) * (__IO uint32_t *)(wAppxAddr + 4);         //用户代码区第二个字为程序开始地址(复位地址)
        MSR_MSP(*(__IO uint32_t *)wAppxAddr);                               //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        tJumpToApp();                                                       //跳转到APP.
    }
}
