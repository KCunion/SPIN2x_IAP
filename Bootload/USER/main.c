#include "interrupt_tab.h"
#include "flash_op.h"
#include "module.h"
#include "uart.h"
#include "iap.h"
#include "sys.h"
#include "key.h"
#include "led.h"
//define�жϴ�����ָ���б������0X20000000
//��process.c�б�д������Interrupt_Init()�ж�tNVIC_TABLE�ṹ����г�ʼ��
struct NVIC_TABLE_t tNVIC_TABLE __attribute__((at(0X20000000)));

struct {
    uint32_t wAppAddr;
    uint16_t hwAppLenth;
    uint8_t chKeyVal;
    event_t tReceivedEvent;
    bool bProtectFlag;
}static s_tAppControlBlock = {
    .wAppAddr                   = 0x08010000,               //APP ��ַ
    .hwAppLenth                 = 0,                        //APP��С
    .chKeyVal                   = 0,                        //������ֵ
    .tReceivedEvent.bAutoReset  = AUTO,                     //�¼����պ��Զ���λ
    .tReceivedEvent.bIsSet      = RESET,                    //���¼�
    .bProtectFlag               = true,                     //�Ƿ�򿪶�����    false
};

static fsm_rt_t Received_App(void);                     //�ж�APP�����Ƿ�������
static void Upgrade_Firmware(event_t *ptReceived);      //���¹̼�����
static void Run_FlashCode(void);                        //����FLASH����
static void Run_SramCode(void);                         //����SRAM����

int main()
{
    Led_Init();
    Key_Init();
    Uart1_Init(9600);
    if (false != s_tAppControlBlock.bProtectFlag ) {
        Flash_ReadOut_Protection();                     //�򿪶�����
    }
    printf("�ȴ��·��û�Ӧ�ó���!\r\n");
    while (1) {
        Breath_Led();                                   //LED�����Ƴ���

        if (fsm_rt_cpl == Received_App()) {                 //���APP�Ƿ�������
            SET_EVENT(&s_tAppControlBlock.tReceivedEvent);  //����ReceivedEvent�¼�
        }

        s_tAppControlBlock.chKeyVal = Key_Scan(0);          //��鰴�����²����ؼ�ֵ
        
        switch (s_tAppControlBlock.chKeyVal) {
            case WKUP_PRES:
                Upgrade_Firmware(&s_tAppControlBlock.tReceivedEvent);   //���¹̼������Ӻ����еȴ�ReceivedEvent�¼�
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
            s_tReceived.chDelay = 0XFF;                 //��ʼ��delay����
            //break;
        case RECEIVED:
            if (g_hwUartRxCnt) {                        //UART��ʼ��������
                s_tReceived.hwLastCount = g_hwUartRxCnt;
                s_tReceived.tState = DELAY;
            }
            break;
        case DELAY:
            if (-- s_tReceived.chDelay < 1) {           //��ʱ�ȴ��������
                s_tReceived.tState = REC_COMPLETE;
            }
            break;
        case REC_COMPLETE:
            if (s_tReceived.hwLastCount == g_hwUartRxCnt) { //�������(��ʱ������UART���ռ���δ�仯)
                APP_RECEIVED_FSM_RESET();
                s_tAppControlBlock.hwAppLenth = g_hwUartRxCnt;
                s_tReceived.hwLastCount = 0;
                g_hwUartRxCnt = 0;
                printf("�û�����������!\r\n");
                printf("���볤��:%dBytes\r\n", s_tAppControlBlock.hwAppLenth);
                return fsm_rt_cpl;                          //״̬�������
            } else {
                s_tReceived.hwLastCount = g_hwUartRxCnt;    //����δ���(��ʱ������UART���ռ��������仯)
                s_tReceived.chDelay = 0XFF;                 //���½�����ʱ�ȴ�
                s_tReceived.tState = DELAY;
            }
            break;
    }
    return fsm_rt_on_going;                                 //״̬��������
}

static void Upgrade_Firmware(event_t *ptReceived)
{
    if (NULL != ptReceived)
    {
        if (WAIT_EVENT(ptReceived)) {                       //�ȴ�ptReceived�¼�
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
