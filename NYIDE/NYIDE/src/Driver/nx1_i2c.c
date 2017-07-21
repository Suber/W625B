#include "nx1_i2c.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define  I2C_WAIT_COMPLETE          0
#define  I2C_FINISH                 0
#define  I2C_ERR_ARB_LOSE           C_I2C_ARB_LOSE_Flag     //(1<<4)
#define  I2C_ERR_ACK                C_I2C_ACK               //(1<<10)
#define  I2C_ERR_NACK               (C_I2C_ACK | 0x10000)
#define  I2C_ERR_TIMEOUT            (1<<15)



//------------------------------------------------------------------//
// I2C Init
// Parameter: 
//            addr   : 7-bit address
// return value:
//          NONE
//------------------------------------------------------------------//    
void I2C_Init(U16 addr)
{
    P_I2C->Addr  = addr&C_I2C_Addr;
    P_I2C->Ctrl  = C_I2C_Mode_Master | C_I2C_Addr_Mode_7bit | C_I2C_En | \
                   C_I2C_STD_Setup_Time | C_I2C_STD_Hold_Time | \
                   C_I2C_SCL_LowDuty_50 | I2C_CLOCK_SPEED | \
                   (C_I2C_Spike_Time<<C_I2C_Spike_Time_Offset);
}
//------------------------------------------------------------------//
// Wait I2C Transmission
// Parameter:
//            None
// return value:
//            I2C_WAIT_COMPLETE, I2C_ERR_ARB_LOSE, ...
//------------------------------------------------------------------//
U32 I2C_WaitComplete(void)
{
    volatile U32 sts=0;
    U32 timeout=0;
    
    while(1)
    {
        sts = P_I2C->Status;
        if(sts & C_I2C_CMPL_Flag)
        {
            P_I2C->Status = P_I2C->Status;
            return I2C_WAIT_COMPLETE;
        }
        else if(sts & C_I2C_ARB_LOSE_Flag)
        {
            P_I2C->Status = P_I2C->Status;
            return I2C_ERR_ARB_LOSE;
        }
        else if(++timeout>I2C_TIMEOUT_TH)
        {
            P_I2C->Status = P_I2C->Status;
            return I2C_ERR_TIMEOUT;
        }
    }
}
//------------------------------------------------------------------//
// Wait I2C Read Byte
// Parameter:
//            None
// return value:
//            I2C_WAIT_COMPLETE, I2C_ERR_ARB_LOSE, ...
//------------------------------------------------------------------//
U32 I2C_WaitReadByte(void)
{
    volatile U32 sts=0;

    while(1)
    {
        sts = P_I2C->Status;
        if(sts & C_I2C_BYTE_Rx_Flag)
        {
            P_I2C->Status = P_I2C->Status;
            return I2C_WAIT_COMPLETE;
        }
        else if(sts & C_I2C_ARB_LOSE_Flag)
        {
            P_I2C->Status = P_I2C->Status;
            return I2C_ERR_ARB_LOSE;
        }
    }
}
//------------------------------------------------------------------//
// Write Multiple byte and Address is two bytes
// Parameter: 
//            addr : 
//            len  : 1~64 (4byte alignment)
//            data : write data
// return value:
//          I2C_FINISH, I2C_ERR_ARB_LOSE, ...
//------------------------------------------------------------------//    
U32 I2C_WriteMultiByte(U16 addr, U16 len, U8 data[])
{
    U16 i;
    U32 ret;

    //START + Device + Address
    P_I2C->Data = (addr>>8) & 0xFF;
    P_I2C->Data = addr & 0xFF;
    P_I2C->Mode = C_I2C_Master_Dir_Tx | C_I2C_PHA_Start | C_I2C_PHA_Addr | C_I2C_PHA_Data | 2;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    ret = I2C_WaitComplete();
    if(ret)     return ret;
    if(!(P_I2C->Status & C_I2C_ACK))    return I2C_ERR_ACK;

    //Write DATA
    for( i=0 ; len>0; len-=4)
    {
        P_I2C->Data = data[i++];
        P_I2C->Data = data[i++];
        P_I2C->Data = data[i++];
        P_I2C->Data = data[i++];
        P_I2C->Mode = C_I2C_PHA_Data | 4;
        P_I2C->Cmd  = C_I2C_CMD_Issue;

        ret = I2C_WaitComplete();
        if(ret)     return ret;
        if(!(P_I2C->Status & C_I2C_ACK))    return I2C_ERR_ACK;
    }

    //STOP
    P_I2C->Mode = C_I2C_PHA_Stop;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    ret = I2C_WaitComplete();
    if(ret)     return ret;

    return I2C_FINISH;
}
//------------------------------------------------------------------//
// Read Multiple byte and Address is two bytes
// Parameter: 
//            addr : 
//            len  : 1~64
//            data : read data (4byte alignment)
// return value:
//          I2C_FINISH, I2C_ERR_ARB_LOSE, ...
//------------------------------------------------------------------// 
U32 I2C_ReadMultiByte(U16 addr, U16 len, U8 data[])
{
    U16 i;
    U32 ret;

    //START + Device + Address
    P_I2C->Data = (addr>>8) & 0xFF;
    P_I2C->Data = addr & 0xFF;
    P_I2C->Mode = C_I2C_Master_Dir_Tx | C_I2C_PHA_Start | C_I2C_PHA_Addr | C_I2C_PHA_Data | 2;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    ret = I2C_WaitComplete();
    if(ret)     return ret;
    if(!(P_I2C->Status & C_I2C_ACK))    return I2C_ERR_ACK;

    //Re-START + Device
    P_I2C->Mode = C_I2C_Master_Dir_Rx | C_I2C_PHA_Start | C_I2C_PHA_Addr;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    ret = I2C_WaitComplete();
    if(ret)     return ret;
    if(!(P_I2C->Status & C_I2C_ACK))    return I2C_ERR_ACK;

    //Read DATA
    P_I2C->Mode = C_I2C_Master_Dir_Rx | C_I2C_PHA_Data | len;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    for( i=0 ; len>0; len--)
    {
        ret = I2C_WaitReadByte();
        if(ret)     return ret;
        if( len != 1 )
            P_I2C->Cmd  = C_I2C_CMD_Resp_ACK;
        else
            P_I2C->Cmd  = C_I2C_CMD_Resp_NACK;
        data[i++] = P_I2C->Data;
    }

    //STOP
    P_I2C->Mode = C_I2C_PHA_Stop;
    P_I2C->Cmd  = C_I2C_CMD_Issue;
    ret = I2C_WaitComplete();
    if(ret)     return ret;

    return I2C_FINISH;
}
