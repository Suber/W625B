#ifndef NX1_DAC_H
#define NX1_DAC_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : DAC_Init, DAC_Cmd
//  Parameter  : dacx
//------------------------------------------------------------------//
#define DAC_CH0                     ((CH_TypeDef *)P_DAC_CH0_Ctrl)
#define DAC_CH1                     ((CH_TypeDef *)P_DAC_CH1_Ctrl)

//------------------------------------------------------------------//
//  Function   : DAC_Init
//  Parameter  : trig_src
//------------------------------------------------------------------//
#define DAC_MANUAL_TRIG             C_DAC_Trig_Manual
#define DAC_TIMER0_TRIG             C_DAC_Trig_Timer0
#define DAC_TIMER1_TRIG             C_DAC_Trig_Timer1
#define DAC_TIMER2_TRIG             C_DAC_Trig_Timer2

//------------------------------------------------------------------//
//  Function   : DAC_Init
//  Parameter  : fifo_level
//------------------------------------------------------------------//
#define DAC_FIFO_THD                4

//------------------------------------------------------------------//
//  Function   : None
//  Parameter  : DAC Fifo Length
//------------------------------------------------------------------//
#define DAC_FIFO_LEN                8



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void DAC_Init(CH_TypeDef * dacx,U8 trig_src,U8 fifo_level,U8 upsample);
extern void DAC_SetPPIpGain(U8 gain);
extern void DAC_Cmd(CH_TypeDef * dacx,U8 cmd);
extern void DAC_LineInCmd(U8 cmd);
extern void SetSampleRate(CH_TypeDef * dacx,U16 sample_rate);





#endif //#ifndef NX1_DAC_H
