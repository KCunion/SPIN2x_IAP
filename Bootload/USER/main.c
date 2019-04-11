#include "delay.h"
#include "sys.h"
#include "led.h"
#include "uart.h"
#include "key.h"
#include "iap.h"
#include "module.h"

static struct {
    uint32_t wAppAddr;
    uint16_t hwAppLenth;
    event_t tReceivedEvent;
    uint8_t chKeyVal;
} s_tAppControlBlock = {
    0x08010000,
    0,
    AUTO,RESET,
    0
};

static fsm_rt_t Received_App(void);                     //�ж�APP�����Ƿ�������
static void Upgrade_Firmware(event_t *ptReceived);      //���¹̼�����
static void Run_FlashCode(void);                        //����FLASH����
static void Run_SramCode(void);                         //����SRAM����

int main()
{
//    delay_init();
    Led_Init();
    Key_Init();
    Uart1_Init(9600);
    printf("�ȴ��·��û�Ӧ�ó���!\r\n");
    while (1) {
        Breath_Led();

        if (fsm_rt_cpl == Received_App()) {
            SET_EVENT(&s_tAppControlBlock.tReceivedEvent);
        }

        s_tAppControlBlock.chKeyVal = Key_Scan(0);          //��鰴�����²����ؼ�ֵ
        
        switch (s_tAppControlBlock.chKeyVal) {
            case WKUP_PRES:
                Upgrade_Firmware(&s_tAppControlBlock.tReceivedEvent);   //���¹̼�����
                break;
            case KEY3_PRES:
                Run_FlashCode();                                        //����FLASH����
                break;
            case KEY4_PRES:
                Run_SramCode();                                         //����SRAM����
                break;
        }
    }
}

#define APP_RECEIVED_FSM_RESET() \
do {\
    s_tReceived.tState = START;\
} while(0)
static fsm_rt_t Received_App(void)
{
    static struct {
        enum {
            START = 0,
            RECEIVED,
            DELAY,
            REC_COMPLETE
        } tState;
        uint8_t chDelay;
        uint16_t hwLastCount;
    }s_tReceived = {START,0,0};
    switch (s_tReceived.tState) {
        case START:
            s_tReceived.tState = RECEIVED;
            s_tReceived.chDelay = 255;
            //break;
        case RECEIVED:
            if (g_hwUartRxCnt) {
                s_tReceived.hwLastCount = g_hwUartRxCnt;
                s_tReceived.tState = DELAY;
            }
            break;
        case DELAY:
            if (-- s_tReceived.chDelay < 1) {
                s_tReceived.tState = REC_COMPLETE;
            }
            break;
        case REC_COMPLETE:
            if (s_tReceived.hwLastCount == g_hwUartRxCnt) {
                APP_RECEIVED_FSM_RESET();
                s_tAppControlBlock.hwAppLenth = g_hwUartRxCnt;
                s_tReceived.hwLastCount = 0;
                g_hwUartRxCnt = 0;
                printf("�û�����������!\r\n");
                printf("���볤��:%dBytes\r\n", s_tAppControlBlock.hwAppLenth);
                return fsm_rt_cpl;
            } else {
                s_tReceived.hwLastCount = g_hwUartRxCnt;
                s_tReceived.chDelay = 255;
                s_tReceived.tState = DELAY;
            }
            break;
    }
    return fsm_rt_on_going;
}

static void Upgrade_Firmware(event_t *ptReceived)
{
    if (NULL != ptReceived)
    {
        if (WAIT_EVENT(ptReceived)) {
            printf("��ʼ���¹̼�...\r\n");
            if (((*(__IO uint32_t *)(0X20000600 + 4)) & 0xFF000000) == 0x08000000) {        //�ж��Ƿ�Ϊ0X08XXXXXX.
                Iap_WriteAppBin(s_tAppControlBlock.wAppAddr, g_chUartRxBuf,
                                s_tAppControlBlock.hwAppLenth);                             //����FLASH����
                printf("�̼��������!\r\n");
            } else {
                printf("��FLASHӦ�ó���!\r\n");
            }
        } else {
            printf("û�п��Ը��µĹ̼�!\r\n");
        }
    }
}

static void Run_FlashCode(void)
{
    printf("��ʼִ��FLASH�û�����!\r\n");
    if (((*(__IO uint32_t *)(s_tAppControlBlock.wAppAddr + 4)) & 0xFF000000) == 0x08000000) {   //�ж��Ƿ�Ϊ0X08XXXXXX.
        Iap_LoadApp(s_tAppControlBlock.wAppAddr);           //ִ��FLASH APP����
        printf("Ӧ�ó������,�޷�ִ��!\r\n");
    } else {
        printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
    }
}

static void Run_SramCode(void)
{
    printf("��ʼִ��SRAM�û�����!!\r\n");
    if (((*(__IO uint32_t *)(0X20000600 + 4)) & 0xFF000000) == 0x20000000) {
        Iap_LoadApp(0X20000600);                            //SRAM��ַ
        printf("Ӧ�ó������,�޷�ִ��!\r\n");
    } else {
        printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
    }
}
