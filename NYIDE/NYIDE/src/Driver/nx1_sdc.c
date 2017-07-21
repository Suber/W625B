#include "nx1_sdc.h"
#include "nx1_smu.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#define SD_BLOCKING_MODE
//#define SDC_DEBUG
#ifdef SDC_DEBUG
#define Dprintf  dprintf
#else
#define Dprintf(...) 
#endif

#define SDC_FIFO_THRESHOLD  SDC_FIFO_LEN
#define SDC_RETRY_COUNT 200000

//------------------------------------------------------------------//
// SDC status flag  
//------------------------------------------------------------------//
#define SDC_STS_RSP_CRC_FAIL            C_SDC_Rsp_CRC_Fail_Status
#define SDC_STS_DATA_CRC_FAIL           C_SDC_Data_CRC_Fail_Status
#define SDC_STS_RSP_TIMEOUT             C_SDC_Rsp_Timeout_Status
#define SDC_STS_DATA_TIMEOUT            C_SDC_Data_Timeout_Status
#define SDC_STS_RSP_CRC_OK              C_SDC_Rsp_CRC_OK_Status
#define SDC_STS_DATA_CRC_OK             C_SDC_Data_CRC_OK_Status
#define SDC_STS_CMD_SEND                C_SDC_CMD_Sent_Status
#define SDC_STS_DATA_END                C_SDC_Trans_Finish_Status
#define SDC_STS_FIFO_UNDERRUN           C_SDC_FIFO_Ava_Write_Status
#define SDC_STS_FIFO_OVERRUN            C_SDC_FIFO_Ava_Read_Status
#define SDC_STS_CARD_CHANGE             C_SDC_Card_DT_Change_Status

//------------------------------------------------------------------//
// SDC Command Control 
//------------------------------------------------------------------//
#define SDC_CMD_NEED_RSP                C_SDC_Rep_Wait_En
#define SDC_CMD_LONG_RSP                C_SDC_Rep_Long
#define SDC_CMD_APP_CMD                 C_SDC_APP_CMD
#define SDC_CMD_CMD_EN                  C_SDC_CMD_En

//------------------------------------------------------------------//
// SDC Read/Write Control
//------------------------------------------------------------------//
#define SDC_DATACTL_DATA_WRITE          C_SDC_Data_Write
#define SDC_DATACTL_DATA_READ           C_SDC_Data_Read
#define SDC_DATACTL_DATA_EN             C_SDC_Data_En 
#define SDC_DATACTL_FIFOTH              C_SDC_FIFO_Level_Half
#define SDC_DATACTL_FIFO_RST            C_SDC_FIFO_Reset 
#define SDC_FIFO_LEN                    8
#define BLOCK_SIZE                      512

//------------------------------------------------------------------//
// SDC Command Index
//------------------------------------------------------------------//
#define SD_GO_IDLE_STATE_CMD            0
#define SD_MMC_OP_COND_CMD              1
#define SD_ALL_SEND_CID_CMD             2
#define SD_SEND_RELATIVE_ADDR_CMD       3
#define SD_SET_DSR_CMD                  4
#define SD_SWITCH_FUNC_CMD              6
#define SD_SELECT_CARD_CMD              7
#define SD_SEND_INTERFACE_COND_CMD      8
#define SD_SEND_CSD_CMD                 9
#define SD_SEND_CID_CMD                 10
#define SD_STOP_TRANSMISSION_CMD        12
#define SD_SEND_STATUS_CMD              13
#define SD_GO_INACTIVE_STATE_CMD        15
#define SD_SET_BLOCKLEN_CMD             16
#define SD_READ_SINGLE_BLOCK_CMD        17
#define SD_READ_MULTIPLE_BLOCK_CMD      18
#define SD_WRITE_SINGLE_BLOCK_CMD       24
#define SD_WRITE_MULTIPLE_BLOCK_CMD     25
#define SD_PROGRAM_CSD_CMD              27
#define SD_ERASE_SECTOR_START_CMD       32
#define SD_ERASE_SECTOR_END_CMD         33
#define SD_ERASE_CMD                    38
#define SD_LOCK_UNLOCK_CMD              42
#define SD_APP_CMD                      55
#define SD_GET_CMD                      56
/* ACMD commands */
#define SD_SET_BUS_WIDTH_CMD            6
#define SD_STATUS_CMD                   13
#define SD_APP_OP_COND                  41
#define SD_SEND_SCR_CMD                 51

//------------------------------------------------------------------//
// SDC Argument
//------------------------------------------------------------------//
#define SD_OCR_BUSY_BIT				    0x80000000
#define SD_OCR_CCS_BIT				    0x40000000
#define SD_BUS_WIDTH_1_BIT			    0
#define SD_BUS_WIDTH_4_BIT			    2

//------------------------------------------------------------------//
// SDC Bus Width Mode Select
//------------------------------------------------------------------//
#define SDC_BUS_WIDTH_REG_WIDE_BUS      C_SDC_Bus_Width_4IO_En
#define SDC_WIDE_BUS_SUPPORT            C_SDC_Bus_Width_4IO

//------------------------------------------------------------------//
// SDC Card Type
//------------------------------------------------------------------//
#define SD_V1			                1	
#define	SD_V2			                2	
#define	SD_HC			                3

//////////////////////////////////////////////////////////////////////
//  Structure Type Define
//////////////////////////////////////////////////////////////////////
typedef struct{
	U32 NotUsed:1;	
	U32 CRC:7;
	U32 MMCardReserved1:2;
	U32 FILE_FORMAT:2;
	U32 TMP_WRITE_PROTECT:1;
	U32 PERM_WRITE_PROTECT:1;
	U32 COPY:1;
	U32 FILE_FORMAT_GRP:1;
	
	U32 Reserved2:5;
	U32 WRITE_BL_PARTIAL:1;
	U32 WRITE_BL_LEN:4;
	U32 R2W_FACTOR:3;
	U32 MMCardReserved0:2;
	U32 WP_GRP_ENABLE:1;
	
	
	U32 WP_GRP_SIZE:7;
	U32 ERASE_SECTOR_SIZE:7;
	U32 ERASE_BLK_ENABLE:1;
	U32 C_SIZE_MULT:3;
	U32 VDD_W_CURR_MAX:3;
	U32 VDD_W_CURR_MIN:3;
	U32 VDD_R_CURR_MAX:3;
	U32 VDD_R_CURR_MIN:3;
	
	U32 C_SIZE_1:2;
	U32 C_SIZE_2:10;
		
	U32 Reserved1:2;
	U32 DSR_IMP:1;
	U32 READ_BLK_MISALIGN:1;
	U32 WRITE_BLK_MISALIGN:1;
	U32 READ_BL_PARTIAL:1;
	
	U32 READ_BL_LEN:4;	
	U32 CCC:12;
	
	
	U32 TRAN_SPEED_RateUnit:3;
	U32 TRAN_SPEED_TimeValue:4;
	U32 TRAN_SPEED_Reserved:1;
	
	U32 NSAC:8;
	
	U32 TAAC_TimeUnit:3;
	U32 TAAC_TimeValue:4;
	U32 TAAC_Reserved:1;
	
	U32 Reserved0:2;
	U32 MMC_SPEC_VERS:4;
	U32 CSD_STRUCTURE:2;	
}SD_CSD_BIT_Struct;



//////////////////////////////////////////////////////////////////////
//  Static Variables
//////////////////////////////////////////////////////////////////////
static U32 CardResp[4];
static U16 SDCardRCA;
static U8 SDCardType;
static volatile U8 SDCardBusyFlag;
static volatile U32 *volatile pSDCardBuf;



//------------------------------------------------------------------//
// Send command 
// Parameter: 
//          cmd : command
//          arg : argument
//          resp: pointer to response buffer  
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDC_SendCommand(U32 cmd, U32 arg, U32 *resp)
{
    U32 sts, retry = 0;
    
    P_SDC->CLR = C_SDC_Rsp_CRC_Fail_Status | C_SDC_Rsp_Timeout_Status | C_SDC_Rsp_CRC_OK_Status | C_SDC_CMD_Sent_Status;
    
    P_SDC->ARG = arg;
    
    P_SDC->CMD = cmd | C_SDC_CMD_En;
    
    retry=SDC_RETRY_COUNT;
    
    while(retry--)
    {
        sts = P_SDC->Status;

        if(!(cmd & C_SDC_Rep_Wait_En))
        {
            if(sts & C_SDC_CMD_Sent_Status)
            {
                P_SDC->CLR = C_SDC_CMD_Sent_Status;
                return SD_OK;
            }
        }
        else
        {
            if(sts & C_SDC_Rsp_Timeout_Status)
            {
                P_SDC->CLR = C_SDC_Rsp_Timeout_Status;
                return SD_ERROR;
            }
            else if(sts & C_SDC_Rsp_CRC_Fail_Status)
            {
                P_SDC->CLR = C_SDC_Rsp_CRC_Fail_Status;
                return SD_ERROR;
            }
            else if(sts & C_SDC_Rsp_CRC_OK_Status)
            {
                P_SDC->CLR = C_SDC_Rsp_CRC_OK_Status;
               
                if(cmd & C_SDC_Rep_Long)
                {    
                    resp[0]= P_SDC->RSP_Status0;    
                    resp[1]= P_SDC->RSP_Status1;    
                    resp[2]= P_SDC->RSP_Status2;    
                    resp[3]= P_SDC->RSP_Status3;    
                }
                else
                {  
                    resp[0]= P_SDC->RSP_Status0;    
                }
                return SD_OK;
            }
        }
        //Delayms(10);
    }
   
    return SD_ERROR;
}
//------------------------------------------------------------------//
// Config data transfer 
// Parameter: 
//          len     : data length in bytes 
//          rw_mode : C_SDC_Data_Read, SDC_DATACTL_DATA_WRITE
//          timeout : timeout      
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_ConfigDataTransfer(U32 len,U8 rw_mode,U32 timeout)
{
    P_SDC->RW_Ctrl = C_SDC_FIFO_Reset;
    
    P_SDC->TimeOut = timeout*2;
    
    P_SDC->LEN = len;
    
    P_SDC->RW_Ctrl = C_SDC_Block_Size_512 | rw_mode | C_SDC_Data_En;//|SDC_DATACTL_FIFOTH;
    
    //dprintf("%x\r\n",SDC->DataCtl);
}
//------------------------------------------------------------------//
// Read data from fifo
// Parameter: 
//          block_size : block size in bytes
//          buf        : pointer to data buffer
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDC_ReadData(U32 block_size, U32 *buf)
{
    U8 len,size=SDC_FIFO_THRESHOLD * 4,i;
    U32 retry;
    
    while(block_size)
    {
        
        retry=SDC_RETRY_COUNT;
        while(!(P_SDC->Status&C_SDC_FIFO_Ava_Read_Status))
        {
            if(!retry--)
            {
                return SD_ERROR;
            }    
        } 
       
        P_SDC->CLR = C_SDC_FIFO_Ava_Read_Status;
          
        if(block_size>size)
        {
            len = SDC_FIFO_THRESHOLD;
        }
        else
        {
            len=block_size>>2;
        }        
  
        for(i=0;i<len;i++)
        {
            *buf++=  P_SDC->Data;  
        }    
        block_size-=(len<<2);
    }   
    return SD_OK; 
}
//------------------------------------------------------------------//
// Write data from fifo
// Parameter: 
//          block_size : block size in bytes
//          buf        : pointer to data buffer
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDC_WriteData(U32 block_size, U32 *buf)
{
    U8 len,size=SDC_FIFO_THRESHOLD * 4,i;
    U32 retry;
    
    while(block_size)
    {
        retry=SDC_RETRY_COUNT;
        while(!(P_SDC->Status&C_SDC_FIFO_Ava_Write_Status))
        {
            if(!retry--)
            {
                return SD_ERROR;
            }    
        }
       
        P_SDC->CLR=C_SDC_FIFO_Ava_Write_Status;
          
        if(block_size>size)
        {
            len=SDC_FIFO_THRESHOLD;
        }
        else
        {
            len=block_size>>2;
        }        
  
        for(i=0;i<len;i++)
        {
            P_SDC->Data=*buf++;  
        }    
        block_size-=(len<<2);
    }   
    return SD_OK; 
}
//------------------------------------------------------------------//
// Wait data transfer end
// Parameter: 
//          NONE
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
int SDC_CheckDataTransterEnd(void)
{
    U32 retry;
    retry=SDC_RETRY_COUNT*3;
    
    while(retry--)
    { 
        if(P_SDC->Status&C_SDC_Trans_Finish_Status)
        {
            P_SDC->CLR=C_SDC_Trans_Finish_Status;
            return SD_OK;
        }  
        if(P_SDC->Status&C_SDC_Data_Timeout_Status)
        {
            P_SDC->CLR=C_SDC_Data_Timeout_Status;
            Dprintf("SDC_CheckDataTransterEnd timeout\r\n");
    
            return SD_ERROR;
        }      
        //dprintf("%x\r\n",SDC->Status);
        //Delayms(10);  
    }
    
    Dprintf("SDC_CheckDataTransterEnd retry\r\n");
    
    return SD_ERROR;    
}
//------------------------------------------------------------------//
// SDC reset
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_Reset(void)
{
    P_SDC->CMD=C_SDC_Reset;
    
    do{
        
    }while(P_SDC->CMD& C_SDC_Reset);
}   
//------------------------------------------------------------------//
// Set SD clock as max.
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_SetMaxClk(void)
{
    P_SDC->CLK &= ~C_SDC_CLK_Div;
}
//------------------------------------------------------------------//
// ENABLE or Disable clock
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_ClkCmd(U8 cmd)
{
    if(cmd==ENABLE)
    {
        P_SDC->CLK &= (~C_SDC_CLK_Dis);
    }
    else
    {
        P_SDC->CLK |= C_SDC_CLK_Dis;
    }    
}    
//------------------------------------------------------------------//
// Check if card is inserted
// Parameter: 
//          NONE
// return value:
//          0:removed, 1:inserted
//------------------------------------------------------------------//    
U8 SDC_CheckCard(void)
{
    if(P_SDC->Status&C_SDC_Card_DT_Removed_Status)
    {
        return (0);
    }
    else
    {
        return (1);
    }    
}    
//------------------------------------------------------------------//
// Check if card is locked
// Parameter: 
//          NONE
// return value:
//          0:non-locked, 1:locked
//------------------------------------------------------------------//    
U8 SDC_CheckWP(void)
{
    if(P_SDC->Status&C_SDC_Card_WP_Status)
    {
        return (1);
    }
    else
    {
        return (0);
    }    
}    
//------------------------------------------------------------------//
// Set SDC mask register
// Parameter: 
//          msak:mask value
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_IntMask(U32 mask)
{
    P_SDC->INT_Mask=mask; //P_SDC_INT_Mask
}   
//------------------------------------------------------------------//
// SDC ISR
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SDC_ISR_XXX(void)
{
    U32 sts;
    U8 i;
    //U32 *buf;
    sts=P_SDC->Status;
    
    //buf =(U32 *)SDCardBufAddr;     
    //dprintf("%x,%x,%x,%d\r\n",sts,&SDC->Data,(U32)buf,HansDebug2);

    if(sts&C_SDC_FIFO_Ava_Read_Status)
    {
        for(i=0;i<SDC_FIFO_THRESHOLD;i++)
        {
            *pSDCardBuf++ =  P_SDC->Data;  
        }    
    }
    if(sts&C_SDC_FIFO_Ava_Write_Status)
    {
        for(i=0;i<SDC_FIFO_THRESHOLD;i++)
        {
            P_SDC->Data = *pSDCardBuf++;    
        }    
    }
    if(sts&C_SDC_Data_Timeout_Status)
    {
        Dprintf("SDC_STS_DATA_TIMEOUT........\r\n");
    }
    if(sts&C_SDC_Trans_Finish_Status)
    {
        P_SDC->CLR = C_SDC_Trans_Finish_Status;
        SDCardBusyFlag = SD_OK;
    }       
}    
//------------------------------------------------------------------//
// SDCard int
// Parameter: 
//          NONE
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDCardInit(void)
{
    U32 sts,ocr;
    U8 spec20;
    SD_CSD_BIT_Struct *CSDPtr;
    
    SDC_Reset();
//    
//    SDC->ClkCtl=0x7f|0x80;
//    
//    Delay(1000);
    
    SDC_ClkCmd(ENABLE);
     
    sts=SDC_SendCommand(SD_GO_IDLE_STATE_CMD, 0, 0);
    Dprintf("SD_GO_IDLE_STATE_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }    
    sts=SDC_SendCommand(SD_SEND_INTERFACE_COND_CMD| SDC_CMD_NEED_RSP, 0x1AA, 0);
    Dprintf("SD_SEND_INTERFACE_COND_CMD:%d\r\n",sts);
    
    if(sts)
    {
        spec20=0;
    }
    else
    {
        spec20=1;
    }        
    ocr=0;
    do
    {
        sts=SDC_SendCommand(SD_APP_CMD | SDC_CMD_NEED_RSP, 0, CardResp);
        Dprintf("SD_APP_CMD:%d\r\n",sts);
        if(sts)
        {
            return SD_ERROR;
        }    
        if(spec20)
        {
           ocr|= SD_OCR_CCS_BIT;
        }    
        sts=SDC_SendCommand(SD_APP_OP_COND | SDC_CMD_APP_CMD | SDC_CMD_NEED_RSP,ocr, CardResp);
        Dprintf("SD_APP_OP_COND:%d\r\n",sts);
        if(sts)
        {
            return SD_ERROR;
        }    
        ocr=CardResp[0];
       
        //dprintf("%x\r\n",CardResp[0]);
    }while(!(ocr&SD_OCR_BUSY_BIT));
    
    if(spec20)
    {
        if(ocr&SD_OCR_CCS_BIT)
        {
            SDCardType=SD_HC;    
        }
        else
        {
            SDCardType=SD_V2; 
        }        
    }
    else
    {
        SDCardType=SD_V1;
    }        
    sts=SDC_SendCommand(SD_ALL_SEND_CID_CMD | SDC_CMD_NEED_RSP | SDC_CMD_LONG_RSP, 0, CardResp);
    Dprintf("SD_ALL_SEND_CID_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }    
    sts=SDC_SendCommand(SD_SEND_RELATIVE_ADDR_CMD | SDC_CMD_NEED_RSP, 0, CardResp);
    SDCardRCA=CardResp[0]>>16;
    Dprintf("SD_SEND_RELATIVE_ADDR_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }    
    sts=SDC_SendCommand(SD_SEND_CSD_CMD | SDC_CMD_NEED_RSP | SDC_CMD_LONG_RSP, SDCardRCA<<16, CardResp);
    Dprintf("SD_SEND_CSD_CMD:%d\r\n",sts);
    
    CSDPtr=(SD_CSD_BIT_Struct *)CardResp;
    Dprintf("%x,%x\r\n",CSDPtr->TRAN_SPEED_RateUnit,CSDPtr->TRAN_SPEED_TimeValue);
    
    sts=SDC_SendCommand(SD_SELECT_CARD_CMD | SDC_CMD_NEED_RSP, SDCardRCA<<16, CardResp);
    Dprintf("SD_SELECT_CARD_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }    
    sts=SDC_SendCommand(SD_APP_CMD | SDC_CMD_NEED_RSP, SDCardRCA<<16, CardResp);
    Dprintf("SD_APP_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }        
    sts=SDC_SendCommand(SD_SET_BUS_WIDTH_CMD | SDC_CMD_APP_CMD | SDC_CMD_NEED_RSP, SD_BUS_WIDTH_4_BIT, CardResp);
    Dprintf("SD_SET_BUS_WIDTH_CMD:%d\r\n",sts);
    if(sts)
    {
        return SD_ERROR;
    }    
    
    sts=SDC_SendCommand(SD_SET_BLOCKLEN_CMD | SDC_CMD_NEED_RSP, BLOCK_SIZE, CardResp);
    Dprintf("SD_SET_BLOCKLEN_CMD:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }    
    P_SDC->BUS_Mode=SDC_BUS_WIDTH_REG_WIDE_BUS;

#ifndef SD_BLOCKING_MODE    
    SDC_IntMask(SDC_STS_DATA_END|SDC_STS_FIFO_UNDERRUN|SDC_STS_FIFO_OVERRUN|SDC_STS_DATA_TIMEOUT);
#endif
    SDC_SetMaxClk();
    SDC_ClkCmd(ENABLE);
        
//    sts=SDC_SendCommand(SD_SWITCH_FUNC_CMD | SDC_CMD_REG_NEED_RSP, 0x80FFFFF1, CardResp);
//    Dprintf("CMD6:%d\r\n",sts);
//    
//    sts=SDC_SendCommand(SD_SELECT_CARD_CMD | SDC_CMD_REG_NEED_RSP, 0, CardResp);
//    Dprintf("SD_SELECT_CARD_CMD:%d\r\n",sts);
//    
//     sts=SDC_SendCommand(SD_SEND_CSD_CMD | SDC_CMD_REG_NEED_RSP | SDC_CMD_REG_LONG_RSP, SDCardRCA<<16, CardResp);
//    Dprintf("SD_SEND_CSD_CMD:%d\r\n",sts);
//    
//    CSDPtr=(SD_CSD_BIT_Struct *)CardResp;
//    Dprintf("%x,%x\r\n",CSDPtr->TRAN_SPEED_RateUnit,CSDPtr->TRAN_SPEED_TimeValue);
//    
    Dprintf("SDCardType:%d\r\n",SDCardType); 

    return SD_OK;
}    
//------------------------------------------------------------------//
// SDCard read single block
// Parameter: 
//          buf: pointer to data buffer
//          block:block index
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDCardReadBlock(U32* buf,U32 block)
{
    U8 sts;
    
    if (SDCardType == SD_HC) 
    {
	    block = block; 
	}
    else if(SDCardType == SD_V1 || SDCardType == SD_V2) 
    {
        block = block << 9; 
    }	
		
    SDC_ConfigDataTransfer(BLOCK_SIZE,SDC_DATACTL_DATA_READ,0x10000);
    
    SDCardBusyFlag=SD_BUSY;
    pSDCardBuf=buf;

    sts=SDC_SendCommand(SD_READ_SINGLE_BLOCK_CMD | SDC_CMD_NEED_RSP, block, CardResp);
    Dprintf("SD_READ_SINGLE_BLOCK_CMD:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }    

#ifdef SD_BLOCKING_MODE    
    sts=SDC_ReadData(BLOCK_SIZE,buf);
    Dprintf("SDC_ReadData:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }    
    
    sts=SDC_CheckDataTransterEnd();
    Dprintf("SDC_CheckDataTransterEnd:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }
#endif        
    return SD_OK;
}    
//------------------------------------------------------------------//
// Check if SDCard transfer busy 
// Parameter: 
//          NONE
// return value:
//          SD_OK,SD_BUSY
//------------------------------------------------------------------//    
U8 SDCardBusy(void){
#ifdef SD_BLOCKING_MODE
    return (SD_OK);
#else    
    //dprintf("%x\r\n",SDC->Status);
    return (SDCardBusyFlag);
#endif

}    
//------------------------------------------------------------------//
// SDCard write single block
// Parameter: 
//          buf: pointer to data buffer
//          index:block index
// return value:
//          SD_OK,SD_ERROR
//------------------------------------------------------------------//    
U8 SDCardWriteBlock(U32* buf,U32 block)
{
    U8 sts;
    
    if (SDCardType == SD_HC) 
    {
	    block = block; 
	} 
    else if(SDCardType == SD_V1 || SDCardType == SD_V2) 
    {
        block = block << 9; 
    }	
		
    SDC_ConfigDataTransfer(BLOCK_SIZE,SDC_DATACTL_DATA_WRITE,0x1000000);
    
    SDCardBusyFlag=SD_BUSY;
    pSDCardBuf=buf;
    
    sts=SDC_SendCommand(SD_WRITE_SINGLE_BLOCK_CMD | SDC_CMD_NEED_RSP, block, CardResp);
 
    Dprintf("SD_WRITE_SINGLE_BLOCK_CMD:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }    
       
#ifdef SD_BLOCKING_MODE    
    sts=SDC_WriteData(BLOCK_SIZE,buf);
    Dprintf("SDC_WriteData:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }    
    
    sts=SDC_CheckDataTransterEnd();
    Dprintf("SDC_CheckDataTransterEnd:%d\r\n",sts);  
    if(sts)
    {
        return SD_ERROR;
    }
#endif        
    return SD_OK;
}    
