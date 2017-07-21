#include "nx1_uart.h"


//------------------------------------------------------------------//
// Uart Init
// Parameter: 
//          uartx : UART
//          baud_rate : UART_BAUDRATE_115200~UART_BAUDRATE_4800
// return value:
//          NONE
//------------------------------------------------------------------//    
void UART_Init(UART_TypeDef *uartx,U16 baud_rate)
{
    uartx->BaudRate = baud_rate;
}
//------------------------------------------------------------------//
// Write single data to fifo
// Parameter: 
//          uartx:UART
//          data: data to send          
// return value:
//          NONE
//------------------------------------------------------------------//        
void UART_SendData(UART_TypeDef *uartx,U8 data)
{
    uartx->Data=data;
}    
//------------------------------------------------------------------//
// Enable or Disable Tx/Rx interrupt
// Parameter: 
//          uartx:UART
//          irq:UART_RXFIFO_NOEMPTY,UART_TX_DONE  
//          cmd:ENABLE,DISABLE         
// return value:
//          NONE
//------------------------------------------------------------------//        
void UART_IntCmd(UART_TypeDef *uartx,U8 irq,U8 cmd)
{
    if(cmd==ENABLE)
    {
        uartx->Ctrl|=irq;
    }
    else
    {
        uartx->Ctrl&=(~irq);
    }    
}    
//------------------------------------------------------------------//
// Read single data from fifo
// Parameter: 
//          uartx:UART       
// return value:
//          received data
//------------------------------------------------------------------//        
U8 UART_ReceiveData(UART_TypeDef *uartx)
{
    return (uartx->Data&0xFF);
}    
//------------------------------------------------------------------//
// Wait TX finish
// Parameter: 
//          uartx:UART       
// return value:
//          None
//------------------------------------------------------------------//        
void UART_WaitTXDone(UART_TypeDef *uartx)
{
    do{        
    }while (!(uartx->Flag & C_UART_Tx_FIFO_Empty_Flag));
    //uartx->Flag = C_UART_Tx_FIFO_Empty_Flag;
}
//------------------------------------------------------------------//
// Clear TX Done Status
// Parameter: 
//          uartx : UART
//          sts   : clear flag
// return value:
//          None
//------------------------------------------------------------------//        
void UART_ClrTXDoneSts(UART_TypeDef *uartx)
{
    uartx->Flag = C_UART_Tx_FIFO_Empty_Flag;
}  
//------------------------------------------------------------------//
// Get Status
// Parameter: 
//          uartx:UART       
// return value:
//          FIFO status
//------------------------------------------------------------------//        
U8 UART_GetFIFOSts(UART_TypeDef *uartx)
{
    U8 sts;

    sts= uartx->Ctrl & uartx->Flag;
    return sts;
}  
