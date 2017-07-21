#ifndef NX1_IR_H
#define NX1_IR_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : IR_Init
//  Parameter  : stop
//------------------------------------------------------------------//
#define IR_STOP_HIGH                C_IR_Idle_High
#define IR_STOP_LOW                 C_IR_Idle_Low

//------------------------------------------------------------------//
//  Function   : IR_Init
//  Parameter  : divide
//------------------------------------------------------------------//
#define IR_CLKDIV_1                 C_IR_CLK_Div_1
#define IR_CLKDIV_2                 C_IR_CLK_Div_2
#define IR_CLKDIV_3                 C_IR_CLK_Div_3
#define IR_CLKDIV_4                 C_IR_CLK_Div_4
#define IR_CLKDIV_5                 C_IR_CLK_Div_5
#define IR_CLKDIV_6                 C_IR_CLK_Div_6
#define IR_CLKDIV_7                 C_IR_CLK_Div_7
#define IR_CLKDIV_8                 C_IR_CLK_Div_8
#define IR_CLKDIV_9                 C_IR_CLK_Div_9
#define IR_CLKDIV_10                C_IR_CLK_Div_10
#define IR_CLKDIV_11                C_IR_CLK_Div_11
#define IR_CLKDIV_12                C_IR_CLK_Div_12
#define IR_CLKDIV_13                C_IR_CLK_Div_13
#define IR_CLKDIV_14                C_IR_CLK_Div_14
#define IR_CLKDIV_15                C_IR_CLK_Div_15
#define IR_CLKDIV_16                C_IR_CLK_Div_16



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void IR_Init(U8 divide,U8 stop,U16 counter);
extern void IR_Cmd(U8 cmd);





#endif//#ifndef NX1_PWM_H
