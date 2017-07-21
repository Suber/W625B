#include "nx1_smu.h"
#include "include.h"


//------------------------------------------------------------------//
// Select cpu clock
// Parameter: 
//          divide    : SMU_SYSCLKDIV_1~SMU_SYSCLKDIV_128
//          clock_src : SMU_SYSCLK_FROM_HICLK,SMU_SYSCLK_FROM_LOCLK
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_CpuClock(U32 divide,U8 clock_src)
{
    P_SMU->CLK_Ctrl = divide | clock_src | C_HiCLK_En;
}    
//------------------------------------------------------------------//
// Periphral reset
// Parameter: 
//          periphs:SMU_PERIPH_IR,....
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_PeriphReset(U32 periphs)
{
    P_SMU->FUNC_RST |= periphs;
}    
//------------------------------------------------------------------//
// Enable or disable periphral clock
// Parameter: 
//          periphs: SMU_PERIPH_IR,....
//          cmd    : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_PeriphClkCmd(U32 periphs,U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_SMU->FUNC_Ctrl |= periphs;
    }
    else
    {
        P_SMU->FUNC_Ctrl &= (~periphs);
    }   
}       
//------------------------------------------------------------------//
// Cpu core reset
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_CpuReset(void)
{
    P_SMU->RST_SW_Ctrl = (C_RST_SW_PROT_Value | C_RST_SW_CPU_Exe);
}    
//------------------------------------------------------------------//
// Whole chip reset
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_ChipReset(void)
{   
    P_SMU->RST_SW_Ctrl = (C_RST_SW_PROT_Value | C_RST_SW_Chip_Exe);
}   
//------------------------------------------------------------------//
// Enter Sleep mode
// Parameter: 
//          mode: SMU_GREEN_MODE, SMU_HALT_MODE, SMU_STANDBY_MODE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_Sleep(U8 mode)
{   
    P_SMU->PWR_Ctrl &= (~C_SLP_Mode);
    P_SMU->PWR_Ctrl |= mode;
    STANDBY(2);
        
    //15ms delay for XIP Flash ready
#if (_XIP_IO_MODE==XIP_1_IO_MODE) || (_XIP_IO_MODE==XIP_2_IO_MODE) || (_XIP_IO_MODE==XIP_4_IO_MODE)    
#if OPTION_HIGH_FREQ_CLK==24000000
	DelayLoop(42);
#elif OPTION_HIGH_FREQ_CLK==16000000
	DelayLoop(27);
#elif OPTION_HIGH_FREQ_CLK==12000000
	DelayLoop(21);
#else//32M
	DelayLoop(54);
#endif    
#endif
}     
//------------------------------------------------------------------//
// Get reset source
// Parameter: 
//          NONE
// return value:
//          reset source : SMU_RESETSRC_WDT, SMU_RESETSRC_POR_LVR,
//                         SMU_RESETSRC_EXT, SMU_RESETSRC_CHIP, SMU_RESETSRC_CPU 
//------------------------------------------------------------------//    
U8 SMU_GetResetSrc(void)
{
    return P_SMU->RST_Flag;
}    
//------------------------------------------------------------------//
// Clear reset source
// Parameter: 
//          value: write 1 to clear
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_ClrResetSrc(U8 value)
{
    P_SMU->RST_Flag=value;
}    
//------------------------------------------------------------------//
// Trig SWA interrupt
// Parameter: 
//          cmd: BIT_SET,BIT_RESET
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_SWATrig(U8 value)
{
    P_SMU->SW_INT=value;
}
//------------------------------------------------------------------//
// Enable or disable LDO33
// Parameter: 
//          cmd: ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_LDO33Cmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_SMU->PWR_Ctrl |= C_LDOSPI0_En;
    }
    else
    {
        P_SMU->PWR_Ctrl &= (~C_LDOSPI0_En);
    }    
}   
//------------------------------------------------------------------//
// Enable or disable DCM
// Parameter: 
//          cmd: ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_DCMCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_SMU->PWR_Ctrl |= C_CSC_Bias_En;
    }
    else
    {
        P_SMU->PWR_Ctrl &= (~C_CSC_Bias_En);
    }    
}   
//------------------------------------------------------------------//
// Enable or disable LVD
// Parameter: 
//          cmd: ENABLE,DISABLE
//          voltage: SMU_LVD_2_2V,SMU_LVD_2_4V,SMU_LVD_2_6V,SMU_LVD_3_2V,SMU_LVD_3_4V,SMU_LVD_3_6V
// return value:
//          NONE
//------------------------------------------------------------------//    
void SMU_LVDCmd(U8 cmd,U16 voltage)
{
    P_SMU->PWR_Ctrl &= (~C_LVD);
    P_SMU->PWR_Ctrl |= voltage;
    
    if(cmd==ENABLE)
    {
        P_SMU->PWR_Ctrl |= C_LVD_En;
    }
    else
    {
        P_SMU->PWR_Ctrl &= (~C_LVD_En);
    }    
}   
//------------------------------------------------------------------//
// Get LVD status
// Parameter: 
//          NONE
// return value:
//          0 : not low voltage 
//          1 : low voltage
//------------------------------------------------------------------//    
U8 SMU_GetLVDSts(void)
{
    if(P_SMU->PWR_Ctrl&C_LVD_Flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }    
}   
