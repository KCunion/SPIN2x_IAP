#include "key.h"
#include "delay.h"
#include "module.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);     //ʹ��PORTA,PORTB,PORTCʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;                      //PA0,K2��KEY2��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                   //���ó���������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                          //��ʼ��GPIOA0

    GPIO_InitStructure.GPIO_Pin  = (GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11);     //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //PA0���ó���������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                          //��ʼ��GPIOB.10,11
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
        bool bLoosenFlag;               //�������ɿ���־ Loosen
        uint16_t hwDelay;               //������ʱ����ѭ������
    } s_tKeyCB = {START,false,0};

    if (chMode) {
        s_tKeyCB.bLoosenFlag = true;    //֧������
    }

    switch (s_tKeyCB.tState) {
        case START:
            s_tKeyCB.hwDelay = 0X4FFF;  //������ʱ��������
            s_tKeyCB.tState = PRESS;
            //break;
        case PRESS:
            if ( s_tKeyCB.bLoosenFlag && 
                (KEY1 == 1 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)) {
                s_tKeyCB.tState = DELAY;        //���������£�������ʱ����״̬
            } else if (KEY1 == 0 && KEY3 == 1 && KEY4 == 1 && KEY2 == 1) {
                s_tKeyCB.bLoosenFlag = true;    //�����ѱ��ɿ�
            }
            break;
        case DELAY:                     //��ʱ����
            if (-- s_tKeyCB.hwDelay < 1) {
                s_tKeyCB.tState = PRESS_AGAIN;
            }
            break;
        case PRESS_AGAIN:               //��ʱ�������ٴμ�ⰴ���Ƿ񱻰���
            s_tKeyCB.bLoosenFlag = false;
            KEY_SCAN_FSM_RESET();       //��λ״̬��
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
    return KEY_NONE;                       //�ް�������
}
