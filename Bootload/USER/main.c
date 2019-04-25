#include "sys.h"
#include "uart.h"
#include "key.h"
#include "led.h"
#include "flash_op.h"
#include "iap.h"
#include "module.h"
#include "interrupt_tab.h"
/******************************************************************************************
*       程序功能：                IAP Demo
*       芯片型号：               MM32SPIN27
*       版本号：                    1.0
*       作者：                     Shawn
*       注意事项：
*       1、本例程仅作为IAP功能参考演示，不得承担因本例程直接或间接导致的任何后果！
*       2、本例程针对MM32 M0所作：由于读保护下RAM访问FLASH受限，因此读保护后将中断向量表拷贝
*          到RAM有风险，而本例程不拷贝中断向量表，而是在中断服务函数中跳转到中断处理函数，中
*          断处理函数指针存放于RAM起始地址0X20000000，跳转到APP函数后修改函数指针以达到切换中
*          断处理函数的目的，细节参考interrupt_tab.h和process.c。
*          M3有SCB，不需要从中断服务函数中跳转到中断处理函数的操作。
*       3、APP目前没有提供SRAM程序，只有FLASH代码。
*       4、读保护部分代码仅适用于，MM32SPIN2x/05系列，MM32F103_n版本，
*                                  MM32F031_n/q版本,MM32L0xx系列，MM32L37x系列
*       5、移植注意事项：
*           a、修改interrupt_tab.h中的NVIC_TABLE_t结构体，中断处理程序列表和所移植芯片一致。
*           b、启动文件中添加Interrupt_Init()的声明和调用。
*           以上两个步骤在BootLoad和App都需要修改。
*           c、keil中配置RAM起始地址需要把NVIC_TABLE_t的内存预留出来
********************************************************************************************/
//define中断处理函数指针列表，存放于0X20000000
//在process.c中编写，并在Interrupt_Init()中对tNVIC_TABLE结构体进行初始化
struct NVIC_TABLE_t tNVIC_TABLE __attribute__((at(0X20000000)));

struct {
    uint32_t wAppAddr;
    uint16_t hwAppLenth;
    uint8_t chKeyVal;
    event_t tReceivedEvent;
    bool bProtectFlag;
}static s_tAppControlBlock = {
    .wAppAddr                   = 0x08010000,               //APP 地址
    .hwAppLenth                 = 0,                        //APP大小
    .chKeyVal                   = 0,                        //按键键值
    .tReceivedEvent.bAutoReset  = AUTO,                     //事件接收后自动复位
    .tReceivedEvent.bIsSet      = RESET,                    //无事件
    .bProtectFlag               = true,                     //是否打开读保护    false
};

static fsm_rt_t Received_App(void);                     //判断APP代码是否接收完成
static void Upgrade_Firmware(event_t *ptReceived);      //更新固件程序
static void Run_FlashCode(void);                        //运行FLASH代码
static void Run_SramCode(void);                         //运行SRAM代码

int main()
{
    Led_Init();
    Key_Init();
    Uart1_Init(9600);
    if (false != s_tAppControlBlock.bProtectFlag ) {
        Flash_ReadOut_Protection();                     //打开读保护
    }
    printf("等待下发用户应用程序!\r\n");
    while (1) {
        Breath_Led();                                   //LED呼吸灯程序

        if (fsm_rt_cpl == Received_App()) {                 //检查APP是否接收完成
            SET_EVENT(&s_tAppControlBlock.tReceivedEvent);  //发送ReceivedEvent事件
        }

        s_tAppControlBlock.chKeyVal = Key_Scan(0);          //检查按键按下并返回键值
        
        switch (s_tAppControlBlock.chKeyVal) {
            case WKUP_PRES:
                Upgrade_Firmware(&s_tAppControlBlock.tReceivedEvent);   //更新固件程序，子函数中等待ReceivedEvent事件
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
            s_tReceived.chDelay = 0XFF;                 //初始化delay次数
            //break;
        case RECEIVED:
            if (g_hwUartRxCnt) {                        //UART开始接收数据
                s_tReceived.hwLastCount = g_hwUartRxCnt;
                s_tReceived.tState = DELAY;
            }
            break;
        case DELAY:
            if (-- s_tReceived.chDelay < 1) {           //延时等待接收完成
                s_tReceived.tState = REC_COMPLETE;
            }
            break;
        case REC_COMPLETE:
            if (s_tReceived.hwLastCount == g_hwUartRxCnt) { //接收完成(延时结束后UART接收计数未变化)
                APP_RECEIVED_FSM_RESET();
                s_tAppControlBlock.hwAppLenth = g_hwUartRxCnt;
                s_tReceived.hwLastCount = 0;
                g_hwUartRxCnt = 0;
                printf("用户程序接收完成!\r\n");
                printf("代码长度:%dBytes\r\n", s_tAppControlBlock.hwAppLenth);
                return fsm_rt_cpl;                          //状态机已完成
            } else {
                s_tReceived.hwLastCount = g_hwUartRxCnt;    //接收未完成(延时结束后UART接收计数发生变化)
                s_tReceived.chDelay = 0XFF;                 //重新进入延时等待
                s_tReceived.tState = DELAY;
            }
            break;
    }
    return fsm_rt_on_going;                                 //状态机运行中
}

static void Upgrade_Firmware(event_t *ptReceived)
{
    if (NULL != ptReceived)
    {
        if (WAIT_EVENT(ptReceived)) {                       //等待ptReceived事件
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
        printf("应用程序错误,无法执行!\r\n");               //代码执行成功不会返回到此位置
    } else {
        printf("非FLASH应用程序,无法执行!\r\n");
    }
}

static void Run_SramCode(void)
{
    printf("开始执行SRAM用户代码!!\r\n");
    if (((*(__IO uint32_t *)(0X20000600 + 4)) & 0xFF000000) == 0x20000000) {
        Iap_LoadApp(0X20000600);                            //SRAM地址
        printf("应用程序错误,无法执行!\r\n");               //代码执行成功不会返回到此位置
    } else {
        printf("非SRAM应用程序,无法执行!\r\n");
    }
}
