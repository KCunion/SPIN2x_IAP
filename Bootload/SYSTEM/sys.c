#include "sys.h"

void System_Clock_Init(uint8_t chPLL)
{
    
}	

//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

