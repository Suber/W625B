#include "nx1_gpio.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define GPIO_PORT_OFFSET            16



//------------------------------------------------------------------//
// GPIO pin init
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
//          mode  : 
//                  GPIO_MODE_IN_PU
//                  GPIO_MODE_IN_FLOAT
//                  GPIO_MODE_ALT
//                  GPIO_MODE_ALT2  (Switch GPIOA4~7 to ADC CH4~7)
//                  GPIO_MODE_OUT
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_Init(GPIO_TypeDef *gpiox,U8 pin,U8 mode)
{
    if(mode==GPIO_MODE_IN_PU)
    {
        gpiox->MFP &= (~(1<<pin));
        gpiox->DIR  |= (1<<pin);
        gpiox->CFG &= (~(1<<pin));
    }
    else if(mode==GPIO_MODE_IN_FLOAT)
    {
        gpiox->MFP &= (~(1<<pin));
        gpiox->DIR  |= (1<<pin);
        gpiox->CFG |= (1<<pin);
    }
    else if(mode==GPIO_MODE_OUT)
    {
        gpiox->MFP &= (~(1<<pin));
        gpiox->DIR  &= (~(1<<pin));
    }
    else if(mode==GPIO_MODE_ALT)
    {
        gpiox->MFP |= (1<<pin);
    }
    else if(mode==GPIO_MODE_ALT2 && gpiox==P_PORTA)
    {
        gpiox->MFP &= (~(1<<pin));
        pin+=GPIO_ALT2_OFFSET;
        gpiox->MFP |= (1<<pin);
    }
}
//------------------------------------------------------------------//
// Read specific gpio pin
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
// return value:
//          pin value
//------------------------------------------------------------------//    
U8 GPIO_Read(GPIO_TypeDef *gpiox,U8 pin)
{
    return ((gpiox->PAD>>pin)&0x1);
}      
//------------------------------------------------------------------//
// Write specific gpio pin
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
//          value : BIT_SET,BIT_RESET
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_Write(GPIO_TypeDef *gpiox,U8 pin,U8 value) 
{
    GIE_DISABLE();
 	if(value&0x1)
    {
        gpiox->Data |= (1<<pin);
    }
    else
    {
        gpiox->Data &= (~(1<<pin));
    }
    GIE_ENABLE();
}        
//------------------------------------------------------------------//
// Toggle specific gpio pin
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_Toggle(GPIO_TypeDef *gpiox,U8 pin) 
{
    GIE_DISABLE();
    gpiox->Data ^= (1<<pin);
    GIE_ENABLE();
}        
//------------------------------------------------------------------//
// Write gpio port
// Parameter: 
//          gpiox:gpio group, GPIOA,GPIOB,GPIOC
//          value:value to be written to the output data register
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_WritePort(GPIO_TypeDef *gpiox,U16 value) 
{
    GIE_DISABLE();    
    gpiox->Data=value;
    GIE_ENABLE();    
}   
//------------------------------------------------------------------//
// Read gpio port
// Parameter: 
//          gpiox:gpio group, GPIOA,GPIOB,GPIOC
// return value:
//          port value
//------------------------------------------------------------------//    
U16 GPIO_ReadPort(GPIO_TypeDef *gpiox) 
{
    return (gpiox->PAD);
}   
//------------------------------------------------------------------//
// Read port mode
// Parameter: 
//          gpiox:gpio group, GPIOA,GPIOB,GPIOC
// return value:
//          mode:
//                1 : Input mode for each pin
//                0 : Output mode for each pin
//------------------------------------------------------------------//    
U16 GPIO_ReadPortMode(GPIO_TypeDef *gpiox)
{
    return (gpiox->DIR); 
}
//------------------------------------------------------------------//
// Write port mode
// Parameter: 
//          gpiox:gpio group, GPIOA,GPIOB,GPIOC
//          mode:
//                1 : Input mode for each pin
//                0 : Output mode for each pin
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_WritePortMode(GPIO_TypeDef *gpiox,U16 mode)
{
    gpiox->DIR = mode;
}
//------------------------------------------------------------------//
// Read mode
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
// return value:
//          mode
//------------------------------------------------------------------//    
U8 GPIO_ReadMode(GPIO_TypeDef *gpiox,U8 pin)
{
    return ((gpiox->DIR>>pin)&0x1);
}
//------------------------------------------------------------------//
// Write pin mode
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
//          mode  :
//                  1:input, 0:output
// return value:
//          NONE
//------------------------------------------------------------------//    
void GPIO_WriteMode(GPIO_TypeDef *gpiox,U8 pin,U8 mode)
{
    if(mode)
    {
        gpiox->DIR |= (1<<pin);
    }
    else
    {
        gpiox->DIR &= (~(1<<pin));
    }     
}
//------------------------------------------------------------------//
// Set pin current
// Parameter: 
//          gpiox   : gpio group, GPIOA,GPIOB,GPIOC
//          pin     : pin number(0~15)
//          setting :
//                     GPIO_SINK_NORMAL
//                     GPIO_SINK_LARGE
//                     GPIO_CC_NORMAL
//                     GPIO_CC_LARGE
// return value:
//          void
//------------------------------------------------------------------//    
void GPIO_SetCurrent(GPIO_TypeDef *gpiox,U8 pin,U8 setting)
{
    pin += GPIO_PORT_OFFSET;
    //PA8~PA15 is Constant Sink Current 
    if(setting&GPIO_CURRENT_CC && gpiox==P_PORTA && pin>=GPIO_CC_PIN_NUM)
    {
        gpiox->CFG &= (~(1<<pin));
    }
    else
    {
        gpiox->CFG |= (1<<pin);
    }    
    
    if(setting&GPIO_LARGE_CURRENT)
    {
        gpiox->Wakeup_Mask &= (~(1<<pin));
    }
    else
    {
        gpiox->Wakeup_Mask |= (1<<pin);
    }    
}    
//------------------------------------------------------------------//
// Enable or Disable wakeup pin
// Parameter: 
//          gpiox : gpio group, GPIOA,GPIOB,GPIOC
//          pin   : pin number(0~15)
//          cmd   : ENABLE,DISABLE
// return value:
//          void
//------------------------------------------------------------------//    
void GPIO_WakeUpCmd(GPIO_TypeDef *gpiox,U8 pin,U8 cmd)
{
    if(cmd)
    {
        gpiox->Wakeup_Mask |= (1<<pin);
    }
    else
    {
        gpiox->Wakeup_Mask &= (~(1<<pin));
    }    
}   
//------------------------------------------------------------------//
// Set pull-resistor mode
// Parameter: 
//          gpiox : GPIOA,GPIOB,GPIOC
//          part  : 0:low part, 1:hight part
//          mode  : GPIO_STRONG_PULL,GPIO_WEAK_PULL
// return value:
//          void
//------------------------------------------------------------------//    
void GPIO_SetPullMode(GPIO_TypeDef *gpiox,U8 part,U8 mode)
{
    U8 pullmode;
    if(gpiox==GPIOA)
    {
        if(part)
        {
            //PORTA High
            P_PORTA->PULL_HIGH_CFG &= (~C_PULL_HIGH_PAH);   
            pullmode = (mode==GPIO_STRONG_PULL)? C_PULL_HIGH_PAH_100K : C_PULL_HIGH_PAH_1M;
            P_PORTA->PULL_HIGH_CFG |= pullmode;
        }
        else
        {
            //PORTA Low
            P_PORTA->PULL_HIGH_CFG &= (~C_PULL_HIGH_PAL);   
            pullmode = (mode==GPIO_STRONG_PULL)? C_PULL_HIGH_PAL_100K : C_PULL_HIGH_PAL_1M;
            P_PORTA->PULL_HIGH_CFG |= pullmode;
        }    
    }
    else if(gpiox==GPIOB)
    {
        if(part)
        {
            //PORTB High
            P_PORTA->PULL_HIGH_CFG &= (~C_PULL_HIGH_PBH);   
            pullmode = (mode==GPIO_STRONG_PULL)? C_PULL_HIGH_PBH_100K : C_PULL_HIGH_PBH_1M;
            P_PORTA->PULL_HIGH_CFG |= pullmode;            
        }
        else
        {
            //PORTB Low
            P_PORTA->PULL_HIGH_CFG &= (~C_PULL_HIGH_PBL);   
            pullmode = (mode==GPIO_STRONG_PULL)? C_PULL_HIGH_PBL_100K : C_PULL_HIGH_PBL_1M;
            P_PORTA->PULL_HIGH_CFG |= pullmode;  
        }    
    }
    else if(gpiox==GPIOC)
    {
        //PORTC
        P_PORTA->PULL_HIGH_CFG &= (~C_PULL_HIGH_PCL);   
        pullmode = (mode==GPIO_STRONG_PULL)? C_PULL_HIGH_PCL_100K : C_PULL_HIGH_PCL_1M;
        P_PORTA->PULL_HIGH_CFG |= pullmode;
    }    
}    
//------------------------------------------------------------------//
// Init external trigger
// Parameter: 
//          trig_src:
//                    GPIO_EXT_RISING
//                    GPIO_EXT_FALLING
//                    GPIO_EXT_RISING_FALLING
// return value:
//          void
//------------------------------------------------------------------//    
void GPIO_ExtTrigInit(U8 trig_src)
{
    P_PORT_EXT->EXT_INT_Trig= trig_src;  
}    
//------------------------------------------------------------------//
// Enable or Disable external interrupt
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          void
//------------------------------------------------------------------//    
void GPIO_ExtIntCmd(U8 cmd)
{
    if(cmd)
    {
        P_PORT_EXT->EXT_INT_Ctrl= 1;
    }
    else
    {
        P_PORT_EXT->EXT_INT_Ctrl= 0;
    }      
}    
