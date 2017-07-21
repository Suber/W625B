/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "include.h"
#include "nx1_dac.h"

#if _MIDI_PLAY

/* Defines -------------------------------------------------------------------*/
#define AUDIO_OUT_SIZE     MIDI_DEC_OUT_SIZE
#define AUDIO_OUT_COUNT    MIDI_AUDIO_OUT_COUNT
#define AUDIO_BUF_SIZE     MIDI_AUDIO_BUF_SIZE

#define SMOOTH_ENABLE          1

/* Constant Table ------------------------------------------------------------*/
//static const U16 VolTbl[16]={
//    0,
//    2920-DAC_DATA_OFFSET,
//    3471-DAC_DATA_OFFSET,
//    4125-DAC_DATA_OFFSET,
//    4903-DAC_DATA_OFFSET,
//    5827-DAC_DATA_OFFSET,
//    6925-DAC_DATA_OFFSET,
//    8231-DAC_DATA_OFFSET,
//    9783-DAC_DATA_OFFSET,
//    11627-DAC_DATA_OFFSET,
//    13818-DAC_DATA_OFFSET,
//    16423-DAC_DATA_OFFSET,
//    19519-DAC_DATA_OFFSET,
//    23198-DAC_DATA_OFFSET,
//    27571-DAC_DATA_OFFSET,
//    32767-DAC_DATA_OFFSET,
//}; 

/* Static Variables ----------------------------------------------------------*/
static volatile struct AUDIO_BUF_CTL{
    S32 process_count;	//decode/encode count
    S32 fifo_count;	    //decode/encode count
    //U16 vol;            //volume
    U8 in_size;         //data frame size
    U8 state;           //channel state
    U8 shadow_state;    //channel shadow state
//    U8 ramp;            //smooth ramp
    U8 index;
}AudioBufCtl;

static U8 ChVol;

static volatile struct{
    U16 *buf;
    U16 size;
    U16 offset;
}BufCtl;   

#if SMOOTH_ENABLE
static volatile struct{
//    U32 p_index;            //play index    
    S16 count;              //sample count in process
    U8  step_sample;        //sample size for each step
    U8  smooth;             //up or down or none
    U8  state;              //current state
//    U8  p_parameter;        //play parameter
}SmoothCtl;
#endif

/* Dynamic Allocate RAM-------------------------------------------------------*/
#if _USE_HEAP_RAM
static U32 *MIDIRam=NULL;
static U16 *AudioBuf=NULL;
#else
static U32 MIDIRam[MIDI_RAM_ALLOCATE_SIZE];
static U16 AudioBuf[AUDIO_BUF_SIZE];
#endif
/* Extern Functions ----------------------------------------------------------*/
extern void SPI_BurstRead(U8 *buf,U32 addr,U16 len);
extern void CB_MIDI_PlayStart(void);
extern void CB_MIDI_PlayEnd(void);
extern void CB_InitDac(CH_TypeDef *chx,U16 sample_rate);
extern void CB_DacCmd(CH_TypeDef *chx,U8 cmd);
extern void CB_WriteDac(U8 size,U16* psrc16);
extern void CB_Midi_GetStartAddr(U32 *index,U32 *addr);
//------------------------------------------------------------------//
// Allocate MIDI memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
#if _USE_HEAP_RAM
static void MIDI_AllocateMemory(U16 func)
{
    if(MIDIRam==NULL && (func&MEMORY_ALLOCATE_PLAY))    // MIDIRam & AudioBuf together
    {
        MIDIRam =  (U32 *)MemAlloc(MIDI_RAM_ALLOCATE_SIZE<<2);
        AudioBuf = (U16 *)MemAlloc(AUDIO_BUF_SIZE<<1);
        //dprintf("MIDI Allocate mem\r\n");
    }
    //else
        //dprintf("MIDI Allocate twice. Ignore...\r\n");
    
}
//------------------------------------------------------------------//
// Free PCM memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
static void MIDI_FreeMemory(U16 func)
{
    if(MIDIRam!=NULL && (func&MEMORY_ALLOCATE_PLAY))    
    {
        MemFree(MIDIRam);
        MIDIRam=NULL;
        MemFree(AudioBuf);
        AudioBuf=NULL;
        //dprintf("MIDI Free Mem\r\n");
    }
    //else
    //{
    //    dprintf("MIDI Already Free\r\n");
    //}
}
#endif
//------------------------------------------------------------------//
// ISR
// Parameter: 
//          size: fifo fill size 
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_DacIsr(U8 size){
    U16 temp;
    S16 *psrc16,gain;
    U8 i,len,size2;
    
    if(AudioBufCtl.shadow_state==STATUS_PLAYING){
        
        temp=BufCtl.size-BufCtl.offset;
        if(size>temp){
            size=temp;
        }   
        //dprintf("size,%d\r\n",size);
        AudioBufCtl.fifo_count+=size;   
        
#if SMOOTH_ENABLE      
        //smooth process
        if(SmoothCtl.smooth!=SMOOTH_NONE){  
            size2= size;
            psrc16=(S16 *)(BufCtl.buf+BufCtl.offset);
        
            while(size2){
                if(SmoothCtl.state==SMOOTH_STEP){
                    if(SmoothCtl.smooth==SMOOTH_DOWN){
                        for(i=0;i<size2;i++){
                            *psrc16++=0;
                        }       
                        AudioBufCtl.shadow_state=AudioBufCtl.state;
                        if(AudioBufCtl.state==STATUS_STOP){
                            CB_MIDI_PlayEnd();   
                        }    
                    }
                    SmoothCtl.smooth=SMOOTH_NONE;
                    break;            
                }
                if(SmoothCtl.smooth==SMOOTH_DOWN){
                    gain=MAX_SMOOTH_GAIN-(SmoothCtl.state+1)*(MAX_SMOOTH_GAIN/SMOOTH_STEP);
                }else{
                    gain=(SmoothCtl.state+1)*(MAX_SMOOTH_GAIN/SMOOTH_STEP);
                }    
                len=SmoothCtl.step_sample-SmoothCtl.count;
            
                if(len>size2){
                    len=size2;
                }    
                 
                for(i=0;i<len;i++){
                    *psrc16=((S32)(*psrc16)*gain)>>15;
                    psrc16++;
                }         
                SmoothCtl.count+=len;
                size2-=len;
                if(SmoothCtl.count==SmoothCtl.step_sample){
                    SmoothCtl.count=0;
                    SmoothCtl.state++;   
                    //dprintf("s:%d,%d\r\n",SmoothCtl.state,gain);
                }          
            }  
        }
#endif     
        psrc16=(S16 *)(BufCtl.buf+BufCtl.offset);
        
//        for(i=0;i<size;i++){
//            *pdest32=*psrc16;
//            *psrc16++=0;            
//        }   
        CB_WriteDac(size,(U16 *)psrc16);
        
        BufCtl.offset+=size;

        if(BufCtl.offset==BufCtl.size){
            BufCtl.offset=0;    
        }       
            
        if(AudioBufCtl.fifo_count>AudioBufCtl.process_count){
            if(MIDIChkEnd()){
                AudioBufCtl.state=STATUS_STOP;
                AudioBufCtl.shadow_state=AudioBufCtl.state;
                CB_MIDI_PlayEnd();
            }
            //dprintf("%d\r\n",MIDIChkEnd());
            //dprintf("stop m,%d,%d\r\n",AudioBufCtl.fifo_count,AudioBufCtl.process_count);
        }
    }else{
//        for(i=0;i<size;i++){
//            *pdest32=0;           
//        }   
        CB_WriteDac(size,(U16 *)0);
    }                 
}    
#if SMOOTH_ENABLE
//------------------------------------------------------------------//
// Smooth up or down
// Parameter: 
//          ch: audio channel
//          smooth: SMOOTH_UP,SMOOTH_DOWN,SMOOTH_NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
static void SmoothFunc(U8 smooth){
	SmoothCtl.count=0;  
	SmoothCtl.state=0;  
	
    if(smooth==SMOOTH_UP){
	    SmoothCtl.smooth=SMOOTH_UP;
	    SmoothCtl.step_sample=MIDI_SAMPLE_RATE*SMOOTH_UP_TIME/1000/SMOOTH_STEP;
    }else if(smooth==SMOOTH_DOWN){
	    SmoothCtl.smooth=SMOOTH_DOWN;
	    SmoothCtl.step_sample=MIDI_SAMPLE_RATE*SMOOTH_DOWN_TIME/1000/SMOOTH_STEP;
    }else{
        SmoothCtl.smooth=SMOOTH_NONE;
    }
}     
#endif
//------------------------------------------------------------------//
// Decode one audio frame
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
static void DecodeFrame(void)
{
    U8 i;
    U16 *audio_buf;
    
    audio_buf=AudioBuf+(AUDIO_OUT_SIZE*AudioBufCtl.index);
    
    AudioBufCtl.index++;
    if(AudioBufCtl.index==AUDIO_OUT_COUNT){
        AudioBufCtl.index=0;    
    }    
    
//    USER_MIDI_ReadData((U8 *)audio_buf,AudioBufCtl.in_size);
    i=0;
    MIDIDecode(&i,(S16 *)audio_buf,1);
    //dprintf("len:%d\r\n",i);
    AudioBufCtl.process_count+=i;
   
    for(i=0;i<AUDIO_OUT_SIZE;i++){
        audio_buf[i]=((S16)audio_buf[i]*(32767-DAC_DATA_OFFSET))>>Q15_SHIFT;
    }       
}    
//------------------------------------------------------------------//
// Start play voice
// Parameter: 
//          index: file index or absolute address
//          mode:OTP_MODE,SPI_MODE
// return value:
//          0:ok
//          others:fail
//------------------------------------------------------------------// 
U8 MIDI_Play(U32 index,U8 mode){
    U8 *temp_buf;
    U32 addr;
    //struct AUDIO_HEADER *pheader;
    
    if(AudioBufCtl.shadow_state==STATUS_PLAYING){
#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_DOWN){
//            SmoothCtl.p_parameter|=P_WAIT;
//            SmoothCtl.p_parameter|=(mode<<P_MODE_SHIFT);
//            SmoothCtl.p_parameter|=(dac<<P_DAC_SHIFT);
//            SmoothCtl.p_parameter|=ramp;
//            SmoothCtl.p_index=index;
            AudioBufCtl.state=STATUS_STOP;
            SmoothFunc(SMOOTH_DOWN);
            while(AudioBufCtl.shadow_state==STATUS_PLAYING){
                AUDIO_ServiceLoop();
            }
//            return 0;      
//        }else
#endif        
//        {
//            AudioBufCtl.state=STATUS_STOP;         
//        }    
    }
#if _USE_HEAP_RAM    
#if _VR_FUNC
    VR_Halt();
#endif    
    MIDI_AllocateMemory(MEMORY_ALLOCATE_PLAY);
#endif
    memset((void *)&AudioBufCtl, 0, sizeof(AudioBufCtl));
    
//    AudioBufCtl.ramp=ramp;
    
//    SmoothCtl.p_parameter=0;
    
    BufCtl.offset=0;
    BufCtl.size=AUDIO_BUF_SIZE;//sizeof(AudioBuf)/2;
    BufCtl.buf=AudioBuf;
    
    temp_buf=(U8 *)AudioBuf;
//    pheader=(struct AUDIO_HEADER *)temp_buf;
//        
//    AudioBufCtl.samples=pheader->samples;
//    AudioBufCtl.in_size=pheader->decode_in_size;
//    
//    dprintf("pheader->decode_in_size,%d\r\n",pheader->decode_in_size);
#if SMOOTH_ENABLE    
//    if(ramp&RAMP_UP){
        SmoothFunc(SMOOTH_UP);
//    }else{
//        SmoothFunc(SMOOTH_NONE);
//    }  
#endif    
    if(mode==SPI_MODE){
        CB_Midi_GetStartAddr(&index,&addr);
        MIDIInit(index,addr,MIDIRam);
    }else if(mode==OTP_MODE){
        MIDIInit(index,0,MIDIRam);
    }    
//    for(i=0;i<AUDIO_BUF_COUNT;i++){ 
//        DecodeFrame();
//    }
    memset((void *)AudioBuf, 0, AUDIO_BUF_SIZE<<1);
    //AudioBufCtl.process_count=AUDIO_BUF_SIZE;
    
    DecodeFrame();
       
#if _MIDI_PLAY_AUDIO_CH==0
    CB_InitDac(DAC_CH0,MIDI_SAMPLE_RATE);
#else
    CB_InitDac(DAC_CH1,MIDI_SAMPLE_RATE);
#endif      
   
    AudioBufCtl.state=  STATUS_PLAYING;
    AudioBufCtl.shadow_state=AudioBufCtl.state;
    
    CB_MIDI_PlayStart();   
    
#if _MIDI_PLAY_AUDIO_CH==0
    CB_DacCmd(DAC_CH0,ENABLE);
#else
    CB_DacCmd(DAC_CH1,ENABLE);
#endif
    
  
    return 0;
}   
//------------------------------------------------------------------//
// Pause playing
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_Pause(void){
    if(AudioBufCtl.state==STATUS_PLAYING){
#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_DOWN){
            AudioBufCtl.state=STATUS_PAUSE;
            SmoothFunc(SMOOTH_DOWN);
//        }else
#endif        
//        {
//            AudioBufCtl.state=STATUS_PAUSE;
//        }    
    }
} 
//------------------------------------------------------------------//
// Resume playing
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_Resume(void){
    if(AudioBufCtl.state==STATUS_PAUSE){
#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_UP){
            SmoothFunc(SMOOTH_UP);
//        }
#endif        
        AudioBufCtl.state=STATUS_PLAYING;
        AudioBufCtl.shadow_state=AudioBufCtl.state;
    }          
} 
//------------------------------------------------------------------//
// Stop playing
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_Stop(void){
    if(AudioBufCtl.state==STATUS_PLAYING||AudioBufCtl.state==STATUS_PAUSE){ 
#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_DOWN){
            AudioBufCtl.state=STATUS_STOP;
            SmoothFunc(SMOOTH_DOWN);
//        }else
#endif        
//        {
//            AudioBufCtl.state=STATUS_STOP;
//            CB_MIDI_PlayEnd();
//        }     
    }
} 
//------------------------------------------------------------------//
// Get current status
// Parameter: 
//          NONE
// return value:
//          status:STATUS_PLAYING,STATUS_PAUSE,STATUS_STOP
//------------------------------------------------------------------// 
U8 MIDI_GetStatus(void){
    return AudioBufCtl.shadow_state;    
}
//------------------------------------------------------------------//
// Set audio volume
// Parameter: 
//          vol:0~15
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_SetVolume(U8 vol){   
    if(vol>CH_VOL_15){
        vol=CH_VOL_15;
    }    
    ChVol=vol;  
}
//------------------------------------------------------------------//
// Codec init
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void MIDI_Init(void){
    ChVol=CH_VOL_15;      
}
//------------------------------------------------------------------//
// Service loop
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
void MIDI_ServiceLoop(void){
    U16 vol;
//    if((SmoothCtl.p_parameter&P_WAIT)&&(AudioBufCtl.state==STATUS_STOP)){
//        SmoothCtl.p_parameter&=(~P_WAIT);
//        MIDI_Play(SmoothCtl.p_index,(SmoothCtl.p_parameter&P_MODE_MASK)>>P_MODE_SHIFT,
//                   (SmoothCtl.p_parameter&P_DAC_MASK)>>P_DAC_SHIFT,SmoothCtl.p_parameter&P_RAMP_MASK);  
//    }    
    
    if(AudioBufCtl.shadow_state==STATUS_PLAYING){
       
        vol=  (ChVol+1)*(MixerVol+1); 
        
        if(!ChVol){
            vol=0;
        }    
        if(vol==256){
            vol=255;
        }    
        MIDISetVolume(vol);

        if((AudioBufCtl.fifo_count+(AUDIO_BUF_SIZE-AUDIO_OUT_SIZE)-AudioBufCtl.process_count)>=0){
            DecodeFrame();
        }    
    }       
#if _USE_HEAP_RAM                            

    if((AudioBufCtl.shadow_state==STATUS_STOP) && MIDIRam!=NULL )        
    {
        MIDI_FreeMemory(MEMORY_ALLOCATE_PLAY);
    }
#endif    
    
}    
#endif
