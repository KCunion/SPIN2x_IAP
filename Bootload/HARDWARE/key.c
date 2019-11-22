#include "key.h"
#include "delay.h"
#include "module.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);     //使能PORTA,PORTB,PORTC时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;                      //PA0,K2（KEY2）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                   //设置成下拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化GPIOA0

    GPIO_InitStructure.GPIO_Pin  = (GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11);     //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //PA0设置成上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化GPIOB.10,11
}

#define KEY_SCAN_FSM_RESET() \
do {\
    s_tKeyCB.tState = START;\
} while(0)
uint8_t Key_Scan(uint8_t chMode)
{
    static struct {
        enum {
            START = 0,
            PRESS,
            DELAY,
            PRESS_AGAIN
        } tState;
        bool bLoosenFlag;               //按键按松开标志 Loosen
        uint16_t hwDelay;               //按键延时消抖循环次数
    } s_tKeyCB = {START,false,0};

    if (chMode) {
        s_tKeyCB.bLoosenFlag = true;    //支持连按
    }

    switch (s_tKeyCB.tState) {
        case START:
            s_tKeyCB.hwDelay = 0X4FFF;  //设置延时消抖次数
            s_tKeyCB.tState = PRESS;
            //break;
        case PRESS:
            if ( s_tKeyCB.bLoosenFlag && 
                (KEY1 == 1 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)) {
                s_tKeyCB.tState = DELAY;        //按键被按下，进入延时消抖状态
            } else if (KEY1 == 0 && KEY3 == 1 && KEY4 == 1 && KEY2 == 1) {
                s_tKeyCB.bLoosenFlag = true;    //按键已被松开
            }
            break;
        case DELAY:                     //延时消抖
            if (-- s_tKeyCB.hwDelay < 1) {
                s_tKeyCB.tState = PRESS_AGAIN;
            }
            break;
        case PRESS_AGAIN:               //延时消抖后再次检测按键是否被按下
            s_tKeyCB.bLoosenFlag = false;
            KEY_SCAN_FSM_RESET();       //复位状态机
            if (KEY1 == 1) {
                return KEY1_PRES;
            }
            else if (KEY2 == 0) {
                return KEY2_PRES;
            }
            else if (KEY3 == 0) {
                return KEY3_PRES;
            }
            else if (KEY4 == 0) {
                return KEY4_PRES;
            }
            break;
    }
    return KEY_NONE;                       //无按键按下
}
