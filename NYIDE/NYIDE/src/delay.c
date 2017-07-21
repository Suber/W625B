#include "include.h"
#include "nx1_wdt.h"

#pragma GCC optimize ("O0")
void DelayLoop(U32 count){
    U32 i,j;
    
    for(i=0;i<count;i++){
    	WDT_Clear();
        for(j=0;j<1000;j++){
        
        }       
    }    
}   
void DelayLoop2(U32 count){
    U32 i;
    
    for(i=0;i<count;i++){
        
    }    
}   
void Delayms(U32 ms){
    U32 time;
    
    time=SysTick;
    while(1){
        if((SysTick-time)>=ms){
            break;
        }    
    }    
}   
