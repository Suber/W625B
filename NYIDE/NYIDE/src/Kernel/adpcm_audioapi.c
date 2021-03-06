/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "include.h"
#include "nx1_dac.h"

#if _ADPCM_PLAY||_ADPCM_RECORD

/* Defines -------------------------------------------------------------------*/
#if _ADPCM_ROBOT_SOUND_02
#define AUDIO_OUT_SIZE     SPEED_CTL_OUT_SIZE
#define AUDIO_OUT_COUNT    8
#define AUDIO_BUF_SIZE     (AUDIO_OUT_SIZE*AUDIO_OUT_COUNT)
#elif _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE
#define AUDIO_OUT_SIZE     SPEED_CTL_OUT_SIZE
#define AUDIO_OUT_COUNT    6
#define AUDIO_BUF_SIZE     (AUDIO_OUT_SIZE*AUDIO_OUT_COUNT)
#elif (_ADPCM_ECHO_SOUND||_ADPCM_ROBOT_SOUND_01||_ADPCM_ROBOT_SOUND_02||_ADPCM_REVERB_SOUND) && _ADPCM_RECORD
#define AUDIO_OUT_SIZE     SPEED_CTL_OUT_SIZE
#define AUDIO_OUT_COUNT    4
#define AUDIO_BUF_SIZE     (AUDIO_OUT_SIZE*AUDIO_OUT_COUNT)
#elif (_ADPCM_ECHO_SOUND||_ADPCM_ROBOT_SOUND_01||_ADPCM_ROBOT_SOUND_02||_ADPCM_REVERB_SOUND)
#define AUDIO_OUT_SIZE     SPEED_CTL_OUT_SIZE
#define AUDIO_OUT_COUNT    2
#define AUDIO_BUF_SIZE     (AUDIO_OUT_SIZE*AUDIO_OUT_COUNT)
#elif _ADPCM_RECORD 
#define AUDIO_OUT_SIZE     ADPCM_DEC_OUT_SIZE
#define AUDIO_OUT_COUNT    2
#define AUDIO_BUF_SIZE     (AUDIO_OUT_SIZE*AUDIO_OUT_COUNT)
#endif

#define DATA_BUF_SIZE       (ADPCM_DEC_IN_SIZE*3)

#if _ADPCM_ECHO_SOUND
#define ADPCM_ECHO_DATA_LENGTH         45
#define ADPCM_ECHO_BUF_SIZE            ADPCM_ECHO_DATA_LENGTH*30
#endif



#define AGC_CTL_ENABLE      1

#define TARGET_RMS     	MINUS_7DB                           // Adjustable, Target RMS signal magnitude 			
#define ALPHA           4096//4096//4096                    // 0.125; coeff. of 1st order IIR LPF 
#define BETA            1024//(2048/2)                      // 0.125; step size constant 


#define NOISE_GATE_ENABLE   1

#define NG_ON_OFF_SAMPLE_16K        160//10ms
#define NG_ATTACK_SAMPLE_16K        80//5ms
#define NG_RELEASE_SAMPLE_16K       (320*5)//100ms

#define NG_ON_OFF_SAMPLE_8K         80//10ms
#define NG_ATTACK_SAMPLE_8K         40//5ms
#define NG_RELEASE_SAMPLE_8K        (160*5)//100ms

#define SMOOTH_ENABLE          1

#define HPF_ENABLE          1

#define HPF_ORDER		    2
#define HPF_QFORMAT         14

#define LIMITER_THRESHOLD   30000
/* Static Functions ----------------------------------------------------------*/
static void WriteHeader(void);

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
#if _ADPCM_SPEED_CHANGE
static const float SpeedRatioTbl[25]={
    0.5,
    0.54,
    0.58,
    0.62,
    0.67,
    0.71,
    0.75,
    0.79,
    0.83,
    0.87,
    0.92,
    0.96,
    1,
    1.08,
    1.17,
    1.25,
    1.33,
    1.42,
    1.45,
    1.58,
    1.67,
    1.75,
    1.83,
    1.92,
    2,
};  
#endif

#if _ADPCM_PITCH_CHANGE
static const float PitchRatioTbl[25]={
    0.5,
    0.53,
    0.56,
    0.59,
    0.63,
    0.67,
    0.72,
    0.75,
    0.79,
    0.84,
    0.89,
    0.94,
    1,
    1.06,
    1.12,
    1.19,
    1.26,
    1.33,
    1.41,
    1.50,
    1.59,
    1.68,
    1.78,
    1.89,
    2,
};  
#endif

#if _ADPCM_ROBOT_SOUND 
static const U16 CarrierTbl[16]={
    220,
    250,
    280,
    310,
    340,
    370,
    400,
    440,
    495,
    550,
    606,
    660,
    715,
    770,
    825,
    880,
};    
#endif 

#if _ADPCM_ECHO_SOUND
static const U8 EchoCoef[3][8] = {
    {  4,  4,  5,  5,  6,  6,  7,  7 },
    { 30, 35, 40, 43, 47, 52, 55, 60 },
    {  9, 12, 15, 17, 22, 25, 27, 29 }};
#endif

#if HPF_ENABLE
//Q14 ,cutoff 200Hz
static const S16 CoeffB_16K[HPF_ORDER+1] = {
    15522,
    -31044,
    15522
};
 
static const S16 CoeffA_16K[HPF_ORDER+1] = {
    16384,
    -30949,
    14661
};

static const S16 CoeffB_8K[HPF_ORDER+1] = {
    14660,
    -29320,
    14660
};
 
static const S16 CoeffA_8K[HPF_ORDER+1] = {
    16384,
    -29141,
    13120
};
#endif
/* Static Variables ----------------------------------------------------------*/
static volatile struct AUDIO_BUF_CTL{
    S32 process_count;	//decode/encode count
    S32 fifo_count;	    //decode/encode count
    U32 samples;	    //samples
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
    U32 sample_count;	    //samples
    U32 fifo_count2;	    //decode/encode count
#endif       
    //U16 vol;            //volume
    U16 sample_rate;    //sample_rate
    U8 out_size;        //audio frame size
    U8 in_size;         //data frame size
    U8 state;           //channel state
    U8 shadow_state;    //channel shadow state
//    U8 ramp;            //smooth ramp
    U8 out_count;
    U8 index;
}AudioBufCtl;
//
//#if _ADPCM_RECORD
//static U32 StorageIndex;
//#endif
#if _ADPCM_RECORD
#if AGC_CTL_ENABLE
static U8 AgcCnt;
#endif

#if HPF_ENABLE
static S32 y[HPF_ORDER+1]; //output samples
static S16 x[HPF_ORDER+1]; //input samples
static S16 const*CoeffB,*CoeffA;
#endif
static U32 RecordSize;

#endif

static U16 AudioVol;
static U8  OpMode,ChVol;

static volatile struct{
    U16 *buf;
    U16 size;
    U16 offset;
    U16 rd_offset;
    U16 wr_offset;
}BufCtl;   

#if SMOOTH_ENABLE
static volatile struct{
//    U32 p_index;            //play index
    S16 count;              //sample count in process
    U8  step_sample;        //sample size for each step
    U8  smooth;             //up or down or none
    U8  state;              //current state
//    U8  p_parameter;        //play paramete
}SmoothCtl;
#endif

static const U8 Signature[]="NX1";

#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ROBOT_SOUND_02
static struct{
    U16 index;
    U8 speed_ratio_index;            //speed control ratio
    U8 pitch_ratio_index;            //pitch control ratio
#if _ADPCM_ROBOT_SOUND_02
    U16 rb_ptr;
#endif
}SpeedCtl;


#endif

#if _ADPCM_ROBOT_SOUND 
static struct{
    U8 enable;
    U8 change;
    U8 index;
}RobotCtl;
#endif 

#if _ADPCM_ROBOT_SOUND_01
static struct{
    U8 enable;
    U8 type;
    U16 thres;
    U16 detectthres;
}Robot01Ctl;
#endif 
#if _ADPCM_ROBOT_SOUND_02
static struct{
    U8 enable;
    U8 type;
    U16 thres;
}Robot02Ctl;
#endif 

#if _ADPCM_REVERB_SOUND
static U8 ReverbCmd,ReverbEffect;
#endif

#if _ADPCM_ECHO_SOUND
static EchoCtl_T EchoCtl;
static S8 EchoCmd;
#endif

static U8  StorageMode;
static U32 StartAddr,CurAddr;


/* Dynamic Allocate RAM-------------------------------------------------------*/
#if _USE_HEAP_RAM
#if !(_ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND) || _ADPCM_RECORD
static U8  *DataBuf=NULL;
#endif
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ROBOT_SOUND_02
static U16 *SpeedCtlBuf=NULL;
#endif
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_RECORD || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
static U16 *AudioBuf=NULL;
#endif
#if _ADPCM_ECHO_SOUND
static S16 *EchoBuf=NULL;
#endif

#if _ADPCM_ROBOT_SOUND_01
#if _ROBOT_SOUND_COMPRESS
#define ADPCM_ROBOT_BUF_SIZE      800
#else
#define ADPCM_ROBOT_BUF_SIZE      1600
#endif
static S16 *Robot01_buf=NULL;
#endif

#if _ADPCM_ROBOT_SOUND_02
#if _ROBOT_SOUND_COMPRESS
#define ADPCM_ROBOT02_BUF_SIZE      (36+320+1000)
#else
#define ADPCM_ROBOT02_BUF_SIZE      (36+320+2000)
#endif
static S16 *Robot02_buf=NULL;
#endif

#if _ADPCM_REVERB_SOUND
#define ADPCM_REVERB_BUF_SIZE   830 //word
static S16 *Reverb_x=NULL;
static S16 *Reverb_y=NULL;
#endif


#if _ADPCM_RECORD
#if AGC_CTL_ENABLE
static U16 *AgcBuf=NULL;
#endif
#endif

#else
#if !(_ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND) || _ADPCM_RECORD
static U8 DataBuf[DATA_BUF_SIZE];
#endif
#if _ADPCM_ROBOT_SOUND_02
static U16 SpeedCtlBuf[SPEED_CTL_FRAME_SIZE+40];
#elif _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE 
static U16 SpeedCtlBuf[SPEED_CTL_FRAME_SIZE];
#endif
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_RECORD || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND 
static U16 AudioBuf[AUDIO_BUF_SIZE];
#endif
#if _ADPCM_ECHO_SOUND
static S16 EchoBuf[ADPCM_ECHO_BUF_SIZE];
#endif
#if _ADPCM_ROBOT_SOUND_01
#if _ROBOT_SOUND_COMPRESS
#define ADPCM_ROBOT_BUF_SIZE      810
#else
#define ADPCM_ROBOT_BUF_SIZE      1620
#endif
static S16 Robot01_buf[ADPCM_ROBOT_BUF_SIZE];
#endif

#if _ADPCM_ROBOT_SOUND_02
#if _ROBOT_SOUND_COMPRESS
#define ADPCM_ROBOT02_BUF_SIZE      (36+320+1000)
#else
#define ADPCM_ROBOT02_BUF_SIZE      (36+320+2000)
#endif
static S16 Robot02_buf[ADPCM_ROBOT02_BUF_SIZE];
#endif

#if _ADPCM_REVERB_SOUND
#define ADPCM_REVERB_BUF_SIZE   830 //word
static S16 Reverb_x[ADPCM_REVERB_BUF_SIZE];
static S16 Reverb_y[ADPCM_REVERB_BUF_SIZE];
#endif


#if _ADPCM_RECORD
#if AGC_CTL_ENABLE
static U16 AgcBuf[AGC_FRAME_SIZE];
#endif
#endif

#endif

/* Extern Functions ----------------------------------------------------------*/
extern void CB_ReadData(U8 mode, U32 *cur_addr, U8 *buf,U16 size);
extern void CB_Seek(U32 offset);
extern void CB_WriteData(U8 mode, U32 *cur_addr,U8 *buf,U16 size);
extern void CB_WriteHeader(U8 mode, U32 *cur_addr,U8 *buf,U16 size);
extern void CB_SetStartAddr(U32 addr,U8 mode,U8 rw,U32 *start_addr);
extern void CB_ADPCM_PlayStart(void);
extern void CB_ADPCM_PlayEnd(void);
extern void CB_ADPCM_RecStart(void);
extern void CB_ADPCM_RecEnd(void);
extern void CB_InitDac(CH_TypeDef *chx,U16 sample_rate);
extern void CB_DacCmd(CH_TypeDef *chx,U8 cmd);
extern void CB_ChangeSR(CH_TypeDef *chx,U16 sample_rate);
extern void CB_InitAdc(U16 sample_rate);
extern void CB_AdcCmd(U8 cmd);
extern void CB_WriteDac(U8 size,U16* psrc16);

//------------------------------------------------------------------//
// Allocate ADPCM memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
#if _USE_HEAP_RAM
static void ADPCM_AllocateMemory(U16 func)
{
    if(func&MEMORY_ALLOCATE_PLAY)    //DataBuf & AudioBuf & SpeedCtlBuf together
    {
#if !(_ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND) || _ADPCM_RECORD
        if(DataBuf==NULL)   DataBuf = MemAlloc(DATA_BUF_SIZE);
#endif

#if  _ADPCM_ROBOT_SOUND_02
        if(SpeedCtlBuf==NULL)   SpeedCtlBuf = MemAlloc((SPEED_CTL_FRAME_SIZE+40)<<1);
        if(AudioBuf==NULL)
            AudioBuf = (U16 *)MemAlloc(AUDIO_BUF_SIZE<<1);
#elif _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE 
        if(SpeedCtlBuf==NULL)   SpeedCtlBuf = MemAlloc(SPEED_CTL_FRAME_SIZE<<1);
        if(AudioBuf==NULL)
            AudioBuf = (U16 *)MemAlloc(AUDIO_BUF_SIZE<<1);
#elif _ADPCM_ECHO_SOUND||_ADPCM_ROBOT_SOUND_01||_ADPCM_REVERB_SOUND
        if(AudioBuf==NULL)
            AudioBuf = (U16 *)MemAlloc(AUDIO_BUF_SIZE<<1); 
#endif 

        //dprintf("ADPCM Allocate DataBuf %x\r\n",DataBuf);
    }
    //else
        //dprintf("Allocate twice. func(%x)...\r\n",func);
#if _ADPCM_RECORD
    if(func&MEMORY_ALLOCATE_RECORD)   
    {
        if(AudioBuf==NULL)
            AudioBuf = (U16 *)MemAlloc(AUDIO_BUF_SIZE<<1);
#if AGC_CTL_ENABLE            
        if(AgcBuf==NULL) 
            AgcBuf = (U16 *)MemAlloc(AGC_FRAME_SIZE<<1);
#endif
    }
    //else
        //dprintf("Allocate twice REC. func(%x)...\r\n",func);
#endif
    
}
//------------------------------------------------------------------//
// Free ADPCM memory
// Parameter: 
//          func: Choose Allocate function
// return value:
//          NONE
//------------------------------------------------------------------// 
static void ADPCM_FreeMemory(U16 func)
{
    if(func&MEMORY_ALLOCATE_PLAY)    
    {
#if !(_ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND) || _ADPCM_RECORD   
        if(DataBuf!=NULL)
        {
            MemFree(DataBuf);
            DataBuf=NULL;
        }
#endif        
        
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ROBOT_SOUND_02
        if(SpeedCtlBuf!=NULL)
        {
            MemFree(SpeedCtlBuf);
            SpeedCtlBuf=NULL;
        }
#endif
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
        if(AudioBuf!=NULL)
        {
            MemFree(AudioBuf);
            AudioBuf=NULL;
        }
#endif
#if _ADPCM_ROBOT_SOUND_01    
        if(Robot01_buf!=NULL)
        {
            MemFree(Robot01_buf);
            Robot01_buf = NULL;
        }
#endif
#if _ADPCM_ROBOT_SOUND_02    
        if(Robot02_buf!=NULL)
        {
            MemFree(Robot02_buf);
            Robot02_buf = NULL;
        }
#endif
#if _ADPCM_REVERB_SOUND
        if(Reverb_x!=NULL)
        {
            MemFree(Reverb_x);
            Reverb_x = NULL;
            MemFree(Reverb_y);
            Reverb_y = NULL;
        }
#endif
#if _ADPCM_ECHO_SOUND
        if(EchoBuf!=NULL)
        {
    		MemFree(EchoBuf);
    		EchoBuf=NULL;
        }
#endif
        
        //dprintf("Free Mem\r\n");
    }
    //else
    //{
    //    dprintf("Already Free\r\n");
    //}
#if  _ADPCM_RECORD 
    if(func&MEMORY_ALLOCATE_RECORD)   
    {
        if(AudioBuf!=NULL)
        {
            MemFree(AudioBuf);
            AudioBuf=NULL;
        }
#if AGC_CTL_ENABLE        
        if(AgcBuf!=NULL)
        {
            MemFree(AgcBuf);
            AgcBuf=NULL;
        }
#endif        
        //dprintf("Free Mem REC\r\n");
    }
    //else
    //    dprintf("Already Free REC\r\n");
#endif    
}
#endif

//------------------------------------------------------------------//
// Get 1 byte, called by adpcm decode
// Parameter: 
//          NONE
// return value:
//          data
//------------------------------------------------------------------//
S8 ADPCM0GetByte(void){
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND 
    S8 data;
    
    CB_ReadData(StorageMode,&CurAddr,&data,1);
    
    return data;  
#else
    S8 data;
    
    data=DataBuf[BufCtl.rd_offset];
    BufCtl.rd_offset++;
    if(BufCtl.rd_offset==DATA_BUF_SIZE){
        BufCtl.rd_offset=0;
    }

    return data;
#endif 
  
}    
//------------------------------------------------------------------//
// Dac ISR
// Parameter: 
//          size: fifo fill size 
//           buf: fifo buffer
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_DacIsr(U8 size,S16 *buf){
    S16 temp;
    S16 *psrc16,gain;
    U8 i,len,size2;
    
    if(AudioBufCtl.shadow_state==STATUS_PLAYING){

#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE  || _ADPCM_ECHO_SOUND  || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
        temp=BufCtl.size-BufCtl.offset;
        if(size>temp){
            size=temp;
        }   
#else
        for(i=0;i<size;i++){
            buf[i]=ADPCM0Decode();
            buf[i]=((S16)buf[i]*AudioVol)>>Q15_SHIFT;
        }    

#if _ADPCM_ROBOT_SOUND  
        if(RobotCtl.enable){
            if(RobotCtl.change){
                RoboticVoiceInit(AudioBufCtl.sample_rate, CarrierTbl[RobotCtl.index], size);
                //dprintf("RobotCtl:%d\r\n",RobotCtl.carrier);
                RobotCtl.change=0;
            }    
            RoboticVoiceProcess((short *)buf,(short *)buf);
        }
#endif                    
#endif        
//        //dprintf("size,%d\r\n",size);
        AudioBufCtl.fifo_count+=size;   

#if SMOOTH_ENABLE      
        //smooth process
        if(SmoothCtl.smooth!=SMOOTH_NONE){  
            size2= size;
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE|| _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
            psrc16=(S16 *)(BufCtl.buf+BufCtl.offset);
#else
            psrc16=buf;
#endif        
            while(size2){
                if(SmoothCtl.state==SMOOTH_STEP){
                    if(SmoothCtl.smooth==SMOOTH_DOWN){
                        for(i=0;i<size2;i++){
                            *psrc16++=0;
                        }       
                        AudioBufCtl.shadow_state=AudioBufCtl.state;
                        if(AudioBufCtl.state==STATUS_STOP){
                            CB_ADPCM_PlayEnd();   
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

#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE|| _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
        psrc16=(S16 *)(BufCtl.buf+BufCtl.offset);
        
        CB_WriteDac(size,(U16 *)psrc16);
        
        BufCtl.offset+=size;

        if(BufCtl.offset==BufCtl.size){
            BufCtl.offset=0;    
        }       

#else        
        CB_WriteDac(size,(U16 *)buf);
#endif

#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE||_ADPCM_ROBOT_SOUND_02
        if(AudioBufCtl.fifo_count>=AudioBufCtl.fifo_count2){
            AudioBufCtl.state=STATUS_STOP;
            AudioBufCtl.shadow_state=AudioBufCtl.state;
            CB_ADPCM_PlayEnd();
            //dprintf("stop a,%d,%d\r\n",AudioBufCtl.fifo_count,AudioBufCtl.fifo_count2);
        }
#else        
        if(AudioBufCtl.fifo_count>=AudioBufCtl.samples){
            AudioBufCtl.state=STATUS_STOP;
            AudioBufCtl.shadow_state=AudioBufCtl.state;
            CB_ADPCM_PlayEnd();
            //dprintf("stop a,%d,%d\r\n",AudioBufCtl.fifo_count,AudioBufCtl.samples);
        }
#endif        
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
        SmoothCtl.step_sample=AudioBufCtl.sample_rate*SMOOTH_UP_TIME/1000/SMOOTH_STEP;
    }else if(smooth==SMOOTH_DOWN){
	    SmoothCtl.smooth=SMOOTH_DOWN;
	    SmoothCtl.step_sample=AudioBufCtl.sample_rate*SMOOTH_DOWN_TIME/1000/SMOOTH_STEP;
    }else{
        SmoothCtl.smooth=SMOOTH_NONE;
    }
}     
#endif
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE||_ADPCM_ROBOT_SOUND_02
//------------------------------------------------------------------//
// Decode one audio frame
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
static void DecodeFrame(void)
{
    U16 *audio_buf,i,j,out_cnt;
    U32 access_ptr;
    U32 speed_ctl_size; 
    U16 size;
    U16 ctrl_length;
    
#if _ADPCM_ROBOT_SOUND_02
    if(Robot02Ctl.enable)   ctrl_length = SPEED_CTL_FRAME_SIZE + 40;
    else                    ctrl_length = SPEED_CTL_FRAME_SIZE;
#else
    ctrl_length = SPEED_CTL_FRAME_SIZE;
#endif
    
    while(SpeedCtl.index<ctrl_length){
        
        if(AudioBufCtl.sample_count<AudioBufCtl.samples){
            audio_buf=SpeedCtlBuf+SpeedCtl.index;
            
            size=ctrl_length-SpeedCtl.index;
            
            if((size+AudioBufCtl.sample_count)>AudioBufCtl.samples){
                size=AudioBufCtl.samples-AudioBufCtl.sample_count;
            }    
            for(i=0;i<size;i++){
                audio_buf[i]=ADPCM0Decode();    
            }    
            SpeedCtl.index+=size;
            AudioBufCtl.sample_count+=size;
            AudioBufCtl.fifo_count2=AudioBufCtl.fifo_count+(AUDIO_BUF_SIZE*2);
        }else{
            audio_buf=SpeedCtlBuf+SpeedCtl.index;
            
            size=ctrl_length-SpeedCtl.index;
            
            memset((void *)audio_buf, 0, size*2);
            
            SpeedCtl.index+=size;
        }    
    }    

#if _ADPCM_ROBOT_SOUND_02  
    
    if(Robot02Ctl.enable)
    {
        audio_buf=SpeedCtlBuf+SpeedCtl.rb_ptr;
#if _ROBOT_SOUND_COMPRESS
        ROBOT_EFFECT_COMPRESS_02((S16 *)audio_buf);
#else
        ROBOT_EFFECT_02((S16 *)audio_buf);
#endif
        
        SpeedCtl.rb_ptr += 320;
        out_cnt = (SpeedCtl.rb_ptr)/AUDIO_OUT_SIZE; //how many times output audio_buf
        speed_ctl_size = out_cnt*AUDIO_OUT_SIZE; //residual points that will process next time
        access_ptr = 0;
        for(j=0;j<out_cnt;j++)
        {
            audio_buf=AudioBuf+(AUDIO_OUT_SIZE*AudioBufCtl.index);
            for(i=0;i<AUDIO_OUT_SIZE;i++)
            {
                audio_buf[i] = ((S16)SpeedCtlBuf[access_ptr] * AudioVol)>>Q15_SHIFT;
                if(speed_ctl_size<ctrl_length)
                {
                    SpeedCtlBuf[access_ptr] = SpeedCtlBuf[speed_ctl_size];
                    speed_ctl_size++;
                }
                access_ptr++;
            }
            AudioBufCtl.index++;
            if(AudioBufCtl.index==AUDIO_OUT_COUNT){
                AudioBufCtl.index=0;    
            }    
            AudioBufCtl.process_count+=AUDIO_OUT_SIZE;
            SpeedCtl.rb_ptr -= AUDIO_OUT_SIZE;
            SpeedCtl.index -=  AUDIO_OUT_SIZE;
        }             
    }
    else
#endif                      
    {
        audio_buf=AudioBuf+(AUDIO_OUT_SIZE*AudioBufCtl.index);
        
        AudioBufCtl.index++;
        if(AudioBufCtl.index==AUDIO_OUT_COUNT){
            AudioBufCtl.index=0;    
        }    
        
        if(SpeedCtl.speed_ratio_index==RATIO_INIT_INDEX && SpeedCtl.pitch_ratio_index==RATIO_INIT_INDEX){
            speed_ctl_size=AUDIO_OUT_SIZE;
            for(i=0;i<AUDIO_OUT_SIZE;i++){
                audio_buf[i]=SpeedCtlBuf[i];
            }   
        }else{   
            speed_ctl_size=ctrl_length;
         
        //GPIO_Write(GPIOA,0,0);    
            TimeStretchProcess(audio_buf, SpeedCtlBuf, &speed_ctl_size);
            speed_ctl_size=ctrl_length-speed_ctl_size; 
        }
        //dprintf("%d,%d\r\n",speed_ctl_size,size);
        SpeedCtl.index-=speed_ctl_size;
        for(i=0;i<SpeedCtl.index;i++){
            SpeedCtlBuf[i]=SpeedCtlBuf[i+speed_ctl_size];
        }    
        AudioBufCtl.process_count+=AUDIO_OUT_SIZE;
            

#if _ADPCM_ROBOT_SOUND  
        if(RobotCtl.enable){
            if(RobotCtl.change){
                RoboticVoiceInit(AudioBufCtl.sample_rate, CarrierTbl[RobotCtl.index], AUDIO_OUT_SIZE);
                //dprintf("RobotCtl:%d\r\n",RobotCtl.carrier);
                RobotCtl.change=0;
            }    
            RoboticVoiceProcess((short *)audio_buf,(short *)audio_buf);
        }
#endif    
#if _ADPCM_ROBOT_SOUND_01
        if(Robot01Ctl.enable)
        {
#if _ROBOT_SOUND_COMPRESS                    
            ROBOT_EFFECT_COMPRESS_01((S16 *)audio_buf);
#else
            ROBOT_EFFECT_01((S16 *)audio_buf);
#endif
        }
#endif


#if _ADPCM_REVERB_SOUND
        if(ReverbCmd)
        {
            Reverb_Effect((short *)audio_buf,AUDIO_OUT_SIZE);
        }
#endif                             

#if _ADPCM_ECHO_SOUND
        if(EchoCmd)
        {
            EchoVoiceProcess(audio_buf, AUDIO_OUT_SIZE, &EchoCtl);
        }
#endif
        //volume 
        for(i=0;i<AUDIO_OUT_SIZE;i++){
            audio_buf[i]=((S16)audio_buf[i]*AudioVol)>>Q15_SHIFT;
        }
    }


   
}    
#elif _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_REVERB_SOUND
//------------------------------------------------------------------//
// Decode one audio frame
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
static void DecodeFrame(void)
{
    U16 *audio_buf,i;
    U32 speed_ctl_size; 
    U16 size;

    size = AUDIO_OUT_SIZE;
    if((size+AudioBufCtl.sample_count)>AudioBufCtl.samples)
    {
        size=AudioBufCtl.samples-AudioBufCtl.sample_count;
    } 
    audio_buf=AudioBuf+(AUDIO_OUT_SIZE*AudioBufCtl.index);
    for(i=0;i<size;i++)
    {
        audio_buf[i]=ADPCM0Decode();   
    } 
    AudioBufCtl.sample_count+=size;
    AudioBufCtl.index++;
    if(AudioBufCtl.index==AUDIO_OUT_COUNT){
        AudioBufCtl.index=0;    
    }    
    AudioBufCtl.process_count+=AUDIO_OUT_SIZE;

#if _ADPCM_ROBOT_SOUND_01
    if(Robot01Ctl.enable)
    {
#if _ROBOT_SOUND_COMPRESS                    
        ROBOT_EFFECT_COMPRESS_01((S16 *)audio_buf);
#else
        ROBOT_EFFECT_01((S16 *)audio_buf);
#endif
    }
#endif

#if _ADPCM_REVERB_SOUND
    if(ReverbCmd)
    {
        Reverb_Effect((short *)audio_buf,AUDIO_OUT_SIZE);
    }
#endif                 

#if _ADPCM_ECHO_SOUND
    if(EchoCmd)
    {
        EchoVoiceProcess(audio_buf, AUDIO_OUT_SIZE, &EchoCtl);
    }
#endif
    //volume 
    for(i=0;i<AUDIO_OUT_SIZE;i++){
        audio_buf[i]=((S16)audio_buf[i]*AudioVol)>>Q15_SHIFT;
    }   
}
#endif

#if _ADPCM_RECORD 
//------------------------------------------------------------------//
// Stop record
// Parameter: 
//          None
// return value:
//          None
//------------------------------------------------------------------// 
static void  StopRecord(void)
{
    CB_AdcCmd(DISABLE);
    WriteHeader();
    AudioBufCtl.state=STATUS_STOP;   
#if _USE_HEAP_RAM                            
    ADPCM_FreeMemory(MEMORY_ALLOCATE_RECORD);
#endif       
    CB_ADPCM_RecEnd();
}
#endif
//------------------------------------------------------------------//
// Start play voice
// Parameter: 
//          index: file index or absolute address
//          mode:OTP_MODE,SPI_MODE,SD_MODE
// return value:
//          0:ok
//          others:fail
//------------------------------------------------------------------// 
U8 ADPCM_Play(U32 index,U8 mode){
    struct AUDIO_HEADER *pheader;
#if !(_ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE|| _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND)
    U8 header[HEADER_LEN];
#endif   
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
    
//    DebugBuf=(U16 *)0x2800;
#if _USE_HEAP_RAM    
#if _VR_FUNC
    VR_Halt();
#endif    
    ADPCM_AllocateMemory(MEMORY_ALLOCATE_PLAY);
#endif        
#if _ADPCM_ECHO_SOUND
    if(EchoCmd)
    {
#if _USE_HEAP_RAM
        if(EchoBuf==NULL)
            EchoBuf = (U16 *)MemAlloc(ADPCM_ECHO_BUF_SIZE<<1);
#endif         
        memset((void *)EchoBuf, 0, ADPCM_ECHO_BUF_SIZE<<1);
    }
#endif
    memset((void *)&AudioBufCtl, 0, sizeof(AudioBufCtl));
    
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ROBOT_SOUND_02

    AudioBufCtl.fifo_count2=-1;
#endif      
//    AudioBufCtl.ramp=ramp;
    
 //   SmoothCtl.p_parameter=0;
    
    CB_SetStartAddr(index,mode,0,&StartAddr);
    CurAddr=StartAddr;
    StorageMode=mode;

#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE|| _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
    CB_ReadData(StorageMode,&CurAddr,(U8 *)AudioBuf,HEADER_LEN);
    
    pheader=(struct AUDIO_HEADER *)AudioBuf;
#else
    CB_ReadData(StorageMode,&CurAddr,(U8 *)header,HEADER_LEN);
    
    pheader=(struct AUDIO_HEADER *)header;
#endif    
    //dprintf("Play:%x,%x,%x\r\n",pheader->signature[0],pheader->signature[1],pheader->signature[2]);
    
    if(pheader->codec!=CODEC_ADPCM){  
        return E_AUDIO_UNSUPPORT_CODEC;    
    }     
    
    AudioBufCtl.samples=pheader->samples;
    AudioBufCtl.in_size=pheader->decode_in_size;
    AudioBufCtl.out_size=pheader->decode_out_size;
    AudioBufCtl.sample_rate=pheader->sample_rate;

    
    BufCtl.offset=0;
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE|| _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
    BufCtl.size=AUDIO_BUF_SIZE;
    BufCtl.buf=AudioBuf;
#endif

//    
//    dprintf("pheader->decode_in_size,%d\r\n",pheader->decode_in_size);
//    dprintf("pheader->decode_out_size,%d\r\n",pheader->decode_out_size);
//    dprintf("pheader->sample_rate,%d\r\n",pheader->sample_rate);
//    
#if SMOOTH_ENABLE
//    if(ramp&RAMP_UP){
        SmoothFunc(SMOOTH_UP);
//    }else{
//        SmoothFunc(SMOOTH_NONE);
//    }  
#endif
    
    SetADPCM0Mode(pheader->decode_in_size,pheader->decode_out_size);    
    ADPCM0DecInit();

#if _ADPCM_PLAY_AUDIO_CH==0
    CB_InitDac(DAC_CH0,pheader->sample_rate);
#else
    CB_InitDac(DAC_CH1,pheader->sample_rate);
#endif    
   
#if _ADPCM_SPEED_CHANGE
    SpeedCtl.speed_ratio_index=RATIO_INIT_INDEX;
    SpeedCtl.index=0;
    TimeStretchInit(1,SpeedRatioTbl[SpeedCtl.speed_ratio_index]);
#endif
#if _ADPCM_PITCH_CHANGE
    SpeedCtl.pitch_ratio_index=RATIO_INIT_INDEX;
    SpeedCtl.index=0;
    TimeStretchInit(0,1/PitchRatioTbl[SpeedCtl.pitch_ratio_index]);
#if _ADPCM_PLAY_AUDIO_CH==0    
    CB_ChangeSR(DAC_CH0,AudioBufCtl.sample_rate*PitchRatioTbl[SpeedCtl.pitch_ratio_index]);
#else
    CB_ChangeSR(DAC_CH1,AudioBufCtl.sample_rate*PitchRatioTbl[SpeedCtl.pitch_ratio_index]);
#endif
#endif         

#if _ADPCM_ROBOT_SOUND_01
    if(Robot01Ctl.enable)
    {
#if _USE_HEAP_RAM
        if(Robot01_buf==NULL)   Robot01_buf = (S16 *) MemAlloc(ADPCM_ROBOT_BUF_SIZE<<1);
#endif
        memset((void *)Robot01_buf, 0, ADPCM_ROBOT_BUF_SIZE*2);
        ROBOT_EFFECT_01_Init(Robot01_buf,Robot01Ctl.type,Robot01Ctl.thres,Robot01Ctl.detectthres,AudioBufCtl.sample_rate,AUDIO_OUT_SIZE,90,1620);
    }
#endif
#if _ADPCM_ROBOT_SOUND_02
    SpeedCtl.rb_ptr=0;
    SpeedCtl.index=0;
    if(Robot02Ctl.enable)
    {
#if _USE_HEAP_RAM
        if(Robot02_buf==NULL)   Robot02_buf = (S16 *) MemAlloc(ADPCM_ROBOT02_BUF_SIZE<<1);
#endif        
        memset((void *)Robot02_buf, 0, ADPCM_ROBOT02_BUF_SIZE*2);
        ROBOT_EFFECT_02_Init(Robot02_buf,Robot02Ctl.type,Robot02Ctl.thres,AudioBufCtl.sample_rate);
    }
#endif

#if _ADPCM_REVERB_SOUND
    if(ReverbCmd)
    {
#if _USE_HEAP_RAM
        if(Reverb_x==NULL)
        {
            Reverb_x = (S16 *) MemAlloc(ADPCM_REVERB_BUF_SIZE<<1);
            Reverb_y = (S16 *) MemAlloc(ADPCM_REVERB_BUF_SIZE<<1);
        }
#endif        
        Reverb_Effect_Init(Reverb_x,Reverb_y,ReverbEffect);
    }
#endif
//     
//    DecodeFrame();
//    DecodeFrame();
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ECHO_SOUND || _ADPCM_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_02 || _ADPCM_REVERB_SOUND
    memset((void *)AudioBuf, 0, AUDIO_BUF_SIZE*2);
    DecodeFrame();
#else
    CB_ReadData(StorageMode,&CurAddr,DataBuf,DATA_BUF_SIZE);    
    BufCtl.rd_offset=0;
    BufCtl.wr_offset=0;
#endif

    OpMode=0;
    AudioBufCtl.state=  STATUS_PLAYING;
    AudioBufCtl.shadow_state=AudioBufCtl.state;
    
    CB_ADPCM_PlayStart();   
#if _ADPCM_PLAY_AUDIO_CH==0
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
void ADPCM_Pause(void){
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
#if _ADPCM_RECORD 
    if(AudioBufCtl.state==STATUS_RECORDING){
        CB_AdcCmd(DISABLE);
        AudioBufCtl.state=STATUS_PAUSE;      
    }    
#endif         
} 
//------------------------------------------------------------------//
// Resume playing
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_Resume(void){
    if(AudioBufCtl.state==STATUS_PAUSE&&(!OpMode)){

#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_UP){
            SmoothFunc(SMOOTH_UP);
//        }
#endif        
        AudioBufCtl.state=STATUS_PLAYING;
        AudioBufCtl.shadow_state=AudioBufCtl.state;
    }    
#if _ADPCM_RECORD 
    if(AudioBufCtl.state==STATUS_PAUSE&&OpMode){
        AudioBufCtl.state=STATUS_RECORDING;  
        CB_AdcCmd(ENABLE);
    }    
#endif               
} 
//------------------------------------------------------------------//
// Stop playing
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_Stop(void){
    if((AudioBufCtl.state==STATUS_PLAYING||AudioBufCtl.state==STATUS_PAUSE)&&(!OpMode)){ 
#if SMOOTH_ENABLE        
//        if(AudioBufCtl.ramp&RAMP_DOWN){
            AudioBufCtl.state=STATUS_STOP;
            SmoothFunc(SMOOTH_DOWN);
//        }else
#endif            
//        {
//            AudioBufCtl.state=STATUS_STOP;
//            CB_ADPCM_PlayEnd();
//        }
    }            
#if _ADPCM_RECORD 
    if((AudioBufCtl.state==STATUS_RECORDING||AudioBufCtl.state==STATUS_PAUSE)&&OpMode){
        StopRecord();
    }    
#endif     
} 
//------------------------------------------------------------------//
// Get current status
// Parameter: 
//          NONE
// return value:
//          status:STATUS_PLAYING,STATUS_PAUSE,STATUS_STOP
//------------------------------------------------------------------// 
U8 ADPCM_GetStatus(void){
    return AudioBufCtl.shadow_state;    
}
//------------------------------------------------------------------//
// Set audio volume
// Parameter: 
//          vol:0~15
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetVolume(U8 vol){   
    
    if(vol>CH_VOL_15){
        vol=CH_VOL_15;
    }    
    ChVol=  vol;  
}
//------------------------------------------------------------------//
// Set speed ratio
// Parameter: 
//          index: -12~12
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetSpeed(S8 index){   
    
#if _ADPCM_SPEED_CHANGE
    if(AudioBufCtl.sample_rate<=SPEED_CTL_SAMPLE_RATE){
        index+=RATIO_INIT_INDEX;
        SpeedCtl.speed_ratio_index=index;
#if _ADPCM_PITCH_CHANGE        
        TimeStretchInit(0,SpeedRatioTbl[index]/PitchRatioTbl[SpeedCtl.pitch_ratio_index]);
#else
        TimeStretchInit(0,SpeedRatioTbl[index]);
#endif
    }        
#endif     
}
//------------------------------------------------------------------//
// Set pitch ratio
// Parameter: 
//          index: -12~12
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetPitch(S8 index){   
    
#if _ADPCM_PITCH_CHANGE
    if(AudioBufCtl.sample_rate<=SPEED_CTL_SAMPLE_RATE){
        index+=RATIO_INIT_INDEX;
        SpeedCtl.pitch_ratio_index=index;
#if _ADPCM_SPEED_CHANGE
        TimeStretchInit(0,SpeedRatioTbl[SpeedCtl.speed_ratio_index]/PitchRatioTbl[index]);
#else
        TimeStretchInit(0,1/PitchRatioTbl[index]);
#endif
#if _ADPCM_PLAY_AUDIO_CH==0    
        CB_ChangeSR(DAC_CH0,AudioBufCtl.sample_rate*PitchRatioTbl[index]);
#else
        CB_ChangeSR(DAC_CH1,AudioBufCtl.sample_rate*PitchRatioTbl[index]);
#endif
    }
#endif     
}
//------------------------------------------------------------------//
// Enable/Disable Robot Sound Effect
// Parameter: 
//          cmd: 1:enable,0:disable
//          index: carrier index
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetRobotSound(U8 cmd,U8 index){
#if _ADPCM_ROBOT_SOUND   
   
    RobotCtl.enable=cmd;
    
    if(RobotCtl.enable){
        RobotCtl.change=1;
        RobotCtl.index=index;
    }    
#endif    
}   
#if _ADPCM_ROBOT_SOUND_01 
//------------------------------------------------------------------//
// Enable/Disable Robot Sound Effect
// Parameter: 
//          cmd: 1:enable,0:disable
//          type: robot type 0~2
//          thres: robot threshold 0~7
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetRobot01Sound(U8 cmd,U8 type, U8 thres){
    Robot01Ctl.enable=cmd;
    
    if(Robot01Ctl.enable)
    {
        Robot01Ctl.type=type;
        Robot01Ctl.thres= Robot_Thres[thres]; 
        Robot01Ctl.detectthres = 30;
#if _USE_HEAP_RAM
        if(Robot01_buf==NULL)
        {
            Robot01_buf = (S16 *)MemAlloc(ADPCM_ROBOT_BUF_SIZE<<1);
        }
#endif           
        memset((void *)Robot01_buf, 0, ADPCM_ROBOT_BUF_SIZE*2);
        ROBOT_EFFECT_01_Init(Robot01_buf,Robot01Ctl.type,Robot01Ctl.thres,Robot01Ctl.detectthres,AudioBufCtl.sample_rate,AUDIO_OUT_SIZE,90,1620);
    }    
}
#endif
#if _ADPCM_ROBOT_SOUND_02 
//------------------------------------------------------------------//
// Enable/Disable Robot Sound Effect
// Parameter: 
//          cmd: 1:enable,0:disable
//          type: robot type 0~2
//          thres: robot threshold 0~7
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetRobot02Sound(U8 cmd,U8 type, U8 thres){
    Robot02Ctl.enable=cmd;
    
    if(Robot02Ctl.enable)
    {
        Robot02Ctl.type=type;
        Robot02Ctl.thres= Robot02_Thres[thres];     
#if _USE_HEAP_RAM
        if(Robot02_buf==NULL)
        {
            Robot02_buf = (S16 *)MemAlloc(ADPCM_ROBOT02_BUF_SIZE<<1);
        }
#endif        
        memset((void *)Robot02_buf, 0, ADPCM_ROBOT02_BUF_SIZE*2);
        ROBOT_EFFECT_02_Init(Robot02_buf,Robot02Ctl.type,Robot02Ctl.thres,AudioBufCtl.sample_rate);
    }    
#if _USE_HEAP_RAM
    else
    {
        if(Robot02_buf!=NULL)
        {
            MemFree(Robot02_buf);
            Robot02_buf = NULL;
        }
    }
#endif     
}
#endif
#if _ADPCM_REVERB_SOUND
//------------------------------------------------------------------//
// Enable/Disable Reverb Sound Effect
// Parameter: 
//          cmd: 1:enable,0:disable
//          index: Effect index
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_SetReverbSound(U8 cmd,U8 index){
    ReverbCmd = cmd;
    if(cmd)
    {
#if _USE_HEAP_RAM
        if(Reverb_x==NULL)
        {
            Reverb_x = (S16 *)MemAlloc(ADPCM_REVERB_BUF_SIZE<<1);
            Reverb_y = (S16 *)MemAlloc(ADPCM_REVERB_BUF_SIZE<<1);
        }
#endif        
        ReverbEffect = index;
        Reverb_Effect_Init(Reverb_x,Reverb_y,index);
    }    
#if _USE_HEAP_RAM
    else
    {
        if(Reverb_x!=NULL)
        {
            MemFree(Reverb_x);
            Reverb_x = NULL;
            MemFree(Reverb_y);
            Reverb_y = NULL;
        }
    }
#endif    
}        
#endif
#if _ADPCM_ECHO_SOUND
//------------------------------------------------------------------//
// Enable/Disable Echo Sound Effect
// Parameter:
//          cmd: 1:enable,0:disable
//          index: echo type
// return value:
//          NONE
//------------------------------------------------------------------//
void ADPCM_SetEchoSound(U8 cmd,U8 index){

    if(cmd == ENABLE)
    {
#if _USE_HEAP_RAM
        if(EchoBuf==NULL)   EchoBuf = (U16 *)MemAlloc(ADPCM_ECHO_BUF_SIZE<<1);
#endif         
        EchoCtl.flag = 0;
        EchoCtl.count= 0;
        EchoCtl.frameCount = 0;
        EchoCtl.buf = EchoBuf;
        EchoCtl.EchoBufL = ADPCM_ECHO_DATA_LENGTH;
        EchoCtl.EchoNB = EchoCoef[0][index];;
        EchoCtl.EchoWT = EchoCoef[1][index];;
        EchoCtl.RingBufPeriod = EchoCoef[2][index];
        EchoCmd = ENABLE;
    }
    else
    {
#if _USE_HEAP_RAM
        if(EchoBuf!=NULL)
        {
            MemFree(EchoBuf);
            EchoBuf=NULL;
        }
#endif          
        EchoCmd = DISABLE;
    }
}
#endif
//------------------------------------------------------------------//
// Codec init
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_Init(void){
    ChVol=CH_VOL_15;  
      
#if _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE || _ADPCM_ROBOT_SOUND_02
     SpeedCtl.speed_ratio_index=RATIO_INIT_INDEX;
     SpeedCtl.pitch_ratio_index=RATIO_INIT_INDEX;
#endif        
#if _ADPCM_ECHO_SOUND
    EchoCmd = DISABLE;
#endif
}
//------------------------------------------------------------------//
// Start record voice
// Parameter: 
//          index: file index or absolute address
//          mode:SPI_MODE,SD_MODE
//          sample_rate:sample rate
//          size: record size
// return value:
//          0:ok
//          others:fail
//------------------------------------------------------------------// 
#if _ADPCM_RECORD
U8 ADPCM_Record(U32 index,U8 mode,U16 sample_rate,U32 size){
    U8 i;
#if _USE_HEAP_RAM    
    ADPCM_AllocateMemory(MEMORY_ALLOCATE_PLAY|MEMORY_ALLOCATE_RECORD);
#endif    
    memset((void *)&AudioBufCtl, 0, sizeof(AudioBufCtl));
    
    AudioBufCtl.sample_rate=sample_rate;
    AudioBufCtl.out_size=32;
    AudioBufCtl.in_size=21;

#if NOISE_GATE_ENABLE
    if(sample_rate<16000){
        NGInit(NG_RELEASE_SAMPLE_8K,NG_ATTACK_SAMPLE_8K,NG_ON_OFF_SAMPLE_8K);
    }else{
        NGInit(NG_RELEASE_SAMPLE_16K,NG_ATTACK_SAMPLE_16K,NG_ON_OFF_SAMPLE_16K);
    }    
#endif

#if HPF_ENABLE    
    for (i=0;i<=HPF_ORDER;i++) 
	{
		x[i]=0;
		y[i]=0;
	}
	
	CoeffB=CoeffB_16K;
	CoeffA=CoeffA_16K;
	if(sample_rate<16000){
        CoeffB=CoeffB_8K;
	    CoeffA=CoeffA_8K;
    }    
#endif	
//    StorageIndex=index;
#if AGC_CTL_ENABLE    
    AgcCnt=0;
    AgcInit(ALPHA,BETA,TARGET_RMS);   
    LimiterInit(LIMITER_THRESHOLD);
#endif
     
    CB_SetStartAddr(index,mode,1,&StartAddr);
    CurAddr=StartAddr+HEADER_LEN;
    StorageMode=mode;
    
    AudioBufCtl.out_count=AUDIO_BUF_SIZE/AudioBufCtl.out_size;
    BufCtl.offset=0;   
    BufCtl.size=AudioBufCtl.out_size*AudioBufCtl.out_count;
    BufCtl.buf=AudioBuf;
    
    SetADPCM0Mode(AudioBufCtl.in_size,AudioBufCtl.out_size);       
    AdpcmEncInit();
    
    AudioBufCtl.samples+= HEADER_LEN;
       
    CB_InitAdc(sample_rate);
    
    OpMode=1;
    AudioBufCtl.state=STATUS_RECORDING;
    
    CB_ADPCM_RecStart();
    
    CB_AdcCmd(ENABLE);
    RecordSize=size;
    

    return 0;
}
//extern const U8  VOICEFILEX[ ];
//U32 VoiceIdx=0;
//------------------------------------------------------------------//
// Adc ISR
// Parameter: 
//          size: fifo fill size 
//       pdest32: fifo address
// return value:
//          NONE
//------------------------------------------------------------------// 
void ADPCM_AdcIsr(U8 size,volatile U32 *pdest32){
    S16 *pdst16;
    U8 i;
    U16 temp;
    
    if(AudioBufCtl.state==STATUS_RECORDING){
      
        temp=BufCtl.size-BufCtl.offset;
        if(size>temp){
            size=temp;
        }   
#if AGC_CTL_ENABLE        
        temp=AGC_FRAME_SIZE-AgcCnt;
        if(size>temp){
            size=temp;
        }   
#endif          
        if((AudioBufCtl.fifo_count+size-AudioBufCtl.process_count)>(AudioBufCtl.out_count*AudioBufCtl.out_size)){
            for(i=0;i<size;i++){
                temp=(*pdest32-32768);    
            }   
            //dprintf("E\r\n");
            size=0;
        }    
        //dprintf("size,%d\r\n",size);
        AudioBufCtl.fifo_count+=size;   
        

        pdst16=(S16 *)(BufCtl.buf+BufCtl.offset);
        
        for(i=0;i<size;i++){
            temp=(*pdest32-32768);   
            *pdst16++=temp;
#if AGC_CTL_ENABLE            
            AgcBuf[AgcCnt+i]=temp;
#endif              
//            temp=(*pdest32-32768);     
//            *pdst16++= (VOICEFILEX[VoiceIdx]|(VOICEFILEX[VoiceIdx+1]<<8));
//            VoiceIdx+=2;
        }   
        BufCtl.offset+=size;
        
#if AGC_CTL_ENABLE        
        AgcCnt+=size;
        if(AgcCnt==AGC_FRAME_SIZE){
            AgcCnt=0;

            AgcProcess((S16 *)AgcBuf,AGC_FRAME_SIZE);   
        }     
#endif      
        if(BufCtl.offset==BufCtl.size){
            BufCtl.offset=0;    
        }       
    }             
}   
//------------------------------------------------------------------//
// Encode one frame
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//  
static void EncodeFrame(void)
{
    S16 *audio_buf;
    U8 n,i;
    S32 out;
     
    audio_buf=AudioBuf+AudioBufCtl.out_size*AudioBufCtl.index;
    
    AudioBufCtl.index++;
    if(AudioBufCtl.index==AudioBufCtl.out_count){
        AudioBufCtl.index=0;   
    }    
    
#if AGC_CTL_ENABLE    
    LimiterProcess(audio_buf,audio_buf,LIMITER_THRESHOLD,AudioBufCtl.out_size,0);
#endif

#if HPF_ENABLE    
    for(i=0;i<AudioBufCtl.out_size;i++) {
        
        for(n=HPF_ORDER; n>0; n--) {
            x[n] = x[n-1];
            y[n] = y[n-1];
        }
    
        x[0] = audio_buf[i];
        out = CoeffB[0] * x[0];
        for(n=1; n<=HPF_ORDER; n++){
            out += ((S32)CoeffB[n] * x[n] - (S32)CoeffA[n] * y[n]);
        }    			
        out = out >> HPF_QFORMAT;
    	y[0] = out; 
    	if(out > 32767){
            out = 32767;
        }else if(out < -32768){
            out = -32768;	
        } 
        audio_buf[i] = (S16)out;   
    }
#endif
    	    
#if NOISE_GATE_ENABLE
    NGProcess(audio_buf,audio_buf,AudioBufCtl.out_size);
#endif       
		
    ADPCMEncode(AudioBufCtl.out_size,audio_buf,DataBuf);
    
    AudioBufCtl.process_count+=AudioBufCtl.out_size;
    
    CB_WriteData(StorageMode,&CurAddr,(U8 *)DataBuf,AudioBufCtl.in_size);
    AudioBufCtl.samples+=AudioBufCtl.in_size;
    if((AudioBufCtl.samples+AudioBufCtl.in_size)>=RecordSize)
    {
        StopRecord();
    }
}   

//------------------------------------------------------------------//
// Write audio header
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
static void WriteHeader(void){
    struct AUDIO_HEADER *pheader;

    pheader=(struct AUDIO_HEADER *)DataBuf;
    
    memset(pheader,0,HEADER_LEN);
    
    pheader->decode_in_size=AudioBufCtl.in_size;
    pheader->decode_out_size=AudioBufCtl.out_size;
    pheader->sample_rate=AudioBufCtl.sample_rate;
    pheader->header_size=HEADER_LEN;
    pheader->file_size=AudioBufCtl.samples;
    pheader->codec=CODEC_ADPCM;
    
    memcpy(pheader->signature,Signature,sizeof(Signature));

    pheader->samples=AudioBufCtl.process_count;
    
    CurAddr=StartAddr;
    CB_WriteHeader(StorageMode,&CurAddr,(U8 *)DataBuf,HEADER_LEN);
}  
#endif
//------------------------------------------------------------------//
// Service loop
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
void ADPCM_ServiceLoop(void){
    U16 size;
#if _ADPCM_ROBOT_SOUND_02
    U16 process_size;
#endif
    
//    if((SmoothCtl.p_parameter&P_WAIT)&&(AudioBufCtl.state==STATUS_STOP)){
//        SmoothCtl.p_parameter&=(~P_WAIT);
//        ADPCM_Play(SmoothCtl.p_index,(SmoothCtl.p_parameter&P_MODE_MASK)>>P_MODE_SHIFT,
//                   (SmoothCtl.p_parameter&P_DAC_MASK)>>P_DAC_SHIFT,SmoothCtl.p_parameter&P_RAMP_MASK);  
//    }    
        
    if(AudioBufCtl.shadow_state==STATUS_PLAYING){
       	GIE_DISABLE();
        AudioVol=  (ChVol+1)*(MixerVol+1); 
        
        if(!ChVol){
            AudioVol=0;
        }    
        if(AudioVol==256){
            AudioVol=255;
        }    
        AudioVol=AudioVol<<7;
		GIE_ENABLE();
#if     _ADPCM_ROBOT_SOUND_02
        if(Robot02Ctl.enable)     process_size=360; 
        else                    process_size=AUDIO_OUT_SIZE;
        
        if((AudioBufCtl.fifo_count+(AUDIO_BUF_SIZE-process_size)-AudioBufCtl.process_count)>=0){
            
            DecodeFrame();
        }
#elif  _ADPCM_SPEED_CHANGE||_ADPCM_PITCH_CHANGE||_ADPCM_ECHO_SOUND||_ADPCM_ROBOT_SOUND_01||_ADPCM_REVERB_SOUND
        if((AudioBufCtl.fifo_count+(AUDIO_BUF_SIZE-AUDIO_OUT_SIZE)-AudioBufCtl.process_count)>=0)
        {
            DecodeFrame();
        }
#else

         
        if(BufCtl.rd_offset!=BufCtl.wr_offset)
        {
            if(BufCtl.rd_offset>BufCtl.wr_offset)
            {
                size=BufCtl.rd_offset-BufCtl.wr_offset;    
            }
            else
            {
                size=DATA_BUF_SIZE-BufCtl.wr_offset;  
            }     
            CB_ReadData(StorageMode,&CurAddr,DataBuf+BufCtl.wr_offset,size);    
            BufCtl.wr_offset+=size;
            if(BufCtl.wr_offset==DATA_BUF_SIZE)
            {
                BufCtl.wr_offset=0;    
            }  
        } 
          
                 
             
#endif
    }

#if _ADPCM_RECORD    
    if(AudioBufCtl.state==STATUS_RECORDING){
        if((AudioBufCtl.fifo_count-AudioBufCtl.process_count)>=AudioBufCtl.out_size){
            //dprintf("%d\r\n",VoiceIdx);
            EncodeFrame();
        }          
    }   
#endif        
#if _USE_HEAP_RAM                            
#if _ADPCM_RECORD
    if((AudioBufCtl.shadow_state==STATUS_STOP) && (AudioBufCtl.state!=STATUS_RECORDING) && DataBuf!=NULL )        
#else
    if(AudioBufCtl.shadow_state==STATUS_STOP)        
#endif
    {
        ADPCM_FreeMemory(MEMORY_ALLOCATE_PLAY);
    }
#endif
}    
#endif
