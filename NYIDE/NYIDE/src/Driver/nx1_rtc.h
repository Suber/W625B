#ifndef NX1_RTC_H
#define NX1_RTC_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : RTC_IntCmd
//  Parameter  : freq
//------------------------------------------------------------------//
#define RTC_4KHZ                    C_RTC_4KHz_IEn
#define RTC_1KHZ                    C_RTC_1KHz_IEn
#define RTC_64HZ                    C_RTC_64Hz_IEn
#define RTC_2HZ                     C_RTC_2Hz_IEn



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void RTC_IntCmd(U8 freq,U8 cmd);
extern void RTC_ClrSts(void);





#endif //#ifndef NX1_RTC_H
