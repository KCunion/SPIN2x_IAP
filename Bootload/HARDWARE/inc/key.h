#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY1    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)         //读取按键1
#define KEY2    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)         //读取按键2 
#define KEY3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)        //读取按键3
#define KEY4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)        //读取按键4

#define KEY_NONE    0       //无按键按下
#define KEY1_PRES	1		//KEY1 
#define KEY2_PRES	2		//KEY2  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4

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

