#ifndef NX1_TIMER_H
#define NX1_TIMER_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : TIMER_Init, TIMER_Cmd, TIMER_SetClkSrc, TIMER_IntCmd
//  Parameter  : timerx
//------------------------------------------------------------------//
#define TIMER0                      P_TIMER0
#define TIMER1                      P_TIMER1
#define TIMER2                      P_TIMER2
#define PWMATIMER                   ((TIMER_TypeDef *)PWMA_BASE)
#define PWMBTIMER                   ((TIMER_TypeDef *)PWMB_BASE)

//------------------------------------------------------------------//
//  Function   : TIMER_Init
//  Parameter  : divide
//------------------------------------------------------------------//
#define TIMER_CLKDIV_1              C_TMR_CLK_Div_1
#define TIMER_CLKDIV_2              C_TMR_CLK_Div_2
#define TIMER_CLKDIV_4              C_TMR_CLK_Div_4
#define TIMER_CLKDIV_8              C_TMR_CLK_Div_8
#define TIMER_CLKDIV_16             C_TMR_CLK_Div_16
#define TIMER_CLKDIV_32             C_TMR_CLK_Div_32
#define TIMER_CLKDIV_64             C_TMR_CLK_Div_64
#define TIMER_CLKDIV_128            C_TMR_CLK_Div_128

//------------------------------------------------------------------//
//  Function   : TIMER_SetClkSrc
//  Parameter  : source
//------------------------------------------------------------------//
#define TIMER_CLKSRC_HICLK          C_TMR_CLK_Src_HiCLK
#define TIMER_CLKSRC_LOCLK          C_TMR_CLK_Src_LoCLK
#define TIMER_CLKSRC_NOSYNC_EX      C_TMR_CLK_Src_ExtCLK_Async
#define TIMER_CLKSRC_SYNC_EX        C_TMR_CLK_Src_ExtCLK_Sync



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void TIMER_Init(TIMER_TypeDef *timerx,U8 divide,U16 counter);
extern void TIMER_Cmd(TIMER_TypeDef *timerx,U8 cmd);
extern void TIMER_SetClkSrc(TIMER_TypeDef *timerx,U16 source);
extern void TIMER_IntCmd(TIMER_TypeDef *timerx,U8 cmd);





#endif //#ifndef NX1_TIMER_H
