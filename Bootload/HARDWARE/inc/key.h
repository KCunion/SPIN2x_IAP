#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY1    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)         //��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)          //��ȡ����2 
#define KEY3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)         //��ȡ����3
#define KEY4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)         //��ȡ����4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY1 
#define KEY4_PRES	4		//KEY1

//������ʼ������
extern void Key_Init(void);//IO��ʼ��
//����������������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP����
//KEY3_PRES��KEY3����
//KEY4_PRES��KEY4����
extern uint8_t Key_Scan(uint8_t chMode);

#endif

