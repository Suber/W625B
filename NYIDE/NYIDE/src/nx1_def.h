//==========================================================================
// File Name          : nx1_def.h
// Description        : Load from NYIDE. Produce automatically by NYIDE.
// Last modified date : 2016/10/20
//==========================================================================
#ifndef NX1_DEF
#define NX1_DEF



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Option Setting from NYIDE
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
// IC BODY
#define OPTION_IC_BODY              0          	
// BODY Version
#define OPTION_IC_VER               0           // 0: A, 1: B 
// RAM Size
#define OPTION_DLM_SIZE             0x2000      // Unit is byte
// High Clock Source
#define OPTION_HIGH_CLK_SRC         0      		// 0: Internal, 1: External 
// High Frequency Clock
#define OPTION_HIGH_FREQ_CLK        16000000    // Unit is Hz 
// Low Clock Source
#define OPTION_LOW_CLK_SRC          0       	// 0: Internal, 1: External
// VDD Voltage
#define OPTION_VDD_VOLTAGE          1      		// 0: 4.5V, 1:3.0V
// SPI0 Voltage
#define OPTION_SPI0_VOLTAGE         0     		// 0: 3.3V, 1:1.8V



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  MIDI Setting from Q-MIDI
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
// Channel Number
#define QMIDI_CH_NUM                8     		// Unit is channel.
// Timbre Format
#define QMIDI_TIMBRE_FORMAT         1			// 1: PCM, 2: ULAW
// Sample Rate
#define QMIDI_SAMPLE_RATE           24000  		// Unit is Hz
// Storage Type
#define QMIDI_STORAGE_TYPE			1	

//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Cyberon VR Setting
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
// VR Storage Type
#define _VR_STORAGE_TYPE            0          // 0: OTP, 1: SPI
// VR Sample Rate
#define _VR_SAMPLE_RATE             0
// VR RAM Usage
#define _VR_ENGINE_RAM_SIZE         0

//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Action Setting from Q-Visio
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
// Action timebase
#define _QVISIO_TIMEBASE            0         // Unit is us

#endif //#ifndef NX1_DEF
