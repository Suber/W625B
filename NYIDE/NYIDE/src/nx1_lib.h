//==========================================================================
// File Name          : nx1_lib.h
// Description        : NX1 library
// Last modified date : 2016/10/20
//==========================================================================
#ifndef NX1_LIB_H
#define NX1_LIB_H



//====================================================================================
//  Type Define
//====================================================================================
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned long long  U64;
typedef signed char         S8;
typedef signed short        S16;
typedef signed int          S32;
typedef signed long long    S64;
typedef enum {
    R_OK = 0
} RESULT;



//====================================================================================
//  Constant Define
//====================================================================================
// Time base
#define TIMEBASE_OFF                0
#define TIMEBASE_250us              250
#define TIMEBASE_1000us             1000

// WDT timeout
#define WDT_TIMEOUT_750ms           0
#define WDT_TIMEOUT_188ms           1

// CPU clock divider
#define CPU_CLK_DIV_1               (7<<5)
#define CPU_CLK_DIV_2               (6<<5)
#define CPU_CLK_DIV_4               (5<<5)
#define CPU_CLK_DIV_8               (4<<5)

// LVD voltage
#define LVD_VOLTAGE_OFF             0xFF
#define LVD_VOLTAGE_2V2             (0<<12)
#define LVD_VOLTAGE_2V4             (1<<12)
#define LVD_VOLTAGE_2V6             (2<<12)
#define LVD_VOLTAGE_3V2             (3<<12)
#define LVD_VOLTAGE_3V4             (4<<12)
#define LVD_VOLTAGE_3V6             (5<<12)

// HW channel ID
#define HW_CH0                      0
#define HW_CH1                      1

// AUDIO channel ID
#define AUDIO_CH0                   0
#define AUDIO_CH1                   1
#define AUDIO_CH2                   2

// ADC/DAC Timer
#define USE_TIMER0                  0
#define USE_TIMER1                  1
#define USE_TIMER2                  2

// DAC PP gain
#define PP_GAIN_50                  0
#define PP_GAIN_66                  1
#define PP_GAIN_83                  2
#define PP_GAIN_100                 3

// Ramp setting
#define RAMP_NONE                   0
#define RAMP_UP                     1
#define RAMP_DOWN                   2
#define RAMP_UP_DOWN                3

// Channel Volume Setting
#define CH_VOL_0                    0
#define CH_VOL_1                    1
#define CH_VOL_2                    2
#define CH_VOL_3                    3
#define CH_VOL_4                    4
#define CH_VOL_5                    5
#define CH_VOL_6                    6
#define CH_VOL_7                    7
#define CH_VOL_8                    8
#define CH_VOL_9                    9
#define CH_VOL_10                   10
#define CH_VOL_11                   11
#define CH_VOL_12                   12
#define CH_VOL_13                   13
#define CH_VOL_14                   14
#define CH_VOL_15                   15

// Mixer Volume Setting
#define MIX_VOL_0                   0
#define MIX_VOL_1                   1
#define MIX_VOL_2                   2
#define MIX_VOL_3                   3
#define MIX_VOL_4                   4
#define MIX_VOL_5                   5
#define MIX_VOL_6                   6
#define MIX_VOL_7                   7
#define MIX_VOL_8                   8
#define MIX_VOL_9                   9
#define MIX_VOL_10                  10
#define MIX_VOL_11                  11
#define MIX_VOL_12                  12
#define MIX_VOL_13                  13
#define MIX_VOL_14                  14
#define MIX_VOL_15                  15

// Speed index
#define SPEED_N12                   (-12)
#define SPEED_N11                   (-11)
#define SPEED_N10                   (-10)
#define SPEED_N9                    (-9) 
#define SPEED_N8                    (-8)
#define SPEED_N7                    (-7)
#define SPEED_N6                    (-6)
#define SPEED_N5                    (-5)
#define SPEED_N4                    (-4)
#define SPEED_N3                    (-3)
#define SPEED_N2                    (-2)
#define SPEED_N1                    (-1)
#define SPEED_P0                    0
#define SPEED_P1                    1
#define SPEED_P2                    2
#define SPEED_P3                    3
#define SPEED_P4                    4
#define SPEED_P5                    5
#define SPEED_P6                    6
#define SPEED_P7                    7
#define SPEED_P8                    8
#define SPEED_P9                    9
#define SPEED_P10                   10
#define SPEED_P11                   11
#define SPEED_P12                   12

// Pitch index
#define PITCH_N12                   (-12)
#define PITCH_N11                   (-11)
#define PITCH_N10                   (-10)
#define PITCH_N9                    (-9) 
#define PITCH_N8                    (-8)
#define PITCH_N7                    (-7)
#define PITCH_N6                    (-6)
#define PITCH_N5                    (-5)
#define PITCH_N4                    (-4)
#define PITCH_N3                    (-3)
#define PITCH_N2                    (-2)
#define PITCH_N1                    (-1)
#define PITCH_P0                    0
#define PITCH_P1                    1
#define PITCH_P2                    2
#define PITCH_P3                    3
#define PITCH_P4                    4
#define PITCH_P5                    5
#define PITCH_P6                    6
#define PITCH_P7                    7
#define PITCH_P8                    8
#define PITCH_P9                    9
#define PITCH_P10                   10
#define PITCH_P11                   11
#define PITCH_P12                   12

// Robot sound index
#define ROBOT_0                     0
#define ROBOT_1                     1
#define ROBOT_2                     2
#define ROBOT_3                     3
#define ROBOT_4                     4
#define ROBOT_5                     5
#define ROBOT_6                     6
#define ROBOT_7                     7
#define ROBOT_8                     8
#define ROBOT_9                     9
#define ROBOT_10                    10
#define ROBOT_11                    11
#define ROBOT_12                    12
#define ROBOT_13                    13
#define ROBOT_14                    14
#define ROBOT_15                    15

// Sample rate
#define SAMPLE_RATE_48000           48000
#define SAMPLE_RATE_44100           44100
#define SAMPLE_RATE_32000           32000
#define SAMPLE_RATE_22050           22050
#define SAMPLE_RATE_16000           16000
#define SAMPLE_RATE_8000            8000

// Codec
#define CODEC_MIDI                  0
#define CODEC_SBC                   1
#define CODEC_ADPCM                 2
#define CODEC_CELP                  3
#define CODEC_PCM                   4
#define CODEC_NOT_MIDI              0xFF

// Audio status
#define STATUS_STOP                 0
#define STATUS_PLAYING              1
#define STATUS_PAUSE                2
#define STATUS_RECORDING            3

// ID of address mode
#define ADDRESS_MODE                0x80000000

// Storage mode
#define SPI_MODE                    0x00
#define SD_MODE                     0x01
#define OTP_MODE                    0x02
#define MIDI_MODE                   0x04

// SPI mode
#define SPI_1_1_1_MODE              0
#define SPI_1_2_2_MODE              1
#define SPI_1_4_4_MODE              2

// SPI mode
#define XIP_1_IO_MODE               0
#define XIP_2_IO_MODE               2
#define XIP_4_IO_MODE               3

// Action 
#define ACTION_PORTA                (0<<4)
#define ACTION_PORTB                (1<<4)
#define ACTION_PORTC                (2<<4)
#define ACTION_REPEAT               1
#define ACTION_NO_REPEAT            0
#define ACTION_BUSY_HIGH            1
#define ACTION_BUSY_LOW             0

// Direct Key
//#define PRESS_TIME_10ms             10       //Baseon TIMEBASE_1000us
//#define SCAN_TICK_4ms               4        //Baseon TIMEBASE_1000us
#define PRESS_TIME_80ms             320       //Baseon TIMEBASE_250us
#define PRESS_TIME_40ms             160       //Baseon TIMEBASE_250us
#define PRESS_TIME_20ms             80
#define SCAN_TICK_1ms               4        //Baseon TIMEBASE_250us,For scan matrix key  

// Turn on or off
#define ENABLE                      1
#define DISABLE                     0

// Bit Set
#define BIT_SET                     1
#define BIT_RESET                   0

// RAM address
#define SDRAM_ADDR                  0x400000

// GPIO Setting
#define GPIO_INPUT_PULLHIGH         0
#define GPIO_INPUT_FLOAT            1
#define GPIO_OUTPUT_ALT             2
#define GPIO_OUTPUT_NORMAL          3
#define GPIO_OUTPUT_ALT2            4
#define GPIO_PIN_OFF                0xFF

// Capture Setting
#define CAP_RISING                  (1<<5)
#define CAP_FALLING                 (1<<4)
#define CAP_RISING_FALLING          (CAP_RISING | CAP_FALLING)


//====================================================================================
//  Include 
//====================================================================================
#include "nx1_config.h"



//====================================================================================
//  External Reference
//====================================================================================
extern volatile U32 SysTick;//ms
extern void SysTickInit(U32 freq);
extern U8   DIRECTKEY_GetKey(void);

extern void MIDI_ServiceLoop(void);
extern void MIDI_Init(void);
extern void MIDI_SetVolume(U8 vol);
extern U8   MIDI_GetStatus(void);
extern void MIDI_Stop(void);
extern void MIDI_Resume(void);
extern void MIDI_Pause(void);
extern U8   MIDI_Play(U32 index,U8 mode);
extern void MIDI_DacIsr(U8 size);

extern void SBC_ServiceLoop(void);
extern void SBC_Init(void);
extern void SBC_SetVolume(U8 vol);
extern U8   SBC_GetStatus(void);
extern void SBC_Stop(void);
extern void SBC_Resume(void);
extern void SBC_Pause(void);
extern U8   SBC_Play(U32 index,U8 mod);
extern void SBC_DacIsr(U8 size);
extern void SBC_AdcIsr(U8 size,volatile U32 *pdest32);
extern void SBC_SetSpeed(S8 index);
extern void SBC_SetPitch(S8 index);
extern void SBC_SetRobotSound(U8 cmd,U8 index);
extern void SBC_SetRobot01Sound(U8 cmd,U8 type, U8 thres);
extern void SBC_SetReverbSound(U8 cmd,U8 index);
extern void SBC_SetEchoSound(U8 cmd,U8 index);
extern U8   SBC_Record(U32 index,U8 mode,U32 size);

extern void SBC_CH2_ServiceLoop(void);
extern void SBC_CH2_Init(void);
extern void SBC_CH2_SetVolume(U8 vol);
extern U8   SBC_CH2_GetStatus(void);
extern void SBC_CH2_Stop(void);
extern void SBC_CH2_Resume(void);
extern void SBC_CH2_Pause(void);
extern U8   SBC_CH2_Play(U32 index,U8 mode);
extern void SBC_CH2_DacIsr(U8 size);
extern void SBC_CH2_SetSpeed(S8 index);
extern void SBC_CH2_SetPitch(S8 index);
extern void SBC_CH2_SetRobotSound(U8 cmd,U8 index);
extern void SBC_CH2_SetEchoSound(U8 cmd,U8 index);

extern void ADPCM_ServiceLoop(void);
extern void ADPCM_Init(void);
extern void ADPCM_SetVolume(U8 vol);
extern U8   ADPCM_GetStatus(void);
extern void ADPCM_Stop(void);
extern void ADPCM_Resume(void);
extern void ADPCM_Pause(void);
extern U8   ADPCM_Play(U32 index,U8 mode);
extern void ADPCM_DacIsr(U8 size,S16 *buf);
extern void ADPCM_AdcIsr(U8 size,volatile U32 *pdest32);
extern void ADPCM_SetSpeed(S8 index);
extern void ADPCM_SetPitch(S8 index);
extern void ADPCM_SetRobotSound(U8 cmd,U8 index);
extern void ADPCM_SetEchoSound(U8 cmd,U8 index);
extern U8   ADPCM_Record(U32 index,U8 mode,U16 sample_rate,U32 size);

extern void ADPCM_CH2_ServiceLoop(void);
extern void ADPCM_CH2_Init(void);
extern void ADPCM_CH2_SetVolume(U8 vol);
extern U8   ADPCM_CH2_GetStatus(void);
extern void ADPCM_CH2_Stop(void);
extern void ADPCM_CH2_Resume(void);
extern void ADPCM_CH2_Pause(void);
extern U8   ADPCM_CH2_Play(U32 index,U8 mode);
extern void ADPCM_CH2_DacIsr(U8 size,S16 *buf);
extern void ADPCM_CH2_SetSpeed(S8 index);
extern void ADPCM_CH2_SetPitch(S8 index);
extern void ADPCM_CH2_SetRobotSound(U8 cmd,U8 index);
extern void ADPCM_CH2_SetEchoSound(U8 cmd,U8 index);

extern void VR_Start(const U32 *cybase,const U32 *model);
extern void VR_Halt(void);
extern void VR_Stop(void);
extern S8 VR_ServiceLoop(void);
extern void VR_AdcIsr(U8 size,volatile U32 *pdest32);

extern void AUDIO_ServiceLoop(void);
extern U16  AUDIO_GetSpiFileCount(void);
extern void AUDIO_Init(void);
extern void AUDIO_SetChVolume(U8 ch,U8 vol);
extern void AUDIO_SetMixerVolume(U8 vol);
extern U8   AUDIO_GetStatus(U8 ch);
extern void AUDIO_Stop(U8 ch);
extern void AUDIO_Resume(U8 ch);
extern void AUDIO_Pause(U8 ch);
extern U8   AUDIO_Play(U8 ch,U32 index,U8 mode);
extern void AUDIO_SetSpeed(U8 ch,S8 index);
extern void AUDIO_SetPitch(U8 ch,S8 index);
extern void AUDIO_SetRobotSound(U8 ch,U8 cmd,U8 index);

extern void CELP_ServiceLoop(void);
extern void CELP_Init(void);
extern void CELP_SetVolume(U8 vol);
extern U8   CELP_GetStatus(void);
extern void CELP_Stop(void);
extern void CELP_Resume(void);
extern void CELP_Pause(void);
extern U8   CELP_Play(U32 index,U8 mode);
extern void CELP_DacIsr(U8 size);
extern void CELP_SetSpeed(S8 index);
extern void CELP_SetPitch(S8 index);
extern void CELP_SetRobotSound(U8 cmd,U8 index);
extern void CELP_SetEchoSound(U8 cmd,U8 index);

extern void PCM_ServiceLoop(void);
extern void PCM_Init(void);
extern void PCM_SetVolume(U8 vol);
extern U8 PCM_GetStatus(void);
extern void PCM_Stop(void);
extern void PCM_Resume(void);
extern void PCM_Pause(void);
extern U8 PCM_Play(U32 index,U8 mode);
extern void PCM_DacIsr(U8 size);
extern void PCM_SetSpeed(S8 index);
extern void PCM_SetPitch(S8 index);
extern void PCM_SetRobotSound(U8 cmd,U8 index);
extern void PCM_SetEchoSound(U8 cmd,U8 index);
extern U8 PCM_Record(U32 index,U8 mode,U16 sample_rate,U32 size);

extern void PCM_CH2_ServiceLoop(void);
extern void PCM_CH2_Init(void);
extern void PCM_CH2_SetVolume(U8 vol);
extern U8 PCM_CH2_GetStatus(void);
extern void PCM_CH2_Stop(void);
extern void PCM_CH2_Resume(void);
extern void PCM_CH2_Pause(void);
extern U8 PCM_CH2_Play(U32 index,U8 mode);
extern void PCM_CH2_DacIsr(U8 size);
extern void PCM_CH2_SetSpeed(S8 index);
extern void PCM_CH2_SetPitch(S8 index);
extern void PCM_CH2_SetRobotSound(U8 cmd,U8 index);
extern void PCM_CH2_SetEchoSound(U8 cmd,U8 index);
extern U8 PCM_CH2_Record(U32 index,U8 mode,U16 sample_rate,U32 size);

extern void PCM_CH3_ServiceLoop(void);
extern void PCM_CH3_Init(void);
extern void PCM_CH3_SetVolume(U8 vol);
extern U8 PCM_CH3_GetStatus(void);
extern void PCM_CH3_Stop(void);
extern void PCM_CH3_Resume(void);
extern void PCM_CH3_Pause(void);
extern U8 PCM_CH3_Play(U32 index,U8 mode);
extern void PCM_CH3_DacIsr(U8 size);
extern void PCM_CH3_SetSpeed(S8 index);
extern void PCM_CH3_SetPitch(S8 index);
extern void PCM_CH3_SetRobotSound(U8 cmd,U8 index);
extern void PCM_CH3_SetEchoSound(U8 cmd,U8 index);
extern U8 PCM_CH3_Record(U32 index,U8 mode,U16 sample_rate,U32 size);


extern void RT_ServiceLoop(void);
extern void RT_Init(void);
extern void RT_DacIsr(U8 size);
extern U8 RT_Play(U16 sample_rate);
extern U8 RT_Stop(void);
extern void RT_SetRobot01Sound(U8 cmd,U8 type, U8 thres);
extern void RT_SetRobot02Sound(U8 cmd,U8 type, U8 thres);
extern void RT_SetReverbSound(U8 cmd,U8 index);

extern void CB_NYQEvent(U8 io,U8 data);
extern void CB_NYQMark(U8 id);
extern void CB_VioEvent(U8 io,U8 data);
extern void CB_MidiMark(U8 id);

extern U32  Random(U32 mi);
extern void Delayms(U32 ms);

extern U8   ACTION_PlayIO(U8 ch,const U32 file,U8 port,U8 repeat,U8 busy,U8 mode);
extern void ACTION_StopIO(U8 ch);
extern void ACTION_ResumeIO(U8 ch);
extern void ACTION_PauseIO(U8 ch);
extern U8   ACTION_GetIOStatus(U8 ch);

extern U8   AUDIO_SetADPCMRecMode(U8 bit_mode,U16 frame_size,U16 sample_rate);
extern U8   AUDIO_SetSubbandRecMode(U16 bit_rate,U16 bandwidth);

extern void SYS_Init(void);
extern void SYS_ServiceLoop(void);


extern void NY_GPIOConfig(U8 pin, U8 setting);
extern void NY_GPIOWrite(U8 pin,U8 data);
extern U8   NY_GPIORead(U8 pin);

extern U8 RESOURCE_GetResourceCnt(void);
extern U32 RESOURCE_GetAddress(U8 idx);
extern U8 RESOURCE_GetType(U8 idx);

extern const U32  PlayList[];



#endif //#ifndef NX1_LIB_H
