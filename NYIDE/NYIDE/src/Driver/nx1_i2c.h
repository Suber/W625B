#ifndef NX1_I2C_H
#define NX1_I2C_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : None
//  Parameter  : Timeout Threshold when I2C go into unexpected state.
//------------------------------------------------------------------//
#define  I2C_TIMEOUT_TH             2000

//------------------------------------------------------------------//
//  Function   : None
//  Parameter  : Set I2C Speed: Standard, Fast, Fast+
//------------------------------------------------------------------//
#define  I2C_CLOCK_SPEED            C_I2C_STD_SCLHi_Time



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////
extern void I2C_Init(U16 addr);
extern U32 I2C_WriteMultiByte(U16 addr, U16 len, U8 data[]);
extern U32 I2C_ReadMultiByte(U16 addr, U16 len, U8 data[]);





#endif//#ifndef NX1_I2C_H
