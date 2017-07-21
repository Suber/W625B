#ifndef NX1_PWM_H
#define NX1_PWM_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : PWM_Init, PWM_InitDuty, PWM_Cmd, PWM_SetDuty
//  Parameter  : pwmx
//------------------------------------------------------------------//
#define PWMA                        P_PWMA
#define PWMB                        P_PWMB

//------------------------------------------------------------------//
//  Function   : PWM_Init, PWM_InitDuty
//  Parameter  : mode
//------------------------------------------------------------------//
#define PWM_START_HIGH              C_PWM_Duty_Pol_High
#define PWM_START_LOW               C_PWM_Duty_Pol_Low

//------------------------------------------------------------------//
//  Function   : PWM_CaptureIntCmd
//  Parameter  : irq
//------------------------------------------------------------------//
#define PWM_INT_CAPTURE_RISING      C_PWMA_CAP_Rising_IEn
#define PWM_INT_CAPTURE_FALLING     C_PWMA_CAP_Falling_IEn

//------------------------------------------------------------------//
//  Function   : PWM_Init, PWM_InitDuty, PWM_Cmd, PWM_SetDuty
//  Parameter  : idx
//------------------------------------------------------------------//
#define  PWMx0                      0
#define  PWMx1                      1
#define  PWMx2                      2
#define  PWMx3                      3



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void PWM_Init(PWM_TypeDef *pwmx,U8 idx,U8 mode,U16 count);
extern void PWM_InitDuty(PWM_TypeDef *pwmx,U8 idx,U8 mode,U8 duty);
extern void PWM_Cmd(PWM_TypeDef *pwmx,U8 idx,U8 cmd);
extern void PWM_SetDuty(PWM_TypeDef *pwmx,U8 idx,U8 duty);
extern void PWM_CapturePinSelect(U8 pin);
extern void PWM_CaptureCmd(U8 cmd);
extern void PWM_CaptureIntCmd(U8 irq,U8 cmd);
extern  U32 PWM_CaptureData(void);
extern void PWM_CapturePinSelect(U8 pin);





#endif //#ifndef NX1_PWM_H
