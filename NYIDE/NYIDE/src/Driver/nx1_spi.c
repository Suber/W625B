#include "nx1_spi.h"
#include "nx1_gpio.h"
#include "nx1_wdt.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define SPI_ADDR_BYTE               3                           
#define SPI_DATA_NUM_OFFSET         16



//////////////////////////////////////////////////////////////////////
//  Static Variables
//////////////////////////////////////////////////////////////////////
static U8 SpiCmdBusMode, SpiAddrBusMode, SpiDataBusMode;



//------------------------------------------------------------------//
// SPI Init
// Parameter: 
//          divide:SPI_CLKDIV_1,SPI_CLKDIV_2...
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_Init(U32 divide)
{
    SPI_TypeDef *SPI = P_SPI0;
        
    SPI->Ctrl = divide | C_SPI_Pol_Low | C_SPI_Phase_1st_Edge;
    GPIO_Write(GPIOB,0,1);
}
//------------------------------------------------------------------//
// Set SPI bus mode
// Parameter: 
//          cmd_bus_mode  : SPI_DATA_1,SPI_DATA_2,SPI_DATA_4
//          addr_bus_mode : SPI_DATA_1,SPI_DATA_2,SPI_DATA_4
//          data_bus_mode : SPI_DATA_1,SPI_DATA_2,SPI_DATA_4
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_SetBusMode(U8 cmd_bus_mode,U8 addr_bus_mode,U8 data_bus_mode)
{
    SpiCmdBusMode = cmd_bus_mode;
    SpiAddrBusMode= addr_bus_mode;
    SpiDataBusMode= data_bus_mode;
}
//------------------------------------------------------------------//
// Send command+address
// Parameter: 
//          cmd  : command code
//          addr : address
//          dummy: dummy byte 
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_SendCmdAddr(U8 cmd,U32 addr,U8 dummy)
{
    SPI_TypeDef *SPI = P_SPI0;
    
    SPI->Data=cmd;
    
    SPI->Ctrl &= (~(C_SPI_FIFO_Length | C_SPI_DataMode));
    SPI->Ctrl |= C_SPI_Tx_Start | SpiCmdBusMode;
    
    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy);
   
    SPI->Data=((addr&0xFF)<<16)|(addr&0xFF00)|(addr>>16);
    SPI->Ctrl &= (~(C_SPI_FIFO_Length | C_SPI_DataMode));
    SPI->Ctrl |= (C_SPI_Tx_Start |((SPI_ADDR_BYTE+dummy-1)<<SPI_DATA_NUM_OFFSET)| SpiAddrBusMode); 

    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy);
    
    SPI->Ctrl &= (~(C_SPI_DataMode));
    SPI->Ctrl |= SpiDataBusMode;
} 
//------------------------------------------------------------------//
// Send command
// Parameter: 
//          cmd : command code
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_SendCmd(U8 cmd)
{
    SPI_TypeDef *SPI = P_SPI0;
       
    SPI->Data=cmd;
       
    SPI->Ctrl &= (~(C_SPI_FIFO_Length|C_SPI_DataMode));
    SPI->Ctrl |= C_SPI_Tx_Start| SpiCmdBusMode;
    
    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy); 
    
    
    SPI->Ctrl &= (~C_SPI_DataMode);
    SPI->Ctrl |= SpiDataBusMode;
} 
//------------------------------------------------------------------//
// Send data
// Parameter: 
//          data : data buffer
//          len  : length to write(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_SendData(U32 *data,U8 len)
{
    U8 i,j,k;
    SPI_TypeDef *SPI = P_SPI0;

    do{
        if(len>SPI_FIFO_SIZE)
        {
            k=SPI_FIFO_SIZE;
        }
        else
        {    
            k=len;
        }
        j=(k+3)>>2;
        for(i=0;i<j;i++)
        {
            SPI->Data=*data++;    
        }    
        SPI->Ctrl &= (~C_SPI_FIFO_Length);
        SPI->Ctrl |= (C_SPI_Tx_Start | ((k-1)<<SPI_DATA_NUM_OFFSET));
        
        do{
        }while(SPI->Ctrl&C_SPI_Tx_Busy);

        len-=k;
    }while(len);
}     
//------------------------------------------------------------------//
// Receive data
// Parameter: 
//          data : data buffer
//          len  : length to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_ReceiveData(U32 *data,U8 len)
{
    U8 i,j,k;
    SPI_TypeDef *SPI = P_SPI0;
        
    do{
        if(len>SPI_FIFO_SIZE)
        {
            k=SPI_FIFO_SIZE;
        }
        else
        {    
            k=len;
        }
        
        SPI->Ctrl &= (~C_SPI_FIFO_Length);        
        SPI->Ctrl |= (C_SPI_Rx_Start | ((k-1)<<SPI_DATA_NUM_OFFSET));
       
        do{
        }while(SPI->Ctrl&C_SPI_Rx_Busy); 
        
        j=(k+3)>>2;
        
        for(i=0;i<j;i++)
        {
            *data++=SPI->Data;
        }    
        len-=k;
    }while(len);
}    
//------------------------------------------------------------------//
// Enable or Disable WREN
// Parameter: 
//          cmd : ENABLE or DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_WRENCmd(U8 cmd)
{
    U8 wren;
    
    if(cmd==ENABLE)
    {
        wren=SPI_WREN_CMD;
    }
    else
    {
        wren=SPI_WRDI_CMD;
    }
    
    GPIO_Write(GPIOB,0,0);
    
    SPI_SendCmd(wren);
    
    GPIO_Write(GPIOB,0,1);
    
}    
//------------------------------------------------------------------//
// Sector erase
// Parameter: 
//          addr:sector address
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_SectorErase(U32 addr)
{
    GPIO_Write(GPIOB,0,0);
    
    SPI_SendCmdAddr(SPI_SECTOR_ERASE_CMD,addr,0);
    
    GPIO_Write(GPIOB,0,1);
}    
//------------------------------------------------------------------//
// Block erase
// Parameter: 
//          addr:block address
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_BlockErase(U32 addr)
{
    GPIO_Write(GPIOB,0,0);
    
    SPI_SendCmdAddr(SPI_BLOCK_ERASE_CMD,addr,0);
    
    GPIO_Write(GPIOB,0,1);
}   
//------------------------------------------------------------------//
// Reset SPI 
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_Reset(void)
{
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
    
    
    GPIO_Write(GPIOB,0,0);
      
    SPI_SendCmd(SPI_RESETEN_CMD);
   
    GPIO_Write(GPIOB,0,1);
    
    GPIO_Write(GPIOB,0,0);
    
    SPI_SendCmd(SPI_RESET_CMD);
   
    GPIO_Write(GPIOB,0,1);
}         
//------------------------------------------------------------------//
// Enable or Disable QE  
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_QECmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        SPI_WriteStsReg(2,SPI_QE_BIT);
    }else
    {
        SPI_WriteStsReg(2,0);
    }            
}     
//------------------------------------------------------------------//
// Enable or Disable 4-byte address mode  
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_4ByteAddrCmd(U8 cmd)
{
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
     
    GPIO_Write(GPIOB,0,0);
      
    if(cmd==ENABLE)
    {
        SPI_SendCmd(SPI_ENTER_4BYTE_ADDR_CMD);
    }else
    {
        SPI_SendCmd(SPI_EXIT_4BYTE_ADDR_CMD);
    }            
    GPIO_Write(GPIOB,0,1);
}       
//static U8 tempBuf[64];
//------------------------------------------------------------------//
// Burst read from SPI
// Parameter: 
//          buf  : data buffer
//          addr : start address
//          len  : length to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_BurstRead(U8 *buf,U32 addr,U16 len)
{
    U8 read_len,i;
    U16 offset=0;
    U8 temp[SPI_FIFO_SIZE];  
    
    GPIO_Write(GPIOB,0,0);

#if _SPI_ACCESS_MODE==SPI_1_1_1_MODE
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);  
    SPI_SendCmdAddr(SPI_READ_CMD,addr,0);  
#elif _SPI_ACCESS_MODE==SPI_1_2_2_MODE    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_2,SPI_DATA_2);  
    SPI_SendCmdAddr(SPI_2READ_CMD,addr,1);  
#elif _SPI_ACCESS_MODE==SPI_1_4_4_MODE
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_4,SPI_DATA_4); 
    SPI_SendCmdAddr(SPI_4READ_CMD,addr,3);
#endif       
//    
    while(len)
    {
        if(len>SPI_FIFO_SIZE)
        {
            read_len=SPI_FIFO_SIZE;
        }
        else
        {
            read_len=len;
        }        
           
       //if(read_len!=SPI_FIFO_SIZE){
       if(1)
       {    
            SPI_ReceiveData((U32 *)temp,read_len);
            for(i=0;i<read_len;i++)
            {
                buf[offset+i]=temp[i];
            }    
       }
       else
       {    
            SPI_ReceiveData((U32 *)(buf+offset),read_len);
       }
       len-=read_len;
       offset+=read_len;
    }    
    GPIO_Write(GPIOB,0,1);
}   
//------------------------------------------------------------------//
// Set start address
// Parameter: 
//          addr:start address
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_FastReadStart(U32 addr)
{   
    GPIO_Write(GPIOB,0,0);
    
#if _SPI_ACCESS_MODE==SPI_1_1_1_MODE
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);  
    SPI_SendCmdAddr(SPI_READ_CMD,addr,0);  
#elif _SPI_ACCESS_MODE==SPI_1_2_2_MODE    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_2,SPI_DATA_2);  
    SPI_SendCmdAddr(SPI_2READ_CMD,addr,1);  
#elif _SPI_ACCESS_MODE==SPI_1_4_4_MODE
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_4,SPI_DATA_4); 
    SPI_SendCmdAddr(SPI_4READ_CMD,addr,3);
#endif    
    
//    SPI0->Ctl&=(~SPI_DATA_NUM_SEL);
//        
//    SPI0->Ctl|=((SPI_FIFO_SIZE-1)<<SPI_DATA_NUM_OFFSET);
}    
//------------------------------------------------------------------//
// Read data
// Parameter: 
//          *buf:data buffer,alignment of 4
//          len:legth to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//
void SPI_FastRead(U32 *buf,U32 len)
{
    U8 last_bytes,i;
    
    last_bytes= len&(SPI_FIFO_SIZE-1);
    len-=last_bytes;
    
    if(len){
        P_SPI0->Ctrl&=(~C_SPI_FIFO_Length);
        P_SPI0->Ctrl|=((SPI_FIFO_SIZE-1)<<SPI_DATA_NUM_OFFSET);   
        while(len){
            //GPIOB->Data&=(~(1<<10));
            P_SPI0->Ctrl|=C_SPI_Rx_Start;    
            do{
            }while(P_SPI0->Ctrl&C_SPI_Rx_Busy); 
            
            *buf++=P_SPI0->Data;
            *buf++=P_SPI0->Data;
            *buf++=P_SPI0->Data;
            *buf++=P_SPI0->Data;
            // GPIOB->Data|=(1<<10);
            len-=SPI_FIFO_SIZE;
        };  
    }
  
    if(last_bytes){
        P_SPI0->Ctrl&=(~C_SPI_FIFO_Length);
        P_SPI0->Ctrl|=C_SPI_Rx_Start|((last_bytes-1)<<SPI_DATA_NUM_OFFSET); 
         
        do{
        }while(P_SPI0->Ctrl&C_SPI_Rx_Busy);       
        
        last_bytes=last_bytes>>2;
        
        for(i=0;i<last_bytes;i++){
            *buf++=P_SPI0->Data;
        }    
    }    
}    
//------------------------------------------------------------------//
// Stop read
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_FastReadStop(void)
{
    GPIO_Write(GPIOB,0,1);
} 
//------------------------------------------------------------------//
// Erase Spi
// Parameter: 
//          addr:start address
//          len:legth to erase(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_BurstErase(U32 addr,U32 len)
{
    U32 erase_len;
    U32 Data;
    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
    
    while(len)
    {
//        if(len>SPI_SECTOR_SIZE){
//            erase_len=SPI_SECTOR_SIZE;
//        }else{
//            erase_len=len;
//        }                
        if(len>SPI_BLOCK_SIZE)
        {
            erase_len=SPI_BLOCK_SIZE;
        }
        else
        {
            erase_len=len;
        }        
        SPI_WRENCmd(ENABLE);
        
        //SPI_SectorErase(addr);
        SPI_BlockErase(addr);
        
        GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(SPI_READ_STS_REG1_CMD);
        do{
            SPI_ReceiveData(&Data,1);        
        }while(Data&0x1);
        
        GPIO_Write(GPIOB,0,1);
        WDT_Clear();
    
        len-=erase_len;
        addr+=erase_len;
    }    
}    
//------------------------------------------------------------------//
// Burst write to SPI
// Parameter: 
//          buf:data buffer
//          addr:start address
//          len:legth to write(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_BurstWrite(U8 *buf,U32 addr,U16 len)
{
    U8 write_len,i;
    U16 page_size;
    U16 offset=0;
    U32 Data;
    U8 temp[SPI_FIFO_SIZE];
    
    while(len)
    {
        SPI_WRENCmd(ENABLE);       
        SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
        
        
        GPIO_Write(GPIOB,0,0);
        SPI_SendCmdAddr(SPI_PAGE_PRG_CMD,addr+offset,0);
        
        page_size=SPI_PAGE_SIZE-((addr+offset)&(SPI_PAGE_SIZE-1));
        
        if(page_size>len)
        {
            page_size=len;    
        }    
        
        while(page_size)
        {           
            if(page_size>SPI_FIFO_SIZE)
            {
                write_len=SPI_FIFO_SIZE;
            }
            else
            {
                write_len=page_size;
            }        
            for(i=0;i<write_len;i++)
            {
                temp[i]=buf[offset+i];
            }    
            SPI_SendData((U32 *)temp,write_len);
            
            offset+=write_len;
            page_size-=write_len;
            len-=write_len;
        }            
        GPIO_Write(GPIOB,0,1);
        
        SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
        GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(SPI_READ_STS_REG1_CMD);
        do{
            SPI_ReceiveData(&Data,1);
        
        }while(Data&0x1);        
        GPIO_Write(GPIOB,0,1);
    
    }
}    
//------------------------------------------------------------------//
// SPI Init
// Parameter: 
//          divide:SPI_CLKDIV_1,SPI_CLKDIV_2...
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI1_Init(U32 divide)
{
    SPI_TypeDef *SPI = P_SPI1;
        
    SPI->Ctrl=divide | C_SPI_Pol_Low | C_SPI_Phase_1st_Edge;
}
//------------------------------------------------------------------//
// Send command
// Parameter: 
//          cmd:command code
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI1_SendCmd(U8 cmd)
{
    SPI_TypeDef *SPI = P_SPI1;
    
    SPI->Ctrl  = cmd;    
    SPI->Ctrl &= (~(C_SPI_FIFO_Length | C_SPI_DataMode));
    SPI->Ctrl |= C_SPI_Tx_Start | SpiCmdBusMode;    
    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy); 
    
    
    SPI->Ctrl &= (~C_SPI_DataMode);
    SPI->Ctrl |= SpiDataBusMode;
} 
//------------------------------------------------------------------//
// Send command+address
// Parameter: 
//          cmd  : command code
//          addr : address
//          dummy: dummy byte 
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI1_SendCmdAddr(U8 cmd,U32 addr,U8 dummy)
{
    SPI_TypeDef *SPI = P_SPI1;
    
    SPI->Data=cmd;    
    SPI->Ctrl &= (~(C_SPI_FIFO_Length));
    SPI->Ctrl |= C_SPI_Tx_Start;    
    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy); 
   
    SPI->Data = ((addr&0xFF)<<16) | (addr&0xFF00) | (addr>>16);
    SPI->Ctrl &= (~(C_SPI_FIFO_Length));
    SPI->Ctrl |= (C_SPI_Tx_Start | ((SPI_ADDR_BYTE+dummy-1)<<SPI_DATA_NUM_OFFSET)); 
    do{
    }while(SPI->Ctrl&C_SPI_Tx_Busy);
} 
//------------------------------------------------------------------//
// Send data
// Parameter: 
//          data : data buffer
//          len  : length to write(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI1_SendData(U32 *data,U8 len)
{
    U8 i,j,k;
    SPI_TypeDef *SPI = P_SPI1;

    do{
        if(len>SPI1_FIFO_SIZE)
        {
            k=SPI1_FIFO_SIZE;
        }
        else
        {    
            k=len;
        }
        j=(k+3)>>2;
        for(i=0;i<j;i++)
        {
            SPI->Data = *data++;    
        }    
    
        SPI->Ctrl &= (~C_SPI_FIFO_Length);
        SPI->Ctrl |= (C_SPI_Tx_Start | ((k-1)<<SPI_DATA_NUM_OFFSET));        
        do{
        }while(SPI->Ctrl&C_SPI_Tx_Busy); 

        len-=k;
    }while(len);
} 
//------------------------------------------------------------------//
// Receive data
// Parameter: 
//          data:data buffer
//          len:length to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI1_ReceiveData(U32 *data,U8 len)
{
    U8 i,j,k;
    SPI_TypeDef *SPI = P_SPI1;
    
    do{
        if(len>SPI1_FIFO_SIZE)
        {
            k=SPI1_FIFO_SIZE;
        }
        else
        {    
            k=len;
        }
        SPI->Ctrl&=(~C_SPI_FIFO_Length);
        SPI->Ctrl|=(C_SPI_Rx_Start | ((k-1)<<SPI_DATA_NUM_OFFSET));
        do{
        }while(SPI->Ctrl&C_SPI_Rx_Busy); 
        j=(k+3)>>2;
        for(i=0;i<j;i++)
        {
            *data++=SPI->Data;
        }    
        len-=k;
    }while(len);
}   
//------------------------------------------------------------------//
// Write status register(1 byte)
// Parameter: 
//          reg : 1,2,3
//          data:data to write
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_WriteStsReg(U8 reg,U32 data)
{
    U32 buf;
    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
    
    SPI_WRENCmd(ENABLE);
    
    GPIO_Write(GPIOB,0,0);     
    if(reg==1) 
    {
        SPI_SendCmd(SPI_WRITE_STS_REG1_CMD);
    }else if(reg==2) 
    {
        SPI_SendCmd(SPI_WRITE_STS_REG2_CMD);
    }else if(reg==3) 
    {
        SPI_SendCmd(SPI_WRITE_STS_REG3_CMD);
    }            
    SPI_SendData(&data,1);
    
    GPIO_Write(GPIOB,0,1);
    
    GPIO_Write(GPIOB,0,0);
        
    SPI_SendCmd(SPI_READ_STS_REG1_CMD);
    do{
        SPI_ReceiveData(&buf,1);
    }while(buf&0x1);
    
    GPIO_Write(GPIOB,0,1);
}   
//------------------------------------------------------------------//
// Write status register(2 byte)
// Parameter: 
//          data:data to write
// return value:
//          NONE
//------------------------------------------------------------------//    
void SPI_WriteStsReg2(U32 data)
{
    U32 buf;
    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
    
    SPI_WRENCmd(ENABLE);
    
    GPIO_Write(GPIOB,0,0);     
   
    SPI_SendCmd(SPI_WRITE_STS_REG1_CMD);
      
    SPI_SendData(&data,2);
    
    GPIO_Write(GPIOB,0,1);
    
    GPIO_Write(GPIOB,0,0);
        
    SPI_SendCmd(SPI_READ_STS_REG1_CMD);
    do{
        SPI_ReceiveData(&buf,1);
    }while(buf&0x1);
    
    GPIO_Write(GPIOB,0,1);
} 
//------------------------------------------------------------------//
// read status register
// Parameter: 
//          reg : 1,2,3
// return value:
//          Status data
//------------------------------------------------------------------//      
U32 SPI_ReadStsReg(U8 reg)
{
    U32 Data;
    
    SPI_SetBusMode(SPI_DATA_1,SPI_DATA_1,SPI_DATA_1);
    
    GPIO_Write(GPIOB,0,0);
    
    if(reg==1) 
    {
        SPI_SendCmd(SPI_READ_STS_REG1_CMD);
    }else if(reg==2) 
    {
        SPI_SendCmd(SPI_READ_STS_REG2_CMD);    
    }else if(reg==3) 
    {
        SPI_SendCmd(SPI_READ_STS_REG3_CMD);    
    }
    
    Data=0;
        
    SPI_ReceiveData(&Data,1);
        
    GPIO_Write(GPIOB,0,1);
    
    return Data;
}    
void SPI_XipIoMode(U8 io_mode)
{
    P_SPI0->Addr_Mode &= (~C_SPI_CMD); //3Byte Addr only
    P_SPI0->Addr_Mode |= io_mode;
}