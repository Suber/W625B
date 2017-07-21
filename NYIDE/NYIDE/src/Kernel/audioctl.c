/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "include.h"
#include "nx1_dac.h"
#include "nx1_adc.h"
#include "nx1_spi.h"
#include "nx1_smu.h"
#include "nx1_intc.h"
#include "nx1_timer.h"


//////////////////////////////////////////////////////////////////////
//  Defines
//////////////////////////////////////////////////////////////////////
#if _DAC_CH0_TIMER==USE_TIMER1
#define DAC_CH0_TIMER               P_TIMER1
#define DAC_CH0_TRIGER              DAC_TIMER1_TRIG
#elif _DAC_CH0_TIMER==USE_TIMER2
#define DAC_CH0_TIMER               P_TIMER2
#define DAC_CH0_TRIGER              DAC_TIMER2_TRIG
#else
#define DAC_CH0_TIMER               P_TIMER0
#define DAC_CH0_TRIGER              DAC_TIMER0_TRIG
#endif

#if _DAC_CH1_TIMER==USE_TIMER0
#define DAC_CH1_TIMER               P_TIMER0
#define DAC_CH1_TRIGER              DAC_TIMER0_TRIG
#elif _DAC_CH1_TIMER==USE_TIMER2
#define DAC_CH1_TIMER               P_TIMER2
#define DAC_CH1_TRIGER              DAC_TIMER2_TRIG
#else
#define DAC_CH1_TIMER               P_TIMER1
#define DAC_CH1_TRIGER              DAC_TIMER1_TRIG
#endif

#if _ADC_TIMER==USE_TIMER1
#define ADC_TIMER                   P_TIMER1
#define ADC_TRIGER                  ADC_TIMER1_TRIG
#elif _ADC_TIMER==USE_TIMER2
#define ADC_TIMER                   P_TIMER2
#define ADC_TRIGER                  ADC_TIMER2_TRIG
#else
#define ADC_TIMER                   P_TIMER0
#define ADC_TRIGER                  ADC_TIMER0_TRIG
#endif



//////////////////////////////////////////////////////////////////////
//  Variables
//////////////////////////////////////////////////////////////////////
//static volatile struct{
//    U32 index;          //play index
//    U8  parameter;      //play parameter
//    U8  ch;             //channel
//}PlayCtl;
static U8 DACUpsample=0,PPGain;
U8 MixerVol;
static U8 ChInfo[_AUDIO_API_CH_NUM];
static U8 AudioVol[_AUDIO_API_CH_NUM];

const U32 PlayList[]= 
{
	(U32) 0,
};

#if _SBC_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_01 || _CELP_ROBOT_SOUND_01 || _PCM_ROBOT_SOUND_01 || _SBC_CH2_ROBOT_SOUND_01 || _ADPCM_CH2_ROBOT_SOUND_01 ||  _RT_ROBOT_SOUND_01
const U16 Robot_Thres[8] = { 0, 328, 655, 983, 1638, 2294, 3277, 4260 };
#endif
#if _SBC_ROBOT_SOUND_02 || _ADPCM_ROBOT_SOUND_02 || _CELP_ROBOT_SOUND_02 || _PCM_ROBOT_SOUND_02 || _SBC_CH2_ROBOT_SOUND_02 || _ADPCM_CH2_ROBOT_SOUND_02 || _RT_ROBOT_SOUND_02
const U16 Robot02_Thres[8] = { 328, 655, 983, 1638, 2294, 3277, 4260, 5243 };
#endif

#pragma weak PlayList
//------------------------------------------------------------------//
// Set PP gain. When InitDac has called, this PP gain will apply on HW.
// Parameter: 
//          gain:dac gain(0~15)
// return value:
//          NONE
//------------------------------------------------------------------//    
void SetPPGain(U8 gain)
{
#define PP_GAIN_VER       (((*(U8 *)0x170)>>7)&0x01)

    if(!PP_GAIN_VER) gain = gain^0xF; 
    PPGain=gain;
} 
//------------------------------------------------------------------//
// Enable or DISABLE upsample 
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//    
void SetUpsample(U8 cmd)
{
    if(cmd)
    {
        DACUpsample=1;
    }else
    {
        DACUpsample=0;
    }        
}
//------------------------------------------------------------------//
// Change sample rate
// Parameter: 
//          chx         : DAC_CH0, DAC_CH1
//          sample_rate : sample rate
// return value:
//          NONE
//------------------------------------------------------------------//    
void SetSampleRate(CH_TypeDef *chx,U16 sample_rate)
{
    U16 divisor;
        
    if(!sample_rate)
    {
        return;
    }    
    //divisor=(((float)_SYS_CLK/sample_rate)+0.5)-1;
    divisor=((((_SYS_CLK*64)/sample_rate)+32)>>6)-1;
    
    //up=0;
    if(DACUpsample)
    {
        divisor = divisor>>2;
        //up=1;
    }
    
    if(chx==DAC_CH0)
    {
        DAC_CH0_TIMER->Data=divisor;
    }     
    else if(chx==DAC_CH1)
    {
        DAC_CH1_TIMER->Data=divisor;
    }           
}     
//------------------------------------------------------------------//
// Init Dac 
// Parameter: 
//          chx: DAC_CH0, DAC_CH1
//          sample_rate:sample rate
// return value:
//          NONE
//------------------------------------------------------------------//   
void InitDac(CH_TypeDef *chx,U16 sample_rate)
{
    U8 i,up;
    U16 divisor;

#if _VR_FUNC
    VR_Halt();
#endif
    
	SMU_PeriphClkCmd(SMU_PERIPH_DAC,ENABLE);

    
    //divisor=(((float)_SYS_CLK/sample_rate)+0.5)-1;
    divisor=((((_SYS_CLK*64)/sample_rate)+32)>>6)-1;    
    
    up=0;
    if(DACUpsample)
    {
        divisor = divisor>>2;
        up=1;
    }
    
    if(chx==DAC_CH0)
    {
        TIMER_Init(DAC_CH0_TIMER,TIMER_CLKDIV_1,divisor);
        TIMER_Cmd(DAC_CH0_TIMER,ENABLE);//to trigger DAC fifo
    
        if(!(chx->CH_Ctrl & C_DAC_Trig_Timer_En))
        {
            DAC_Init(chx,DAC_CH0_TRIGER,DAC_FIFO_THD,up);
        }
    }
    
    else if(chx==DAC_CH1)
    {
        TIMER_Init(DAC_CH1_TIMER,TIMER_CLKDIV_1,divisor);
        TIMER_Cmd(DAC_CH1_TIMER,ENABLE);//to trigger DAC fifo
        
        if(!(chx->CH_Ctrl & C_DAC_Trig_Timer_En))
        {
            DAC_Init(chx,DAC_CH1_TRIGER,DAC_FIFO_THD,up);
        }
    }   
    DAC_PPCmd(ENABLE);
    DAC_SetPPIpGain(PPGain);    
    if(!(chx->CH_Ctrl&C_DAC_Trig_Timer_En))
    {
        for(i=0;i<DAC_FIFO_LEN;i++)
        {
            chx->CH_Data=0;
        }
    }
    if(chx==DAC_CH0)
    {
        INTC_IrqCmd(IRQ_BIT_DAC_CH0,ENABLE);
    }else
    {
        INTC_IrqCmd(IRQ_BIT_DAC_CH1,ENABLE);
    }
}    
//------------------------------------------------------------------//
// Init Adc Flow 
// Parameter: 
//          sample_rate:sample rate
// return value:
//          NONE
//------------------------------------------------------------------//   
void InitAdc(U16 sample_rate)
{
    U16 divisor;
    
	SMU_PeriphClkCmd(SMU_PERIPH_ADC,ENABLE);
    SMU_PeriphReset(SMU_PERIPH_ADC);
    
    //divisor=(((float)_SYS_CLK/sample_rate)+0.5)-1;
    divisor=((((_SYS_CLK*64)/sample_rate)+32)>>6)-1;
    
      
    TIMER_Init(ADC_TIMER,TIMER_CLKDIV_1,divisor);
    TIMER_Cmd(ADC_TIMER,ENABLE);//to trigger ADC fifo  
    ADC_Init(ADC_TRIGER,ADC_CH_MIC,ADC_FIFO_THD);
    
    ADC_MicBiasCmd(ENABLE);
#if _RT_PLAY
    ADC_SetGain(0);
    ADC_MicPreGain(C_ADC_MIC_AMP_Gain_15dB);
#else
    ADC_SetGain(31);
    ADC_MicPreGain(C_ADC_MIC_AMP_Gain_30dB);
#endif
    //Delayms(200);
    //ADC_SetScanMode(ADC_SCAN_2);
    
    INTC_IrqCmd(IRQ_BIT_ADC,ENABLE);
}
//------------------------------------------------------------------//
// Audio Control Init
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void AudioCtlInit(void){
   // U8 i;

    MixerVol=CH_VOL_15;
    SetUpsample(ENABLE);    

 
#if _PP_GAIN==0
    SetPPGain(3);
#elif _PP_GAIN==1
    SetPPGain(6);
#elif _PP_GAIN==2
    SetPPGain(9);
#elif _PP_GAIN==3
    SetPPGain(14);
#endif 
}  
//------------------------------------------------------------------//
// Get current status
// Parameter: 
//          ch:audio channel
// return value:
//          status:STATUS_PLAYING,STATUS_PAUSE,STATUS_STOP
//------------------------------------------------------------------// 
U8 AUDIO_GetStatus(U8 ch){
    U8 sts,codec;    

    codec=ChInfo[ch];

    sts=STATUS_STOP;
#if _ADPCM_PLAY    
    if(codec==CH_ADPCM){
        sts=ADPCM_GetStatus();
    }
#endif  
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2){
        sts=ADPCM_CH2_GetStatus();
    }
#endif  
#if _SBC_PLAY    
    if(codec==CH_SBC){
        sts=SBC_GetStatus();
    }
#endif   
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2){
        sts=SBC_CH2_GetStatus();
    }
#endif   
#if _CELP_PLAY    
    if(codec==CH_CELP){
        sts=CELP_GetStatus();
    }
#endif   
#if _MIDI_PLAY    
    if(codec==CH_MIDI){
        sts=MIDI_GetStatus();
    }
#endif 
#if _PCM_PLAY    
    if(codec==CH_PCM){
        sts=PCM_GetStatus();
    }
#endif   
    return sts;
}
//------------------------------------------------------------------//
// Set audio channel volume
// Parameter: 
//          ch:audio channel
//          vol:0~15
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetChVolume(U8 ch,U8 vol){ 
    U8 codec;
      
    codec=ChInfo[ch];

    AudioVol[ch]=vol;
    
#if _ADPCM_PLAY    
    if(codec==CH_ADPCM){
        ADPCM_SetVolume(vol);
    }    
#endif  
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2){
        ADPCM_CH2_SetVolume(vol);
    }    
#endif   
#if _SBC_PLAY    
    if(codec==CH_SBC){
        SBC_SetVolume(vol);
    }    
#endif   
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2){
        SBC_CH2_SetVolume(vol);
    }    
#endif   
#if _CELP_PLAY    
    if(codec==CH_CELP){
        CELP_SetVolume(vol);
    }    
#endif   
#if _MIDI_PLAY    
    if(codec==CH_MIDI){
        MIDI_SetVolume(vol);
    }    
#endif   
#if _PCM_PLAY    
    if(codec==CH_PCM){
        PCM_SetVolume(vol);
    }    
#endif   
}
//------------------------------------------------------------------//
// Set audio mixer volume
// Parameter: 
//          vol:0~15
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetMixerVolume(U8 vol){ 
    MixerVol=vol;
}
//------------------------------------------------------------------//
// Pause playing
// Parameter: 
//          ch:audio channel
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_Pause(U8 ch){
    U8 codec;
      
    codec=ChInfo[ch];

#if _ADPCM_PLAY    
    if(codec==CH_ADPCM){
        ADPCM_Pause();
    }          
#endif  
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2){
        ADPCM_CH2_Pause();
    }          
#endif
#if _SBC_PLAY    
    if(codec==CH_SBC){
        SBC_Pause();
    }          
#endif  
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2){
        SBC_CH2_Pause();
    }          
#endif   
#if _CELP_PLAY    
    if(codec==CH_CELP){
        CELP_Pause();
    }          
#endif   
#if _MIDI_PLAY    
    if(codec==CH_MIDI){
        MIDI_Pause();
    }          
#endif   
#if _PCM_PLAY    
    if(codec==CH_PCM){
        PCM_Pause();
    }          
#endif         
}
//------------------------------------------------------------------//
// Get file count in SPI
// Parameter: 
//          NONE
// return value:
//          type
//------------------------------------------------------------------//     
U16 AUDIO_GetSpiFileCount(void){
    U8 tempbuf[4];
    U32 addr;
     
    SPI_BurstRead(tempbuf,SPI_RESERVED_SIZE-4,4);
    addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 
               
    SPI_BurstRead(tempbuf,addr,SPI_FILE_COUNT_SIZE);
    
    return(tempbuf[0]|(tempbuf[1]<<8));
}    
//------------------------------------------------------------------//
// Get codec type from index
// Parameter: 
//          index: file index or absolute address
//          mode:OTP_MODE,SPI_MODE,SD_MODE
// return value:
//          type
//------------------------------------------------------------------// 
static U8 GetCodec(U32 index,U8 mode){
    U8 tempbuf[SPI_TAB_INFO_ENTRY_SIZE];
    U32 addr;
    U8 sts,codec,offset;
    struct AUDIO_HEADER *header;
    
    header=(struct AUDIO_HEADER *)0;
    offset=(U32)&(header->codec);
    if(mode==SPI_MODE){
        if(index&ADDRESS_MODE){
            addr=index&(~ADDRESS_MODE);
            SPI_BurstRead((U8 *)&codec,addr+offset,1);
        }else{
            SPI_BurstRead(tempbuf,SPI_RESERVED_SIZE-4,4);
            addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 
               
            SPI_BurstRead(tempbuf,addr+SPI_BASE_INFO_SIZE+(SPI_TAB_INFO_ENTRY_SIZE*index),SPI_TAB_INFO_ENTRY_SIZE);
        
            addr=addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24));
            
            addr&=(~SPI_DATA_TYPE_MASK);
            if((tempbuf[3]>>4)==SPI_DATA_TYPE_MIDI){
                codec=CODEC_MIDI;
            }else{
                SPI_BurstRead((U8 *)&codec,addr+offset,1);
            }        
            
            //sts=tempbuf[3]>>4;
        }    
    }else if(mode==OTP_MODE){
        addr=PlayList[index];
        codec=*((U8 *)(addr+offset));
    }  
    //dprintf("codec:%x\r\n",codec);
   
    return codec;
}  
//------------------------------------------------------------------//
// Start play voice
// Parameter: 
//          ch:audio channel
//          index: file index or absolute address
//          mode:OTP_MODE,SPI_MODE,SD_MODE
// return value:
//          0:ok
//          others:fail
//------------------------------------------------------------------// 
U8 AUDIO_Play(U8 ch,U32 index,U8 mode){
    U8 sts,codec,info,temp,i;
    
    if(AUDIO_GetStatus(ch)!=STATUS_STOP){
        AUDIO_Stop(ch);
//        PlayCtl.parameter|=P_WAIT;
//        PlayCtl.parameter|=(mode<<P_MODE_SHIFT);
//        PlayCtl.parameter|=ramp;
//        PlayCtl.index=index;
//        PlayCtl.ch=ch;
        while(AUDIO_GetStatus(ch)==STATUS_PLAYING){
        	AUDIO_ServiceLoop();
        }
//        return 0;      
    }
    
    if(mode&MIDI_MODE)
    {
        mode&=(~MIDI_MODE);
        if(mode==OTP_MODE)
        {
            codec=CODEC_MIDI;
        }else
        {
            codec=GetCodec(index,mode);
        }            
    }else
    {    
        codec=GetCodec(index,mode);
    }
    
    for(i=0;i<_AUDIO_API_CH_NUM;i++){
        if(ChInfo[i]!=CH_UNKNOWN){
            sts=AUDIO_GetStatus(i);
            if(sts==STATUS_STOP){
                ChInfo[i]=CH_UNKNOWN;    
            }    
        }
    }    
    
    sts=E_AUDIO_UNSUPPORT_CODEC;
    
    info=ChInfo[ch];
#if _ADPCM_PLAY    
    if(codec==CODEC_ADPCM){
        
        if(info==CH_ADPCM){
            temp=0;        
        }else if(info==CH_ADPCM_CH2){
            temp=1;
        }else{
            temp=0;
            for(i=0;i<_AUDIO_API_CH_NUM;i++){
                if(ChInfo[i]==CH_ADPCM||ChInfo[i]==CH_ADPCM_CH2){
                    temp++;    
                }    
            }    
            if(temp==1){
                for(i=0;i<_AUDIO_API_CH_NUM;i++){
                    if(ChInfo[i]==CH_ADPCM){
                        temp=1;    
                    }    
                    if(ChInfo[i]==CH_ADPCM_CH2){
                        temp=0;    
                    }    
                }        
            }    
        }        
        //dprintf("adpcm temp:%d\r\n",temp);
        if(temp==0){    
            sts=ADPCM_Play(index,mode);     
    
            ChInfo[ch]=CH_ADPCM;
        }else if(temp==1){
#if _ADPCM_CH2_PLAY            
            sts=ADPCM_CH2_Play(index,mode);     
    
            ChInfo[ch]=CH_ADPCM_CH2;
#endif            
        }    
    }
#endif    

#if _SBC_PLAY    
    if(codec==CODEC_SBC){
        
        if(info==CH_SBC){
            temp=0;        
        }else if(info==CH_SBC_CH2){
            temp=1;
        }else{
            temp=0;
            for(i=0;i<_AUDIO_API_CH_NUM;i++){
                if(ChInfo[i]==CH_SBC||ChInfo[i]==CH_SBC_CH2){
                    temp++;    
                }    
            }    
            if(temp==1){
                for(i=0;i<_AUDIO_API_CH_NUM;i++){
                    if(ChInfo[i]==CH_SBC){
                        temp=1;    
                    }    
                    if(ChInfo[i]==CH_SBC_CH2){
                        temp=0;    
                    }    
                }        
            }    
        }        
        //dprintf("sbc temp,%d\r\n",temp);
        if(temp==0){    
            sts=SBC_Play(index,mode);     
    
            ChInfo[ch]=CH_SBC;
        }else if(temp==1){
#if _SBC_CH2_PLAY           
            sts=SBC_CH2_Play(index,mode);
    
            ChInfo[ch]=CH_SBC_CH2;
#endif            
        }    
    }
#endif    

#if _CELP_PLAY    
    if(codec==CODEC_CELP){
        
        if(info==CH_CELP){
            temp=0;        
        }else{
            temp=0;
            for(i=0;i<_AUDIO_API_CH_NUM;i++){
                if(ChInfo[i]==CH_CELP){
                    temp++;    
                }    
            }    
        }        
        //dprintf("celp temp,%d\r\n",temp);
        if(temp==0){    
            sts=CELP_Play(index,mode);     
    
            ChInfo[ch]=CH_CELP;
        }
    }
#endif  

#if _MIDI_PLAY    
    if(codec==CODEC_MIDI){
        
        if(info==CH_MIDI){
            temp=0;        
        }else{
            temp=0;
            for(i=0;i<_AUDIO_API_CH_NUM;i++){
                if(ChInfo[i]==CH_MIDI){
                    temp++;    
                }    
            }    
        }        
        //dprintf("midi temp,%d\r\n",temp);
        if(temp==0){    
            sts=MIDI_Play(index,mode);     
    
            ChInfo[ch]=CH_MIDI;
        }
    }
#endif  

#if _PCM_PLAY    
    if(codec==CODEC_PCM){
        
        if(info==CODEC_PCM){
            temp=0;        
        }else{
            temp=0;
            for(i=0;i<_AUDIO_API_CH_NUM;i++){
                if(ChInfo[i]==CODEC_PCM){
                    temp++;    
                }    
            }    
        }        
        //dprintf("pcm temp,%d\r\n",temp);
        if(temp==0){    
            sts=PCM_Play(index,mode);     
            
            ChInfo[ch]=CH_PCM;
        }
    }
#endif  
    AUDIO_SetChVolume(ch,AudioVol[ch]);
    return sts;
}    
//------------------------------------------------------------------//
// Resume playing
// Parameter: 
//          ch:audio channel
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_Resume(U8 ch){
    U8 codec;
      
    codec=ChInfo[ch];

#if _ADPCM_PLAY    
    if(codec==CH_ADPCM){
        ADPCM_Resume();   
    }     
#endif   
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2){
        ADPCM_CH2_Resume();   
    }     
#endif  
#if _SBC_PLAY    
    if(codec==CH_SBC){
        SBC_Resume();   
    }     
#endif  
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2){
        SBC_CH2_Resume();   
    }     
#endif 
#if _CELP_PLAY    
    if(codec==CH_CELP){
        CELP_Resume();   
    }     
#endif 
#if _MIDI_PLAY    
    if(codec==CH_MIDI){
        MIDI_Resume();   
    }     
#endif 
#if _PCM_PLAY    
    if(codec==CH_PCM){
        PCM_Resume();   
    }     
#endif 
}    
//------------------------------------------------------------------//
// Stop playing
// Parameter: 
//          ch:audio channel
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_Stop(U8 ch){
    U8 codec;
      
    codec=ChInfo[ch];

#if _ADPCM_PLAY    
    if(codec==CH_ADPCM){
        ADPCM_Stop();     
    }   
#endif   
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2){
        ADPCM_CH2_Stop();     
    }   
#endif   
#if _SBC_PLAY    
    if(codec==CH_SBC){
        SBC_Stop();     
    }   
#endif   
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2){
        SBC_CH2_Stop();     
    }   
#endif   
#if _CELP_PLAY    
    if(codec==CH_CELP){
        CELP_Stop();     
    }   
#endif 
#if _MIDI_PLAY    
    if(codec==CH_MIDI){
        MIDI_Stop();     
    }   
#endif 
#if _PCM_PLAY    
    if(codec==CH_PCM){
        PCM_Stop();     
    }   
#endif 
} 
//------------------------------------------------------------------//
// Set speed ratio
// Parameter: 
//          ch:audio channel
//          index: -12~12
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetSpeed(U8 ch, S8 index)
{  
    U8 codec;
      
    codec=ChInfo[ch];
    
#if _ADPCM_PLAY    
    if(codec==CH_ADPCM)	ADPCM_SetSpeed(index);
#endif    
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2) ADPCM_CH2_SetSpeed(index);
#endif  
#if _SBC_PLAY    
    if(codec==CH_SBC) SBC_SetSpeed(index);
#endif    
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2) SBC_CH2_SetSpeed(index);
#endif  
#if _CELP_PLAY    
    if(codec==CH_CELP) CELP_SetSpeed(index);
#endif   
#if _PCM_PLAY    
    if(codec==CH_PCM) PCM_SetSpeed(index);
#endif       
}    
//------------------------------------------------------------------//
// Set pitch ratio
// Parameter: 
//          ch:audio channel
//          index: -12~12
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetPitch(U8 ch, S8 index)
{
    U8 codec;
      
    codec=ChInfo[ch];   
#if _ADPCM_PLAY    
    if(codec==CH_ADPCM) ADPCM_SetPitch(index);
#endif  
#if _ADPCM_CH2_PLAY    
    if(codec==CH_ADPCM_CH2) ADPCM_CH2_SetPitch(index);  
#endif  
#if _SBC_PLAY    
    if(codec==CH_SBC) SBC_SetPitch(index);   
#endif    
#if _SBC_CH2_PLAY    
    if(codec==CH_SBC_CH2) SBC_CH2_SetPitch(index);  
#endif
#if _CELP_PLAY    
    if(codec==CH_CELP) CELP_SetPitch(index);   
#endif    
#if _PCM_PLAY    
    if(codec==CH_PCM) PCM_SetPitch(index);
#endif      
}    
//------------------------------------------------------------------//
// Enable/Disable Robot Sound Effect
// Parameter: 
//          ch:audio channel
//          cmd: 1:enable,0:disable
//          index: carrier index,0~15
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetRobotSound(U8 ch, U8 cmd,U8 index)
{
    U8 codec;
      
    codec=ChInfo[ch];   
#if _ADPCM_PLAY      
    if(codec==CH_ADPCM) ADPCM_SetRobotSound(cmd,index);        
#endif    
#if _ADPCM_CH2_PLAY       
    if(codec==CH_ADPCM_CH2) ADPCM_CH2_SetRobotSound(cmd,index);        
#endif   
#if _SBC_PLAY       
    if(codec==CH_SBC) SBC_SetRobotSound(cmd,index);        
#endif     
#if _SBC_CH2_PLAY       
    if(codec==CH_SBC_CH2) SBC_CH2_SetRobotSound(cmd,index);        
#endif   
#if _CELP_PLAY      
    if(codec==CH_CELP) CELP_SetRobotSound(cmd,index);        
#endif  
#if _PCM_PLAY    
    if(codec==CH_PCM) PCM_SetRobotSound(cmd,index);   
#endif      
}    
//------------------------------------------------------------------//
// Enable/Disable Robot01 Sound Effect
// Parameter: 
//          ch: audio channel
//          cmd: 1:enable,0:disable
//          type: robot01 type ,0~2
//          trhes: threshold of input voice ,0~7
//                 0 is the smallest threshold
//                 1 is the largest threshold
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetRobot01Sound(U8 ch, U8 cmd,U8 type, U8 thres){
    U8 codec;
      
    codec=ChInfo[ch];
#if _ADPCM_PLAY && _ADPCM_ROBOT_SOUND_01
    if(codec==CH_ADPCM)  ADPCM_SetRobot01Sound(cmd,type,thres);
#endif    

#if _ADPCM_CH2_PLAY && _ADPCM_CH2_ROBOT_SOUND_01
    if(codec==CH_ADPCM_CH2)  ADPCM_CH2_SetRobot01Sound(cmd,type,thres);        
#endif   

#if _SBC_PLAY && _SBC_ROBOT_SOUND_01
    if(codec==CH_SBC) SBC_SetRobot01Sound(cmd,type,thres);        
#endif     

#if _SBC_CH2_PLAY && _SBC_CH2_ROBOT_SOUND_01
    if(codec==CH_SBC_CH2) SBC_CH2_SetRobot01Sound(cmd,type,thres);        
#endif   

#if _CELP_PLAY && _CELP_ROBOT_SOUND_01
    if(codec==CH_CELP) CELP_SetRobot01Sound(cmd,type,thres);        
#endif  
#if _PCM_PLAY && _PCM_ROBOT_SOUND_01
    if(codec==CH_PCM) PCM_SetRobot01Sound(cmd,type,thres);   
#endif      
} 
//------------------------------------------------------------------//
// Enable/Disable Robot02 Sound Effect
// Parameter: 
//          ch: audio channel
//          cmd: 1:enable,0:disable
//          type: robot01 type ,0~2
//          trhes: threshold of input voice ,0~7
//                 0 is the smallest threshold
//                 1 is the largest threshold
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetRobot02Sound(U8 ch, U8 cmd,U8 type, U8 thres){
    U8 codec;
      
    codec=ChInfo[ch];
#if _ADPCM_PLAY && _ADPCM_ROBOT_SOUND_02
    if(codec==CH_ADPCM)  ADPCM_SetRobot02Sound(cmd,type,thres);
#endif    

#if _ADPCM_CH2_PLAY && _ADPCM_CH2_ROBOT_SOUND_02
    if(codec==CH_ADPCM_CH2)  ADPCM_CH2_SetRobot02Sound(cmd,type,thres);        
#endif   

#if _SBC_PLAY && _SBC_ROBOT_SOUND_02
    if(codec==CH_SBC) SBC_SetRobot02Sound(cmd,type,thres);        
#endif     

#if _SBC_CH2_PLAY && _SBC_CH2_ROBOT_SOUND_02
    if(codec==CH_SBC_CH2) SBC_CH2_SetRobot02Sound(cmd,type,thres);        
#endif   

#if _CELP_PLAY && _CELP_ROBOT_SOUND_02
    if(codec==CH_CELP) CELP_SetRobot02Sound(cmd,type,thres);        
#endif  
#if _PCM_PLAY && _PCM_ROBOT_SOUND_02
    if(codec==CH_PCM) PCM_SetRobot02Sound(cmd,type,thres);   
#endif      
} 
//------------------------------------------------------------------//
// Enable/Disable Reverb Sound Effect
// Parameter: 
//          ch: audio channel
//          cmd: 1:enable,0:disable
//          index: Echo index,0~7
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetReverbSound(U8 ch, U8 cmd,U8 index){
    U8 codec;
      
    codec=ChInfo[ch];
#if _ADPCM_PLAY && _ADPCM_REVERB_SOUND
    if(codec==CH_ADPCM)  ADPCM_SetReverbSound(cmd,index);
#endif    

#if _ADPCM_CH2_PLAY && _ADPCM_CH2_REVERB_SOUND
    if(codec==CH_ADPCM_CH2)  ADPCM_CH2_SetReverbSound(cmd,index);        
#endif   

#if _SBC_PLAY && _SBC_REVERB_SOUND
    if(codec==CH_SBC) SBC_SetReverbSound(cmd,index);        
#endif     

#if _SBC_CH2_PLAY && _SBC_CH2_REVERB_SOUND    
    if(codec==CH_SBC_CH2) SBC_CH2_SetReverbSound(cmd,index);        
#endif   

#if _CELP_PLAY && _CELP_REVERB_SOUND
    if(codec==CH_CELP) CELP_SetReverbSound(cmd,index);        
#endif  
#if _PCM_PLAY && _PCM_REVERB_SOUND
    if(codec==CH_PCM) PCM_SetReverbSound(cmd,index);   
#endif      
} 
//------------------------------------------------------------------//
// Enable/Disable Echo Sound Effect
// Parameter: 
//          ch: audio channel
//          cmd: 1:enable,0:disable
//          index: Echo index,0~7
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_SetEchoSound(U8 ch, U8 cmd,U8 index){
    U8 codec;
      
    codec=ChInfo[ch];
#if _ADPCM_PLAY && _ADPCM_ECHO_SOUND
    if(codec==CH_ADPCM)  ADPCM_SetEchoSound(cmd,index);
#endif    

#if _ADPCM_CH2_PLAY && _ADPCM_CH2_ECHO_SOUND
    if(codec==CH_ADPCM_CH2)  ADPCM_CH2_SetEchoSound(cmd,index);        
#endif   

#if _SBC_PLAY && _SBC_ECHO_SOUND
    if(codec==CH_SBC) SBC_SetEchoSound(cmd,index);        
#endif     

#if _SBC_CH2_PLAY && _SBC_CH2_ECHO_SOUND    
    if(codec==CH_SBC_CH2) SBC_CH2_SetEchoSound(cmd,index);        
#endif   

#if _CELP_PLAY && _CELP_ECHO_SOUND
    if(codec==CH_CELP) CELP_SetEchoSound(cmd,index);        
#endif  
#if _PCM_PLAY && _PCM_ECHO_SOUND
    if(codec==CH_PCM) PCM_SetEchoSound(cmd,index);   
#endif      
} 
//------------------------------------------------------------------//
// Codec init
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------// 
void AUDIO_Init(void){
    U8 i;
    
    for(i=0;i<_AUDIO_API_CH_NUM;i++)
    {
        AudioVol[i]=CH_VOL_15;
    }
#if _ADPCM_PLAY    
    ADPCM_Init();
#endif    
#if _ADPCM_CH2_PLAY
    ADPCM_CH2_Init();     
#endif
#if _SBC_PLAY    
    SBC_Init();
#endif    
#if _SBC_CH2_PLAY
    SBC_CH2_Init();     
#endif
#if _CELP_PLAY
    CELP_Init();     
#endif
#if _MIDI_PLAY
    MIDI_Init();     
#endif
#if _PCM_PLAY
    PCM_Init();     
#endif
#if _PCM_CH2_PLAY
    PCM_CH2_Init();     
#endif
#if _PCM_CH3_PLAY
    PCM_CH3_Init();     
#endif
#if _RT_PLAY
    RT_Init();     
#endif

}
//------------------------------------------------------------------//
// Service loop
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//     
void AUDIO_ServiceLoop(void){
    
//    if((PlayCtl.parameter&P_WAIT))
//    {
//        if(AUDIO_GetStatus(PlayCtl.ch)==STATUS_STOP)
//        {    
//            PlayCtl.parameter&=(~P_WAIT);
//            AUDIO_Play(PlayCtl.ch,PlayCtl.index,(PlayCtl.parameter&P_MODE_MASK)>>P_MODE_SHIFT,
//                   PlayCtl.parameter&P_RAMP_MASK);
//        }             
//    }    
#if _ADPCM_PLAY    
    ADPCM_ServiceLoop();   
#endif
#if _ADPCM_CH2_PLAY
    ADPCM_CH2_ServiceLoop();   
#endif
#if _SBC_PLAY    
    SBC_ServiceLoop();   
#endif
#if _SBC_CH2_PLAY
    SBC_CH2_ServiceLoop();   
#endif
#if _CELP_PLAY    
    CELP_ServiceLoop();   
#endif
#if _MIDI_PLAY    
    MIDI_ServiceLoop();   
#endif
#if _PCM_PLAY    
    PCM_ServiceLoop();   
#endif
#if _PCM_CH2_PLAY    
    PCM_CH2_ServiceLoop();   
#endif
#if _PCM_CH3_PLAY    
    PCM_CH3_ServiceLoop();   
#endif
#if _RT_PLAY
    RT_ServiceLoop();   
#endif
}       
