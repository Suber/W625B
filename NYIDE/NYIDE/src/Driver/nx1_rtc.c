#include "nx1_rtc.h"


//------------------------------------------------------------------//
// RTC interrupt enable or disable
// Parameter: 
//          freq:RTC_2HZ,RTC_64HZ,RTC_1KHZ,RTC_4KHZ
//          cmd: ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void RTC_IntCmd(U8 freq,U8 cmd)
{
    if(cmd){
        P_RTC->INT_Ctrl|=freq;
    }else{
        P_RTC->INT_Ctrl&=(~freq);
    }        
}
//------------------------------------------------------------------//
// Clear Status
// Parameter: 
//          None      
// return value:
//          None
//------------------------------------------------------------------//        
void RTC_ClrSts(void)
{
    P_RTC->Flag &= P_RTC->INT_Ctrl;
}
