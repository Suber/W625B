#ifndef NX1_WDT_H
#define NX1_WDT_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   :  WDT_Init
//  Parameter  :  period
//------------------------------------------------------------------//
#define WDT_TIME_188ms              C_WDT_Timeout_188ms
#define WDT_TIME_750ms              C_WDT_Timeout_750ms



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void WDT_Init(U8 period);
extern void WDT_Clear(void);





#endif //#ifndef NX1_WDT_H
