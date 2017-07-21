#include "nx1_dac.h"
#include "nx1_smu.h"
#include "nx1_timer.h"
#include "nx1_intc.h"


//////////////////////////////////////////////////////////////////////
//  Define
//////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------//
// DAC Init
// Parameter: 
//          chx        : DAC_CH0, DAC_CH1
//          trig_src   : DAC_TIMER0_TRIG,DAC_TIMER1_TRIG,DAC_TIMER2_TRIG
//          fifo_level : fifo threshold, DAC_FIFO_THD
//          upsample: 
//                    1: upsample *4, 
//                    0: no upsample
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_Init(CH_TypeDef *chx,U8 trig_src,U8 fifo_level,U8 upsample)
{
    chx->CH_Ctrl &= ~(C_DAC_Up_Sample_En | C_DAC_Trig | C_DAC_Data_Mixer_En);
    chx->CH_Ctrl |= (trig_src | C_DAC_Data_Mixer_En);
    
    if(upsample)
    {
        chx->CH_Ctrl |= C_DAC_Up_Sample_En;
    }
    chx->CH_FIFO = (C_DAC_FIFO_Reset | (fifo_level<<C_DAC_FIFO_Level_Offset) );
    
}    
//------------------------------------------------------------------//
// Set PP gain immediately.
// Parameter: 
//          gain:dac gain(0~15)
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_SetPPIpGain(U8 gain)
{
    P_DAC->CH0.CH_Ctrl &= (~C_DAC_PPGain);
    P_DAC->CH0.CH_Ctrl |= ((gain&0xF)<<C_DAC_PPGain_Offset);
}    
//------------------------------------------------------------------//
// ENABLE or Disable Push-pll
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_PPCmd(U8 cmd)  //Check PPCmd function!!
{
    if(cmd==ENABLE)
    {
        P_DAC->CH0.CH_Ctrl &= (~C_DAC_Out_Mode);
        P_DAC->CH0.CH_Ctrl |= (C_DAC_Out_Mode_PushPull | C_DAC_En);
    }
    else
    {
        P_DAC->CH0.CH_Ctrl &= (~C_DAC_En);
    }        
}    
//------------------------------------------------------------------//
// ENABLE or Disable voltage DAC
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_VoltageDacCmd(U8 cmd)  //Check DacCmd function !!
{
    if(cmd==ENABLE)
    {
        P_DAC->CH0.CH_Ctrl |= (C_DAC_En|C_DAC_Out_Mode_VolDAC);
    }
    else
    {
        P_DAC->CH0.CH_Ctrl &= (~(C_DAC_En|C_DAC_Out_Mode_VolDAC));
    }        
}    
//------------------------------------------------------------------//
// ENABLE or Disable HW channel converter.
// Parameter: 
//          chx  : DAC_CH0, DAC_CH1
//          cmd  : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_Cmd(CH_TypeDef *chx,U8 cmd)
{  
    if(cmd==ENABLE)
    {
        chx->CH_Ctrl |= C_DAC_Trig_Timer_En;
        chx->CH_FIFO |= C_DAC_FIFO_Level_IEn; 
    }
    else
    {
    	chx->CH_Ctrl &= (~C_DAC_Trig_Timer_En);
    	chx->CH_FIFO &= (~C_DAC_FIFO_Level_IEn); 
    }       
}    
//------------------------------------------------------------------//
// ENABLE or Disable DAC line-in
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void DAC_LineInCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_DAC->CH0.CH_Ctrl |= C_DAC_EXT_AUX_In_En;        
    }
    else
    {
    	P_DAC->CH0.CH_Ctrl &= (~C_DAC_EXT_AUX_In_En);
    }       
}       
//------------------------------------------------------------------//
// DAC ramp control 
// Parameter: 
//          chx: DAC_CH0, DAC_CH1
//          cmd:RAMP_UP,RAMP_DOWN
// return value:
//          NONE
//------------------------------------------------------------------//     
void DacRamp(CH_TypeDef *chx, U8 cmd)
{
    extern void DelayLoop2(U32 count);
    U16 data;
    
    SMU_PeriphClkCmd(SMU_PERIPH_DAC,ENABLE);

    if(cmd==RAMP_UP)
    {
        chx->CH_Ctrl &= (~C_DAC_Trig);
        data=0x8000;
//        data=0x7FFF;
//        data=0;
        chx->CH_Data=data;

        //DAC_PPCmd(ENABLE);
    
        DAC_VoltageDacCmd(ENABLE);
        while(1)
        {
            data++;
            chx->CH_Data=data; 
            if(data==0)
            {
                break;
            }      
            DelayLoop2(1);
        }    
    }
    else
    {
        chx->CH_Ctrl &= (~C_DAC_Trig);
        data=0;
        chx->CH_Data=data;
        while(1)
        {
            data--;
            chx->CH_Data=data; 
            if(data==0x8000)
            {
                break;
            }       
        }    
        //DAC_PPCmd(DISABLE);
        DAC_VoltageDacCmd(DISABLE);
    }    
}    
