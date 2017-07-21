//==========================================================================
// File Name          : nx1_config.h
// Description        : NX1 C Module Setting
// Version            : V1.03 Beta
// Last modified date : 2017/07/03
//==========================================================================
#ifndef NX1_CONFIG_H
#define NX1_CONFIG_H
#include "nx1_def.h"
#ifndef  __ASSEMBLY__
#include "nx1_lib.h"
#endif



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * IC Body Setting *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define _IC_BODY                    OPTION_IC_BODY      // Set by NYIDE
#define _BODY_VER                   OPTION_IC_VER       // Set by NYIDE
#define _MODULE_VER                 17070301            // C Module Version



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * System Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  System Parameter Setting
//====================================================================================
#define _SYS_CLK                    OPTION_HIGH_FREQ_CLK// Set by NYIDE
#define _SYS_TIMEBASE               TIMEBASE_250us      // Set time base,For RTC can output 1KHz tone
#define _SYS_WDT_TIME               WDT_TIMEOUT_750ms   // Set watch dog time out
#define _SYS_SMU_CPU_CLK_DIV        CPU_CLK_DIV_1       // Set CPU divider
#define _SYS_SMU_LVD_VOLTAGE        LVD_VOLTAGE_OFF     //LVD_VOLTAGE_2V2     // Set LVD voltage                                     



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * AUDIO Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  AUDIO Parameter Setting
//====================================================================================
#define _AUDIO_MODULE               ENABLE              // Enable or Disable AUDIO Module
#define _DAC_CH0_TIMER              USE_TIMER0          // Set HW channel 0 timer
#define _DAC_CH1_TIMER              USE_TIMER1          // Set HW channel 1 timer
#define _ADC_TIMER                  USE_TIMER2          // Set ADC timer
#define _PP_GAIN                    PP_GAIN_100        //PP_GAIN_83          // Set DAC push pull gain
#define _ROBOT_SOUND_COMPRESS       DISABLE



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * SBC Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  SBC Parameter Setting
//====================================================================================
#define _SBC_PLAY                   ENABLE              // Enable or Disable playback 
#define _SBC_SPEED_CHANGE           ENABLE              // Enable or Disable speed function 
#define _SBC_PITCH_CHANGE           DISABLE             // Enable or Disable pitch function 
#define _SBC_ROBOT_SOUND            DISABLE             // Enable or Disable robot function  
#define _SBC_ROBOT_SOUND_01         DISABLE             // Enable or Disable simple robot function
#define _SBC_ROBOT_SOUND_02         DISABLE             // Enable or Disable complex robot function
#define _SBC_REVERB_SOUND           DISABLE             // Enable or Disable reverb function
#define _SBC_ECHO_SOUND             DISABLE             // Enable or Disable echo function
#define _SBC_PLAY_AUDIO_CH          HW_CH0              // Select HW channel 0 or 1
#define _SBC_RECORD                 DISABLE             // Enable or Disable recorder
//====================================================================================
//  Second SBC Parameter Setting 
//====================================================================================
#define _SBC_CH2_PLAY               DISABLE             // Enable or Disable playback 
#define _SBC_CH2_SPEED_CHANGE       DISABLE             // Enable or Disable speed function 
#define _SBC_CH2_PITCH_CHANGE       DISABLE             // Enable or Disable pitch function 
#define _SBC_CH2_ROBOT_SOUND        DISABLE             // Enable or Disable robot function  
#define _SBC_CH2_ROBOT_SOUND_01     DISABLE             // Enable or Disable simple robot function  
#define _SBC_CH2_ROBOT_SOUND_02     DISABLE             // Enable or Disable complex robot function
#define _SBC_CH2_REVERB_SOUND       DISABLE             // Enable or Disable reverb function
#define _SBC_CH2_ECHO_SOUND         DISABLE             // Enable or Disable echo function
#define _SBC_CH2_PLAY_AUDIO_CH      HW_CH1              // Select HW channel 0 or 1



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * ADPCM Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  ADPCM Parameter Setting
//====================================================================================
#define _ADPCM_PLAY                 DISABLE             // Enable or Disable playback
#define _ADPCM_SPEED_CHANGE         DISABLE             // Enable or Disable speed function 
#define _ADPCM_PITCH_CHANGE         DISABLE             // Enable or Disable pitch function 
#define _ADPCM_ROBOT_SOUND          DISABLE             // Enable or Disable robot function 
#define _ADPCM_ROBOT_SOUND_01       DISABLE             // Enable or Disable simple robot function  
#define _ADPCM_ROBOT_SOUND_02       DISABLE             // Enable or Disable complex robot function
#define _ADPCM_REVERB_SOUND         DISABLE             // Enable or Disable reverb function 
#define _ADPCM_ECHO_SOUND           DISABLE             // Enable or Disable echo function
#define _ADPCM_PLAY_AUDIO_CH        HW_CH1              // Select HW channel 0 or 1
#define _ADPCM_RECORD               DISABLE             // Enable or Disable recorder
//====================================================================================
//  Second ADPCM Parameter Setting 
//====================================================================================
#define _ADPCM_CH2_PLAY             DISABLE             // Enable or Disable playback 
#define _ADPCM_CH2_SPEED_CHANGE     DISABLE             // Enable or Disable speed function 
#define _ADPCM_CH2_PITCH_CHANGE     DISABLE             // Enable or Disable pitch function 
#define _ADPCM_CH2_ROBOT_SOUND      DISABLE             // Enable or Disable robot function  
#define _ADPCM_CH2_ROBOT_SOUND_01   DISABLE             // Enable or Disable simple robot function  
#define _ADPCM_CH2_ROBOT_SOUND_02   DISABLE             // Enable or Disable complex robot function
#define _ADPCM_CH2_REVERB_SOUND     DISABLE             // Enable or Disable reverb function 
#define _ADPCM_CH2_ECHO_SOUND       DISABLE             // Enable or Disable echo function
#define _ADPCM_CH2_PLAY_AUDIO_CH    HW_CH0              // Select HW channel 0 or 1



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * CELP Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  CELP Parameter Setting
//====================================================================================
#define _CELP_PLAY                  DISABLE             // Enable or Disable playback
#define _CELP_SPEED_CHANGE          DISABLE             // Enable or Disable speed function 
#define _CELP_PITCH_CHANGE          DISABLE             // Enable or Disable pitch function 
#define _CELP_ROBOT_SOUND           DISABLE             // Enable or Disable robot function
#define _CELP_ROBOT_SOUND_01        DISABLE             // Enable or Disable simple robot function  
#define _CELP_ROBOT_SOUND_02        DISABLE             // Enable or Disable complex robot function
#define _CELP_REVERB_SOUND          DISABLE             // Enable or Disable reverb function  
#define _CELP_ECHO_SOUND            DISABLE             // Enable or Disable echo function
#define _CELP_PLAY_AUDIO_CH         HW_CH0              // Select HW channel 0 or 1



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * PCM Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  PCM Parameter Setting
//====================================================================================
#define _PCM_PLAY                   ENABLE             // Enable or Disable playback
#define _PCM_SPEED_CHANGE           DISABLE             // Enable or Disable speed function 
#define _PCM_PITCH_CHANGE           DISABLE             // Enable or Disable pitch function 
#define _PCM_ROBOT_SOUND            DISABLE             // Enable or Disable robot function
#define _PCM_ECHO_SOUND             DISABLE             // Enable or Disable echo function  
#define _PCM_ROBOT_SOUND_01         DISABLE             // Enable or Disable simple robot function  
#define _PCM_ROBOT_SOUND_02         DISABLE             // Enable or Disable complex robot function
#define _PCM_REVERB_SOUND           DISABLE             // Enable or Disable reverb function
#define _PCM_PLAY_AUDIO_CH          HW_CH1              // Select HW channel 0 or 1
#define _PCM_RECORD                 DISABLE             // Enable or Disable recorder
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Second PCM Parameter Setting
//====================================================================================
#define _PCM_CH2_PLAY               ENABLE             // Enable or Disable playback
#define _PCM_CH2_SPEED_CHANGE       DISABLE             // Enable or Disable speed function 
#define _PCM_CH2_PITCH_CHANGE       DISABLE             // Enable or Disable pitch function 
#define _PCM_CH2_ROBOT_SOUND        DISABLE             // Enable or Disable robot function
#define _PCM_CH2_ECHO_SOUND         DISABLE             // Enable or Disable echo function  
#define _PCM_CH2_ROBOT_SOUND_01     DISABLE             // Enable or Disable simple robot function  
#define _PCM_CH2_ROBOT_SOUND_02     DISABLE             // Enable or Disable complex robot function
#define _PCM_CH2_REVERB_SOUND       DISABLE             // Enable or Disable reverb function
#define _PCM_CH2_PLAY_AUDIO_CH      HW_CH1              // Select HW channel 0 or 1
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Third PCM Parameter Setting
//====================================================================================
#define _PCM_CH3_PLAY               DISABLE             // Enable or Disable playback
#define _PCM_CH3_SPEED_CHANGE       DISABLE             // Enable or Disable speed function 
#define _PCM_CH3_PITCH_CHANGE       DISABLE             // Enable or Disable pitch function 
#define _PCM_CH3_ROBOT_SOUND        DISABLE             // Enable or Disable robot function
#define _PCM_CH3_ECHO_SOUND         DISABLE             // Enable or Disable echo function  
#define _PCM_CH3_ROBOT_SOUND_01     DISABLE             // Enable or Disable simple robot function  
#define _PCM_CH3_ROBOT_SOUND_02     DISABLE             // Enable or Disable complex robot function
#define _PCM_CH3_REVERB_SOUND       DISABLE             // Enable or Disable reverb function
#define _PCM_CH3_PLAY_AUDIO_CH      HW_CH1              // Select HW channel 0 or 1





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * Real Time Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Real time Parameter Setting
//====================================================================================
#define _RT_PLAY                    DISABLE             // Enable or Disable playback
#define _RT_ROBOT_SOUND             DISABLE             // Enable or Disable robot function
#define _RT_ECHO_SOUND              DISABLE             // Enable or Disable echo function
#define _RT_PLAY_AUDIO_CH           HW_CH1              // Select HW channel 0 or 1



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * MIDI Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  MIDI Parameter Setting
//====================================================================================
#define _MIDI_PLAY                  ENABLE              // Enable or Disable playback
#define _MIDI_CH_NUM                QMIDI_CH_NUM        // Select the number of MIDI channels 
#define _MIDI_TIMBRE_FORMAT         QMIDI_TIMBRE_FORMAT // Set by Q-MIDI
#define _MIDI_SAMPLE_RATE           QMIDI_SAMPLE_RATE   // Set sample rate
#define _MIDI_PLAY_AUDIO_CH         HW_CH0              // Select HW channel 0 or 1



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * VR Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Voice Recognition Parameter Setting
//====================================================================================
#define _VR_FUNC                    DISABLE             // Enable or Disable Voice Recognition 



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * DirectKey Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  DirectKey Parameter Setting
//====================================================================================
#define _DIRECTKEY_MODULE           ENABLE              // Enable or Disable DirectKey
#define _DIRECTKEY_PRESS_RESET      PRESS_TIME_20ms/SCAN_TICK_1ms   
#define _DIRECTKEY_PRESS_OK         PRESS_TIME_40ms/SCAN_TICK_1ms
#define _DIRECTKEY_SCAN_TICK        SCAN_TICK_1ms       // Scan tick time
#define _DIRECTKEY_PA0              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA1              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA2              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA3              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA4              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA5              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA6              DISABLE             // Enable or Disable key
#define _DIRECTKEY_PA7              DISABLE             // Enable or Disable key

////////////////////////////////////////////////////////////////////
#define _DIRECTKEY_PA12             ENABLE
#define _DIRECTKEY_PA13             ENABLE
#define _DIRECTKEY_PA14             ENABLE
////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * Timer Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Timer Parameter Setting
//====================================================================================
#define _TIMER_MODULE               ENABLE              // Enable or Disable timer
#define _TIMER_TMR0_INT             DISABLE             // Enable or Disable TMR0 INT
#define _TIMER_TMR1_INT             DISABLE             // Enable or Disable TMR1 INT
#define _TIMER_TMR2_INT             DISABLE             // Enable or Disable TMR2 INT



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * UART Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  UART Parameter Setting
//====================================================================================
#define _UART_MODULE                DISABLE              // 0:DISABLE,1:ENABLE 



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * GPIO Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  GPIO Parameter Setting
//====================================================================================
#define _GPIO_MODULE                ENABLE              // Enable or Disable GPIO
#define _GPIO_PA0_MODE              GPIO_INPUT_PULLHIGH  //GPIO_PIN_OFF        // Set PA0
#define _GPIO_PA1_MODE              GPIO_INPUT_PULLHIGH // Set PA1
#define _GPIO_PA2_MODE              GPIO_INPUT_PULLHIGH // Set PA2 
#define _GPIO_PA3_MODE              GPIO_INPUT_PULLHIGH // Set PA3
#define _GPIO_PA4_MODE              GPIO_INPUT_PULLHIGH // Set PA4
#define _GPIO_PA5_MODE              GPIO_INPUT_PULLHIGH // Set PA5
#define _GPIO_PA6_MODE              GPIO_INPUT_PULLHIGH // Set PA6
#define _GPIO_PA7_MODE              GPIO_INPUT_PULLHIGH //GPIO_PIN_OFF        // Set PA7
#define _GPIO_PA8_MODE              GPIO_OUTPUT_NORMAL // Set PA8
#define _GPIO_PA9_MODE              GPIO_OUTPUT_NORMAL // Set PA9 
#define _GPIO_PA10_MODE             GPIO_OUTPUT_NORMAL // Set PA10 
#define _GPIO_PA11_MODE             GPIO_INPUT_PULLHIGH // Set PA11 
#define _GPIO_PA12_MODE             GPIO_INPUT_PULLHIGH // Set PA12 
#define _GPIO_PA13_MODE             GPIO_INPUT_PULLHIGH //GPIO_PIN_OFF        // Set PA13 
#define _GPIO_PA14_MODE             GPIO_INPUT_PULLHIGH  // Set PA14 
#define _GPIO_PA15_MODE             GPIO_OUTPUT_NORMAL  // Set PA15 
#define _GPIO_PB0_MODE              GPIO_PIN_OFF  // Set PB0 
#define _GPIO_PB1_MODE              GPIO_PIN_OFF  // Set PB1 
#define _GPIO_PB2_MODE              GPIO_PIN_OFF  // Set PB2 
#define _GPIO_PB3_MODE              GPIO_PIN_OFF // Set PB3 
#define _GPIO_PB4_MODE              GPIO_INPUT_PULLHIGH // Set PB4 
#define _GPIO_PB5_MODE              GPIO_INPUT_PULLHIGH // Set PB5 
#define _GPIO_PB6_MODE              GPIO_INPUT_PULLHIGH // Set PB6 
#define _GPIO_PB7_MODE              GPIO_INPUT_PULLHIGH // Set PB7 
#define _GPIO_PB8_MODE              GPIO_INPUT_PULLHIGH        // Set PB8 
#define _GPIO_PB9_MODE              GPIO_INPUT_PULLHIGH        // Set PB9 
#define _GPIO_PB10_MODE             GPIO_INPUT_PULLHIGH        // Set PB10 
#define _GPIO_PB11_MODE             GPIO_INPUT_PULLHIGH        // Set PB11 
#define _GPIO_PB12_MODE             GPIO_INPUT_PULLHIGH        // Set PB12 
#define _GPIO_PB13_MODE             GPIO_INPUT_PULLHIGH        // Set PB13 
#define _GPIO_PB14_MODE             GPIO_INPUT_PULLHIGH        // Set PB14 
#define _GPIO_PB15_MODE             GPIO_INPUT_PULLHIGH        // Set PB15 
#define _GPIO_PC0_MODE              GPIO_INPUT_PULLHIGH        // Set PC0 
#define _GPIO_PC1_MODE              GPIO_INPUT_PULLHIGH        // Set PC1 
#define _GPIO_PC2_MODE              GPIO_INPUT_PULLHIGH        // Set PC2 
#define _GPIO_PC3_MODE              GPIO_INPUT_PULLHIGH        // Set PC3 
#define _GPIO_PC4_MODE              GPIO_INPUT_PULLHIGH        // Set PC4 
#define _GPIO_PC5_MODE              GPIO_INPUT_PULLHIGH        // Set PC5 
#define _GPIO_PC6_MODE              GPIO_INPUT_PULLHIGH        // Set PC6 
#define _GPIO_PC7_MODE              GPIO_INPUT_PULLHIGH        // Set PC7 



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * SPI Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  SPI Parameter Setting
//====================================================================================
#define _SPI_MODULE                 ENABLE              // Enable or Disable SPI
#define _SPI_ACCESS_MODE            SPI_1_1_1_MODE      // SPI data mode



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * XIP Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  XIP Parameter Setting
//====================================================================================
#define _XIP_IO_MODE                XIP_2_IO_MODE       // SPI data mode



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * I2C Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  I2C Parameter Setting
//====================================================================================
#define _I2C_MODULE                 DISABLE             // Enable or Disable I2C
#define _I2C_MASTER_ADDRESS         0x50                // I2C master address



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * PWM Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  PWM Parameter Setting
//====================================================================================
#define _PWMA_MODULE                DISABLE             // Enable or Disable PWMA
#define _PWMB_MODULE                DISABLE             // Enable or Disable PWMB
#define _PWMA0_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMA1_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMA2_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMA3_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMB0_DUTY                 0xFF                  // Unit is %. 0xFF is off
#define _PWMB1_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMB2_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMB3_DUTY                 0xFF                // Unit is %. 0xFF is off
#define _PWMA_INT                   DISABLE             // Enable or Disable PWMA INT
#define _PWMB_INT                   DISABLE             // Enable or Disable PWMB INT



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * CAPTURE Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Capture Parameter Setting
//====================================================================================
#define _CAPTURE_MODULE             DISABLE             // Enable or Disable Capture
#define _CAPTURE_PAx                0                   // Set capture pin in PAx
#define _CAPTURE_INT                DISABLE             // Enable or Disable Capture
#define _CAPTURE_INT_MODE           CAP_RISING_FALLING  // Set Interrupt Mode



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * IR Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Capture Parameter Setting
//====================================================================================
#define _IR_MODULE                  DISABLE             // Enable or Disable IR



//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * Action Module *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  Action Parameter Setting
//====================================================================================
#define _ACTION_MODULE              DISABLE         	// Enable or Disable ACTION
#define _VIO_CH_NUM                 8               	// The number of action channel


//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  * Software Setting *
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define _UNIFIED_API                ENABLE              // Enable or Disable unified API
#define _AUDIO_API_CH_NUM           3                   // HW or SW channel number
#define _RANDOM_GEN                 0                   // Enable or Disabl
#define _USE_HEAP_RAM               DISABLE        		// Enable or Disable Heap Area



#endif //#ifndef NX1_CONFIG_H
