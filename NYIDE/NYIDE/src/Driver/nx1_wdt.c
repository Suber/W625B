#include "nx1_wdt.h"


//------------------------------------------------------------------//
// WDT init
// Parameter: 
//          period:WDT_TIME_188ms,WDT_TIME_750ms
// return value:
//          NONE
//------------------------------------------------------------------//    
void WDT_Init(U8 period)
{
    P_WDT->Ctrl = period;
}  
//------------------------------------------------------------------//
// Clear wdt reset counter
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void WDT_Clear(void)
{
    P_WDT->CLR = C_WDT_CLR_Data;
}   
