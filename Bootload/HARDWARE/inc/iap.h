#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  

//�û������Լ�����Ҫ����
#define FLASH_SIZE          (128) 	 		//��ѡоƬ��FLASH������С(��λΪK)
#define FLASH_SECTOR_SIZE   (1024)          //�ֽ�
#define FLASH_BASE_ADDR     (0x08000000) 	//FLASH����ʼ��ַ

typedef  void (*iapfun_t)(void);            //����һ���������͵Ĳ���.   
   
//Ӧ�ó������д��
//wAppxAddr:Ӧ�ó������ʼ��ַ
//pchAppBuf:Ӧ�ó���CODE.
//wAppSize:Ӧ�ó����С(�ֽ�).
extern void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize);
//��ת��Ӧ�ó����
//wAppxAddr:�û�������ʼ��ַ.
extern void Iap_LoadApp(uint32_t wAppxAddr);

#endif
