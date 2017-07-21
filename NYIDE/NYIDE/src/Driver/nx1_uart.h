#ifndef NX1_UART_H
#define NX1_UART_H

#include "nx1_reg.h"

//////////////////////////////////////////////////////////////////////
// Constant Define
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------//
//  Function   :  UART_Init, UART_SendData, UART_ReceiveData, UART_IntCmd, 
//                UART_WaitTXDone, UART_ClrTXDoneSts, UART_GetFIFOSts
//  Parameter  :  uartx
//------------------------------------------------------------------//
#define UART                        P_UART

//------------------------------------------------------------------//
//  Function   :  UART_IntCmd
//  Parameter  :  irq
//------------------------------------------------------------------//    
#define UART_RXFIFO_FULL            C_UART_Rx_FIFO_Full_Flag
#define UART_TX_DONE                C_UART_Tx_FIFO_Empty_Flag
#define UART_RXFIFO_NOEMPTY         C_UART_Rx_FIFO_NoEMPTY_Flag
 
//------------------------------------------------------------------//
//  Function   :  UART_Init
//  Parameter  :  baud_rate
//------------------------------------------------------------------//
#define UART_BAUDRATE_4800          C_UART_BaudRate_4800
#define UART_BAUDRATE_9600          C_UART_BaudRate_9600
#define UART_BAUDRATE_14400         C_UART_BaudRate_14400
#define UART_BAUDRATE_19200         C_UART_BaudRate_19200
#define UART_BAUDRATE_38400         C_UART_BaudRate_38400
#define UART_BAUDRATE_57600         C_UART_BaudRate_57600
#define UART_BAUDRATE_115200        C_UART_BaudRate_115200



//////////////////////////////////////////////////////////////////////
// External Function
//////////////////////////////////////////////////////////////////////

extern void UART_Init(UART_TypeDef *uartx,U16 baud_rate);
extern void UART_SendData(UART_TypeDef *uartx,U8 data);
extern U8   UART_ReceiveData(UART_TypeDef *uartx);
extern void UART_IntCmd(UART_TypeDef *uartx,U8 irq,U8 cmd);
extern void UART_WaitTXDone(UART_TypeDef *uartx);
extern void UART_ClrTXDoneSts(UART_TypeDef *uartx);
extern U8   UART_GetFIFOSts(UART_TypeDef *uartx);





#endif //#ifndef NX1_UART_H
