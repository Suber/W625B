#include "nx1_pwm.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define PWM_DUTY_VALUE_OFFSET       16
#define PWM_CAPTURE_PIN_SEL_OFFSET  16
#define PWM_TIMER_OFFSET            16


//------------------------------------------------------------------//
// PWM init
// Parameter: 
//          pwmx  : PWMA,PWMB
//          idx   : PWM out index, PWMx0~PWMx3
//          mode  : PWM_START_HIGH,PWM_START_LOW
//          count : duty compare counter value. unit is register value.
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_Init(PWM_TypeDef *pwmx,U8 idx,U8 mode,U16 count)
{
    volatile U32 *pOutDuty;
    
    //pOutDuty is pointer of PWM IO Channel.
    pOutDuty = (U32 *)(((U32)pwmx&0xFFFFF0) + (idx<<2)+PWM_TIMER_OFFSET);
    *pOutDuty = (count<<PWM_DUTY_VALUE_OFFSET)|mode;
}
//------------------------------------------------------------------//
// PWM init
// Parameter: 
//          pwmx : PWMA,PWMB
//          idx  : PWM out index, PWMx0~PWMx3
//          mode : PWM_START_HIGH,PWM_START_LOW
//          duty : duty cycle, 0~100. unit is %.
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_InitDuty(PWM_TypeDef *pwmx,U8 idx,U8 mode,U8 duty)
{
    volatile U32 *pOutDuty;
    U16 count;
    
    if(duty>100) duty=100;
    duty=100-duty;    
    
    if(pwmx==PWMA)
    {
    	count = P_PWMA->TMR_Data;
    }
    else
    {
    	count = P_PWMB->TMR_Data;
    }            
    //count=   ((float)count*duty/100)+0.5;
    count=   (((count*duty*128)/100)+64)>>7;
    pOutDuty = (U32 *)(((U32)pwmx&0xFFFFF0) + (idx<<2)+PWM_TIMER_OFFSET);
    *pOutDuty = (count<<PWM_DUTY_VALUE_OFFSET) | mode;
}
//------------------------------------------------------------------//
// Enable or Disable PWM
// Parameter: 
//          pwmx : PWMA,PWMB
//          idx  : PWM out index, PWMx0~PWMx3
//          cmd  : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_Cmd(PWM_TypeDef *pwmx,U8 idx,U8 cmd)
{
    volatile U32 *pOutDuty;
    
    pOutDuty = (U32 *)(((U32)pwmx&0xFFFFF0) + (idx<<2)+PWM_TIMER_OFFSET);
    
    if(cmd==ENABLE)
    {
        *pOutDuty |= C_PWM_OUT_En;
    }
    else
    {
    	*pOutDuty &= (~C_PWM_OUT_En);
    }        
}
//------------------------------------------------------------------//
// Set PWM duty cycle
// Parameter: 
//          pwmx : PWMA,PWMB
//          idx  : PWM out index, PWMx0~PWMx3
//          duty : duty cycle,0~100
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_SetDuty(PWM_TypeDef *pwmx,U8 idx,U8 duty)
{
    volatile U32 *pOutDuty;
    U16 count;
    
    if(duty>100) duty=100;
    duty=100-duty; 
        
    if(pwmx==PWMA)
    {
    	count = P_PWMA->TMR_Data;
    }
    else
    {
    	count = P_PWMB->TMR_Data;;
    }         
    //count=  ((float)count*duty/100)+0.5;
    count = (((count*duty*128)/100)+64)>>7;  
    pOutDuty = (U32 *)(((U32)pwmx&0xFFFFF0) + (idx<<2)+PWM_TIMER_OFFSET);
    *pOutDuty = (*pOutDuty & ~C_PWM_Duty) | (count<<PWM_DUTY_VALUE_OFFSET);
}
//------------------------------------------------------------------//
// Enable or Disable PWM capture
// Parameter: 
//          cmd : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_CaptureCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_PWMA->TMR_Ctrl |= (C_PWMA_CAP_En|C_PWM_Data_Reload_Start);
    }
    else
    {
    	P_PWMA->TMR_Ctrl &= (~C_PWMA_CAP_En);
    }        
}
//------------------------------------------------------------------//
// Get capture data
// Parameter: 
//          NONE
// return value:
//          Capture Data, High half word : rising edge latch data
//                        Low  half word : falling edge latch data
//------------------------------------------------------------------//    
U32 PWM_CaptureData(void)
{
   return (P_PWMA->CAP_Data);
}
//------------------------------------------------------------------//
// Enable or Disable capture interrupt
// Parameter: 
//          irq:interrupt source,PWM_INT_CAPTURE_RISING,PWM_INT_CAPTURE_FALLING                                
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_CaptureIntCmd(U8 irq,U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_PWMA->CAP_INT_Ctrl |= irq;
    }
    else
    {
        P_PWMA->CAP_INT_Ctrl &= (~irq);
    }    
}
//------------------------------------------------------------------//
// Select capture pin
// Parameter: 
//          pin:0~15(PA0~PA15)
// return value:
//          NONE
//------------------------------------------------------------------//    
void PWM_CapturePinSelect(U8 pin)
{
    P_PWMA->TMR_Ctrl &= (~C_PWMA_CAP_Pad_Sel);
    P_PWMA->TMR_Ctrl |= (pin<<PWM_CAPTURE_PIN_SEL_OFFSET);
}

