#ifndef NX1_SMU_H
#define NX1_SMU_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : SMU_PeriphReset, SMU_PeriphClkCmd
//  Parameter  : periphs
//------------------------------------------------------------------//
#define SMU_PERIPH_IR               C_Func_IR_En
#define SMU_PERIPH_UART             C_Func_UART_En      
#define SMU_PERIPH_SDC              C_Func_SDC_En
#define SMU_PERIPH_DAC              C_Func_DAC_En
#define SMU_PERIPH_ADC              C_Func_ADC_En
#define SMU_PERIPH_I2C              C_Func_I2C_En
#define SMU_PERIPH_RTC              C_Func_RTC_En
#define SMU_PERIPH_SPI              C_Func_SPI_En
#define SMU_PERIPH_PWM              C_Func_PWM_En
#define SMU_PERIPH_TMR              C_Func_TMR_En
#define SMU_PERIPH_GPIO             C_Func_GPIO_En

//------------------------------------------------------------------//
//  Function   : SMU_CpuClock
//  Parameter  : divide
//------------------------------------------------------------------//
#define SMU_SYSCLKDIV_1             C_CPU_CLK_DIV_1
#define SMU_SYSCLKDIV_2             C_CPU_CLK_DIV_2
#define SMU_SYSCLKDIV_4             C_CPU_CLK_DIV_4
#define SMU_SYSCLKDIV_8             C_CPU_CLK_DIV_8
#define SMU_SYSCLKDIV_16            C_CPU_CLK_DIV_16
#define SMU_SYSCLKDIV_32            C_CPU_CLK_DIV_32
#define SMU_SYSCLKDIV_64            C_CPU_CLK_DIV_64
#define SMU_SYSCLKDIV_128           C_CPU_CLK_DIV_128

//------------------------------------------------------------------//
//  Function   : SMU_GetResetSrc
//  Parameter  : return value
//------------------------------------------------------------------//
#define SMU_RESETSRC_WDT            C_RST_WDT_Flag
#define SMU_RESETSRC_POR_LVR        C_RST_POR_LVR_Flag
#define SMU_RESETSRC_EXT            C_RST_Key_Flag
#define SMU_RESETSRC_CHIP           C_RST_SW_Chip_Flag
#define SMU_RESETSRC_CPU            C_RST_SW_CPU_Flag

//------------------------------------------------------------------//
//  Function   : SMU_Sleep
//  Parameter  : mode
//------------------------------------------------------------------//
#define SMU_GREEN_MODE              C_Green_Mode     
#define SMU_HALT_MODE               C_SLP_Halt_Mode     
#define SMU_STANDBY_MODE            C_SLP_Standby_Mode     

//------------------------------------------------------------------//
//  Function   : SMU_LVDCmd
//  Parameter  : voltage
//------------------------------------------------------------------//
#define SMU_LVD_2_2V                C_LVD_2V2
#define SMU_LVD_2_4V                C_LVD_2V4
#define SMU_LVD_2_6V                C_LVD_2V6
#define SMU_LVD_3_2V                C_LVD_3V2
#define SMU_LVD_3_4V                C_LVD_3V4
#define SMU_LVD_3_6V                C_LVD_3V6

//------------------------------------------------------------------//
//  Function   : SMU_CpuClock
//  Parameter  : clock_src
//------------------------------------------------------------------//
#define SMU_SYSCLK_FROM_LOCLK       C_SYS_CLK_LoCLK                
#define SMU_SYSCLK_FROM_HICLK       C_SYS_CLK_HiCLK    



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void SMU_PeriphReset(U32 periphs);
extern void SMU_PeriphClkCmd(U32 periphs,U8 cmd);
extern void SMU_LVDCmd(U8 cmd,U16 voltage);
extern void SMU_CpuReset(void);
extern void SMU_ChipReset(void);
extern U8   SMU_GetResetSrc(void);
extern void SMU_ClrResetSrc(U8 value);
extern void SMU_SWATrig(U8 value);
extern U8   SMU_GetLVDSts(void);
extern void SMU_LDO33Cmd(U8 cmd);
extern void SMU_Sleep(U8 mode);
extern void SMU_CpuClock(U32 divide,U8 clock_src);
extern void SMU_DCMCmd(U8 cmd);





#endif //#ifndef NX1_SMU_H
