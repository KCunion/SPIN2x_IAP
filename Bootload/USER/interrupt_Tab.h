#ifndef __INTERRUPT_TAB_H
#define __INTERRUPT_TAB_H
//中断处理函数指针表，在中断服务函数中调用函数指针
struct NVIC_TABLE_t{
    void (*pinitial_sp);
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pMemManage_Handler)(void);
    void (*pBusFault_Handler)(void);
    void (*pUsageFault_Handler)(void);
    void (*RESERVED1)(void);
    void (*RESERVED2)(void);
    void (*RESERVED3)(void);
    void (*RESERVED4)(void);
    void (*pSVC_Handler)(void);
    void (*pDebugMonitor_Handler)(void);
    void (*RESERVED5)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);

    void (*pWWDG_Handler)(void);
    void (*pPVD_Handler)(void);
    void (*pPWM_Handler)(void);
    void (*pFLASH_Handler)(void);
    void (*pRCC_Handler)(void);
    void (*pEXTI0_1_Handler)(void);
    void (*pEXTI2_3_Handler)(void);
    void (*pEXTI4_15_Handler)(void);
    void (*pHWDIV_Handler)(void);
    void (*pDMA1_Channel1_Handler)(void);
    void (*pDMA1_Channel2_3_Handler)(void);
    void (*pDMA1_Channel4_5_Handler)(void);
    void (*pADC1_Handler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_Handler)(void);
    void (*pTIM1_CC_Handler)(void);
    void (*pTIM2_Handler)(void);
    void (*pTIM3_Handler)(void);
    void (*pTIM8_BRK_UP_TRG_COM_Handler)(void);
    void (*pTIM8_CC_Handler)(void);
    void (*pTIM14_Handler)(void);
    void (*pADC2_Handler)(void);
    void (*pTIM16_Handler)(void);
    void (*pTIM17_Handler)(void);
    void (*pI2C1_Handler)(void);
    void (*pCOMP1_2_3_4_5_Handler)(void);
    void (*pSPI1_Handler)(void);
    void (*pSPI2_Handler)(void);
    void (*pUART1_Handler)(void);
    void (*pUART2_Handler)(void);
    void (*RESERVED6)(void);
    void (*RESERVED7)(void);
    void (*RESERVED8)(void);
};
extern struct NVIC_TABLE_t tNVIC_TABLE;
#endif
