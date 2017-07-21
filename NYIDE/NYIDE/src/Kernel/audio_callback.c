/* Includes ------------------------------------------------------------------*/

#include "include.h"
#include "nx1_spi.h"
#include "nx1_dac.h"

#if _AUDIO_MODULE==1

extern U16 DacMixBuf[];
//------------------------------------------------------------------//
// Init Dac
// Parameter: 
//          chx         : DAC_CH0, DAC_CH1
//          sample_rate : sample rate
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_InitDac(CH_TypeDef *chx,U16 sample_rate)
{    
    InitDac(chx,sample_rate);
}    
//------------------------------------------------------------------//
// Dac command
// Parameter: 
//          chx         : DAC_CH0, DAC_CH1
//          cmd         : ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_DacCmd(CH_TypeDef *chx,U8 cmd)
{    
    DAC_Cmd(chx,cmd);
}    
//------------------------------------------------------------------//
// Write Dac Fifo
// Parameter: 
//          size: size to write
//          psrc16: source pointer
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_WriteDac(U8 size,U16* psrc16)
{    
    U8 i;

    if(psrc16!=0)
    {
        for(i=0;i<size;i++)
        {
            DacMixBuf[i]+=*psrc16;
            psrc16++;
        }   
    }   
}   
//------------------------------------------------------------------//
// Init ADC
// Parameter: 
//  sample_rate:sample rate
// return value:
//          NONE
//------------------------------------------------------------------//
void CB_InitAdc(U16 sample_rate)
{
    InitAdc(sample_rate);
}   
//------------------------------------------------------------------//
// ADC command
// Parameter: 
//          cmd:ENABLE,DISABLE
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_AdcCmd(U8 cmd)
{    
    ADC_Cmd(cmd);
}     
//------------------------------------------------------------------//
// Change dac sample rate
// Parameter:
//          chx         : DAC_CH0, DAC_CH1
//          sample_rate : sample rate
// return value:
//          NONE
//------------------------------------------------------------------//
void CB_ChangeSR(CH_TypeDef *chx,U16 sample_rate)
{ 
    SetSampleRate(chx,sample_rate);
}
//------------------------------------------------------------------//
// Read data from storage (called by lib)
// Parameter: 
//          mode     : storage mode
//          *cur_addr: current address
//          *buf     : data buffer
//          size     : data size to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_ReadData(U8 mode, U32 *cur_addr, U8 *buf,U16 size){
    U16 i;
    U8 *psrc;
    
    if(mode==SPI_MODE){
        SPI_BurstRead(buf,*cur_addr,size);
    }else if(mode==OTP_MODE){
        psrc=(U8 *)*cur_addr;
        for(i=0;i<size;i++){
            *buf++=  *psrc++;  
        }        
    }        
    *cur_addr+=size;
}    
//------------------------------------------------------------------//
// Write data to storage (called by lib)
// Parameter: 
//          mode     : storage mode
//          *cur_addr: current address
//          *buf     : data buffer
//          size     : data size to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_WriteData(U8 mode, U32 *cur_addr,U8 *buf,U16 size){
    
    if(mode==SPI_MODE){
  
        SPI_BurstWrite(buf,*cur_addr,size);

    }        
    *cur_addr+=size;
}    
//------------------------------------------------------------------//
// Write header to storage (called by lib)
// Parameter: 
//          mode     : storage mode
//          *cur_addr: current address
//          *buf     : data buffer
//          size     : data size to read(bytes)
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_WriteHeader(U8 mode, U32 *cur_addr,U8 *buf,U16 size){
    
    if(mode==SPI_MODE){
        SPI_BurstWrite(buf,*cur_addr,size); 
    }     
}    
//------------------------------------------------------------------//
// Get file addr from index
// Parameter: 
//          index: file index or absolute address
//          mode:OTP_MODE,SPI_MODE,SD_MODE
//          rw:1:write, 0:read
// return value:
//          address
//------------------------------------------------------------------// 
static U32 GetStartAddr(U32 index,U8 mode,U8 rw){
    U8 tempbuf[SPI_TAB_INFO_ENTRY_SIZE];
    U32 addr;
    U8 sts;
    
    if(mode==SPI_MODE){
        if(index&ADDRESS_MODE){
            addr=index&(~ADDRESS_MODE);
        }else{    
            SPI_BurstRead(tempbuf,SPI_RESERVED_SIZE-4,4);
            addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 
            
            SPI_BurstRead(tempbuf,addr+SPI_BASE_INFO_SIZE+(SPI_TAB_INFO_ENTRY_SIZE*index),SPI_TAB_INFO_ENTRY_SIZE);
            addr=addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24));
            
            addr&=(~SPI_DATA_TYPE_MASK);
            //sts=tempbuf[3]>>4;
        }    
    }else if(mode==OTP_MODE){
        addr=PlayList[index];
    }    
    return addr;
}  
//------------------------------------------------------------------//
// Set start address of file(called by lib)
// Parameter: 
//          addr       : file index or absolute address
//          mode       : OTP_MODE,SPI_MODE,SD_MODE
//          rw:1       : write, 0:read
//          *start_addr: current address
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_SetStartAddr(U32 addr,U8 mode,U8 rw,U32 *start_addr){
    *start_addr=GetStartAddr(addr,mode,rw); 
}     
//------------------------------------------------------------------//
// Get start address
// Parameter: 
//          *index:midi file index
//          *addr:Timbre start address
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_Midi_GetStartAddr(U32 *index,U32 *addr){
    U8 temp_buf[SPI_TAB_INFO_ENTRY_SIZE];
    U32 offset;
    
    SPI_BurstRead(temp_buf,SPI_RESERVED_SIZE-4,4);   
    offset=temp_buf[0]|(temp_buf[1]<<8)|(temp_buf[2]<<16)|(temp_buf[3]<<24);
    
    SPI_BurstRead(temp_buf,offset+SPI_BASE_INFO_SIZE+(SPI_TAB_INFO_ENTRY_SIZE*(*index)),SPI_TAB_INFO_ENTRY_SIZE);   
    *index=offset+(temp_buf[0]|(temp_buf[1]<<8)|(temp_buf[2]<<16)|(temp_buf[3]<<24));
    (*index)&=(~SPI_DATA_TYPE_MASK);
    
    SPI_BurstRead(temp_buf,offset+SPI_TIMBRE_ADDR_OFFSET,4);
    
    *addr=offset+(temp_buf[0]|(temp_buf[1]<<8)|(temp_buf[2]<<16)|(temp_buf[3]<<24));
}   
//------------------------------------------------------------------//
// Seek from start address(called by lib)
// Parameter: 
//          offset: seek offset
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_Seek(U32 offset){  
   
}    

void  CB_SBC_PlayStart(void){
}  
void  CB_SBC_PlayEnd(void){
}       
void  CB_SBC_RecStart(void){
}  
void  CB_SBC_RecEnd(void){
}       
void  CB_SBC_CH2_PlayStart(void){
}  
void  CB_SBC_CH2_PlayEnd(void){
}   
void  CB_ADPCM_PlayStart(void){
}  
void  CB_ADPCM_PlayEnd(void){
} 
void  CB_ADPCM_RecStart(void){
}  
void  CB_ADPCM_RecEnd(void){
}
void  CB_ADPCM_CH2_PlayStart(void){
}  
void  CB_ADPCM_CH2_PlayEnd(void){
} 
void  CB_CELP_PlayStart(void){
}  
void  CB_CELP_PlayEnd(void){
}  
void  CB_PCM_PlayStart(void){
}  
void  CB_PCM_PlayEnd(void){
} 
void  CB_PCM_RecStart(void){
}  
void  CB_PCM_RecEnd(void){
}  
void  CB_PCM_CH2_PlayStart(void){
}  
void  CB_PCM_CH2_PlayEnd(void){
} 
void  CB_PCM_CH2_RecStart(void){
}  
void  CB_PCM_CH2_RecEnd(void){
}
void  CB_PCM_CH3_PlayStart(void){
}  
void  CB_PCM_CH3_PlayEnd(void){
} 
void  CB_PCM_CH3_RecStart(void){
}  
void  CB_PCM_CH3_RecEnd(void){
}

void  CB_MIDI_PlayStart(void){
}  
void  CB_MIDI_PlayEnd(void){
}  
void  CB_RT_PlayStart(void){
}
#endif
