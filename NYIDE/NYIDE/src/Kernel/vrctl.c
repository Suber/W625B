#include "include.h"
#include "nx1_adc.h"
#include "nx1_gpio.h"
#include <string.h>
#if _VR_FUNC

#define BUF0_READY          0
#define BUF1_READY          1
#define BUF_NOTREADY        2


#define VR_OFF              0
#define VR_ON               1
#define VR_HALT             2


#define MAX_TIME			300

#define STATE_BUF_OFFSET    0
#define BUF0_OFFSET         (STATE_BUF_OFFSET+VR_STATE_SIZE)         
#define BUF1_OFFSET         (BUF0_OFFSET+(BUF_SIZE*2))
#define ENGINE_BUF_OFFSET   (BUF1_OFFSET+(BUF_SIZE*2))

static S16 *Buf0,*Buf1;
static U8 BufToggle;
static U16 BufIdx;
static U32 hCSpotter;
static U32 ModelAddr,CyBaseAddr;
static U8 VRState;

/* Dynamic Allocate RAM-------------------------------------------------------*/
#if _USE_HEAP_RAM
static U8 *VRRam=NULL;

#else
static U8 VRRam[_VR_RAM_SIZE];
#endif



static volatile U8 BufReady;

#define CSPDLL_API
#define HANDLE	void*
extern CSPDLL_API S32 CSpotter16_GetMemoryUsage_Sep(U8 *lpbyCYBase, U8 *lpbyModel, U32 nMaxTime);
extern CSPDLL_API HANDLE CSpotter16_Init_Sep(U8 *lpbyCYBase, U8 *lpbyModel, U32 nMaxTime, U8 *lpbyMemPool, U32 nMemSize, U8 *lpbyState, U32 nStateSize, U32 *pnErr);
extern CSPDLL_API S32 CSpotter16_Reset(HANDLE hCSpotter);
extern CSPDLL_API S32 CSpotter16_AddSample(HANDLE hCSpotter, S16 *lpsSample, U32 nNumSample);

extern CSPDLL_API S32 CSpotter16_GetMemoryUsage_Sep_FromSPI(U8 *lpbyCYBase, U8 *lpbyModel, U32 nMaxTime);
extern CSPDLL_API HANDLE CSpotter16_Init_Sep_FromSPI(U8 *lpbyCYBase, U8 *lpbyModel, U32 nMaxTime, U8 *lpbyMemPool, U32 nMemSize, U8 *lpbyState, U32 nStateSize, U32 *pnErr);

//------------------------------------------------------------------//
// Allocate VR memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
#if _USE_HEAP_RAM
static void VR_AllocateMemory(U16 func)
{
    if(VRRam==NULL && (func&MEMORY_ALLOCATE_VR))    //VRRam
    {
        //dprintf("VR allocate\r\n");
        VRRam   = (U8 *)MemAlloc(_VR_RAM_SIZE);
    }
    //else
    //    dprintf("Allocate twice. Ignore...\r\n");
    
}
//------------------------------------------------------------------//
// Free VR memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
static void VR_FreeMemory(U16 func)
{
    if(VRRam!=NULL && (func&MEMORY_ALLOCATE_VR))    
    {
        MemFree(VRRam);
        VRRam=NULL;
        //dprintf("VR Free Mem\r\n");
    }
    //else
    //{
    //    dprintf("VR Already Free\r\n");
    //}
}
#endif


#if _VR_STORAGE_TYPE
static U32 SpiReadAddr;
static U32 SpiBuf;



static void StopSpiRead(void)
{
    SpiReadAddr=0;  
    SPI_FastReadStop();  
}    
#endif

U32 SPIMgr_Read(U8 *dest, U8 *src, U32 nSize)
{
#if _VR_STORAGE_TYPE    
    U32 *p32;
    
    if((U32)src==SpiReadAddr){
        SPI_FastRead((U32 *)dest,nSize);  
        SpiReadAddr+=nSize;  
        
        p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }else if((U32)src==(SpiReadAddr-4)){
        p32=(U32 *)dest;
        *p32=SpiBuf;
        nSize-=4;
        dest+=4;
        
        SPI_FastRead((U32 *)dest,nSize);  
        SpiReadAddr+=nSize;  
        
        p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }else{
        SpiReadAddr=(U32)src;
        SpiReadAddr+=nSize;
        
        SPI_FastReadStop();
        SPI_FastReadStart((U32)src);
	    SPI_FastRead((U32 *)dest, nSize);
	    
	    p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }  
#endif
    //SPI_BurstRead((U32)dest, (U32)src, (U32)nSize);          
}

U32 SPIMgr_ReadNoWait(U8 *dest, U8 *src, U32 nSize)
{
#if _VR_STORAGE_TYPE    
    U32 *p32;
    
    if((U32)src==SpiReadAddr){
        SPI_FastRead((U32 *)dest,nSize);  
        SpiReadAddr+=nSize;  
        
        p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }else if((U32)src==(SpiReadAddr-4)){
        p32=(U32 *)dest;
        *p32=SpiBuf;
        nSize-=4;
        dest+=4;
        
        SPI_FastRead((U32 *)dest,nSize);  
        SpiReadAddr+=nSize;  
        
        p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }else{
        SpiReadAddr=(U32)src;
        SpiReadAddr+=nSize;
        
        SPI_FastReadStop();
        SPI_FastReadStart((U32)src);
	    SPI_FastRead((U32 *)dest, nSize);
	    
	    p32=(U32 *)(dest+nSize-4);
        SpiBuf=*p32;
    }  
#endif
    //SPI_BurstRead((U32)dest, (U32)src, (U32)nSize);              
}        
//------------------------------------------------------------------//
// Adc ISR
// Parameter: 
//          size: fifo fill size 
//       pdest32: fifo address
// return value:
//          NONE
//------------------------------------------------------------------// 
void VR_AdcIsr(U8 size,volatile U32 *pdest32){
    U8 i;
    S16 temp,*buf;
    
    if(VRState==VR_ON){
        if(!BufToggle){
            buf=Buf0+BufIdx;
        }else{
            buf=Buf1+BufIdx;
        }        
        
        for(i=0;i<size;i++){
            temp=(*pdest32-32768);//ADC data is unsigned integer. convert it to signed integer.
            
            *buf++=temp;
        }   
        BufIdx+= size;
        
        if(BufIdx==BUF_SIZE){
            if(BufToggle){
                BufReady=BUF1_READY;
            }else{
                BufReady=BUF0_READY;
            }        
            BufToggle^=1;
            BufIdx=0;
        }    
    }
}
//------------------------------------------------------------------//
// Set command model
// Parameter: 
//          cybase: cybase address
//      command_model: model address
// return value:
//          NONE
//------------------------------------------------------------------//     
static void VR_SetModel(U32 cybase, U32 command_model){
    U32 n, nErr;

#if _VR_STORAGE_TYPE==0    
	n = CSpotter16_GetMemoryUsage_Sep((U8 *)cybase, (U8 *)command_model, MAX_TIME);
	if(n>_VR_ENGINE_RAM_SIZE){
        while(1);
    }    

    hCSpotter =(U32) CSpotter16_Init_Sep((U8 *)cybase, (U8 *)command_model, MAX_TIME, (U8 *)(VRRam+ENGINE_BUF_OFFSET), _VR_ENGINE_RAM_SIZE, (U8 *)(VRRam+STATE_BUF_OFFSET), VR_STATE_SIZE, &nErr);
#else
    n = CSpotter16_GetMemoryUsage_Sep_FromSPI((U8 *)cybase, (U8 *)command_model, MAX_TIME);
	
	if(n>_VR_ENGINE_RAM_SIZE){
        while(1);
    }    

    hCSpotter =(U32) CSpotter16_Init_Sep_FromSPI((U8 *)cybase, (U8 *)command_model, MAX_TIME, (U8 *)(VRRam+ENGINE_BUF_OFFSET), _VR_ENGINE_RAM_SIZE, (U8 *)(VRRam+STATE_BUF_OFFSET), VR_STATE_SIZE, &nErr);
    StopSpiRead(); 
    
#endif	
	CSpotter16_Reset((HANDLE)hCSpotter);
}    
//------------------------------------------------------------------//
// Voice Recogniztion start
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
static void VR_Start_(void){   
    U8 sts;
    
    if(VRState==VR_ON){
        VR_Stop();        
    }    
    
    sts=0;
#if _ADPCM_PLAY    
    if(ADPCM_GetStatus()!=STATUS_STOP){
        sts=1;    
    }    
#endif  
#if _ADPCM_CH2_PLAY
    if(ADPCM_CH2_GetStatus()!=STATUS_STOP){
        sts=1;
    }        
#endif  
#if _SBC_PLAY    
    if(SBC_GetStatus()!=STATUS_STOP){
        sts=1;
    }      
#endif   
#if _SBC_CH2_PLAY    
    if(SBC_CH2_GetStatus()!=STATUS_STOP){
        sts=1;
    }      
#endif   
#if _CELP_PLAY  
    if(CELP_GetStatus()!=STATUS_STOP){
        sts=1;
    }      
#endif   
#if _MIDI_PLAY    
    if(MIDI_GetStatus()!=STATUS_STOP){
        sts=1;
    }      
#endif 
#if _PCM_PLAY    
    if(PCM_GetStatus()!=STATUS_STOP){
        sts=1;
    }      
#endif 
    if(sts==1){
        VRState=VR_HALT;
        return;
    }        
#if _USE_HEAP_RAM
    VR_AllocateMemory(MEMORY_ALLOCATE_VR);
#endif
    memset((void *)VRRam+STATE_BUF_OFFSET, 0x0, VR_STATE_SIZE);
    
    Buf0=(S16 *)(VRRam+BUF0_OFFSET);
    Buf1=(S16 *)(VRRam+BUF1_OFFSET);
    
    BufIdx=0;
    BufToggle=0;
    BufReady=BUF_NOTREADY;
    
    InitAdc(_VR_SAMPLE_RATE);
    
    VR_SetModel(CyBaseAddr,ModelAddr);
    
    VRState=VR_ON;
    
    ADC_Cmd(ENABLE);
}   
//------------------------------------------------------------------//
// Voice Recogniztion start
// Parameter: 
//          cybase:cybase table address
//          model:model table address
// return value:
//          NONE
//------------------------------------------------------------------//     
void VR_Start(const U32 *cybase,const U32 *model){
#if _VR_STORAGE_TYPE    
    U8 tempbuf[4];
    U32 start_addr;
    
    SPI_BurstRead(tempbuf,SPI_RESERVED_SIZE-4,4);
    start_addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 
    
    CyBaseAddr=(U32)cybase+start_addr;
    ModelAddr=(U32)model+start_addr;
    
    StopSpiRead();
#else
    CyBaseAddr=(U32)cybase;
    ModelAddr=(U32)model;
#endif

    VR_Start_();    
}
 
//------------------------------------------------------------------//
// Voice Recogniztion halt
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
void VR_Halt(void){
    
    if(VRState==VR_ON){
#if _USE_HEAP_RAM
        VR_FreeMemory(MEMORY_ALLOCATE_VR);   
#endif
        ADC_Cmd(DISABLE);
        VRState=VR_HALT;
    }
}    
//------------------------------------------------------------------//
// Voice Recogniztion stop
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
void VR_Stop(void){
    
    if(VRState==VR_ON){
#if _USE_HEAP_RAM
        VR_FreeMemory(MEMORY_ALLOCATE_VR);   
#endif        
        ADC_Cmd(DISABLE);
    }
    VRState=VR_OFF;
}    
//------------------------------------------------------------------//
// VR Service loop
// Parameter: 
//          NONE
// return value:
//          -1: no command
//        others: command index  
//------------------------------------------------------------------//     
S8 VR_ServiceLoop(void){
    
    S32 sts;
    S8 id;
    
    if(VRState==VR_ON){
        sts=1;
        if(BufReady==BUF0_READY){
            BufReady=BUF_NOTREADY;
            //process buf0 here
           
            sts=CSpotter16_AddSample((HANDLE)hCSpotter, Buf0, BUF_SIZE);
#if _VR_STORAGE_TYPE            
            StopSpiRead();
#endif
        }else if(BufReady==BUF1_READY){
            BufReady=BUF_NOTREADY;
            //process buf1 here
            
            sts=CSpotter16_AddSample((HANDLE)hCSpotter, Buf1, BUF_SIZE);
#if _VR_STORAGE_TYPE              
            StopSpiRead();
#endif            
        }    
        
        if(sts==0){
            id = CSpotter16_GetResult((HANDLE)hCSpotter);
            return id;
        }else{
            return -1;
        }        
    }else if(VRState==VR_HALT){
        sts=0;
#if _ADPCM_PLAY    
        if(ADPCM_GetStatus()!=STATUS_STOP){
            sts=1;    
        }    
#endif  
#if _ADPCM_CH2_PLAY
        if(ADPCM_CH2_GetStatus()!=STATUS_STOP){
            sts=1;
        }        
#endif  
#if _SBC_PLAY    
        if(SBC_GetStatus()!=STATUS_STOP){
            sts=1;
        }      
#endif   
#if _SBC_CH2_PLAY    
        if(SBC_CH2_GetStatus()!=STATUS_STOP){
            sts=1;
        }      
#endif   
#if _CELP_PLAY  
        if(CELP_GetStatus()!=STATUS_STOP){
            sts=1;
        }      
#endif   
#if _MIDI_PLAY    
        if(MIDI_GetStatus()!=STATUS_STOP){
            sts=1;
        }      
#endif 
#if _PCM_PLAY    
        if(PCM_GetStatus()!=STATUS_STOP){
            sts=1;
        }      
#endif 
        if(sts==0){
            VR_Start_();
        }    
    }  
    return -1;  
}    
#endif
