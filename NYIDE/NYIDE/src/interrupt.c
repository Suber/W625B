#include <nds32_intrinsic.h>
#include "nx1_lib.h"


//------------------------------------------------------------------//
// HW ISR Default Handler
//------------------------------------------------------------------//
#pragma weak HW0_ISR   = Default_Handler
#pragma weak HW1_ISR   = Default_Handler
#pragma weak HW2_ISR   = Default_Handler
#pragma weak HW3_ISR   = Default_Handler
#pragma weak HW4_ISR   = Default_Handler
#pragma weak HW5_ISR   = Default_Handler
#pragma weak HW6_ISR   = Default_Handler
#pragma weak HW7_ISR   = Default_Handler
#pragma weak HW8_ISR   = Default_Handler
#pragma weak HW9_ISR   = Default_Handler
#pragma weak HW10_ISR  = Default_Handler
#pragma weak HW11_ISR  = Default_Handler
#pragma weak HW12_ISR  = Default_Handler
#pragma weak HW13_ISR  = Default_Handler
#pragma weak HW14_ISR  = Default_Handler
#pragma weak HW15_ISR  = Default_Handler
#pragma weak HW16_ISR  = Default_Handler
#pragma weak HW17_ISR  = Default_Handler
#pragma weak HW18_ISR  = Default_Handler
#pragma weak HW19_ISR  = Default_Handler
#pragma weak HW20_ISR  = Default_Handler
#pragma weak HW21_ISR  = Default_Handler
#pragma weak HW22_ISR  = Default_Handler
#pragma weak HW23_ISR  = Default_Handler

void HW0_ISR(void);
void HW1_ISR(void);
void HW2_ISR(void);
void HW3_ISR(void);
void HW4_ISR(void);
void HW5_ISR(void);
void HW6_ISR(void);
void HW7_ISR(void);
void HW8_ISR(void);
void HW9_ISR(void);
void HW10_ISR(void);
void HW11_ISR(void);
void HW12_ISR(void);
void HW13_ISR(void);
void HW14_ISR(void);
void HW15_ISR(void);
void HW16_ISR(void);
void HW17_ISR(void);
void HW18_ISR(void);
void HW19_ISR(void);
void HW20_ISR(void);
void HW21_ISR(void);
void HW22_ISR(void);
void HW23_ISR(void);

const void *const CPUVectorTable[24]={
HW0_ISR,
HW1_ISR,
HW2_ISR,
HW3_ISR,
HW4_ISR,
HW5_ISR,
HW6_ISR,
HW7_ISR,
HW8_ISR,
HW9_ISR,
HW10_ISR,
HW11_ISR,
HW12_ISR,
HW13_ISR,
HW14_ISR,
HW15_ISR,
HW16_ISR,
HW17_ISR,
HW18_ISR,
HW19_ISR,
HW20_ISR,
HW21_ISR,
HW22_ISR,
HW23_ISR,
};
void Default_Handler(void) __attribute__((unused));
void Default_Handler(void)
{
    //HW ISR Default Handler
    //If PC come here, HWx_ISR does not set correctly!!!
    //Please check HWx_ISR reference.
    while (1){
        WDT_Clear();
    }
}

//------------------------------------------------------------------//
// Execption Handler
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
__attribute__((unused))
void ExceptionHandler()
{
    //Exception Handler
    //If PC come here, exception catch here
    //Please Check IC Body Selection. Maybe access illegal address.
    while (1){
        WDT_Clear();
    }
}           

static U8 GieNesting=0;
void GIE_ENABLE(void)
{
    if(GieNesting){
        GieNesting--;
    }
    if(GieNesting==0){
	    __nds32__setgie_en();
	    __nds32__dsb();
    }
}

void GIE_DISABLE(void)
{
	__nds32__setgie_dis();
	__nds32__dsb();
	GieNesting++;
}
