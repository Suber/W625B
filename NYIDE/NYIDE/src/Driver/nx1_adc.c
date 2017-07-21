#include "nx1_reg.h"
#include "nx1_adc.h"
#include "nx1_smu.h"
#include "nx1_intc.h"
#include "nx1_timer.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define ADC_MAX_GAIN                31



//------------------------------------------------------------------//
// ADC HW Init
// Parameter: 
//          trig_src   : ADC_SW_TRIG,ADC_TIMER0_TRIG,ADC_TIMER1_TRIG,ADC_TIMER2_TRIG
//          channel    : ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7, ADC_CH_MIC
//          fifo_level : fifo threshold, ADC_FIFO_THD
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_Init(U8 trig_src,U8 channel,U8 fifo_level)
{
    P_ADC->Ctrl = trig_src | C_ADC_CLK_Div_16 | C_ADC_PreCharge_Dis | C_ADC_Sample_Cycle_8; 
    if(trig_src==ADC_SW_TRIG)
    { 
        P_ADC->FIFO_Ctrl |= C_ADC_Data_Store_FIFO | C_ADC_FIFO_Reset;
    }
    else
    {
        P_ADC->FIFO_Ctrl = C_ADC_Data_Store_FIFO| C_ADC_FIFO_Level_IEn | C_ADC_FIFO_Reset | (fifo_level<<C_ADC_FIFO_Level_Offset);
    }        
    if(channel==ADC_CH_MIC)
    {
        P_ADC->Ctrl |= (C_ADC_CH0_From_MIC | C_ADC_CH0) ;
    }
    else
    {
        P_ADC->Ctrl |= channel; //C_ADC_CH0~C_ADC_CH7
    }    
}    
//------------------------------------------------------------------//
// Start single conversion
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_SwTrigStart(void)
{
    P_ADC->Ctrl |= C_ADC_Manual_Start;

    while(!(P_ADC->Flag&C_ADC_CNV_Done_Flag))
    {
        
    }    
   
    P_ADC->Flag |= C_ADC_CNV_Done_Flag;
}   
//------------------------------------------------------------------//
// Get conversion value
// Parameter: 
//          ch: ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3, ADC_CH4, ADC_CH5, ADC_CH6, ADC_CH7, ADC_CH_MIC
// return value:
//          channel value
//------------------------------------------------------------------//    
U16 ADC_GetValue(U8 ch)
{
    if(ch==ADC_CH0 || ch==ADC_CH_MIC)
    {
        return (P_ADC->Data_CH0);
    }
    else if(ch==ADC_CH1)
    {
        return (P_ADC->Data_CH1);
    }
    else if(ch==ADC_CH2)
    {
        return (P_ADC->Data_CH2);
    }
    else if(ch==ADC_CH3)
    {
        return (P_ADC->Data_CH3);
    }
    else if(ch==ADC_CH4)
    {
        return (P_ADC->Data_CH4);
    }
    else if(ch==ADC_CH5)
    {
        return (P_ADC->Data_CH5);
    }
    else if(ch==ADC_CH6)
    {
        return (P_ADC->Data_CH6);
    }
    else if(ch==ADC_CH7)
    {
        return (P_ADC->Data_CH7);         
    }
    else
    {
        return 0;
    }               
}  
//------------------------------------------------------------------//
// Enable or Disable ADC
// Parameter: 
//          cmd: ENABLE, DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_Cmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_ADC->Ctrl |= C_ADC_En;
    }
    else
    {
    	P_ADC->Ctrl &= (~C_ADC_En);
    }      
}    
//------------------------------------------------------------------//
// Enable or Disable microphone bias
// Parameter: 
//          cmd : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_MicBiasCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_ADC->AGC_Ctrl |= C_ADC_MIC_En;
    }
    else
    {
    	P_ADC->AGC_Ctrl &= (~C_ADC_MIC_En);
    }       
}    
//------------------------------------------------------------------//
// Enable or Disable AGC
// Parameter: 
//          cmd: ENABLE, DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_AgCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_ADC->AGC_Ctrl |= C_ADC_AGC_En;
    }
    else
    {
    	P_ADC->AGC_Ctrl &= (~C_ADC_AGC_En);
    }      
}    
//------------------------------------------------------------------//
// Set PGA gain
// Parameter: 
//          gain:PGA gain, 0~31
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_SetGain(U16 gain)
{
    if(gain>ADC_MAX_GAIN)
    {
       gain=ADC_MAX_GAIN;
    }        
    P_ADC->AGC_Ctrl &= (~C_ADC_PGA_Gain);
    P_ADC->AGC_Ctrl |= (gain<<C_ADC_PGA_Gain_Offset);
}    
//------------------------------------------------------------------//
// Set MIC Pre gain
// Parameter: 
//           gain: C_ADC_MIC_AMP_Gain_30dB
//                 C_ADC_MIC_AMP_Gain_15dB
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_MicPreGain(U8 gain)
{
    P_ADC->AGC_Ctrl &= (~C_ADC_MIC_AMP_Gain_30dB);
    P_ADC->AGC_Ctrl |= gain;
}    
//------------------------------------------------------------------//
// Set scan mode
// Parameter: 
//          scan mode:ADC_SCAN_DISABLE,ADC_SCAN_2,ADC_SCAN_3,ADC_SCAN_4
// return value:
//          NONE
//------------------------------------------------------------------//    
void ADC_SetScanMode(U8 mode)
{
    P_ADC->Ctrl &= (~C_ADC_Scan_Mode);
    P_ADC->Ctrl |= mode;  
}      
