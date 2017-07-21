#ifndef NX1_GPIO_H
#define NX1_GPIO_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : GPIO_Init, GPIO_Read, GPIO_Write, GPIO_Toggle, GPIO_WritePort, 
//               GPIO_WritePort, GPIO_ReadPort,GPIO_ReadMode, GPIO_WriteMode, 
//               GPIO_SetCurrent, GPIO_SetPullMode, GPIO_WakeUpCmd
//  Parameter  : gpiox
//------------------------------------------------------------------//
#define GPIOA                       P_PORTA
#define GPIOB                       P_PORTB
#define GPIOC                       P_PORTC

//------------------------------------------------------------------//
//  Function   : GPIO_Init
//  Parameter  : mode
//------------------------------------------------------------------//
// GPIO Input/Output and function setting
#define GPIO_MODE_IN_PU             GPIO_INPUT_PULLHIGH
#define GPIO_MODE_IN_FLOAT          GPIO_INPUT_FLOAT             
#define GPIO_MODE_ALT               GPIO_OUTPUT_ALT
#define GPIO_MODE_OUT               GPIO_OUTPUT_NORMAL
#define GPIO_MODE_ALT2              GPIO_OUTPUT_ALT2
#define GPIO_ALT2_OFFSET            16

//------------------------------------------------------------------//
//  Function   : GPIO_SetPullMode
//  Parameter  : mode
//------------------------------------------------------------------//
#define GPIO_STRONG_PULL            0x1
#define GPIO_WEAK_PULL              0x0    

//------------------------------------------------------------------//
//  Function   : GPIO_SetCurrent
//  Parameter  : setting
//------------------------------------------------------------------//
#define GPIO_SINK_NORMAL            0x00
#define GPIO_SINK_LARGE             0x02
#define GPIO_CC_NORMAL              0x01
#define GPIO_CC_LARGE               0x03
#define GPIO_CURRENT_CC             0x1             // Set, when GPIO_CC_NORMAL or GPIO_CC_LARGE
#define GPIO_CC_PIN_NUM             8               // PA8~PA15 with CC MODE
#define GPIO_LARGE_CURRENT          0x2             // Set, when GPIO_SINK_LARGE or GPIO_CC_LARGE

//------------------------------------------------------------------//
//  Function   : GPIO_ExtTrigInit
//  Parameter  : trig_src
//------------------------------------------------------------------//
#define GPIO_EXT_RISING             C_EXT_INT_Trig_Rising
#define GPIO_EXT_FALLING            C_EXT_INT_Trig_Falling
#define GPIO_EXT_RISING_FALLING     C_EXT_INT_Trig_Edge

                     

//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void GPIO_Init(GPIO_TypeDef *gpiox,U8 pin,U8 mode);
extern U8   GPIO_Read(GPIO_TypeDef *gpiox,U8 pin);
extern void GPIO_Write(GPIO_TypeDef *gpiox,U8 pin,U8 value) ;
extern void GPIO_Toggle(GPIO_TypeDef *gpiox,U8 pin);
extern void GPIO_WritePort(GPIO_TypeDef *gpiox,U16 value);
extern U16  GPIO_ReadPort(GPIO_TypeDef *gpiox);
extern U16  GPIO_ReadPortMode(GPIO_TypeDef *gpiox);
extern void GPIO_WritePortMode(GPIO_TypeDef *gpiox,U16 mode);
extern U8   GPIO_ReadMode(GPIO_TypeDef *gpiox,U8 pin);
extern void GPIO_WriteMode(GPIO_TypeDef *gpiox,U8 pin,U8 mode);
extern void GPIO_SetCurrent(GPIO_TypeDef *gpiox,U8 pin,U8 setting);
extern void GPIO_ExtTrigInit(U8 trig_src);
extern void GPIO_ExtIntCmd(U8 cmd);
extern void GPIO_SetPullMode(GPIO_TypeDef *gpiox,U8 part,U8 mode);
extern void GPIO_WakeUpCmd(GPIO_TypeDef *gpiox,U8 pin,U8 cmd);




  
#endif //#ifndef NX1_GPIO_H
