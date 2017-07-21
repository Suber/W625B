#include "nx1_intc.h"
#include <nds32_intrinsic.h>


//------------------------------------------------------------------//
// Enable or Disable interrupt
// Parameter: 
//          irqs:IRQ_BIT_DAC,....
//          cmd:ENABLE,Disable
// return value:
//          NONE
//------------------------------------------------------------------//    
void INTC_IrqCmd(U32 irqs,U8 cmd)
{
    U32 old;
    
    GIE_DISABLE(); 
    old=__nds32__mfsr(NDS32_SR_INT_MASK2);
    
    if(cmd==ENABLE){
        __nds32__mtsr(old|irqs, NDS32_SR_INT_MASK2);
    }else{
        __nds32__mtsr(old&(~irqs), NDS32_SR_INT_MASK2);
    }
    GIE_ENABLE();         
}    
//------------------------------------------------------------------//
// Clear pending irq
// Parameter: 
//          irqs:IRQ_BIT_DAC,....
// return value:
//          NONE
//------------------------------------------------------------------//    
void INTC_ClrPending(U32 irqs)
{
    __nds32__mtsr(irqs, NDS32_SR_INT_PEND2);
}  
//------------------------------------------------------------------//
// Get irq pending status
// Parameter: 
//          irq:IRQ_BIT_DAC,....
// return value:
//          0: no irq pending, !0:irq pending
//------------------------------------------------------------------//    
U32 INTC_GetPending(U32 irq)
{
   return  (__nds32__mfsr(NDS32_SR_INT_PEND)&irq);
}  
//------------------------------------------------------------------//
// Set irq priority
// Parameter: 
//          irq:IRQ_BIT_DAC,....
//          priority:IRQ_PRIORITY_0(highest),IRQ_PRIORITY_1,IRQ_PRIORITY_2,IRQ_PRIORITY_3
// return value:
//          NONE
//------------------------------------------------------------------//    
void INTC_SetPriority(U32 irq,U8 priority)
{
    U32 old;
    old=__nds32__mfsr(__NDS32_REG_INT_PRI__);
    old=old&(~(3<<irq));
    __nds32__mtsr(old|(priority<<irq), __NDS32_REG_INT_PRI__);
}  
//------------------------------------------------------------------//
// Trigger SWI
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void INTC_TrigSWI(void)
{
	__nds32__mtsr(0x10000, NDS32_SR_INT_PEND);
	__nds32__dsb();
}  
//------------------------------------------------------------------//
// Clear SWI
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void INTC_ClrSWI(void)
{
	__nds32__mtsr(0x0, NDS32_SR_INT_PEND);
	__nds32__dsb();
}  