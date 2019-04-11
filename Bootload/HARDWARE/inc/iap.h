#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  

//用户根据自己的需要设置
#define FLASH_SIZE          (128) 	 		//所选芯片的FLASH容量大小(单位为K)
#define FLASH_SECTOR_SIZE   (1024)          //字节
#define FLASH_BASE_ADDR     (0x08000000) 	//FLASH的起始地址

typedef  void (*iapfun_t)(void);            //定义一个函数类型的参数.   
   
//应用程序代码写入
//wAppxAddr:应用程序的起始地址
//pchAppBuf:应用程序CODE.
//wAppSize:应用程序大小(字节).
extern void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize);
//跳转到应用程序段
//wAppxAddr:用户代码起始地址.
extern void Iap_LoadApp(uint32_t wAppxAddr);

#endif
