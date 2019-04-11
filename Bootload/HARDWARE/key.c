#include "key.h"
#include "delay.h"
#include "module.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE); //使能PORTA,PORTB,PORTC时钟
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PC13，K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC13

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0,K2（WK_UP）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11; //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10,11
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
        uint8_t chKeyUp;
        uint16_t hwDelay;
    } s_tKeyCB = {START,1,0};

    if (chMode) {
        s_tKeyCB.chKeyUp = 1;                     //支持连按
    }

    switch (s_tKeyCB.tState) {
        case START:
            s_tKeyCB.hwDelay = 0X4FFF;
            s_tKeyCB.tState = PRESS;
            //break;
        case PRESS:
            if (s_tKeyCB.chKeyUp && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0)) {
                s_tKeyCB.tState = DELAY;
            } else if (KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0) {
                s_tKeyCB.chKeyUp = 1;
            }
            break;
        case DELAY:
            if (-- s_tKeyCB.hwDelay < 1) {
                s_tKeyCB.tState = PRESS_AGAIN;
            }
            break;
        case PRESS_AGAIN:
            KEY_SCAN_FSM_RESET();
            if (KEY1 == 0) {
                return KEY1_PRES;
            }
            else if (WK_UP == 1) {
                return WKUP_PRES;
            }
            else if (KEY3 == 0) {
                return KEY3_PRES;
            }
            else if (KEY4 == 0) {
                return KEY4_PRES;
            }
            break;
    }
    return 0;                       // 无按键按下
}
