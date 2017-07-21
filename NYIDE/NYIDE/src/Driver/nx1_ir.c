#include "nx1_ir.h"


//------------------------------------------------------------------//
// IR init
// Parameter: 
//          divide  : IR_CLKDIV_1~IR_CLKDIV_16
//          stop    : IR_STOP_HIGH,IR_STOP_LOW
//          counter : preload counter, 1~31, C_IR_CNT_Div
// return value:
//          NONE
//------------------------------------------------------------------//    
void IR_Init(U8 divide,U8 stop,U16 counter)
{
    P_IR->Ctrl = stop | divide | counter;
}    
//------------------------------------------------------------------//
// Enable or Disable IR
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void IR_Cmd(U8 cmd)
{
    if(cmd == ENABLE)
    {
        P_IR->Ctrl |= C_IR_Run;
    }
    else
    {
        P_IR->Ctrl &= (~C_IR_Run); //C_IR_Idle
   	} 	
}
