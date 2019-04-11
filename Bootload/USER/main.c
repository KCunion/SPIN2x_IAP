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

static fsm_rt_t Received_App(void);                     //判断APP代码是否接收完成
static void Upgrade_Firmware(event_t *ptReceived);      //更新固件程序
static void Run_FlashCode(void);                        //运行FLASH代码
static void Run_SramCode(void);                         //运行SRAM代码

int main()
{
//    delay_init();
    Led_Init();
    Key_Init();
    Uart1_Init(9600);
    printf("等待下发用户应用程序!\r\n");
    while (1) {
        Breath_Led();

        if (fsm_rt_cpl == Received_App()) {
            SET_EVENT(&s_tAppControlBlock.tReceivedEvent);
        }

        s_tAppControlBlock.chKeyVal = Key_Scan(0);          //检查按键按下并返回键值
        
        switch (s_tAppControlBlock.chKeyVal) {
            case WKUP_PRES:
                Upgrade_Firmware(&s_tAppControlBlock.tReceivedEvent);   //更新固件程序
                break;
            case KEY3_PRES:
                Run_FlashCode();                                        //运行FLASH代码
                break;
            case KEY4_PRES:
                Run_SramCode();                                         //运行SRAM代码
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
                printf("用户程序接收完成!\r\n");
                printf("代码长度:%dBytes\r\n", s_tAppControlBlock.hwAppLenth);
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
            printf("开始更新固件...\r\n");
            if (((*(__IO uint32_t *)(0X20000600 + 4)) & 0xFF000000) == 0x08000000) {        //判断是否为0X08XXXXXX.
                Iap_WriteAppBin(s_tAppControlBlock.wAppAddr, g_chUartRxBuf,
                                s_tAppControlBlock.hwAppLenth);                             //更新FLASH代码
                printf("固件更新完成!\r\n");
            } else {
                printf("非FLASH应用程序!\r\n");
            }
        } else {
            printf("没有可以更新的固件!\r\n");
        }
    }
}

static void Run_FlashCode(void)
{
    printf("开始执行FLASH用户代码!\r\n");
    if (((*(__IO uint32_t *)(s_tAppControlBlock.wAppAddr + 4)) & 0xFF000000) == 0x08000000) {   //判断是否为0X08XXXXXX.
        Iap_LoadApp(s_tAppControlBlock.wAppAddr);           //执行FLASH APP代码
        printf("应用程序错误,无法执行!\r\n");
    } else {
        printf("非FLASH应用程序,无法执行!\r\n");
    }
}

static void Run_SramCode(void)
{
    printf("开始执行SRAM用户代码!!\r\n");
    if (((*(__IO uint32_t *)(0X20000600 + 4)) & 0xFF000000) == 0x20000000) {
        Iap_LoadApp(0X20000600);                            //SRAM地址
        printf("应用程序错误,无法执行!\r\n");
    } else {
        printf("非SRAM应用程序,无法执行!\r\n");
    }
}
