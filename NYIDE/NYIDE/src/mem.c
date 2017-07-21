#include "nx1_config.h"
#include "include.h"

#if _USE_HEAP_RAM
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * List of Predictive Memory Usage *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Each algorithm HEAP Size
//====================================================================================
#define _SBC_PLAY_HEAP_RAM              2264                // SBC Play heap RAM size
#define _SBC_RECORD_HEAP_RAM            2704                // SBC Rec heap RAM size
#define _SBC_PITCH_SPEED_HEAP_RAM       1360                // SBC pitch/speed heap RAM size
#define _SBC_ROBOT01_HEAP_RAM           3200                // SBC Robot01 heap RAM size
#define _SBC_ROBOT01_COMPRESS_HEAP_RAM  1600                // SBC Robot01 Compress heap RAM size
#define _SBC_ROBOT02_HEAP_RAM           4712                // SBC Robot02 heap RAM size
#define _SBC_ROBOT02_COMPRESS_HEAP_RAM  2712                // SBC Robot02 Compress heap RAM size
#define _SBC_REVERB_HEAP_RAM            3320                // SBC Reverb heap RAM size
#define _SBC_ECHO_HEAP_RAM              2400                // SBC ECHO heap RAM size
#define _SBC_CH2_PLAY_HEAP_RAM          2264  				// SBC Play CH2 heap RAM size
#define _SBC_CH2_PITCH_SPEED_HEAP_RAM   1360                // SBC CH2 pitch/speed RAM size
#define _ADPCM_PLAY_HEAP_RAM            258                 // ADPCM Play heap RAM size
#define _ADPCM_RECORD_HEAP_RAM          866                 // ADPCM Rec heap RAM size
#define _ADPCM_PITCH_SPEED_HEAP_RAM     1800                // ADPCM pitch/speed RAM size
#define _ADPCM_CH2_PLAY_HEAP_RAM        258					// ADPCM Play CH2 heap RAM size
#define _ADPCM_CH2_PITCH_SPEED_HEAP_RAM 1800                // ADPCM CH2 pitch/speed RAM size
#define _CELP_PLAY_HEAP_RAM             1676                // CELP Play heap RAM size
#define _CELP_PITCH_SPEED_HEAP_RAM      1040                // CELP pitch/speed heap RAM size
#define _MIDI_PLAY_HEAP_RAM             ((MIDI_AUDIO_BUF_SIZE<<1)+(MIDI_RAM_ALLOCATE_SIZE<<2)) // MIDI heap RAM size
#define _PCM_PLAY_HEAP_RAM              160                 // PCM Play heap RAM size 
#define _PCM_RECORD_HEAP_RAM            224                 // PCM Rec heap RAM size
#define _PCM_PITCH_SPEED_HEAP_RAM       1720                // PCM pitch/speed heap RAM size
#define _RT_PLAY_HEAP_RAM               5864                // Real time Play heap RAM size
#define _VR_HEAP_RAM                    _VR_RAM_SIZE        // VR heap RAM size
//====================================================================================
//  Each algorithm STACK Size
//====================================================================================
#define _SBC_PLAY_STACK_RAM             388	                // SBC Play stack RAM size
#define _SBC_RECORD_STACK_RAM           396                 // SBC Rec stack RAM size
#define _SBC_CH2_PLAY_STACK_RAM         388                 // SBC Play CH2 stack RAM size
#define _ADPCM_PLAY_STACK_RAM           264                 // ADPCM Play stack RAM size
#define _ADPCM_RECORD_STACK_RAM         268                 // ADPCM Rec stack RAM size
#define _ADPCM_CH2_STACK_RAM            264    			    // ADPCM Play CH2 stack RAM size
#define _CELP_PLAY_STACK_RAM            432	                // CELP Play stack RAM size
#define _MIDI_PLAY_STACK_RAM            316                 // MIDI Play stack RAM size
#define _PCM_PLAY_STACK_RAM             228                 // PCM Play stack RAM size
#define _PCM_RECORD_STACK_RAM           240                 // PCM Rec stack RAM size
#define _RT_PLAY_STACK_RAM           	192                 // Real time stack RAM size
#define _VR_STACK_RAM                   VR_STACK_SIZE 		// VR STACK RAM Size
//====================================================================================
//  RAM Setting 
//  - User decide how many RAM size use simultaneously
//  - Please key in the largest RAM size scenario at the same time
//  - Please use copy/paste to avoid key in error
//  - Example:
//             Voice Recognition(VR), SBC_PLAY, ADPCM_PLAY, CELP_PLAY are used in a project
//             (1) When VR execute, the other algorithms do not work together.
//             (2) SBC_PLAY and ADPCM_PLAY execute at the same time.
//			   (3) When CELP_PLAY execute, the other algorithms do not work together.
//             By this example, user chooses the largest RAM size of these three scenarios.
//             If scenario(1) is largest, please define as following
//
//             #define _RAM_HEAP_SIZE   _VR_HEAP_RAM    //HEAP add all algorithms which play simultaneously.
//             #define _RAM_STACK_SIZE  _VR_STACK_SIZE  //Stack only define largest one.
//====================================================================================
#define _RAM_HEAP_SIZE                 _SBC_PLAY_HEAP_RAM  //+_SBC_ROBOT02_HEAP_RAM 
#define _RAM_STACK_SIZE                _SBC_PLAY_STACK_RAM

const U16 _stack_size = _RAM_STACK_SIZE;
const U16 _ram_size   = OPTION_DLM_SIZE;
U8 __attribute__((section(".heap, \"aw\", @nobits!"))) use_ram[_RAM_HEAP_SIZE+_RAM_STACK_SIZE];

#if 0//debug use
U32 MEM_GetRamSize()
{
    dprintf("RAM_HEAP %d RAM_STACK %d \r\n",_RAM_HEAP_SIZE,_RAM_STACK_SIZE);
}
#endif
#endif

