#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY1    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)         //读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)          //读取按键2 
#define KEY3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)         //读取按键3
#define KEY4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)         //读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY1 
#define KEY4_PRES	4		//KEY1

//按键初始化函数
extern void Key_Init(void);//IO初始化
//非阻塞按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下
//KEY3_PRES，KEY3按下
//KEY4_PRES，KEY4按下
extern uint8_t Key_Scan(uint8_t chMode);

#endif

