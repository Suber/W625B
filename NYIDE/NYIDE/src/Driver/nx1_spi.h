#ifndef NX1_SPI_H
#define NX1_SPI_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   : SPI_SetBusMode
//  Parameter  : cmd_bus_mode, addr_bus_mode, data_bus_mode
//------------------------------------------------------------------//
#define SPI_DATA_1                  C_SPI_DataMode_1IO
#define SPI_DATA_2                  C_SPI_DataMode_2IO
#define SPI_DATA_4                  C_SPI_DataMode_4IO
                                    
//------------------------------------------------------------------//
//  Function   : SPI_Init
//  Parameter  : clk_divisor
//------------------------------------------------------------------//
#define SPI_CLKDIV_1                C_SPI_CLK_Div_1
#define SPI_CLKDIV_2                C_SPI_CLK_Div_2
#define SPI_CLKDIV_4                C_SPI_CLK_Div_4
#define SPI_CLKDIV_8                C_SPI_CLK_Div_8
#define SPI_CLKDIV_16               C_SPI_CLK_Div_16
#define SPI_CLKDIV_32               C_SPI_CLK_Div_32
#define SPI_CLKDIV_64               C_SPI_CLK_Div_64
#define SPI_CLKDIV_128              C_SPI_CLK_Div_128

//------------------------------------------------------------------//
//  Function   : None
//  Parameter  : SPI Size
//------------------------------------------------------------------//                                    
#define SPI_FIFO_SIZE               0x10
#define SPI1_FIFO_SIZE              0x08                                   
#define SPI_SECTOR_SIZE             0x1000
#define SPI_BLOCK_SIZE              0x10000
#define SPI_PAGE_SIZE               0x100

//------------------------------------------------------------------//
//  Function   : SPI_WriteStsReg
//  Parameter  : data
//------------------------------------------------------------------//                            
#define SPI_QE_BIT                  0x02

//------------------------------------------------------------------//
//  Function   : SPI_SendCmdAddr, SPI_SendCmd, SPI1_SendCmd, SPI1_SendCmdAddr
//  Parameter  : cmd
//------------------------------------------------------------------//                            
#define SPI_READ_CMD                0x03
#define SPI_PAGE_PRG_CMD            0x02
#define SPI_FAST_READ_CMD           0x80
#define SPI_2READ_CMD               0xBB
#define SPI_4READ_CMD               0xEB
#define SPI_QUAD_READ_CMD           0x6B
#define SPI_DUAL_READ_CMD           0x3B
#define SPI_WREN_CMD                0x06
#define SPI_WRDI_CMD                0x04
#define SPI_SECTOR_ERASE_CMD        0x20
#define SPI_BLOCK_ERASE_CMD         0xD8
#define SPI_READ_STS_REG1_CMD       0x5
#define SPI_READ_STS_REG2_CMD       0x35
#define SPI_READ_STS_REG3_CMD       0x15 
#define SPI_WRITE_STS_REG1_CMD      0x1
#define SPI_WRITE_STS_REG2_CMD      0x31
#define SPI_WRITE_STS_REG3_CMD      0x11
#define SPI_READ_DEVID_CMD          0x90 
#define SPI_READ_MFID_DEVID_CMD     0x9F
#define SPI_EXIT_QPI_MODE_CMD       0xff
#define SPI_RESETEN_CMD             0x66
#define SPI_RESET_CMD               0x99
#define SPI_ENTER_4BYTE_ADDR_CMD    0xB7
#define SPI_EXIT_4BYTE_ADDR_CMD     0xE9



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void SPI_Init(U32 clk_divisor);
extern void SPI_SendCmdAddr(U8 cmd,U32 addr,U8 dummy);
extern void SPI_SendData(U32 *data,U8 len);
extern void SPI_ReceiveData(U32 *data,U8 len);
extern void SPI_SendCmd(U8 cmd);
extern void SPI_WRENCmd(U8 cmd);
extern void SPI_SetBusMode(U8 cmd_bus_mode,U8 addr_bus_mode,U8 data_bus_mode);
extern void SPI_SectorErase(U32 addr);
extern void SPI_BurstWrite(U8 *buf,U32 addr,U16 len);
extern void SPI_BurstErase(U32 addr,U32 len);
extern void SPI_BurstRead(U8 *buf,U32 addr,U16 len);
extern void SPI_QECmd(U8 cmd);
extern void SPI_Reset(void);
extern void SPI_WriteStsReg(U8 reg,U32 data);
extern void SPI_WriteStsReg2(U32 data);
extern U32  SPI_ReadStsReg(U8 reg);
extern void SPI1_Init(U32 divide);
extern void SPI1_SendCmd(U8 cmd);
extern void SPI1_SendCmdAddr(U8 cmd,U32 addr,U8 dummy);
extern void SPI1_SendData(U32 *data,U8 len);
extern void SPI_XipIoMode(U8 io_mode);

extern void SPI_FastReadStart(U32 addr);
extern void SPI_FastRead(U32 *buf,U32 len);
extern void SPI_FastReadStop(void);
#endif //#ifndef NX1_SPI_H
