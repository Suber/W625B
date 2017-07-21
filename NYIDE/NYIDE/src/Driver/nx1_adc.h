#ifndef NX1_ADC_H
#define NX1_ADC_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : ADC_Init
//  Parameter  : trig_src
//------------------------------------------------------------------//
#define ADC_SW_TRIG                 C_ADC_Trig_Manual
#define ADC_TIMER0_TRIG             C_ADC_Trig_Timer0
#define ADC_TIMER1_TRIG             C_ADC_Trig_Timer1
#define ADC_TIMER2_TRIG             C_ADC_Trig_Timer2

//------------------------------------------------------------------//
//  Function   : ADC_Init, ADC_GetValue
//  Parameter  : channel
//------------------------------------------------------------------//
#define ADC_CH0                     0x0
#define ADC_CH1                     0x1
#define ADC_CH2                     0x2
#define ADC_CH3                     0x3
#define ADC_CH4                     0x4
#define ADC_CH5                     0x5
#define ADC_CH6                     0x6
#define ADC_CH7                     0x7
#define ADC_CH_MIC                  0x8

//------------------------------------------------------------------//
//  Function   : ADC_Init
//  Parameter  : fifo_level
//------------------------------------------------------------------//
#define ADC_FIFO_THD                4

//------------------------------------------------------------------//
//  function   : ADC_Init
//  Parameter  : fifo_level
//------------------------------------------------------------------//
#define ADC_SCAN_DISABLE            C_ADC_Single_Mode
#define ADC_SCAN_2                  C_ADC_Scan_Mode_CH10
#define ADC_SCAN_3                  C_ADC_Scan_Mode_CH210
#define ADC_SCAN_4                  C_ADC_Scan_Mode_CH3210



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void ADC_Init(U8 trig_src,U8 channel,U8 fifo_level);
extern void ADC_Cmd(U8 cmd);
extern void ADC_SwTrigStart(void);
extern void ADC_AgCmd(U8 cmd);
extern void ADC_SetGain(U16 gain);
extern U16  ADC_GetValue(U8 channel);
extern void ADC_SetScanMode(U8 mode);





#endif //#ifndef NX1_ADC_H
