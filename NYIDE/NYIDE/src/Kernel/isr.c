#include "include.h"
#include "nx1_dac.h"
#include "nx1_adc.h"
#include "nx1_timer.h"
#include "nx1_rtc.h"
#include "nx1_intc.h"
#include "nx1_gpio.h"
////#include "nx1_user.h"
//User Var
extern volatile U16 R_Test1KHz;        //For PA15 Ouput 1KHz Tone
extern volatile U8  R_T500us;          //For 1KHz baseon 250us 
extern volatile U8  R_ScanEnter;       //For scan key in main loop
extern volatile U16  R_Debouncecnt;     //For Keydebounce 


//////////////////////////////////////////////////////////////////////
// Define
//////////////////////////////////////////////////////////////////////
// DAC/ADC 
#define DAC_FIFO_BURST_SIZE     (DAC_FIFO_LEN-DAC_FIFO_THD)
#define ADC_FIFO_BURST_SIZE     ADC_FIFO_THD
#define MIX_BUF_SIZE            DAC_FIFO_BURST_SIZE
// time base
#if  _SYS_TIMEBASE==125
#define SYS_TICK_COUNT              8
#define IR_RX_COUNT                 8
//#define KEY_SCAN_COUNT              32        //Baseon 4ms 
#define KEY_SCAN_COUNT              8         //Baseon 1ms 
#elif _SYS_TIMEBASE==250
#define SYS_TICK_COUNT              4
#define IR_RX_COUNT                 4
//#define KEY_SCAN_COUNT              16        //Baseon 4ms 
#define KEY_SCAN_COUNT              4         //Baseon 1ms 
#elif _SYS_TIMEBASE==1000
#define SYS_TICK_COUNT              1
#define IR_RX_COUNT                 1
//#define KEY_SCAN_COUNT              4        //Baseon 4ms 
#define KEY_SCAN_COUNT              1         //Baseon 1ms 
#else
#error _SYS_TIMEBASE undefined
#endif


//////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////
#if _SBC_PLAY || _SBC_CH2_PLAY || _ADPCM_PLAY || _ADPCM_CH2_PLAY || _CELP_PLAY || _PCM_PLAY || _MIDI_PLAY || _RT_PLAY
U16 DacMixBuf[MIX_BUF_SIZE];
static U16 Fifo0Buf[DAC_FIFO_BURST_SIZE];
static U16 Fifo1Buf[DAC_FIFO_BURST_SIZE];
#endif
#if  _SYS_TIMEBASE
static U8 SysTickCount,KeyScanCount;
volatile U32 SysTick=0;
#endif



#if _SBC_PLAY || _SBC_CH2_PLAY || _ADPCM_PLAY || _ADPCM_CH2_PLAY || _CELP_PLAY || _PCM_PLAY || _MIDI_PLAY || _RT_PLAY
//------------------------------------------------------------------//
// DAC CH0 FIFO Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//   
void DAC_CH0_ISR(void){ 
    U8 i;
    for(i=0;i<MIX_BUF_SIZE;i++){
        DacMixBuf[i]=0;        
    }

#if _SBC_PLAY&&( _SBC_PLAY_AUDIO_CH==0)         
    SBC_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _SBC_CH2_PLAY&&( _SBC_CH2_PLAY_AUDIO_CH==0)         
    SBC_CH2_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _MIDI_PLAY&&( _MIDI_PLAY_AUDIO_CH==0)           
    MIDI_DacIsr(DAC_FIFO_BURST_SIZE);
#endif  

#if _ADPCM_PLAY&&( _ADPCM_PLAY_AUDIO_CH==0)             
    ADPCM_DacIsr(DAC_FIFO_BURST_SIZE,Fifo0Buf);
#endif  

#if _ADPCM_CH2_PLAY&&( _ADPCM_CH2_PLAY_AUDIO_CH==0)         
    ADPCM_CH2_DacIsr(DAC_FIFO_BURST_SIZE,Fifo0Buf);
#endif 
      
#if _CELP_PLAY&&( _CELP_PLAY_AUDIO_CH==0)               
    CELP_DacIsr(DAC_FIFO_BURST_SIZE);
#endif  

#if _PCM_PLAY&&( _PCM_PLAY_AUDIO_CH==0)              
    PCM_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _PCM_CH2_PLAY&&( _PCM_PLAY_AUDIO_CH==0)              
    PCM_CH2_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _PCM_CH3_PLAY&&( _PCM_PLAY_AUDIO_CH==0)              
    PCM_CH3_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _RT_PLAY&&( _RT_PLAY_AUDIO_CH==0)              
    RT_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

    for(i=0;i<MIX_BUF_SIZE;i++){
        P_DAC->CH0.CH_Data = DacMixBuf[i]+DAC_DATA_OFFSET;        
    }             
}
//------------------------------------------------------------------//
// DAC CH1 FIFO Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void DAC_CH1_ISR(void){
	U8 i;
    for(i=0;i<MIX_BUF_SIZE;i++){
        DacMixBuf[i]=0;        
    }       
#if _SBC_PLAY&&( _SBC_PLAY_AUDIO_CH==1)        
    SBC_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 
       
#if _SBC_CH2_PLAY&&( _SBC_CH2_PLAY_AUDIO_CH==1)         
    SBC_CH2_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if _MIDI_PLAY&&( _MIDI_PLAY_AUDIO_CH==1)          
    MIDI_DacIsr(DAC_FIFO_BURST_SIZE);
#endif  

#if _ADPCM_PLAY&&( _ADPCM_PLAY_AUDIO_CH==1)           
    ADPCM_DacIsr(DAC_FIFO_BURST_SIZE,Fifo1Buf);
#endif  

#if _ADPCM_CH2_PLAY&&( _ADPCM_CH2_PLAY_AUDIO_CH==1)         
    ADPCM_CH2_DacIsr(DAC_FIFO_BURST_SIZE,Fifo1Buf);
#endif    
  
#if _CELP_PLAY&&( _CELP_PLAY_AUDIO_CH==1)       
    CELP_DacIsr(DAC_FIFO_BURST_SIZE);
#endif    

#if _PCM_PLAY&&( _PCM_PLAY_AUDIO_CH==1)              
    PCM_DacIsr(DAC_FIFO_BURST_SIZE);
#endif
#if _PCM_CH2_PLAY&&( _PCM_PLAY_AUDIO_CH==1)              
    PCM_CH2_DacIsr(DAC_FIFO_BURST_SIZE);
#endif
#if _PCM_CH3_PLAY&&( _PCM_PLAY_AUDIO_CH==1)              
    PCM_CH3_DacIsr(DAC_FIFO_BURST_SIZE);
#endif

#if _RT_PLAY&&( _RT_PLAY_AUDIO_CH==1)              
    RT_DacIsr(DAC_FIFO_BURST_SIZE);
#endif 

#if 1       
    for(i=0;i<MIX_BUF_SIZE;i++){
        P_DAC->CH1.CH_Data = DacMixBuf[i]+DAC_DATA_OFFSET;        
        //DAC1->DAData=DacMixBuf[i];
    }
#else         
    for(i=0;i<MIX_BUF_SIZE;i++){
        DAC1->DAData=TestP[i];        
    }
#endif                              
}   
#endif
//------------------------------------------------------------------//
// ADC Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//   
void ADC_ISR(void){ 
    
#if _VR_FUNC    
    VR_AdcIsr(ADC_FIFO_THD,(U32 *)(&P_ADC->Data_CH0));
#endif
#if _ADPCM_RECORD        
    ADPCM_AdcIsr(ADC_FIFO_BURST_SIZE,(U32 *)(&P_ADC->Data_CH0));
#endif     
#if _SBC_RECORD        
    SBC_AdcIsr(ADC_FIFO_BURST_SIZE,(U32 *)(&P_ADC->Data_CH0));
#endif  
#if _PCM_RECORD        
    PCM_AdcIsr(ADC_FIFO_BURST_SIZE,(U32 *)(&P_ADC->Data_CH0));
#endif  
#if _RT_PLAY   
    RT_AdcIsr(ADC_FIFO_BURST_SIZE,(U32 *)(&P_ADC->Data_CH0));
#endif  
}  
#if _TIMER_TMR0_INT
//------------------------------------------------------------------//
// Timer0 Interrupt Service Routine
// Parameter:
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void TMR0_ISR(void)
{
    TIMER0->Flag = 1;
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}
#endif
#if _TIMER_TMR1_INT
//------------------------------------------------------------------//
// Timer1 Interrupt Service Routine
// Parameter:
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void TMR1_ISR(void)
{
    TIMER1->Flag = 1;
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}
#endif
#if _TIMER_TMR2_INT
//------------------------------------------------------------------//
// Timer2 Interrupt Service Routine
// Parameter:
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void TMR2_ISR(void)
{
    TIMER2->Flag = 1;
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}
#endif
#if _SYS_TIMEBASE
//------------------------------------------------------------------//
// RTC Interrupt Service Routine
// Parameter:
//          NONE
// return value:
//          NONE
// 2017/4/11 v1: add 1KHz tone ouput 
//------------------------------------------------------------------//
void RTC_ISR(void){
    RTC_ClrSts();
    
    if(R_Test1KHz){
        //Test1KHz Enable
        if(R_T500us==0){
            R_T500us=1;
            //Timer 500us OK
            GPIO_Toggle(GPIOA,15);        //PA15 1Khz ouput
            
            R_Test1KHz--;
            if(R_Test1KHz==0){
                 GPIO_Write(GPIOA,15,BIT_RESET); //PA15 1Khz Close
            }
        }
        else{
            R_T500us--;
        }
    }
    
    SysTickCount--;
    if(SysTickCount==0){
        SysTickCount=SYS_TICK_COUNT;
        SysTick+=1;
    }
    
    
    KeyScanCount--;
    if(KeyScanCount==0){
        KeyScanCount=KEY_SCAN_COUNT;
        
        //GPIO_Toggle(GPIOA,14);        //PA14 500Hz ouput
        R_ScanEnter=1;  //Change KeyScan() in main loop
        
        if(R_Debouncecnt!=0)
            R_Debouncecnt--;
        //KeyScan();
    }
    
    ChkVioCtl();
}
//------------------------------------------------------------------//
// Start systick timer
// Parameter: 
//          freq:systick frequency
// return value:
//          NONE 
//------------------------------------------------------------------//
void SysTickInit(U32 freq)
{
    SysTickCount=SYS_TICK_COUNT;
    KeyScanCount=KEY_SCAN_COUNT;
    
    RTC_IntCmd(freq,ENABLE);
    INTC_IrqCmd(IRQ_BIT_RTC,ENABLE);
} 
#endif
#if  _PWMA_INT
//------------------------------------------------------------------//
// PWMA Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//   
void PWMA_ISR(void)
{
    U8 sts;
    
    sts=P_PWMA->TMR_Flag&P_PWMA->TMR_INT_Ctrl;
    P_PWMA->TMR_Flag=sts;
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}    
#endif 
#if  _PWMB_INT
//------------------------------------------------------------------//
// PWMB Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//   
void PWMB_ISR(void)
{
    U8 sts;
    
    sts=P_PWMB->TMR_Flag&P_PWMB->TMR_INT_Ctrl;
    P_PWMB->TMR_Flag=sts;  
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}    
#endif 
#if  _CAPTURE_INT
//------------------------------------------------------------------//
// Capture Interrupt Service Routine
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//   
void CAPTURE_ISR(void)
{
    U8 sts;
    
    sts=P_PWMA->CAP_Flag&P_PWMA->CAP_INT_Ctrl;
    P_PWMA->CAP_Flag=sts;
    //User defined code.
    //Code can not be too long, or CPU performance degrade.
}    
#endif 
