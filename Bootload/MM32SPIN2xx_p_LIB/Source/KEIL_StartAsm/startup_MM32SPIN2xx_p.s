;******************** (C) COPYRIGHT 2018 MindMotion ********************
; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                   ; Top of Stack
                DCD     Reset_Handler                  ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     MemManage_Handler              ; Memory Manage Handler
                DCD     BusFault_Handler               ; Bus Fault Handler
                DCD     UsageFault_Handler             ; Usage Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     DebugMonitor_Handler           ; Debug Monitor Handler
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

               


; External Interrupts
                DCD     WWDG_IRQHandler                ; Window Watchdog
                DCD     PVD_IRQHandler                 ; PVD through EXTI Line detect
                DCD     PWM_IRQHandler                 ; Commutation and input error interrupt
                DCD     FLASH_IRQHandler               ; FLASH
                DCD     RCC_IRQHandler                 ; RCC & CRS
                DCD     EXTI0_1_IRQHandler             ; EXTI Line 0 and 1
                DCD     EXTI2_3_IRQHandler             ; EXTI Line 2 and 3
                DCD     EXTI4_15_IRQHandler            ; EXTI Line 4 to 15
                DCD	    HWDIV_IRQHandler               ; HWDIV
                DCD     DMA1_Channel1_IRQHandler       ; DMA1 Channel 1
                DCD     DMA1_Channel2_3_IRQHandler     ; DMA1 Channel 2 and Channel 3
                DCD     DMA1_Channel4_5_IRQHandler     ; DMA1 Channel 4 and Channel 5
                DCD     ADC1_IRQHandler                ; ADC1
                DCD     TIM1_BRK_UP_TRG_COM_IRQHandler ; TIM1 Break, Update, Trigger and Commutation
                DCD     TIM1_CC_IRQHandler             ; TIM1 Capture Compare
                DCD     TIM2_IRQHandler                ; TIM2
                DCD     TIM3_IRQHandler                ; TIM3
                DCD     TIM8_BRK_UP_TRG_COM_IRQHandler ; TIM8 Brake, Update, Trigger and Commutation
                DCD     TIM8_CC_IRQHandler             ; TIM8 Capture Compare
                DCD     TIM14_IRQHandler               ; TIM14
                DCD     ADC2_IRQHandler                ; ADC2
                DCD     TIM16_IRQHandler               ; TIM16
                DCD     TIM17_IRQHandler               ; TIM17
                DCD     I2C1_IRQHandler                ; I2C1
                DCD     COMP1_2_3_4_5_IRQHandler       ; Comparators
                DCD     SPI1_IRQHandler                ; SPI1
                DCD     SPI2_IRQHandler                ; SPI2
                DCD     UART1_IRQHandler               ; UART1 
                DCD     UART2_IRQHandler               ; UART2
				
                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                EXPORT  Reset_Handler                 [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
                IMPORT  Interrupt_Init



                LDR     R0, =__initial_sp          ; set stack pointer 
                MSR     MSP, R0  

;;Check if boot space corresponds to test memory 

                LDR R0,=0x00000004
                LDR R1, [R0]
                LSRS R1, R1, #24
                LDR R2,=0x1F
                CMP R1, R2
        
                BNE ApplicationStart  
     
ApplicationStart    
                LDR R0,=0x4002101C 
                LDR R1,=0x04000000
                STR R1, [R0]

                LDR R0,=0x40006818 
                LDR R1,=0x00000012
                STR R1, [R0]

                LDR R0,=0x4000681C 
                LDR R1,=0x00000012
                STR R1, [R0]

                LDR R0,=0x4002101C 
                LDR R1,=0x00000000
                STR R1, [R0]
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =Interrupt_Init
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

APPADDR_NMI_Handler                       EQU      0x08010008   
APPADDR_HardFault_Handler                 EQU      0x0801000C   
APPADDR_SVC_Handler                       EQU      0x0801002C   
APPADDR_PendSV_Handler                    EQU      0x08010038 
APPADDR_SysTick_Handler                   EQU      0x0801003C 

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                LDR R0, = APPADDR_NMI_Handler
                LDR R1,[R0]
                BX      R1
                ;B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                LDR R0, = APPADDR_HardFault_Handler
                LDR R1,[R0]
                BX      R1
                ;B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler              [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler               [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler             [WEAK]
                B       .
                ENDP
SVC_Handler  PROC
                EXPORT  SVC_Handler                    [WEAK]
                LDR R0, = APPADDR_SVC_Handler
                LDR R1,[R0]
                BX      R1
                ;B       .
                ENDP
DebugMonitor_Handler\
                PROC
                EXPORT  DebugMonitor_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                LDR R0, = APPADDR_PendSV_Handler
                LDR R1,[R0]
                BX      R1
                ;B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                LDR R0, = APPADDR_SysTick_Handler
                LDR R1,[R0]
                BX      R1
                ;B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                [WEAK]
                EXPORT  PVD_IRQHandler                 [WEAK]
                EXPORT  PWM_IRQHandler            	   [WEAK]
                EXPORT  FLASH_IRQHandler               [WEAK]
                EXPORT  RCC_IRQHandler                 [WEAK]
                EXPORT  EXTI0_1_IRQHandler             [WEAK]
                EXPORT  EXTI2_3_IRQHandler             [WEAK]
                EXPORT  EXTI4_15_IRQHandler            [WEAK]
                EXPORT  HWDIV_IRQHandler               [WEAK]
                EXPORT  DMA1_Channel1_IRQHandler       [WEAK]
                EXPORT  DMA1_Channel2_3_IRQHandler     [WEAK]
                EXPORT  DMA1_Channel4_5_IRQHandler     [WEAK]
                EXPORT  ADC1_IRQHandler                [WEAK]
                EXPORT  TIM1_BRK_UP_TRG_COM_IRQHandler [WEAK]
                EXPORT  TIM1_CC_IRQHandler             [WEAK]
                EXPORT  TIM2_IRQHandler                [WEAK]
                EXPORT  TIM3_IRQHandler                [WEAK]
                EXPORT  TIM8_BRK_UP_TRG_COM_IRQHandler [WEAK]
                EXPORT  TIM8_CC_IRQHandler             [WEAK]
                EXPORT  TIM14_IRQHandler               [WEAK]
                EXPORT  ADC2_IRQHandler                [WEAK]
                EXPORT  TIM16_IRQHandler               [WEAK]
                EXPORT  TIM17_IRQHandler               [WEAK]
                EXPORT  I2C1_IRQHandler                [WEAK]
                EXPORT  COMP1_2_3_4_5_IRQHandler       [WEAK]
                EXPORT  SPI1_IRQHandler                [WEAK]
                EXPORT  SPI2_IRQHandler                [WEAK]
                EXPORT  UART1_IRQHandler               [WEAK]
                EXPORT  UART2_IRQHandler               [WEAK]




WWDG_IRQHandler               
PVD_IRQHandler                
PWM_IRQHandler           
FLASH_IRQHandler              
RCC_IRQHandler                
EXTI0_1_IRQHandler            
EXTI2_3_IRQHandler            
EXTI4_15_IRQHandler           
HWDIV_IRQHandler              
DMA1_Channel1_IRQHandler      
DMA1_Channel2_3_IRQHandler    
DMA1_Channel4_5_IRQHandler    
ADC1_IRQHandler               
TIM1_BRK_UP_TRG_COM_IRQHandler
TIM1_CC_IRQHandler            
TIM2_IRQHandler               
TIM3_IRQHandler               
TIM8_BRK_UP_TRG_COM_IRQHandler
TIM8_CC_IRQHandler            
TIM14_IRQHandler              
ADC2_IRQHandler               
TIM16_IRQHandler              
TIM17_IRQHandler              
I2C1_IRQHandler               
COMP1_2_3_4_5_IRQHandler      
SPI1_IRQHandler               
SPI2_IRQHandler               
UART1_IRQHandler              
UART2_IRQHandler              



                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;******************** (C) COPYRIGHT 2018 MindMotion ********************
