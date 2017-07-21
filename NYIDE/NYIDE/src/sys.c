/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "nx1_lib.h"
#include "nx1_smu.h"
#include "nx1_rtc.h"
#include "nx1_spi.h"
#include "nx1_uart.h"
#include "nx1_timer.h"
#include "nx1_intc.h"
#include "nx1_gpio.h"
#include "nx1_wdt.h"
#include "nx1_pwm.h"
#include "nx1_ir.h"

/* Defines -------------------------------------------------------------------*/
#if  _SYS_TIMEBASE==125
#define SYSTICK_CLOCK               8000//HZ
#elif _SYS_TIMEBASE==250
#define SYSTICK_CLOCK               RTC_4KHZ
#elif _SYS_TIMEBASE==1000
#define SYSTICK_CLOCK               RTC_1KHZ
#else
#error _SYS_TIMEBASE undefined
#endif

#define RANDOM_AI                   41
#define RANDOM_CI                   7

/* Static Variables ----------------------------------------------------------*/
static const U8 LibVer[]="NX1-V1.03-Beta";//20170703

#if _RANDOM_GEN
static U32 Getrandom_x0,Getrandom_firstdata;
static U32 Getrandom_mi;
#endif

extern U8 _end;//bss end

#if _USE_HEAP_RAM
volatile U16 _stack_size;
volatile U16 _ram_size;
#endif

/* Global Variables ----------------------------------------------------------*/
const U16 BitOpTab[]={0x0001,0x0002,0x0004,0x0008,
                      0x0010,0x0020,0x0040,0x0080,
                      0x0100,0x0200,0x0400,0x0800, 
                      0x1000,0x2000,0x4000,0x8000};
                      
//------------------------------------------------------------------//
// Generate random
// Parameter: 
//          mi: input max value
// return value:
//          Random value
//------------------------------------------------------------------// 
#if _RANDOM_GEN   
U32 Random(U32 mi)
{   
    U8 x=0;
    if(Getrandom_mi!=mi){
        Getrandom_mi=mi;
        Getrandom_x0++;
        x=1;
    }
    Getrandom_x0=((RANDOM_AI*Getrandom_x0+RANDOM_CI)%mi);//outbuf[x]=xi_1;//x0=xi_1;
   
    if(x==1){
        Getrandom_firstdata =Getrandom_x0;
    }else {
        if(Getrandom_firstdata==Getrandom_x0){
            Getrandom_mi=0;
            return Random(mi);
        }
    }
    return Getrandom_x0;  
}
#endif
//------------------------------------------------------------------//
// PWM Init in sys.c
// Parameter: 
//          none
// return value:
//          NONE 
//------------------------------------------------------------------//
void SYS_PWMInit(void)
{
    SMU_PeriphClkCmd(SMU_PERIPH_PWM,ENABLE);

    //Each Module setting
#if _PWMA_MODULE
    TIMER_Init(PWMATIMER,TIMER_CLKDIV_32,999);
    TIMER_Cmd(PWMATIMER,ENABLE);
#if _PWMA_INT
    TIMER_IntCmd(PWMATIMER,ENABLE);
    INTC_IrqCmd(IRQ_BIT_PWMA,ENABLE);
#endif
#endif

#if _PWMB_MODULE
    TIMER_Init(PWMBTIMER,TIMER_CLKDIV_32,999);
    TIMER_Cmd(PWMBTIMER,ENABLE);
#if _PWMB_INT
    TIMER_IntCmd(PWMBTIMER,ENABLE);
    INTC_IrqCmd(IRQ_BIT_PWMB,ENABLE);
#endif
#endif

#if _CAPTURE_MODULE
    TIMER_Init(PWMATIMER,TIMER_CLKDIV_32,999);
    TIMER_Cmd(PWMATIMER,ENABLE);
    PWM_CapturePinSelect(_CAPTURE_PAx);    
#if _CAPTURE_INT    
    PWM_CaptureIntCmd(CAP_RISING_FALLING,ENABLE);
    INTC_IrqCmd(IRQ_BIT_CAPTURE,ENABLE);
#endif
#endif

    //Each IO output setting    
#if  _PWMA_MODULE
#if _PWMA0_DUTY!=0xFF
    GPIO_Init(GPIOA,PWMA0_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMA,PWMx0,PWM_START_HIGH,_PWMA0_DUTY);
#endif
#if _PWMA1_DUTY!=0xFF
    GPIO_Init(GPIOA,PWMA1_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMA,PWMx1,PWM_START_HIGH,_PWMA1_DUTY);
#endif
#if _PWMA2_DUTY!=0xFF
    GPIO_Init(GPIOA,PWMA2_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMA,PWMx2,PWM_START_HIGH,_PWMA2_DUTY);
#endif
#if _PWMA3_DUTY!=0xFF
    GPIO_Init(GPIOA,PWMA3_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMA,PWMx3,PWM_START_HIGH,_PWMA3_DUTY);
#endif
#endif
#if _PWMB_MODULE
#if _PWMB0_DUTY!=0xFF
    GPIO_Init(GPIOB,PWMB0_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMB,PWMx0,PWM_START_HIGH,_PWMB0_DUTY);
#endif
#if _PWMB1_DUTY!=0xFF
    GPIO_Init(GPIOB,PWMB1_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMB,PWMx1,PWM_START_HIGH,_PWMB1_DUTY);
#endif
#if _PWMB2_DUTY!=0xFF
    GPIO_Init(GPIOB,PWMB2_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMB,PWMx2,PWM_START_HIGH,_PWMB2_DUTY);
#endif
#if _PWMB3_DUTY!=0xFF
    GPIO_Init(GPIOB,PWMB3_PIN,GPIO_MODE_ALT);
    PWM_InitDuty(PWMB,PWMx3,PWM_START_HIGH,_PWMB3_DUTY);
#endif
#endif
} 
//------------------------------------------------------------------//
// Timer Init in sys.c
// Parameter: 
//          none
// return value:
//          NONE 
//------------------------------------------------------------------//
void SYS_TIMERInit(void)
{
    SMU_PeriphClkCmd(SMU_PERIPH_TMR,ENABLE);
#if _TIMER_TMR0_INT
    TIMER_IntCmd(TIMER0,ENABLE);
    INTC_IrqCmd(IRQ_BIT_TMR0,ENABLE);
#endif
#if _TIMER_TMR1_INT
    TIMER_IntCmd(TIMER1,ENABLE);
    INTC_IrqCmd(IRQ_BIT_TMR1,ENABLE);
#endif
#if _TIMER_TMR2_INT
    TIMER_IntCmd(TIMER2,ENABLE);
    INTC_IrqCmd(IRQ_BIT_TMR2,ENABLE);
#endif    
}
//------------------------------------------------------------------//
// GPIO Init in sys.c
// Parameter: 
//          none
// return value:
//          NONE 
//------------------------------------------------------------------//
void SYS_GPIOInit(void)
{
#if _GPIO_PA0_MODE!=0xFF
    GPIO_Init(GPIOA, 0, _GPIO_PA0_MODE);
#endif
#if _GPIO_PA1_MODE!=0xFF
    GPIO_Init(GPIOA, 1, _GPIO_PA1_MODE);
#endif
#if _GPIO_PA2_MODE!=0xFF
    GPIO_Init(GPIOA, 2, _GPIO_PA2_MODE);
#endif
#if _GPIO_PA3_MODE!=0xFF
    GPIO_Init(GPIOA, 3, _GPIO_PA3_MODE);
#endif
#if _GPIO_PA4_MODE!=0xFF
    GPIO_Init(GPIOA, 4, _GPIO_PA4_MODE);
#endif
#if _GPIO_PA5_MODE!=0xFF
    GPIO_Init(GPIOA, 5, _GPIO_PA5_MODE);
#endif
#if _GPIO_PA6_MODE!=0xFF
    GPIO_Init(GPIOA, 6, _GPIO_PA6_MODE);
#endif
#if _GPIO_PA7_MODE!=0xFF
    GPIO_Init(GPIOA, 7, _GPIO_PA7_MODE);
#endif
#if _GPIO_PA8_MODE!=0xFF
    GPIO_Init(GPIOA, 8, _GPIO_PA8_MODE);
#endif
#if _GPIO_PA9_MODE!=0xFF
    GPIO_Init(GPIOA, 9, _GPIO_PA9_MODE);
#endif
#if _GPIO_PA10_MODE!=0xFF
    GPIO_Init(GPIOA,10, _GPIO_PA10_MODE);
#endif
#if _GPIO_PA11_MODE!=0xFF
    GPIO_Init(GPIOA,11, _GPIO_PA11_MODE);
#endif
#if _GPIO_PA12_MODE!=0xFF
    GPIO_Init(GPIOA,12, _GPIO_PA12_MODE);
#endif
#if _GPIO_PA13_MODE!=0xFF
    GPIO_Init(GPIOA,13, _GPIO_PA13_MODE);
#endif
#if _GPIO_PA14_MODE!=0xFF
    GPIO_Init(GPIOA,14, _GPIO_PA14_MODE);
#endif
#if _GPIO_PA15_MODE!=0xFF
    GPIO_Init(GPIOA,15, _GPIO_PA15_MODE);
#endif
#if _GPIO_PB0_MODE!=0xFF
    GPIO_Init(GPIOB, 0, _GPIO_PB0_MODE);
#endif
#if _GPIO_PB1_MODE!=0xFF
    GPIO_Init(GPIOB, 1, _GPIO_PB1_MODE);
#endif
#if _GPIO_PB2_MODE!=0xFF
    GPIO_Init(GPIOB, 2, _GPIO_PB2_MODE);
#endif
#if _GPIO_PB3_MODE!=0xFF
    GPIO_Init(GPIOB, 3, _GPIO_PB3_MODE);
#endif
#if _GPIO_PB4_MODE!=0xFF
    GPIO_Init(GPIOB, 4, _GPIO_PB4_MODE);
#endif
#if _GPIO_PB5_MODE!=0xFF
    GPIO_Init(GPIOB, 5, _GPIO_PB5_MODE);
#endif
#if _GPIO_PB6_MODE!=0xFF
    GPIO_Init(GPIOB, 6, _GPIO_PB6_MODE);
#endif
#if _GPIO_PB7_MODE!=0xFF
    GPIO_Init(GPIOB, 7, _GPIO_PB7_MODE);
#endif
#if _GPIO_PB8_MODE!=0xFF
    GPIO_Init(GPIOB, 8, _GPIO_PB8_MODE);
#endif
#if _GPIO_PB9_MODE!=0xFF
    GPIO_Init(GPIOB, 9, _GPIO_PB9_MODE);
#endif
#if _GPIO_PB10_MODE!=0xFF
    GPIO_Init(GPIOB,10, _GPIO_PB10_MODE);
#endif
#if _GPIO_PB11_MODE!=0xFF
    GPIO_Init(GPIOB,11, _GPIO_PB11_MODE);
#endif
#if _GPIO_PB12_MODE!=0xFF
    GPIO_Init(GPIOB,12, _GPIO_PB12_MODE);
#endif
#if _GPIO_PB13_MODE!=0xFF
    GPIO_Init(GPIOB,13, _GPIO_PB13_MODE);
#endif
#if _GPIO_PB14_MODE!=0xFF
    GPIO_Init(GPIOB,14, _GPIO_PB14_MODE);
#endif
#if _GPIO_PB15_MODE!=0xFF
    GPIO_Init(GPIOB,15, _GPIO_PB15_MODE);
#endif
#if _GPIO_PC0_MODE!=0xFF
    GPIO_Init(GPIOC, 0, _GPIO_PC0_MODE);
#endif
#if _GPIO_PC1_MODE!=0xFF
    GPIO_Init(GPIOC, 1, _GPIO_PC1_MODE);
#endif
#if _GPIO_PC2_MODE!=0xFF
    GPIO_Init(GPIOC, 2, _GPIO_PC2_MODE);
#endif
#if _GPIO_PC3_MODE!=0xFF
    GPIO_Init(GPIOC, 3, _GPIO_PC3_MODE);
#endif
#if _GPIO_PC4_MODE!=0xFF
    GPIO_Init(GPIOC, 4, _GPIO_PC4_MODE);
#endif
#if _GPIO_PC5_MODE!=0xFF
    GPIO_Init(GPIOC, 5, _GPIO_PC5_MODE);
#endif
#if _GPIO_PC6_MODE!=0xFF
    GPIO_Init(GPIOC, 6, _GPIO_PC6_MODE);
#endif
#if _GPIO_PC7_MODE!=0xFF
    GPIO_Init(GPIOC, 7, _GPIO_PC7_MODE);
#endif
}
//------------------------------------------------------------------//
// Module initialization
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void SYS_ModuleInit(void)
{
    volatile U8 sts;
    
    //Default Initial
    SMU_PeriphClkCmd(0xFFFFFFFF,DISABLE);
    INTC_IrqCmd(0xFFFFFFFF,DISABLE);
    INTC_ClrPending(0xFFFFFFFF);

    SMU_PeriphClkCmd(SMU_PERIPH_GPIO,ENABLE);
    //**** IP Selection by nx1_config.h ****//
    //Module Initial
#if _GPIO_MODULE
    SYS_GPIOInit();
#endif

#if _TIMER_MODULE || _AUDIO_MODULE || _PWMA_MODULE || _PWMB_MODULE
    SYS_TIMERInit();
#endif

#if (_SYS_TIMEBASE!=0) || _RTC_MODULE    
    SMU_PeriphClkCmd(SMU_PERIPH_RTC,ENABLE);
#endif

    SMU_CpuClock(_SYS_SMU_CPU_CLK_DIV,SMU_SYSCLK_FROM_HICLK);

#if _SYS_SMU_LVD_VOLTAGE==0xFF
    SMU_LVDCmd(DISABLE,0);
#else
    SMU_LVDCmd(ENABLE,_SYS_SMU_LVD_VOLTAGE);
#endif

#if _SPI_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_SPI,ENABLE);
    GPIO_Init(GPIOB,SPI0_CEN_MTF_PIN,GPIO_MODE_ALT);
    GPIO_Init(GPIOB,SPI0_SCLK_MTF_PIN,GPIO_MODE_ALT);
#if  (_SPI_ACCESS_MODE==SPI_1_4_4_MODE) || (_XIP_IO_MODE==XIP_1_IO_MODE) || (_XIP_IO_MODE==XIP_2_IO_MODE) || (_XIP_IO_MODE==XIP_4_IO_MODE)      
    GPIO_Init(GPIOB,SPI0_IO2_MTF_PIN,GPIO_MODE_ALT);    
#endif
    SMU_LDO33Cmd(ENABLE);
    SMU_PeriphReset(SMU_PERIPH_SPI);
    //Add 15ms delay for booting Flash safety.
#if OPTION_HIGH_FREQ_CLK==24000000
	DelayLoop(42);
#elif OPTION_HIGH_FREQ_CLK==16000000
	DelayLoop(27);
#elif OPTION_HIGH_FREQ_CLK==12000000
	DelayLoop(21);
#else//32M
	DelayLoop(54);
#endif     

#endif    

#if _AUDIO_MODULE
#if _VR_FUNC||_SBC_RECORD || _ADPCM_RECORD || _PCM_RECORD || _RT_PLAY
    SMU_PeriphClkCmd(SMU_PERIPH_ADC,ENABLE);
    ADC_MicBiasCmd(ENABLE);
#endif
    AudioCtlInit();
    AUDIO_Init(); 
#endif

#if _DIRECTKEY_MODULE
    DIRECTKEY_Init();  
#endif

#if _SYS_TIMEBASE!=0
    SysTickInit(SYSTICK_CLOCK);
#endif
    
#if	_I2C_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_I2C,ENABLE);
    GPIO_Init(GPIOA,I2C_SDA_MTF_PIN,GPIO_MODE_ALT);
    I2C_Init(_I2C_MASTER_ADDRESS);
#endif

#if _IR_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_IR,ENABLE);
    GPIO_Init(GPIOA,IR_TX_PIN,GPIO_MODE_ALT);
    IR_Init(IR_CLKDIV_15,IR_STOP_LOW,C_IR_CNT_Div_28);    
#endif

#if _PWMA_MODULE || _PWMB_MODULE || _CAPTURE_MODULE
    SYS_PWMInit();
#endif
#if _UART_MODULE
	SMU_PeriphClkCmd(SMU_PERIPH_UART,ENABLE);
    //UartCtlInit(UART_BAUDRATE_115200);
#endif
    
    GIE_ENABLE();

#if _SPI_ACCESS_MODE==SPI_1_4_4_MODE    
    sts=SPI_ReadStsReg(2);
    if(sts!=0x2){//QEMode Enable
        SPI_WriteStsReg2(0x200);        
    }
#endif

#if (_XIP_IO_MODE==XIP_1_IO_MODE) || (_XIP_IO_MODE==XIP_2_IO_MODE) || (_XIP_IO_MODE==XIP_4_IO_MODE)
    SPI_XipIoMode(_XIP_IO_MODE);
#endif


#if _RANDOM_GEN 
    Getrandom_x0=2;
    Getrandom_firstdata=0;
    Getrandom_mi=0;
#endif 
#if _SYS_WDT_TIME==1
    WDT_Init(WDT_TIME_188ms);
#else
    WDT_Init(WDT_TIME_750ms);
#endif
}
//------------------------------------------------------------------//
// System initialization
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void SYS_Init(void)
{
    SYS_ModuleInit();
    DelayLoop2(50);
    WDT_Clear();
}   
//------------------------------------------------------------------//
// Module initialization
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void SYS_ModuleInit2(void)
{
    volatile U8 sts;
    
    //Default Initial
    ////SMU_PeriphClkCmd(0xFFFFFFFF,DISABLE);
    ////INTC_IrqCmd(0xFFFFFFFF,DISABLE);
    ////INTC_ClrPending(0xFFFFFFFF);

    ////SMU_PeriphClkCmd(SMU_PERIPH_GPIO,ENABLE);
    //**** IP Selection by nx1_config.h ****//
    //Module Initial
#if _GPIO_MODULE
    SYS_GPIOInit();
#endif

#if _TIMER_MODULE || _AUDIO_MODULE || _PWMA_MODULE || _PWMB_MODULE
    SYS_TIMERInit();
#endif

#if (_SYS_TIMEBASE!=0) || _RTC_MODULE    
    SMU_PeriphClkCmd(SMU_PERIPH_RTC,ENABLE);
#endif

    SMU_CpuClock(_SYS_SMU_CPU_CLK_DIV,SMU_SYSCLK_FROM_HICLK);

#if _SYS_SMU_LVD_VOLTAGE==0xFF
    SMU_LVDCmd(DISABLE,0);
#else
    SMU_LVDCmd(ENABLE,_SYS_SMU_LVD_VOLTAGE);
#endif

#if _SPI_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_SPI,ENABLE);
    GPIO_Init(GPIOB,SPI0_CEN_MTF_PIN,GPIO_MODE_ALT);
    GPIO_Init(GPIOB,SPI0_SCLK_MTF_PIN,GPIO_MODE_ALT);
    GPIO_Init(GPIOB,SPI0_IO0_MTF_PIN,GPIO_MODE_ALT);
    GPIO_Init(GPIOB,SPI0_IO1_MTF_PIN,GPIO_MODE_ALT);
    GPIO_Init(GPIOB,SPI0_IO2_MTF_PIN,GPIO_MODE_ALT);    
    SMU_LDO33Cmd(ENABLE);
    SMU_PeriphReset(SMU_PERIPH_SPI);
#endif    

#if _AUDIO_MODULE
#if _VR_FUNC||_SBC_RECORD || _ADPCM_RECORD || _PCM_RECORD || _RT_PLAY
    SMU_PeriphClkCmd(SMU_PERIPH_ADC,ENABLE);
    ADC_MicBiasCmd(ENABLE);
#endif
    AudioCtlInit();
    AUDIO_Init(); 
#endif

#if _DIRECTKEY_MODULE
    DIRECTKEY_Init();  
#endif

#if _SYS_TIMEBASE!=0
    SysTickInit(SYSTICK_CLOCK);
#endif
    
#if	_I2C_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_I2C,ENABLE);
    GPIO_Init(GPIOA,I2C_SDA_MTF_PIN,GPIO_MODE_ALT);
    I2C_Init(_I2C_MASTER_ADDRESS);
#endif

#if _IR_MODULE
    SMU_PeriphClkCmd(SMU_PERIPH_IR,ENABLE);
    GPIO_Init(GPIOA,IR_TX_PIN,GPIO_MODE_ALT);
    IR_Init(IR_CLKDIV_15,IR_STOP_LOW,C_IR_CNT_Div_28);    
#endif

#if _PWMA_MODULE || _PWMB_MODULE || _CAPTURE_MODULE
    SYS_PWMInit();
#endif
#if _UART_MODULE
	SMU_PeriphClkCmd(SMU_PERIPH_UART,ENABLE);
    //UartCtlInit(UART_BAUDRATE_115200);
#endif
    
    GIE_ENABLE();

#if _SPI_ACCESS_MODE==SPI_1_4_4_MODE    
    sts=SPI_ReadStsReg(2);
    if(sts!=0x2){//QEMode Enable
        SPI_WriteStsReg2(0x200);        
    }
#endif

#if (_XIP_IO_MODE==XIP_1_IO_MODE) || (_XIP_IO_MODE==XIP_2_IO_MODE) || (_XIP_IO_MODE==XIP_4_IO_MODE)
    SPI_XipIoMode(_XIP_IO_MODE);
#endif


#if _RANDOM_GEN 
    Getrandom_x0=2;
    Getrandom_firstdata=0;
    Getrandom_mi=0;
#endif 
#if _SYS_WDT_TIME==1
    WDT_Init(WDT_TIME_188ms);
#else
    WDT_Init(WDT_TIME_750ms);
#endif
}

//------------------------------------------------------------------//
// System initialization
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void SYS_Init2(void)
{
    SYS_ModuleInit2();
    ////DelayLoop2(50);
    WDT_Clear();
}   

//------------------------------------------------------------------//
// System service loop
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void SYS_ServiceLoop(void){
    //SMU_Sleep(SMU_SLEEP);
    WDT_Clear();
}

extern volatile U8 LoopPlayFlag;
extern volatile U16 R_Test1KHz;
extern volatile U8 key;
extern volatile U16 R_Debouncecnt;
extern volatile U8 OneSecFlag2;
extern volatile U8 OneSecFlag;
extern volatile U8 PlayCheckFlag;
extern volatile U8 Mute_Flag;


void SYS_Standby(void)
{
	if ((XIP_GetPlayStatus() == STATUS_STOP)\
	    && (R_Debouncecnt==0) && (key==0) && (R_Test1KHz==0) && (LoopPlayFlag == 0)\
	    && (Mute_Flag == 0) && (PlayCheckFlag == 0) && (OneSecFlag == 0) && (OneSecFlag2 == 0)) {
		
        
		////AUDIO_Stop(0);
		////AUDIO_Stop(1);
		////AUDIO_Stop(2);
	   
		//No Audio Playing&& No Key&& No 1KHz ouput
		GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(0xB9);                //Spi flash Deep_Powerdown
        GPIO_Write(GPIOB,0,1);
		
        //GPIOA->Wakeup_Mask |= 0X007E; ;   //PA6~PA1 Wakeup Enable 1,have wakeup,0,no wakeup
           
        P_PORTA->DIR =0x000078FF;         //PA15~PA8 Output Low  1,input,0,output
        P_PORTA->Data=0x000078FF;         //Data output
        //////DelayLoop(100);
           
        P_PORTA->Data=P_PORTA->PAD;
        P_PORTB->Data=P_PORTB->PAD;
        P_PORTC->Data=P_PORTC->PAD;
        RTC_IntCmd(RTC_4KHZ,DISABLE);
        WDT_Clear();
		
		SMU_LDO33Cmd(DISABLE);
        ////
        
        SMU_Sleep(SMU_HALT_MODE);
        //Wakeup 
        ////SYS_Init2();  
		////SYS_GPIOInit();//will reset key debounce 
		
		SMU_LDO33Cmd(ENABLE);
        ////
        DelayLoop(27);
		
		GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(0xAB);                //Spi flash Release Deep_Powerdown
        GPIO_Write(GPIOB,0,1);
        
		DIRECTKEY_Init();
		
		
		
        RTC_IntCmd(RTC_4KHZ,ENABLE);      //RTC Enable
	}
}


