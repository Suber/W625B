#ifndef INCLUDE_H
#define INCLUDE_H

#ifndef  __ASSEMBLY__
#include "nx1_lib.h"
#endif

#include "nx1_config.h"
#include "nx1_reg.h"

//#define SUPPORT_AGC
//#define SUPPORT_NYQ
//#define SUPPORT_NYW
//#define SUPPORT_WAVE_MARK
//#define SUPPORT_MIDI_MARK
//#define SUPPORT_FILE_STITCH
//#define SUPPORT_RESAMPLE
#define CHECK_UNDERFLOW_OVERFLOW

#define TEMPLATE_RDTEST     0
#define TEMPLATE_APP        2
#define TEMPLATE_FORMAT     TEMPLATE_RDTEST

//#define DEBUG_BLOCKING
#define MENU_ENA

#define ADPCM_DEC_CHANNEL_NUM     2


#define AGC_FRAME_SIZE          32

#if _SBC_CH2_PLAY
#define SBC_DEC_CHANNEL_NUM     2
#else
#define SBC_DEC_CHANNEL_NUM     1
#endif

#define REC_FILE     "RecTemp.bin"

#define SDC_CLK_MTF_PIN         0
#define SDC_CD_MTF_PIN          6
#define SDC_WP_MTF_PIN          7
#define SPI0_CEN_MTF_PIN        0
#define SPI0_SCLK_MTF_PIN       1
#define SPI0_IO0_MTF_PIN        2
#define SPI0_IO1_MTF_PIN        3
#define SPI0_IO2_MTF_PIN        4
#define SPI1_SCLK_MTF_PIN       9

#define I2C_SDA_MTF_PIN         12

#define PWMA0_PIN               8
#define PWMA1_PIN               9
#define PWMA2_PIN               10
#define PWMA3_PIN               11

#define ADC_CH0_PIN             0
#define ADC_CH1_PIN             1
#define ADC_CH2_PIN             2
#define ADC_CH3_PIN             3
#define PWMB0_PIN               12
#define PWMB1_PIN               13
#define PWMB2_PIN               14
#define PWMB3_PIN               15

#define IR_TX_PIN               5

#define PWM_CAP_PIN             11

#define UART1_TX_PIN            6
#define UART1_RX_PIN            7

#define QIO_PORT_NUM            8
#define MIDIMARK_FUNC_NUM       8
#define WAVEMARK_FUNC_NUM       8

#define DAC_DATA_OFFSET         15

#define SMOOTH_DOWN         0
#define SMOOTH_UP           1
#define SMOOTH_NONE         2

#define	MINUS_1DB		(0.8913f*MAX_INT16)
#define	MINUS_2DB		(0.7943f*MAX_INT16)
#define	MINUS_3DB		(0.7079f*MAX_INT16)
#define	MINUS_4DB		(0.6310f*MAX_INT16)
#define	MINUS_5DB		(0.5623f*MAX_INT16)
#define	MINUS_6DB		(0.5012f*MAX_INT16)
#define	MINUS_7DB		(0.4467f*MAX_INT16)
#define	MINUS_8DB		(0.3981f*MAX_INT16)
#define	MINUS_9DB		(0.3548f*MAX_INT16)
#define	MINUS_12DB	    (0.2512f*MAX_INT16)


#define SPI_RESERVED_SIZE           1024
#define SPI_BASE_INFO_SIZE          16
#define SPI_TIMBRE_ADDR_OFFSET      6
#define SPI_TAB_INFO_ENTRY_SIZE     4
#define SPI_FILE_COUNT_SIZE         2
#define SPI_DATA_TYPE_MASK          0xF0000000

#define SPI_DATA_TYPE_MIDI          0x4

#define STORAGE_OTP             0
#define STORAGE_SPI             1
#define STORAGE_SD              2

#define RATIO_INIT_INDEX        12

#define SPEED_CTL_SAMPLE_RATE        16000

#define NYW_FRAME_HEADER_SIZE   5
#define NYQ_FRAME_HEADER_SIZE   6
#define NYQ_DATA_SIZE           2

#define P_WAIT          0x80
#define P_MODE_MASK     0x30
#define P_DAC_MASK      0x0C
#define P_RAMP_MASK     0x03
#define P_MODE_SHIFT    0x04
#define P_DAC_SHIFT     0x02

#define E_AUDIO_OK                      0
#define E_AUDIO_INVALID_HEADER          1
#define E_AUDIO_UNSUPPORT_CODEC         2
#define E_AUDIO_INVALID_SAMPLERATE      3
#define E_AUDIO_INVALID_CHANNEL         4
#define E_AUDIO_UNSUPPORT_NYQ           5
#define E_AUDIO_UNSUPPORT_NYW           6
#define E_AUDIO_INVALID_IDX             7
#define E_AUDIO_SD_OPEN_FAIL            8
#define E_AUDIO_NOT_VOICE               9
#define E_AUDIO_NOT_MIDI                10

#define MIDI_SAMPLE_RATE    _MIDI_SAMPLE_RATE
#define G7211_SAMPLE_RATE   SAMPLE_RATE_16000

#define SPEED_CTL_BIT       0x2

#define REC_SKIP_SAMPLES    20
#define FILE_END_STOP       0
#define FORCE_STOP          1

#define Q15_SHIFT           15

#define SPEED_RATIO_MAX     2
#define SPEED_RATIO_MIN     0.5

#define PITCHSHIFT_RATIO_MAX  2
#define PITCHSHIFT_RATIO_MIN  0.5

#define RESAMPLE_OFFSET     4

#define CH_MIDI             0
#define CH_SBC              1
#define CH_SBC_CH2          2
#define CH_ADPCM            3
#define CH_ADPCM_CH2        4 
#define CH_CELP             5  
#define CH_PCM              6 
#define CH_UNKNOWN          7 

#define CODEC_TX            1
#define CODEC_RX            0

#define CELP_MUTE_SAMPLES   89

#define STANDBY(cond)                   __asm__ volatile ("standby "#cond);

#define AC97_RECORD_GAIN    0x1C

#define STORAGE_BLOCK_SIZE  1024
#define STORAGE_DMA_SIZE    512
//#define ENA_SECTOR_BUF

//#if _MIDI_CH_NUM==4
//#define MIDI_DEC_OUT_SIZE   32//word
#if MIDI_SAMPLE_RATE==16000
#define MIDI_DEC_OUT_SIZE   64//word
#elif MIDI_SAMPLE_RATE==24000
#define MIDI_DEC_OUT_SIZE   96//word
#else
#define MIDI_DEC_OUT_SIZE   128//word
#endif
#if (_MIDI_TIMBRE_FORMAT&0x4)//ADPCM
#define MIDI_PROC_STATE_SIZE    ((124*_MIDI_CH_NUM)>>2)
#else//PCM ULAW
#define MIDI_PROC_STATE_SIZE    ((72*_MIDI_CH_NUM)>>2)//structure change
#endif
#define MIDI_RAM_ALLOCATE_SIZE      (32+MIDI_DEC_OUT_SIZE+MIDI_PROC_STATE_SIZE)
#define MIDI_AUDIO_OUT_COUNT        3
#define MIDI_AUDIO_BUF_SIZE         (MIDI_DEC_OUT_SIZE*MIDI_AUDIO_OUT_COUNT)

#if _VR_SAMPLE_RATE==16000
#if _VR_STORAGE_TYPE
#define BUF_SIZE            (256)
#define VR_STACK_SIZE       564
#else
#define BUF_SIZE            (160)
#define VR_STACK_SIZE       460
#endif
#else
#if _VR_STORAGE_TYPE
#define BUF_SIZE            (128)
#define VR_STACK_SIZE       820
#else
#define BUF_SIZE            (80)
#define VR_STACK_SIZE       812
#endif
#endif
#define VR_STATE_SIZE		96
#define _VR_RAM_SIZE        (_VR_ENGINE_RAM_SIZE+VR_STATE_SIZE+(BUF_SIZE*4))

#define SBC_DEC_IN_SIZE     80//byte  //bit-rate 32000
#define SBC_DEC_OUT_SIZE    320//word

#define ADPCM_DEC_IN_SIZE   86//byte
#define ADPCM_DEC_OUT_SIZE  136//word

#define PCM_DEC_IN_SIZE     80//byte
#define PCM_DEC_OUT_SIZE    40//word

#define CELP_DEC_IN_SIZE    12//byte
#define CELP_DEC_OUT_SIZE   160//word

#define SMOOTH_DOWN_TIME    10//ms
#define SMOOTH_UP_TIME      5//ms
#define SMOOTH_STEP         30

#define MAX_SMOOTH_GAIN     32767//0.9999(Q15 format)

#define MAX_INT16 	        (32767)
#define MIN_INT16 	        (-32768)

#define SPEED_CTL_FRAME_SIZE    360
#define SPEED_CTL_OUT_SIZE      90


#define HEADER_LEN          32
#define SIGNATURE_LEN       3

#define HW_INTERRUPT_NUM        24

#define MEMORY_ALLOCATE_PLAY        (1<<0)
#define MEMORY_ALLOCATE_RECORD      (1<<1)
#define MEMORY_ALLOCATE_VR          (1<<2)

#ifndef  __ASSEMBLY__


struct AUDIO_HEADER{
    U8   signature[SIGNATURE_LEN]; 
    U8   header_size;
    U32  file_size;
    U32  samples;
    U32  nyq_offset;
    U16  decode_in_size;     //byte
    U16  decode_out_size;    //word  
    U16  bit_per_frame;
    U16  sample_rate;
    U16  bandwidth;
    U8   file_type; 
    U8   codec; 
};
#if _SBC_ECHO_SOUND || _ADPCM_ECHO_SOUND || _CELP_ECHO_SOUND || _PCM_ECHO_SOUND || _SBC_CH2_ECHO_SOUND || _ADPCM_CH2_ECHO_SOUND || _RT_ECHO_SOUND
typedef struct{
    U8      frameCount;
    U16     count;
    S8      flag;
    S16     *buf;
    S16     EchoBufL;
    S16     EchoNB;
    S16     EchoWT;
    S16     RingBufPeriod;
    
}EchoCtl_T;
#endif
#if _SBC_ROBOT_SOUND_01 || _ADPCM_ROBOT_SOUND_01 || _CELP_ROBOT_SOUND_01 || _PCM_ROBOT_SOUND_01 || _SBC_CH2_ROBOT_SOUND_01 || _ADPCM_CH2_ROBOT_SOUND_01 || _RT_ROBOT_SOUND_01
extern const U16 Robot_Thres[8];
#endif
#if _SBC_ROBOT_SOUND_02 || _ADPCM_ROBOT_SOUND_02 || _CELP_ROBOT_SOUND_02 || _PCM_ROBOT_SOUND_02 || _SBC_CH2_ROBOT_SOUND_02 || _ADPCM_CH2_ROBOT_SOUND_02 || _RT_ROBOT_SOUND_02
extern const U16 Robot02_Thres[8];
#endif
extern U8 MixerVol;

extern void KeyScan(void);
extern void DIRECTKEY_Init(void);

extern void AudioCtlInit(void);
extern void SetPPGain(U8 gain);
extern void InitDac(CH_TypeDef * dacx,U16 sample_rate);
extern void InitAdc(U16 sample_rate);

extern void ChkNyqCtl(void);
extern void ChkVioCtl(void);

//extern void UartCtlInit(U16 baud_rate);
//extern U8   DebugGetKey(void);

//extern U8   NY_ChkMultiKey(const int key, ...);

extern void LimiterInit(U16 peak);
extern void LimiterProcess(void *InputData, S16 *Out, U16 Threshold, U16 Samples,U8 mode);

extern void AgcInit(U16 alpha,U16 beta,U16 target_rms);
extern void AgcProcess(S16 *in,U16 samples);

extern void NGInit(U16 release_sample,U8 attack_sample,U16 onoff_sample);
extern void NGProcess(S16 *audio_buf,S16 *out_buf,U8 samples);

extern S16  ADPCM0Decode(void);
extern void SetADPCM0Mode(U8 in_frame_mode, U8 out_frame_size);
extern void ADPCM0DecInit(void);

extern S16  ADPCM1Decode(void);
extern void SetADPCM1Mode(U8 in_frame_mode, U8 out_frame_size);
extern void ADPCM1DecInit(void);

extern void MIDIInit(U32 index,U32 timbre_addr,U32 *ram_allocate);
extern void MIDISetVolume(U8 vol);
extern U8   MIDIDecode(U8 *outlen,S16 *Out16Buf,U8 limiter);
extern U8   MIDIChkEnd(void);
extern void MIDI_SetSpeedRatio(S8);
extern S8 MIDI_GetSpeedRatio(void);


extern void AdpcmEncInit(void);
extern U32  ADPCMEncode(U32 samples,U16 *psrc,U8 *pdst);
extern U16  SBCBPF(U8 channel, U8 flag);

extern void SBCDecInit(U8 channel,U16 *ram_allocate,U16 sample_rate,U16 bandwidth,U16 normal_bits_per_frame,
                U16 delta_bits_per_frame_u,U16 delta_bits_per_frame_d,U8 vbr);
extern U32  SBCDecode(U8 channel,U32 TotalSample,U16 *pdst,U8 state);
extern void SBCEncInit(U16 *ram_allocate,U32 bit_rate,U32 bandwidth);
extern U32  SBCEncode(U32 samples,U16 *psrc,U8 *pdst);

extern U32  CELPDecode(U32 samples,U8 *psrc,U16 *pdst);
extern void CELPDecInit(U16 *ram_allocate);

extern U32  TimeStretchInit(char reset,float scale);
extern U32  TimeStretchProcess(U16 *pOutput, U16 *pInput, U32 *num_in_remain);
extern U32  TimeStretchGetBatchOutCnt(void);

extern void RoboticVoiceInit(U16 fs, U16 fc, U16 framesz);
extern void RoboticVoiceProcess(S16 *in, S16 *out);
extern void ROBOT_EFFECT_01_Init(S16 *robot_buf, U8 type,U16 thres,U16 detectthres, S16 SampleRate,S16 framesize,S16 echo_frame_size,S16 echo_buf_length);
extern void ROBOT_EFFECT_01(S16 *input);
extern void ROBOT_EFFECT_COMPRESS_01(S16 *input);

extern void ROBOT_EFFECT_02_Init(S16 *robot_buf, U8 type,U16 thres, S16 SampleRate);
extern void ROBOT_EFFECT_02(S16 *input);
extern void ROBOT_EFFECT_COMPRESS_02(S16 *input);

#if _SBC_ECHO_SOUND || _ADPCM_ECHO_SOUND || _CELP_ECHO_SOUND || _PCM_ECHO_SOUND || _SBC_CH2_ECHO_SOUND || _ADPCM_CH2_ECHO_SOUND || _RT_ECHO_SOUND
extern void EchoVoiceProcess(S16 *audio_buf, S16 datalength, EchoCtl_T *pCtl);
#endif

extern void Reverb_Effect_Init(short *reverb_x, short *reverb_y, U8 effect);
extern void Reverb_Effect(short *inbuf, unsigned short InData_Length);


extern void DelayLoop(U32 count);
extern void DelayLoop2(U32 count);

extern void Matrix_SetKeyScan(U8 cmd);
extern void Matrix_KeyScan(void);
extern void Matrix_KeyScanInit(void);
extern void Matrix_Debounce(U16 ms);

extern void GIE_ENABLE(void);
extern void GIE_DISABLE(void);

#if _USE_HEAP_RAM
extern void *MemAlloc( U16 xWantedSize );
extern void MemFree( void *pv );
#endif

#define NYQ_EVENT_FREQ      128
#define VIO_EVENT_PERIOD    100//in 100us
#endif

#endif //#ifndef INCLUDE_H
