#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "flash_op.h"
#include "iap.h"

iapfun_t tJumpToApp;

void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize)
{
    uint8_t chEraseNumber = wAppSize / FLASH_SECTOR_SIZE;   //������Ҫ����ҳ��ʵ��ҳ�� - 1��
    if (NULL != pchAppBuf) {
        FLASH_Unlock();                                         //����
        do {
            FLASH_ErasePage(wAppxAddr + chEraseNumber * FLASH_SECTOR_SIZE);     //����APP����ʹ�õ�ַҳ���������
            if (0 == chEraseNumber) {
                break;
            }
            chEraseNumber --;
        }while(1);
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);    //д��APP����
        FLASH_Lock();                                           //����
    }
}

void Iap_LoadApp(uint32_t wAppxAddr)
{
    if (((*(__IO uint32_t *)wAppxAddr) & 0x2FFE0000) == 0x20000000) {       //���ջ����ַ�Ƿ�Ϸ�.
        tJumpToApp = (iapfun_t) * (__IO uint32_t *)(wAppxAddr + 4);         //�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
        MSR_MSP(*(__IO uint32_t *)wAppxAddr);                               //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
        tJumpToApp();                                                       //��ת��APP.
    }
}
