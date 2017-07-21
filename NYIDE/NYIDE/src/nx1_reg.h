//==========================================================================
// File Name   : NX1_REG.H
// Description : NX1 registers and constants definition
// Last modified date:    2016/11/03
//==========================================================================


//==================================================================================================================================
//  *** GPIO Configration - { Bit[15:0] }
//==================================================================================================================================
//  DIR     CFG     DAT         Function Description
//  0       x       0       ->  Output Low (Switch to output mode, NX1 will disable pull-high resistor automatically)
//  0       x       1       ->  Output High
//  1       0       x       ->  Input Pull-High (Select 100K/1M resistor by P_Port_PH_CFG. When SDC MFP turn on, PC's Pull-High resistor is 20K)
//  1       1       x       ->  Input Floating
//----------------------------------------------------------------------------------------------------------------------------------
//  PAD     -> Read status from Pad
//  MASK    -> Wakeup function Mask/NonMask (0:Mask, 1:NonMask)
//----------------------------------------------------------------------------------------------------------------------------------
//==================================================================================================================================
//  *** Sink/Constant Sink Mode Setting - { Bit[31:16] }
//==================================================================================================================================
//  MASK    -> Select Current Capacity (0:Large sink, 1:Normal sink)
//  PA_CFG[31:24] for PA[15:8] constant sink current mode setting (0:Constant Sink, 1:Normal Sink)
//----------------------------------------------------------------------------------------------------------------------------------
//==================================================================================================================================
//  *** Normal/Large Driver Mode Setting - { PB Bit[5:0][11:8], PC Bit[7:0] }
//==================================================================================================================================
//  MASK    -> Select Current Capacity (0:Large drive, 1:Normal drive)
//----------------------------------------------------------------------------------------------------------------------------------


//==================================================================================================================================
//  *** Special I/O Function
//==================================================================================================================================
//  PA15    PA14    PA13    PA12    PA11    PA10    PA9     PA8     PA7     PA6     PA5     PA4     PA3     PA2     PA1     PA0
//----------------------------------------------------------------------------------------------------------------------------------
//  EXT_INT EXT_CLK I2C     I2C     PWMA3   PWMA2   PWMA1   PWMA0   UART    UART    IR      EXT_CLK -       -       -       -
//          TM1/2   SCL     SDA                                     RX      TX              TM0
//----------------------------------------------------------------------------------------------------------------------------------
//  CSC     CSC     CSC     CSC     CSC     CSC     CSC     CSC     AN7     AN6     AN5     AN4     AN3     AN2     AN1     AN0     ; Contant Sink and ADC Line in
//----------------------------------------------------------------------------------------------------------------------------------
//  CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     CAP     ; PWMA Timer Capture Mode
//
//==================================================================================================================================
//  PB15    PB14    PB13    PB12    PB11    PB10    PB9     PB8     PB7     PB6     PB5     PB4     PB3     PB2     PB1     PB0
//----------------------------------------------------------------------------------------------------------------------------------
//  PWMB3   PWMB2   PWMB1   PWMB0   SPI1    SPI1    SPI1    SPI1    X'tal   X'tal   SPI0    SPI0    SPI0    SPI0    SPI0    SPI0
//                                  MISO    MOSI    SCLK    CSB     OUT     IN      SIO3    SIO2    SIO1    SIO0    SCLK    CSB
//
//==================================================================================================================================
//  -       -       -       -       -       -       -       -       PC7     PC6     PC5     PC4     PC3     PC2     PC1     PC0
//----------------------------------------------------------------------------------------------------------------------------------
//  -       -       -       -       -       -       -       -       SDC_WP  SDC_CD  SDC_IO3 SDC_IO2 SDC_IO1 SDC_IO0 SDC_CMD SDC_CLK
//----------------------------------------------------------------------------------------------------------------------------------
//  *All GPIO with key change wake-up function
//  *All GPIO with pull-high function - Strong(100K)/Weak(1M)/SDC(20K)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Public Constant Define
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "nx1_def.h"
#include "nx1_lib.h"
#ifndef NX1_REG_H
#define NX1_REG_H


#define C_System_Clock              32000000
//#define C_System_Clock            32000000
//#define C_System_Clock            24000000
//#define C_System_Clock            16000000
//#define C_System_Clock            12000000
//#define C_System_Clock            8000000
//#define C_System_Clock            32768
#define C_System_Clock_ns           (1000000000/C_System_Clock)
#define C_High_Clock                OPTION_HIGH_FREQ_CLK





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//==================================================================================================================================
//  Function Base Address Definition
//  *** Notes, All function need to setting P__SMU_FUNC_Ctrl and P_Px_MFP first ***
//==================================================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SMU_BASE                    0xE00000
#define I2C_BASE                    0xF01000
#define UART_BASE                   0xF02000       
#define TIMER0_BASE                 0xF04000
#define TIMER1_BASE                 0xF04040
#define TIMER2_BASE                 0xF04080
#define RTC_BASE                    0xF06000
#define WDT_BASE                    0xF07000
#define PORTA_BASE                  0xF08000
#define PORTB_BASE                  0xF08020
#define PORTC_BASE                  0xF08040
#define PORT_EXT_BASE               0xF08060
#define SPI0_BASE                   0xF0B000
#define SPI1_BASE                   0xF0B040
#define SDC_BASE                    0xF0E000
#define IR_BASE                     0xF0F000
#define PWMA_BASE                   0xF11000        // Include Capture Function
#define PWMB_BASE                   0xF11040
#define ADC_BASE                    0xF14000
#define DAC_BASE                    0xF15000

#define P_SMU                       ((SMU_TypeDef *)SMU_BASE)
#define P_I2C                       ((I2C_TypeDef *)I2C_BASE)
#define P_UART                      ((UART_TypeDef *)UART_BASE)
#define P_TIMER0                    ((TIMER_TypeDef *)TIMER0_BASE)
#define P_TIMER1                    ((TIMER_TypeDef *)TIMER1_BASE)
#define P_TIMER2                    ((TIMER_TypeDef *)TIMER2_BASE)
#define P_RTC                       ((RTC_TypeDef *)RTC_BASE)
#define P_WDT                       ((WDT_TypeDef *)WDT_BASE)
#define P_PORTA                     ((GPIO_TypeDef *)PORTA_BASE)
#define P_PORTB                     ((GPIO_TypeDef *)PORTB_BASE)
#define P_PORTC                     ((GPIO_TypeDef *)PORTC_BASE)
#define P_PORT_EXT                  ((GPIO_Ext_TypeDef *)PORT_EXT_BASE)
#define P_SPI0                      ((SPI_TypeDef *)SPI0_BASE)
#define P_SPI1                      ((SPI_TypeDef *)SPI1_BASE)
#define P_SDC                       ((SDC_TypeDef *)SDC_BASE)
#define P_IR                        ((IR_TypeDef *)IR_BASE)
#define P_PWMA                      ((PWM_TypeDef *)PWMA_BASE)
#define P_PWMB                      ((PWM_TypeDef *)PWMB_BASE)
#define P_ADC                       ((ADC_TypeDef *)ADC_BASE)
#define P_DAC                       ((DAC_TypeDef *)DAC_BASE)


//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  SMU_BASE (P_SMU)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_SMU_CLK_Ctrl              0xE00000        // System Clock Control
#define P_SMU_PWR_Ctrl              0xE00004        // Power Control
#define P_SMU_SW_INT                0xE00010        // Software Interrupt Control
#define P_SMU_RST_Flag              0xE00020        // Reset Flag
#define P_SMU_RST_SW_Ctrl           0xE00024        // Software Reset Control
#define P_SMU_FUNC_Ctrl             0xE00030        // All Function Enable/Disable
#define P_SMU_FUNC_RST              0xE00034        // Function Reset (Initialilation)


typedef struct{
    volatile U32 CLK_Ctrl;
    volatile U32 PWR_Ctrl;
    volatile U32 Reserved1[2];
    volatile U32 SW_INT;
    volatile U32 Reserved2[3];
    volatile U32 RST_Flag;
    volatile U32 RST_SW_Ctrl;
    volatile U32 Reserved3[2];
    volatile U32 FUNC_Ctrl;
    volatile U32 FUNC_RST;
}SMU_TypeDef;


//====================================================================
//  P_SMU_CLK_Ctrl (0xE00000)
//  System Management Unit, System Clock Control Regsiter
//====================================================================
//  Bit[31:8] : Reserved
//  Bit[7:5] : CPU Clock Divder, Clock Soruce from System Clock
#define C_CPU_CLK                   (7<<5)  // for Function Mask
#define C_CPU_CLK_DIV_1             (7<<5)
#define C_CPU_CLK_DIV_2             (6<<5)
#define C_CPU_CLK_DIV_4             (5<<5)
#define C_CPU_CLK_DIV_8             (4<<5)
#define C_CPU_CLK_DIV_16            (3<<5)
#define C_CPU_CLK_DIV_32            (2<<5)
#define C_CPU_CLK_DIV_64            (1<<5)
#define C_CPU_CLK_DIV_128           (0<<5)
//  Bit[4] : System Clock Source (Slow/Normal Mode Selection)
#define C_SYS_CLK_LoCLK             (1<<4)  // Slow Mode
#define C_SYS_CLK_HiCLK             (0<<4)  // Normal Mode
//  Bit[3:2] : Reserved
//  Bit[1] : Crystal Oscillator Ready Flag
#define C_XTAL_CLK_Ready            (1<<1)
//  Bit[0] : High Clock Source Enable
#define C_HiCLK_En                  (1<<0)
#define C_HiCLK_Dis                 (0<<0)


//====================================================================
//  P_SMU_PWR_Ctrl (0xE00004)
//  Power Control Register
//====================================================================
//  Bit[31:15] : Reserved
//  Bit[14:12] : LVD voltage select
#define C_LVD                       (7<<12) // for Function Mask
#define C_LVD_3V6                   (5<<12)
#define C_LVD_3V4                   (4<<12)
#define C_LVD_3V2                   (3<<12)
#define C_LVD_2V6                   (2<<12)
#define C_LVD_2V4                   (1<<12)
#define C_LVD_2V2                   (0<<12)
//  Bit[11:10] : Reserved
//  Bit[9] : LVD Flag
#define C_LVD_Flag                  (1<<9)  // 1:Detected
//  Bit[8] : LVD Enable/Disable
#define C_LVD_En                    (1<<8)
#define C_LVD_Dis                   (0<<8)
//  Bit[7] : Reserved
//  Bit[6] : SDC Power Down Protection, When Power down then force input pin as '1'
#define C_SDC_PD_Input_Mask         (1<<6)
#define C_SDC_PD_Input_UnMask       (0<<6)
//  Bit[5] : SPI1 Power Down Protection, When Power down then force input pin as '1'
#define C_SPI1_PD_Input_Mask        (1<<5)
#define C_SPI1_PD_Input_UnMask      (0<<5)
//  Bit[4] : LDO SPI0 Output Enable/Disable
#define C_LDOSPI0_En                (1<<4)
#define C_LDOSPI0_Dis               (0<<4)
//  Bit[3] : Constant Current Bias Enable/Disable
#define C_CSC_Bias_En               (1<<3)
#define C_CSC_Bias_Dis              (0<<3)
//  Bit[2] : Reserved
//  Bit[1:0] : Sleep Mode ( The action after instruction "Standby" Executed. )
#define C_SLP_Mode                  3  // for Sleep Mode mask
#define C_Green_Mode                0  // Green Mode, only CPU stop
#define C_SLP_Halt_Mode             1  // Halt Mode, 32768(LoCLK) Disable
#define C_SLP_Standby_Mode          3  // Standby Mode, 32768(LoCLK) Enable


//====================================================================
//  P_SMU_SW_INT (0xE00010)
//  Software Interrupt Control Register
//====================================================================
//  Bit[31:16] : Software Interrupt Protect Value
#define C_SW_INT_PROT_Value         (0x5AC3<<16)    // Write Only
//  Bit[15:1] : Reserved
//  Bit[0] : Software Interrupt Execute Once
#define C_SW_INT_Exe                1


//====================================================================
//  P_SMU_RST_Flag (0xE00020)
//  System Reset Flag
//  Read '1':Reset Occured;     Read '0':Reset Non-Occured
//  Write'1':Clear Flag;        Write'0':No Action
//====================================================================
//  Bit[31:6] : Reserved
//  Bit[5] : Reset Flag, Option Donwnload Fail
#define C_RST_Option_DL_Flag        (1<<5)
//  Bit[4] : Reset Flag, WatchDog
#define C_RST_WDT_Flag              (1<<4)
//  Bit[3] : Reset Flag, Power On Reset/Low Voltage Reset
#define C_RST_POR_LVR_Flag          (1<<3)
//  Bit[2] : Reset Flag, Key Reset (Reset Pin)
#define C_RST_Key_Flag              (1<<2)
//  Bit[1] : Reset Flag, Whole Chip Reset by Software Reset 
#define C_RST_SW_Chip_Flag          (1<<1)
//  Bit[0] : Reset Flag, CPU Reset by Software Reset
#define C_RST_SW_CPU_Flag           (1<<0)


//====================================================================
//  P_SMU_SW_RST_Ctrl (0xE00024)
//  Software Reset Control
//====================================================================
//  Bit[31:16] : Software Reset Protect Value
#define C_RST_SW_PROT_Value         (0x5AC3<<16)    // Write Only
//  Bit[15:2] : Reserved
//  Bit[1] : Whole Chip Reset by Software Control
#define C_RST_SW_Chip_Exe           (1<<1)
//  Bit[0] : CPU Reset by Software Control
#define C_RST_SW_CPU_Exe            (1<<0)


//====================================================================
//  P_SMU_FUNC_Ctrl (0xE00030)
//  All Function Enable/Disable
//====================================================================
//  Bit[31] : Reserved bit Must be '0'
#define C_Reserved_Bit              (0<<31)
//  Bit[30:11] : Reserved
//  Bit[10] : IR Enable/Disable
#define C_Func_IR_En                (1<<10)
#define C_Func_IR_Dis               (0<<10)
//  Bit[9] : UART Enable/Disable
#define C_Func_UART_En              (1<<9)
#define C_Func_UART_Dis             (0<<9)
//  Bit[8] : SDC Enable/Disable
#define C_Func_SDC_En               (1<<8)
#define C_Func_SDC_Dis              (0<<8)
//  Bit[7] : DAC CEnable/Disable
#define C_Func_DAC_En               (1<<7)
#define C_Func_DAC_Dis              (0<<7)
//  Bit[6] : ADC Enable/Disable
#define C_Func_ADC_En               (1<<6)
#define C_Func_ADC_Dis              (0<<6)
//  Bit[5] : I2C Enable/Disable
#define C_Func_I2C_En               (1<<5)
#define C_Func_I2C_Dis              (0<<5)
//  Bit[4] : RTC Enable/Disable
#define C_Func_RTC_En               (1<<4)
#define C_Func_RTC_Dis              (0<<4)
//  Bit[3] : SPI Enable/Disable
#define C_Func_SPI_En               (1<<3)
#define C_Func_SPI_Dis              (0<<3)
//  Bit[2] : PWM Enable/Disable
#define C_Func_PWM_En               (1<<2)
#define C_Func_PWM_Dis              (0<<2)
//  Bit[1] : Timer Enable/Disable
#define C_Func_TMR_En               (1<<1)
#define C_Func_TMR_Dis              (0<<1)
//  Bit[0] : GPIO Enable/Disable
#define C_Func_GPIO_En              (1<<0)
#define C_Func_GPIO_Dis             (0<<0)


//====================================================================
//  P_SMU_FUNC_RST (0xE00034)
//  Function Reset
//====================================================================
//  Bit[31:11] : Reserved
//  Bit[10] : IR Reset
#define C_Func_IR_RST               (1<<10)
//  Bit[9] : UART Reset
#define C_Func_UART_RST             (1<<9)
//  Bit[8] : SDC Reset
#define C_Func_SDC_RST              (1<<8)
//  Bit[7] : DAC Reset
#define C_Func_DAC_RST              (1<<7)
//  Bit[6] : ADC Reset
#define C_Func_ADC_RST              (1<<6)
//  Bit[5] : I2C Reset
#define C_Func_I2C_RST              (1<<5)
//  Bit[4] : Reserved
//  Bit[3] : SPI Reset
#define C_Func_SPI_RST              (1<<3)
//  Bit[2] : PWM Reset
#define C_Func_PWM_RST              (1<<2)
//  Bit[1] : Timer Reset
#define C_Func_TMR_RST              (1<<1)
//  Bit[0] : GPIO Reset
#define C_Func_GPIO_RST             (1<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  I2C_BASE (P_I2C)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_I2C_FIFO_Ctrl             0xF01010        // FIFO Control
#define P_I2C_INT_Ctrl              0xF01014        // Interrupt Control
#define P_I2C_Status                0xF01018        // Status Register
#define P_I2C_Addr                  0xF0101C        // Address Register
#define P_I2C_Data                  0xF01020        // Data Register
#define P_I2C_Mode                  0xF01024        // Protocol Mode Select and Data Count Setting
#define P_I2C_Cmd                   0xF01028        // Command Register
#define P_I2C_Ctrl                  0xF0102C        // I2C Function Control Register

typedef struct{
    volatile U32 Reserved[4];
    volatile U32 FIFO_Ctrl;
    volatile U32 INT_Ctrl;
    volatile U32 Status;
    volatile U32 Addr;
    volatile U32 Data;
    volatile U32 Mode;
    volatile U32 Cmd;
    volatile U32 Ctrl;
}I2C_TypeDef;


//====================================================================
//  P_I2C_FIFO_Ctrl (0xF01010)
//  FIFO Control
//====================================================================
//  Bit[31:2] : Reserved
//  Bit[1:0] : FIFO Size Control
#define C_I2C_FIFO_4Byte            1               // Read Only, Fixed 4-Byte


//====================================================================
//  P_I2C_INT_Ctrl (0xF01014)
//  Interrupt Control
//====================================================================
//  Bit[31:10] : Reserved
//  Bit[9] : Enable/Disable Transaction Completion Interrupt
#define C_I2C_CMPL_IEn              (1<<9)
#define C_I2C_CMPL_IDis             (0<<9)
//  Bit[8] : Enable/Disable Byte Receive Interrupt, when a byte of data is received
#define C_I2C_BYTE_Rx_IEn           (1<<8)
#define C_I2C_BYTE_Rx_IDis          (0<<8)
//  Bit[7] : Enable/Disable Byte Transmit Interrupt, when a byte of data is transmitted
#define C_I2C_BYTE_Tx_IEn           (1<<7)
#define C_I2C_BYTE_Tx_IDis          (0<<7)
//  Bit[6] : Enable/Disable Start Condition Interrupt
#define C_I2C_START_IEn             (1<<6)
#define C_I2C_START_IDis            (0<<6)
//  Bit[5] : Enable/Disable Stop Condition Interrupt
#define C_I2C_STOP_IEn              (1<<5)
#define C_I2C_STOP_IDis             (0<<5)
//  Bit[4] : Enable/Disable Arbitration Lose Interrupt
#define C_I2C_ARB_LOSE_IEn          (1<<4)
#define C_I2C_ARB_LOSE_IDis         (0<<4)
//  Bit[3] : Enable/Disable Address Hit Interrupt, for master mode, when the addressed slave returned an ACK
#define C_I2C_ADDR_HIT_IEn          (1<<3)
#define C_I2C_ADDR_HIT_IDis         (0<<3)
//  Bit[2] : Enable/Disable FIFO Half Interrupt
#define C_I2C_FIFO_HALF_IEn         (1<<2)
#define C_I2C_FIFO_HALF_IDis        (0<<2)
//  Bit[1] : Enable/Disable FIFO Full Interrupt
#define C_I2C_FIFO_FULL_IEn         (1<<1)
#define C_I2C_FIFO_FULL_IDis        (0<<1)
//  Bit[0] : Enable/Disable FIFO Empty Interrupt
#define C_I2C_FIFO_EMPTY_IEn        (1<<0)
#define C_I2C_FIFO_EMPTY_IDis       (0<<0)


//====================================================================
//  P_I2C_Status (0xF01018)
//  Status Control
//====================================================================
//  Bit[31:15] : Reserved
//  Bit[14] : Indicates SDA Status
#define C_I2C_SDA_Status            (1<<14)
//  Bit[13] : Indicates SCL Status
#define C_I2C_SCL_Status            (1<<13)
//  Bit[12] : Indicates General Call Address
#define C_I2C_Gen_Call              (1<<12)
//  Bit[11] : Indicates Bus Busy Status
#define C_I2C_Busy                  (1<<11)
//  Bit[10] : Indicates last action acknowledgement bit
#define C_I2C_ACK                   (1<<10)
//  Bit[9] : Transaction Completion Flag
#define C_I2C_CMPL_Flag             (1<<9)
//  Bit[8] : Byte Receive Flag, when a byte of data is received
#define C_I2C_BYTE_Rx_Flag          (1<<8)
//  Bit[7] : Byte Transmit Flag, when a byte of data is transmitted
#define C_I2C_BYTE_Tx_Flag          (1<<7)
//  Bit[6] : Start Condition Flag
#define C_I2C_START_Flag            (1<<6)
//  Bit[5] : Stop Condition Flag
#define C_I2C_STOP_Flag             (1<<5)
//  Bit[4] : Arbitration Lose Flag
#define C_I2C_ARB_LOSE_Flag         (1<<4)
//  Bit[3] : Address Hit Flag, for master mode, when the addressed slave returned an ACK
#define C_I2C_ADDR_HIT_Flag         (1<<3)
//  Bit[2] : FIFO Half Flag
#define C_I2C_FIFO_HALF_Flag        (1<<2)
//  Bit[1] : FIFO Full Flag
#define C_I2C_FIFO_FULL_Flag        (1<<1)
//  Bit[0] : FIFO Empty Flag
#define C_I2C_FIFO_EMPTY_Flag       (1<<0)


//====================================================================
//  P_I2C_Addr (0xF0101C)
//  Address Register
//====================================================================
//  Bit[31:10] : Reserved
//  Bit[9:0] : I2C Address Register
#define C_I2C_Addr                  0x3FF           // Mapping Range of Address


//====================================================================
//  P_I2C_Data (0xF01020)
//  Data Register
//====================================================================
//  Bit[31:8] : Reserved
//  Bit[7:0] : I2C Data Register
#define C_I2C_Data                  0xFF            // Mapping Range of Data


//====================================================================
//  P_I2C_Mode (0xF01024)
//  Protocol Mode Select and Data Count Setting
//====================================================================
//  Bit[31:13] : Reserved
//  Bit[12] : Master Mode Only, Send Start Condition at Beginning of Transaction
#define C_I2C_PHA_Start             (1<<12)
//  Bit[11] : Master Mode Only, Send the Address after Start Condition
#define C_I2C_PHA_Addr              (1<<11)
//  Bit[10] : Master Mode Only, Send the Data after Address
#define C_I2C_PHA_Data              (1<<10)
//  Bit[9] : Master Mode Only, Send the STOP Condition at the end of a Transaction
#define C_I2C_PHA_Stop              (1<<9)
//  Bit[8] : Transaction Direction, Direction Setting depend on Master/Slave Mode
#define C_I2C_Master_Dir_Rx         (1<<8)
#define C_I2C_Master_Dir_Tx         (0<<8)
#define C_I2C_Slave_Dir_Tx          (1<<8)
#define C_I2C_Slave_Dir_Rx          (0<<8)
//  Bit[7:0] : Data Counts (Bytes)
#define C_I2C_Data_Count            0xFF            // Mapping Range of Bit
#define C_I2C_Data_Count_1          0x01
#define C_I2C_Data_Count_128        0x80
#define C_I2C_Data_Count_255        0xFF
#define C_I2C_Data_Count_256        0x00            // Count Value '0x00' Means 256-Byte


//====================================================================
//  P_I2C_Cmd (0xF01028)
//  Command Register
//====================================================================
//  Bit[31:3] : Reserved
//  Bit[2:0] : Command, Perform the Corresponding Action
#define C_I2C_CMD_NoAction          0x0
#define C_I2C_CMD_Issue             0x1             // Master Mode Only, Issue a data Transaction
#define C_I2C_CMD_Resp_ACK          0x2             // Respond with an 'ACK' to the received byte
#define C_I2C_CMD_Resp_NACK         0x3             // Respond with a 'NACK' to the received byte
#define C_I2C_CMD_CLR_FIFO          0x4             // Clear FIFO
#define C_I2C_CMD_Reset             0x5             // Reset the I2C Controller


//====================================================================
//  P_I2C_Ctrl (0xF0102C)
//  I2C Function Control Register
//====================================================================
//  Bit[31:29] : Reserved
//  Bit[28:24] : Clock Setup Time(0~31), Standard Mode must be >= 250ns, Fast Mode must be >= 100ns, Plus Mode must be >= 50ns
#define C_I2C_Setup_Time            (0x1F<<24)
#define C_I2C_STD_Setup_Time        (((250/C_System_Clock_ns) - 4 - C_I2C_Spike_Time) << 24)
#define C_I2C_Fast_Setup_Time       (((100/C_System_Clock_ns) - 4 - C_I2C_Spike_Time) << 24)
#define C_I2C_Plus_Setup_Time       (((50/C_System_Clock_ns)  - 4 - C_I2C_Spike_Time) << 24)
//  Bit[23:21] : Pulse width of Spikes(0~7), Fast Mode must be <= 50ns, Plus Mode must be <= 50ns
#define C_I2C_Spike_Time_Offset     21
#define C_I2C_Spike_Time            (50/C_System_Clock_ns)
//  Bit[20:16] : Clock Hold Time(0~31), Standard Mode must be >= 300ns, Fast Mode must be >= 300ns
#define C_I2C_Hold_Time             (0x1F<<16)
#define C_I2C_STD_Hold_Time         (((300/C_System_Clock_ns) - 4 - C_I2C_Spike_Time) << 16)
#define C_I2C_Fast_Hold_Time        (((300/C_System_Clock_ns) - 4 - C_I2C_Spike_Time) << 16)
#define C_I2C_Plus_Hold_Time        0
//  Bit[15:14] : Reserved
//  Bit[13] : Clock Low Duty (High/Low Ratio)
#define C_I2C_SCL_LowDuty           (1<<13)
#define C_I2C_SCL_LowDuty_66        (1<<13)         // Two-Time Low + One-Time High, Clock Low Duty 66%
#define C_I2C_SCL_LowDuty_50        (0<<13)         // Clock Low Duty 50%
//  Bit[12:4] : Clock High Period(0~511), Standard Mode must be >= 4us, Fast Mode must be >= 0.6us, Plus Mode must be >= 0.26us
#define C_I2C_SCLHi_Time            (0x1FF<<4)
#define C_I2C_STD_SCLHi_Time        (((4000/C_System_Clock_ns) - 4 - C_I2C_Spike_Time) << 4)
#define C_I2C_Fast_SCLHi_Time       (((600/C_System_Clock_ns)  - 4 - C_I2C_Spike_Time) << 4)
#define C_I2C_Plus_SCLHi_Time       (((260/C_System_Clock_ns)  - 4 - C_I2C_Spike_Time) << 4)
//  Bit[3] : Reserved
//  Bit[2] : Master/Slave Mode Select
#define C_I2C_Mode                  (1<<2)
#define C_I2C_Mode_Master           (1<<2)
#define C_I2C_Mode_Slave            (0<<2)
//  Bit[1] : Addressing Mode
#define C_I2C_Addr_Mode             (1<<1)
#define C_I2C_Addr_Mode_10bit       (1<<1)
#define C_I2C_Addr_Mode_7bit        (0<<1)
//  Bit[0] : I2C Function Enalbe/Disable
#define C_I2C_En                    (1<<0)
#define C_I2C_Dis                   (0<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  UART_BASE (P_UART)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_UART_Data                 0xF02000        // Data Register
#define P_UART_Ctrl                 0xF02004        // Control Register
#define P_UART_Flag                 0xF02008        // Status Register
#define P_UART_BaudRate             0xF0200C        // BaudRate Setting

typedef struct{
    volatile U32 Data;
    volatile U32 Ctrl;
    volatile U32 Flag;
    volatile U32 BaudRate;
}UART_TypeDef;


//====================================================================
//  P_UART_Data (0xF02000)
//  Data Register
//====================================================================
//  Bit[31:8] : Reserved
//  Bit[7:0] : UART Data
#define C_UART_Data                 0xFF            // Mapping Range of Bit


//====================================================================
//  P_UART_Ctrl (0xF02004)
//  Control Register
//====================================================================
//  Bit[31:20] : Reserved
//  Bit[19:18] : Level Setting of UART Tx FIFO
#define C_UART_Tx_FIFO_Level        (3<<18)
#define C_UART_Tx_FIFO_Level_0      (0<<18)
#define C_UART_Tx_FIFO_Level_1      (1<<18)
#define C_UART_Tx_FIFO_Level_2      (2<<18)
#define C_UART_Tx_FIFO_Level_3      (3<<18)
//  Bit[17:16] : Level Setting of UART Rx FIFO
#define C_UART_Rx_FIFO_Level        (3<<16)
#define C_UART_Rx_FIFO_Level_0      (0<<16)
#define C_UART_Rx_FIFO_Level_1      (1<<16)
#define C_UART_Rx_FIFO_Level_2      (2<<16)
#define C_UART_Rx_FIFO_Level_3      (3<<16)
//  Bit[15] : Tx FIFO Reset
#define C_UART_Tx_FIFO_Reset        (1<<15)
//  Bit[14] : Rx FIFO Reset
#define C_UART_Rx_FIFO_Reset        (1<<14)
//  Bit[13] : Reserved
//  Bit[12] : Parity Even/Odd Setting
#define C_UART_Parity_Even          (1<<12)
#define C_UART_Parity_Odd           (0<<12)
//  Bit[11] : Parity Enable/Disable
#define C_UART_Parity_En            (1<<11)
#define C_UART_Parity_Dis           (0<<11)
//  Bit[10] : Stop Width
#define C_UART_STOP_WIDTH_2         (1<<10)         // If Data Length=5, 1.5-Stop, else 2-Stop
#define C_UART_STOP_WIDTH_1         (0<<10)
//  Bit[9:8] : Data Length
#define C_UART_DATA_LENGTH          (3<<8)
#define C_UART_DATA_LENGTH_5        (0<<8)
#define C_UART_DATA_LENGTH_6        (1<<8)
#define C_UART_DATA_LENGTH_7        (2<<8)
#define C_UART_DATA_LENGTH_8        (3<<8)
//  Bit[7] : Reserved
//  Bit[6] : Received Error Interrupt Enable/Disable
#define C_UART_Rx_Err_IEn           (1<<6)
#define C_UART_Rx_Err_IDis          (0<<6)
//  Bit[5] : Interrupt Enable/Disable of UART Tx FIFO Level
#define C_UART_Tx_FIFO_Level_IEn    (1<<5)
#define C_UART_Tx_FIFO_Level_IDis   (0<<5)
//  Bit[4] : Interrupt Enable/Disable of UART Rx FIFO Level
#define C_UART_Rx_FIFO_Level_IEn    (1<<4)
#define C_UART_Rx_FIFO_Level_IDis   (0<<4)
//  Bit[3] : Interrupt Enable/Disable of UART Tx FIFO Full
#define C_UART_Tx_FIFO_Full_IEn     (1<<3)
#define C_UART_Tx_FIFO_Full_IDis    (0<<3)
//  Bit[2] : Interrupt Enable/Disable of UART Rx FIFO Full
#define C_UART_Rx_FIFO_Full_IEn     (1<<2)
#define C_UART_Rx_FIFO_Full_IDis    (0<<2)
//  Bit[1] : Interrupt Enable/Disable of UART Tx FIFO Empty
#define C_UART_Rx_FIFO_EMPTY_IEn    (1<<1)
#define C_UART_Rx_FIFO_EMPTY_IDis   (0<<1)
//  Bit[0] : Interrupt Enable/Disable of UART Rx FIFO NoEmpty
#define C_UART_Rx_FIFO_NoEMPTY_IEn  (1<<0)
#define C_UART_Rx_FIFO_NoEMPTY_IDis (0<<0)


//====================================================================
//  P_UART_Flag (0xF02008)
//  Status Register
//====================================================================
//  Bit[31:12] : Reserved
//  Bit[11] : Frame Error (Stop not high)
#define C_UART_Frame_Err_Flag       (1<<11)
//  Bit[10] : Parity Error (Parity not match)
#define C_UART_Parity_Err_Flag      (1<<10)
//  Bit[9] : Frame Error (FIFO Overrun)
#define C_UART_Overrun_Err_Flag     (1<<9)
//  Bit[8:6] : Reserved
//  Bit[5] : Flag of UART Tx FIFO Level
#define C_UART_Tx_FIFO_Level_Flag   (1<<5)
//  Bit[4] : Flag of UART Rx FIFO Level
#define C_UART_Rx_FIFO_Level_Flag   (1<<4)
//  Bit[3] : Flag of UART Tx FIFO Full
#define C_UART_Tx_FIFO_Full_Flag    (1<<3)
//  Bit[2] : Flag of UART Rx FIFO Full
#define C_UART_Rx_FIFO_Full_Flag    (1<<2)
//  Bit[1] : Flag of UART Tx FIFO Empty
#define C_UART_Tx_FIFO_Empty_Flag   (1<<1)          //Write '1' to clear this Flag
//  Bit[0] : Flag of UART Rx FIFO NoEmpty
#define C_UART_Rx_FIFO_NoEMPTY_Flag (1<<0)


//====================================================================
//  P_UART_BaudRate (0xF0200C)
//  BaudRate Setting
//====================================================================
//  Bit[31:10] : Reserved
//  Bit[9:0] : BaudRate Divisor
#define C_UART_BaudRate_4800        ((C_High_Clock / 8 / 4800)   - 1)
#define C_UART_BaudRate_9600        ((C_High_Clock / 8 / 9600)   - 1)
#define C_UART_BaudRate_14400       ((C_High_Clock / 8 / 14400)  - 1)
#define C_UART_BaudRate_19200       ((C_High_Clock / 8 / 19200)  - 1)
#define C_UART_BaudRate_28800       ((C_High_Clock / 8 / 28800)  - 1)
#define C_UART_BaudRate_31250       ((C_High_Clock / 8 / 31250)  - 1)
#define C_UART_BaudRate_38400       ((C_High_Clock / 8 / 38400)  - 1)
#define C_UART_BaudRate_57600       ((C_High_Clock / 8 / 57600)  - 1)
#define C_UART_BaudRate_115200      ((C_High_Clock / 8 / 115200) - 1)
#define C_UART_BaudRate_128000      ((C_High_Clock / 8 / 128000) - 1)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  TIMER0_BASE (P_Timer0)
//  TIMER1_BASE (P_Timer1)
//  TIMER2_BASE (P_Timer2)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_TMR0_Ctrl                 0xF04000        // Control
#define P_TMR0_Data                 0xF04004        // Data
#define P_TMR0_INT_Ctrl             0xF04008        // Interrupt Control
#define P_TMR0_Flag                 0xF0400C        // Flag

#define P_TMR1_Ctrl                 0xF04040        // Control
#define P_TMR1_Data                 0xF04044        // Data
#define P_TMR1_INT_Ctrl             0xF04048        // Interrupt Control
#define P_TMR1_Flag                 0xF0404C        // Flag

#define P_TMR2_Ctrl                 0xF04080        // Control
#define P_TMR2_Data                 0xF04084        // Data
#define P_TMR2_INT_Ctrl             0xF04088        // Interrupt Control
#define P_TMR2_Flag                 0xF0408C        // Flag

typedef struct{
    volatile U32 Ctrl;
    volatile U32 Data;
    volatile U32 INT_Ctrl;
    volatile U32 Flag;
}TIMER_TypeDef;


//====================================================================
//  P_TMR0_Ctrl (0xF04000)
//  P_TMR1_Ctrl (0xF04040)
//  P_TMR2_Ctrl (0xF04080)
//  Timer Control Register
//====================================================================
//  Bit[31:10] : Reserved
//  Bit[9:8] : Timer Clock Source Select
#define C_TMR_CLK_Src               (3<<8)
#define C_TMR_CLK_Src_ExtCLK_Sync   (0<<8)          // With System Clock Sampling
#define C_TMR_CLK_Src_ExtCLK_Async  (1<<8)          // Without System Clock Sampling
#define C_TMR_CLK_Src_HiCLK         (2<<8)
#define C_TMR_CLK_Src_LoCLK         (3<<8)
//  Bit[7] : Reserved
//  Bit[6:4] : Timer Clock Divisor
#define C_TMR_CLK_Div               (7<<4)
#define C_TMR_CLK_Div_1             (7<<4)
#define C_TMR_CLK_Div_2             (6<<4)
#define C_TMR_CLK_Div_4             (5<<4)
#define C_TMR_CLK_Div_8             (4<<4)
#define C_TMR_CLK_Div_16            (3<<4)
#define C_TMR_CLK_Div_32            (2<<4)
#define C_TMR_CLK_Div_64            (1<<4)
#define C_TMR_CLK_Div_128           (0<<4)
//  Bit[3:2] : Reserved
//  Bit[1] : Timer Auto-reload in Underflow
#define C_TMR_Auto_Reload_En        (1<<1)
#define C_TMR_Auto_Reload_Dis       (0<<1)
//  Bit[0] : Timer Enable/Disable
#define C_TMR_En                    (1<<0)
#define C_TMR_Dis                   (0<<0)


//====================================================================
//  P_TMR0_Data (0xF04004)
//  P_TMR1_Data (0xF04044)
//  P_TMR2_Data (0xF04084)
//  16-bit Down Count Timer, Data/Counter Register
//====================================================================
//  Bit[31:16] : Timer Counter (Current Value)
#define C_TMR_Count                 (0xFFFF<<16)    // Mapping Range of Bit
//  Bit[31:16] : Timer Data (Reload Data Register)
#define C_TMR_Data                  0xFFFF          // Mapping Range of Bit

#define C_Timer_Setting_1K          ((C_High_Clock /  1000) - 1)
#define C_Timer_Setting_2K          ((C_High_Clock /  2000) - 1)
#define C_Timer_Setting_3K          ((C_High_Clock /  3000) - 1)
#define C_Timer_Setting_4K          ((C_High_Clock /  4000) - 1)
#define C_Timer_Setting_5K          ((C_High_Clock /  5000) - 1)
#define C_Timer_Setting_6K          ((C_High_Clock /  6000) - 1)
#define C_Timer_Setting_7K          ((C_High_Clock /  7000) - 1)
#define C_Timer_Setting_8K          ((C_High_Clock /  8000) - 1)
#define C_Timer_Setting_9K          ((C_High_Clock /  9000) - 1)
#define C_Timer_Setting_10K         ((C_High_Clock / 10000) - 1)
#define C_Timer_Setting_11K         ((C_High_Clock / 11000) - 1)
#define C_Timer_Setting_12K         ((C_High_Clock / 12000) - 1)
#define C_Timer_Setting_13K         ((C_High_Clock / 13000) - 1)
#define C_Timer_Setting_14K         ((C_High_Clock / 14000) - 1)
#define C_Timer_Setting_15K         ((C_High_Clock / 15000) - 1)
#define C_Timer_Setting_16K         ((C_High_Clock / 16000) - 1)
#define C_Timer_Setting_20K         ((C_High_Clock / 20000) - 1)
#define C_Timer_Setting_24K         ((C_High_Clock / 24000) - 1)
#define C_Timer_Setting_28K         ((C_High_Clock / 28000) - 1)
#define C_Timer_Setting_32K         ((C_High_Clock / 32000) - 1)
#define C_Timer_Setting_36K         ((C_High_Clock / 36000) - 1)
#define C_Timer_Setting_40K         ((C_High_Clock / 40000) - 1)
#define C_Timer_Setting_44K1        ((C_High_Clock / 44100) - 1)
#define C_Timer_Setting_48K         ((C_High_Clock / 48000) - 1)
#define C_Timer_Setting_64K         ((C_High_Clock / 64000) - 1)
#define C_Timer_Setting_96K         ((C_High_Clock / 96000) - 1)

//====================================================================
//  P_TMR0_INT_Ctrl (0xF04008)
//  P_TMR1_INT_Ctrl (0xF04048)
//  P_TMR2_INT_Ctrl (0xF04088)
//  Timer Interrupt Control Register
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : Timer Interrupt Enable/Disable
#define C_TMR_IEn                   (1<<0)
#define C_TMR_IDis                  (0<<0)


//====================================================================
//  P_TMR0_Flag (0xF0400C)
//  P_TMR1_Flag (0xF0404C)
//  P_TMR2_Flag (0xF0408C)
//  Timer Flag Register
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : Timer Flag
#define C_TMR_Flag                  (1<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  RTC_BASE (P_RTC)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_RTC_INT_Ctrl              0xF06000        // Interrupt Enable/Disable
#define P_RTC_Flag                  0xF06004        // Flag

typedef struct{
    volatile U32 INT_Ctrl;
    volatile U32 Flag;
}RTC_TypeDef;


//====================================================================
//  P_RTC_INT_Ctrl (0xF06000)
//  Real Time Clock(RTC) Interrupt Control Register
//====================================================================
//  Bit[31:4] : Reserved
//  Bit[3] : RTC 4KHz Interrupt Enable/Disable
#define C_RTC_4KHz_IEn              (1<<3)
#define C_RTC_4KHz_IDis             (0<<3)
//  Bit[2] : RTC 1KHz Interrupt Enable/Disable
#define C_RTC_1KHz_IEn              (1<<2)
#define C_RTC_1KHz_IDis             (0<<2)
//  Bit[1] : RTC 64Hz Interrupt Enable/Disable
#define C_RTC_64Hz_IEn              (1<<1)
#define C_RTC_64Hz_IDis             (0<<1)
//  Bit[0] : RTC 2Hz Interrupt Enable/Disable
#define C_RTC_2Hz_IEn               (1<<0)
#define C_RTC_2Hz_IDis              (0<<0)


//====================================================================
//  P_RTC_Flag (0xF06004)
//  Real Time Clock(RTC) Flag, Write '1' to clear Flag
//====================================================================
//  Bit[31:4] : Reserved
//  Bit[3] : RTC 4KHz Flag
#define C_RTC_4KHz_Flag             (1<<3)
//  Bit[2] : RTC 1KHz Flag
#define C_RTC_1KHz_Flag             (1<<2)
//  Bit[1] : RTC 64Hz Flag
#define C_RTC_64Hz_Flag             (1<<1)
//  Bit[0] : RTC 2Hz Flag
#define C_RTC_2Hz_Flag              (1<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  WDT_BASE (P_WDT)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_WDT_Ctrl                  0xF07000        // Watchdog Timeout Setting
#define P_WDT_CLR                   0xF07004        // Watchdog Clear, Write 0x3CA5 to Clear Watchdog Timer

typedef struct{
    volatile U32 Ctrl;
    volatile U32 CLR;
} WDT_TypeDef;


//====================================================================
//  P_WDT_Ctrl (0xF07000)
//  Watchdog Timeout Setting
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : WDT Timeout Setting
#define C_WDT_Timeout_188ms         (1<<0)          // Time Out 188ms
#define C_WDT_Timeout_750ms         (0<<0)          // Time Out 750ms (Default)


//====================================================================
//  P_WDT_CLR (0xF07004)
//  Watchdog Clear, Write 0x3CA5 to Clear Watchdog Timer
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:0] : WatchDog Clear Data
#define C_WDT_CLR_Data              0x3CA5





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  PORTA_BASE (P_PORTA)
//  PORTB_BASE (P_PORTB)
//  PORTC_BASE (P_PORTC)
//  PORT_EXT_BASE (P_PORT_EXT)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_PA_DIR                    0xF08000        // Input/Output (Direction)
#define P_PA_Data                   0xF08004        // Data Port (Buffer Register)
#define P_PA_PAD                    0xF08008        // PAD Status
#define P_PA_Wakeup_Mask            0xF0800C        // Key Wakeup Mask/Unmask
#define P_PA_CFG                    0xF08010        // IO Configration, Setting Pull-High Resistor and Constant Sink
#define P_PA_MFP                    0xF08014        // Multi-Function Peripherial Enalbe/Disable

#define P_PB_DIR                    0xF08020        // Input/Output (Direction)
#define P_PB_Data                   0xF08024        // Data Port (Buffer Register)
#define P_PB_PAD                    0xF08028        // PAD Status
#define P_PB_Wakeup_Mask            0xF0802C        // Key Wakeup Mask/Unmask
#define P_PB_CFG                    0xF08030        // IO Configration, Setting Pull-High Resistor
#define P_PB_MFP                    0xF08034        // Multi-Function Peripherial Enalbe/Disable

#define P_PC_DIR                    0xF08040        // Input/Output (Direction)
#define P_PC_Data                   0xF08044        // Data Port (Buffer Register)
#define P_PC_PAD                    0xF08048        // PAD Status
#define P_PC_Wakeup_Mask            0xF0804C        // Key Wakeup Mask/Unmask
#define P_PC_CFG                    0xF08050        // IO Configuration, Setting Pull-High Resistor
#define P_PC_MFP                    0xF08054        // Multi-Function Peripherial Enalbe/Disable

#define P_EXT_INT_Trig              0xF08060        // External Interrupt Rising/Falling Edge Trigger
#define P_EXT_INT_Ctrl              0xF08064        // External Interrupt Enable/Disable
#define P_EXT_INT_Flag              0xF08068        // External Interrupt Flag
#define P_PULL_HIGH_CFG             0xF08070        // GPIO Port Pull-High Configuration Register (Select Pull-High Resistor 100K/1M)

typedef struct{
    volatile U32 DIR;
    volatile U32 Data;
    volatile U32 PAD;
    volatile U32 Wakeup_Mask;
    volatile U32 CFG;
    volatile U32 MFP;
    volatile U32 Reserved[22];
    volatile U32 PULL_HIGH_CFG;     // only for PA Structure (Select Pull-High Resistor by PA Structure)
}GPIO_TypeDef;

typedef struct{
    volatile U32 EXT_INT_Trig;
    volatile U32 EXT_INT_Ctrl;
    volatile U32 EXT_INT_Flag;
}GPIO_Ext_TypeDef;


//====================================================================
//  P_PA_DIR (0xF08000)
//  P_PB_DIR (0xF08020)
//  P_PC_DIR (0xF08040)
//  GPIO Input/Output Control (Direction)
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:0] : GPIO Input/Output Configration, Mapping to Port[15:0]
#define C_GPIO_Dir_In               0xFFFF
#define C_GPIO_Dir_Out              0x0000


//====================================================================
//  P_PA_Data (0xF08004)
//  P_PB_Data (0xF08024)
//  P_PC_Data (0xF08044)
//  GPIO Data Port (Buffer Register)
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:0] : GPIO Data Port (Buffer Register), Mapping to Port[15:0]
#define C_GPIO_Data                 0xFFFF          // Mapping Range of Bit


//====================================================================
//  P_PA_PAD (0xF08008)
//  P_PB_PAD (0xF08028)
//  P_PC_PAD (0xF08048)
//  GPIO PAD Status
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:0] : GPIO PAD Status, Mapping to Port[15:0]
#define C_GPIO_Pad                  0xFFFF          // Mapping Range of Bit


//====================================================================
//  P_PA_Wakeup_Mask (0xF0800C)
//  P_PB_Wakeup_Mask (0xF0802C)
//  P_PC_Wakeup_Mask (0xF0804C)
//  GPIO Key Wakeup Mask/Unmask and Sink/Drive Current Attribute Setting
//====================================================================
//  Bit[31:16] : Sink Current Mode (include CSC Mode)
#define C_GPIO_Sink_Large           (0x0000<<16)
#define C_GPIO_Sink_Normal          (0xFFFF<<16)    // Mapping Range of Bit
#define C_GPIO_Sink_CSC_Large       (0x00<<24)
#define C_GPIO_Sink_CSC_Normal      (0xFF<<24)      // Mapping Range of Bit, *** Only for PA[15:8]
//  Bit[27:24][21:16] : PB Drive Current Mode
#define C_GPIOB_Drive_Large         (0x000<<16)
#define C_GPIOB_Drive_Normal        (0xF3F<<16)     // Mapping Range of Bit
//  Bit[23:16] : PC Drive Current Mode
#define C_GPIOC_Drive_Large         (0x00<<16)
#define C_GPIOC_Drive_Normal        (0xFF<<16)      // Mapping Range of Bit
//  Bit[15:0] : GPIO Wakeup Mask/UnMask, Mapping to Port[15:0]
#define C_GPIO_Wakeup_Mask          0x0000
#define C_GPIO_Wakeup_UnMask        0xFFFF          // Have Key Wakeup Capability (Default Disabled), Mapping Range of Bit


//====================================================================
//  P_PA_CFG (0xF08010)
//  P_PB_CFG (0xF08020)
//  P_PC_CFG (0xF08040)
//  GPIO Configration, Setting Pull-High Resistor and Constant Sink
//====================================================================
//  Bit[31:24] : Constant Current(CSC) Enable/Disable
#define C_GPIO_Sink_CSC_En          (0x00<<24)
#define C_GPIO_Sink_CSC_Dis         (0xFF<<24)      // Mapping Range of Bit, *** Only for PA[15:8]
//  Bit[23:16] : Reserved
//  Bit[15:0] : Pull-High Enable/Disable, Mapping to Port[15:0]
#define C_GPIO_PH_En                0x0000
#define C_GPIO_PH_Dis               0xFFFF          // Mapping Range of Bit


//====================================================================
//  P_PA_MFP (0xF08014)
//  Port A Multi Function Peripheral(MFP) Control
//====================================================================
//  Bit[31:24] : Reserved
//  Bit[23] : MFP_PA7 - ADC Channel 7
#define C_MFP_ADC_CH7_En            (1<<23)
#define C_MFP_ADC_CH7_Dis           (0<<23)
//  Bit[22] : MFP_PA6 - ADC Channel 6
#define C_MFP_ADC_CH6_En            (1<<22)
#define C_MFP_ADC_CH6_Dis           (0<<22)
//  Bit[21] : MFP_PA5 - ADC Channel 5
#define C_MFP_ADC_CH5_En            (1<<21)
#define C_MFP_ADC_CH5_Dis           (0<<21)
//  Bit[20] : MFP_PA4 - ADC Channel 4
#define C_MFP_ADC_CH4_En            (1<<20)
#define C_MFP_ADC_CH4_Dis           (0<<20)
//  Bit[19:16] : Reserved
//  Bit[15] : MFP_PA15 - External INT pin
#define C_MFP_EXT_INT_En            (1<<15)
#define C_MFP_EXT_INT_Dis           (0<<15)
//  Bit[14] : MFP_PA14 - Timer 1/2 External Clock Source Input
#define C_MFP_TMR1_ExtCLK_En        (1<<14)
#define C_MFP_TMR1_ExtCLK_Dis       (0<<14)
#define C_MFP_TMR2_ExtCLK_En        (1<<14)
#define C_MFP_TMR2_ExtCLK_Dis       (0<<14)
//  Bit[13] : Reserved
//  Bit[12] : MFP_PA[13:12] - I2C , SCL(PA13) and SDA(PA12)
#define C_MFP_I2C_En                (1<<12)
#define C_MFP_I2C_Dis               (0<<12)
//  Bit[11] : MFP_PA11 - PWMA3
#define C_MFP_PWMA3_En              (1<<11)
#define C_MFP_PWMA3_Dis             (0<<11)
//  Bit[10] : MFP_PA10 - PWMA2
#define C_MFP_PWMA2_En              (1<<10)
#define C_MFP_PWMA2_Dis             (0<<10)
//  Bit[9] : MFP_PA9 - PWMA1
#define C_MFP_PWMA1_En              (1<<9)
#define C_MFP_PWMA1_Dis             (0<<9)
//  Bit[8] : MFP_PA8 - PWMA0
#define C_MFP_PWMA0_En              (1<<8)
#define C_MFP_PWMA0_Dis             (0<<8)
//  Bit[7] : MFP_PA7 - UART Rx
#define C_MFP_UART_Rx_En            (1<<7)
#define C_MFP_UART_Rx_Dis           (0<<7)
//  Bit[6] : MFP_PA6 - UART Tx
#define C_MFP_UART_Tx_En            (1<<6)
#define C_MFP_UART_Tx_Dis           (0<<6)
//  Bit[5] : MFP_PA5 - IR
#define C_MFP_IR_En                 (1<<5)
#define C_MFP_IR_Dis                (0<<5)
//  Bit[4] : MFP_PA4 - Timer 0 External Clock Source Input
#define C_MFP_TMR0_ExtCLK_En        (1<<4)
#define C_MFP_TMR0_ExtCLK_Dis       (0<<4)
//  Bit[3] : MFP_PA3 - ADC Channel 3
#define C_MFP_ADC_CH3_En            (1<<3)
#define C_MFP_ADC_CH3_Dis           (0<<3)
//  Bit[2] : MFP_PA2 - ADC Channel 2
#define C_MFP_ADC_CH2_En            (1<<2)
#define C_MFP_ADC_CH2_Dis           (0<<2)
//  Bit[1] : MFP_PA1 - ADC Channel 1
#define C_MFP_ADC_CH1_En            (1<<1)
#define C_MFP_ADC_CH1_Dis           (0<<1)
//  Bit[0] : MFP_PA0 - ADC Channel 0
#define C_MFP_ADC_CH0_En            (1<<0)
#define C_MFP_ADC_CH0_Dis           (0<<0)


//====================================================================
//  P_PB_MFP (0xF08024)
//  Port B Multi Function Peripheral(MFP) Control
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15] : MFP_PB15 - PWMB3
#define C_MFP_PWMB3_En              (1<<15)
#define C_MFP_PWMB3_Dis             (0<<15)
//  Bit[14] : MFP_PB14 - PWMB-IO2
#define C_MFP_PWMB2_En              (1<<14)
#define C_MFP_PWMB2_Dis             (0<<14)
//  Bit[13] : MFP_PB13 - PWMB-IO1
#define C_MFP_PWMB1_En              (1<<13)
#define C_MFP_PWMB1_Dis             (0<<13)
//  Bit[12] : MFP_PB12 - PWMB-IO0
#define C_MFP_PWMB0_En              (1<<12)
#define C_MFP_PWMB0_Dis             (0<<12)
//  Bit[11:10] : Reserved
//  Bit[9] : MFP_PB9 - SPI1, SCLK(PB9) / MOSI(PB10) / MISO(PB11)
#define C_MFP_SPI1_En               (1<<9)
#define C_MFP_SPI1_Dis              (0<<9)
//  Bit[8] : Reserved
//  Bit[7] : MFP_PB7 - Crystal Output, *** read-only, Select Mode by Option
#define C_MFP_XTAL_Out              (1<<7)
#define C_MFP_PB7                   (0<<7)
//  Bit[6] : MFP_PB6 - Crystal Input,  *** read-only, Select Mode by Option
#define C_MFP_XTAL_In               (1<<6)
#define C_MFP_PB6                   (0<<6)
//  Bit[5] : Reserved
//  Bit[4] : MFP_PB4 - SPI0, SIO2(PB4) / SIO3(PB5)
#define C_MFP_SPI0_SIO23_En         (1<<4)
#define C_MFP_SPI0_SIO23_Dis        (0<<4)
//  Bit[3:2] : Reserved
//  Bit[1] : MFP_PB1 - SPI0, SCLK(PB1) / SIO0(PB2) / SIO1(PB3)
#define C_MFP_SPI0_SCLK_SIO01_En    (1<<1)
#define C_MFP_SPI0_SCLK_SIO01_Dis   (0<<1)
//  Bit[0] : MFP_PB0 - SPI0, CSB
#define C_MFP_SPI0_CSB_En           (1<<0)
#define C_MFP_SPI0_CSB_Dis          (0<<0)


//====================================================================
//  P_PC_MFP (0xF08044)
//  Port C Multi Function Peripheral(MFP) Control
//====================================================================
//  Bit[31:8] : Reserved
//  Bit[7] : MFP_PC7 - SDC, Write Protect
#define C_MFP_SDC_WP_En             (1<<7)
#define C_MFP_SDC_WP_Dis            (0<<7)
//  Bit[6] : MFP_PC6 - SDC, Card Detect
#define C_MFP_SDC_CD_En             (1<<6)
#define C_MFP_SDC_CD_Dis            (0<<6)
//  Bit[5:1] : Reserved
//  Bit[0] : MFP_PC6 - SDC, CLK(PC0) / CMD(PC1) / IO0(PC2) / IO1(PC3) / IO2(PC4) / IO3(PC5)
#define C_MFP_SDC_En                (1<<0)
#define C_MFP_SDC_Dis               (0<<0)


//====================================================================
//  P_EXT_INT_Trig (0xF08060)
//  External Interrupt Rising/Falling Edge Trigger
//====================================================================
//  Bit[31:2] : Reserved
//  Bit[1:0] : External Interrupt Trigger Mode
#define C_EXT_INT_Trig_Rising       0               // Default
#define C_EXT_INT_Trig_Falling      1
#define C_EXT_INT_Trig_Edge         2               // Rising edge and Falling edge


//====================================================================
//  P_EXT_INT_Ctrl (0xF08064)
//  External Interrupt Enable/Disable
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : External Interrupt Enable/Disable
#define C_EXT_INT_IEn               1
#define C_EXT_INT_IDis              0


//====================================================================
//  P_EXT_INT_Flag (0xF08068)
//  External Interrupt Flag
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : External Interrupt Flag
#define C_EXT_INT_Flag              1


//====================================================================
//  P_PULL_HIGH_CFG (0xF08070)
//  Port Pull-High Configuration Register (Setting Pull-High Resistor as 100K/1M mode)
//  When SDC MFP turn on, PC's Pull-High resistor is 20K
//====================================================================
//  Bit[31:5] : Reserved
//  Bit[4] : PCL (PC[7:0])
#define C_PULL_HIGH_PCL             (1<<4)
#define C_PULL_HIGH_PCL_100K        (1<<4)
#define C_PULL_HIGH_PCL_1M          (0<<4)
//  Bit[3] : PBH (PB[15:8])
#define C_PULL_HIGH_PBH             (1<<3)
#define C_PULL_HIGH_PBH_100K        (1<<3)
#define C_PULL_HIGH_PBH_1M          (0<<3)
//  Bit[2] : PBL (PB[7:0])
#define C_PULL_HIGH_PBL             (1<<2)
#define C_PULL_HIGH_PBL_100K        (1<<2)
#define C_PULL_HIGH_PBL_1M          (0<<2)
//  Bit[1] : PAH (PA[15:8])
#define C_PULL_HIGH_PAH             (1<<1)
#define C_PULL_HIGH_PAH_100K        (1<<1)
#define C_PULL_HIGH_PAH_1M          (0<<1)
//  Bit[0] : PAL (PA[7:0])
#define C_PULL_HIGH_PAL             (1<<0)
#define C_PULL_HIGH_PAL_100K        (1<<0)
#define C_PULL_HIGH_PAL_1M          (0<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  SPI0_BASE (P_SPI0)
//  SPI1_BASE (P_SPI1)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_SPI0_Ctrl                 0xF0B000        // Control Register
#define P_SPI0_Addr_Mode            0xF0B004        // *** Address Mode Setting, only for SPI0
#define P_SPI0_FIFO_RST             0xF0B008        // FIFO Data Reset
#define P_SPI0_Data                 0xF0B010        // Data Port

#define P_SPI1_Ctrl                 0xF0B040        // Control Register
#define P_SPI1_FIFO_RST             0xF0B048        // FIFO Data Reset
#define P_SPI1_Data                 0xF0B050        // Data Port

typedef struct{
    volatile U32 Ctrl;
    volatile U32 Addr_Mode;         // only for SPI0
    volatile U32 FIFO_RST;
    volatile U32 Reserved;
    volatile U32 Data;
}SPI_TypeDef;


//====================================================================
//  P_SPI0_Ctrl (0xF0B000)
//  P_SPI1_Ctrl (0xF0B040)
//  SPI Control Register
//====================================================================
//  Bit[31:5] : Reserved
//  Bit[20:16] : SPI FIFO Length, SPI0 Maximum 32-Byte, SPI1 Maximum 16-Byte
#define C_SPI_FIFO_Length           (0x1F<<16)      // Mapping Range of Bit
//  Bit[15:11] : Reserved
//  Bit[10:8] : SPI Clock Divisor
#define C_SPI_CLK_Div               (7<<8)
#define C_SPI_CLK_Div_1             (7<<8)
#define C_SPI_CLK_Div_2             (6<<8)
#define C_SPI_CLK_Div_4             (5<<8)
#define C_SPI_CLK_Div_8             (4<<8)
#define C_SPI_CLK_Div_16            (3<<8)
#define C_SPI_CLK_Div_32            (2<<8)
#define C_SPI_CLK_Div_64            (1<<8)
#define C_SPI_CLK_Div_128           (0<<8)
//  Bit[7] : SPI Clock_Idle_Polarity Setting
#define C_SPI_Pol_High              (1<<7)          // CPOL = 1
#define C_SPI_Pol_Low               (0<<7)          // CPOL = 0
//  Bit[6] : SPI Clock Latching Phase Setting
#define C_SPI_Phase_2nd_Edge        (1<<6)          // CPHA = 1
#define C_SPI_Phase_1st_Edge        (0<<6)          // CPHA = 0
//  Bit[5:4] : SPI Data Mode                        // Only for SPI0
#define C_SPI_DataMode              (3<<4)
#define C_SPI_DataMode_1IO          (0<<4)
#define C_SPI_DataMode_2IO          (2<<4)
#define C_SPI_DataMode_4IO          (3<<4)
//  Bit[3:2] : Reserved
//  Bit[1] : Start Send Data (Tx), Write '1' to Start, Read for Busy Flag
#define C_SPI_Tx_Start              (1<<1)
#define C_SPI_Tx_Busy               (1<<1)
//  Bit[0] : Start Receive Data (Rx), Write '1' to Start, Read for Busy Flag
#define C_SPI_Rx_Start              (1<<0)
#define C_SPI_Rx_Busy               (1<<0)


//====================================================================
//  P_SPI0_Addr_Mode (0xF0B004)
//  *** Address Mode Setting, only for SPI0
//====================================================================
//  Bit[31:18] : Reserved
//  Bit[17:8] : SPI DataFlash Bank
#define C_SPI_DataFlash_Bank        (0x2FF<<8)      // Mapping Range of Bit
//  Bit[7:3] : Reserved
//  Bit[2] : SPI Address Mode
#define C_SPI_Addr_Mode_4Byte       (1<<2)
#define C_SPI_Addr_Mode_3Byte       (0<<2)
//  Bit[1:0] : SPI Command Setting
#define C_SPI_CMD                   (3<<0)
#define C_SPI_CMD_1IO               (0<<0)          // Command 0x03
#define C_SPI_CMD_2IO               (2<<0)          // Command 0xBB
#define C_SPI_CMD_4IO               (3<<0)          // Command 0xEB


//====================================================================
//  P_SPI0_FIFO_RST (0xF0B008)
//  P_SPI1_FIFO_RST (0xF0B048)
//  FIFO Data Reset
//====================================================================
//  Bit[31] : Reserved
//  Bit[30] : FIFO Reset
#define C_SPI_FIFO_Reset            (1<<30)
//  Bit[29:0] : Reserved


//====================================================================
//  P_SPI0_Data (0xF0B010)
//  P_SPI1_Data (0xF0B050)
//  Data Port
//====================================================================
//  Bit[31:0] : SPI Data
#define C_SPI_Data                  0xFFFFFFFF      // Mapping Range of Bit





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  SDC_BASE (P_SDC)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_SDC_CMD                   0xF0E000        // Command Register
#define P_SDC_ARG                   0xF0E004        // Argument of Command
#define P_SDC_RSP_Status0           0xF0E008        // Card Response Status 0
#define P_SDC_RSP_Status1           0xF0E00C        // Card Response Status 1
#define P_SDC_RSP_Status2           0xF0E010        // Card Response Status 2
#define P_SDC_RSP_Status3           0xF0E014        // Card Response Status 3
#define P_SDC_RSP_CMD               0xF0E018        // Card Response Command Index
#define P_SDC_RW_Ctrl               0xF0E01C        // Data Read/Write Control
#define P_SDC_TimeOut               0xF0E020        // Data TimeOut Period
#define P_SDC_LEN                   0xF0E024        // Data Length (Byes) to be Transferred
#define P_SDC_Status                0xF0E028        // Status Register
#define P_SDC_CLR                   0xF0E02C        // Clear Status Flag
#define P_SDC_INT_Mask              0xF0E030        // Interrupt Mask/Unmask
#define P_SDC_PWR_Ctrl              0xF0E034        // Power Control Register
#define P_SDC_CLK                   0xF0E038        // Clock Source Divider
#define P_SDC_BUS_Mode              0xF0E03C        // Bus Width Mode Select
#define P_SDC_Data                  0xF0E040        // Data Register
#define P_SDC_MMC_INT               0xF0E044        // MMC Interrupt Response Time (Default=42)
#define P_SDC_GPIO_Out              0xF0E048        // GPIO Data Output Control Register (4-Data Pins)

#define P_SDC_SDIO_Ctrl1            0xF0E06C        // SDIO Mode Control Register 1
#define P_SDC_SDIO_Ctrl2            0xF0E070        // SDIO Mode Control Register 2
#define P_SDC_SDIO_Status           0xF0E074        // SDIO Mode Status Register

#define P_SDC_FIFO_Ctrl             0xF0E09C        // FIFO Control

typedef struct{
    volatile U32 CMD;
    volatile U32 ARG;
    volatile U32 RSP_Status0;
    volatile U32 RSP_Status1;
    volatile U32 RSP_Status2;
    volatile U32 RSP_Status3;
    volatile U32 RSP_CMD;
    volatile U32 RW_Ctrl;
    volatile U32 TimeOut;
    volatile U32 LEN;
    volatile U32 Status;
    volatile U32 CLR;
    volatile U32 INT_Mask;
    volatile U32 PWR_Ctrl;
    volatile U32 CLK;
    volatile U32 BUS_Mode;
    volatile U32 Data;
    volatile U32 MMC_INT;
    volatile U32 GPIO_Out;
    volatile U32 Reserved1[8];
    volatile U32 SDIO_Ctrl1;
    volatile U32 SDIO_Ctrl2;
    volatile U32 SDIO_Status;
    volatile U32 Reserved2[9];
    volatile U32 FIFO_Ctrl;
}SDC_TypeDef;


//====================================================================
//  P_SDC_CMD (0xF0E000)
//  SDC Command Register   
//====================================================================
//  Bit[31:13] : Reserved
//  Bit[11] : MMC Controller Stop, Must be clear CMD_En First
#define C_SDC_MMC_Stop              (1<<11)
//  Bit[10] : SDC Controller Reset
#define C_SDC_Reset                 (1<<10)
//  Bit[9] : SDC Controller Command Enable
#define C_SDC_CMD_En                (1<<9)
#define C_SDC_CMD_Dis               (0<<9)
//  Bit[8] : Indicate Application Command
#define C_SDC_APP_CMD               (1<<8)
//  Bit[7] : Long/Short Reponse Selection
#define C_SDC_Rep_Long              (1<<7)          // 136-bit Reponse, R2
#define C_SDC_Rep_Short             (0<<7)          // 48-bit Reponse, R1
//  Bit[6] : Wait Reponse Enable/Disable
#define C_SDC_Rep_Wait_En           (1<<6)
#define C_SDC_Rep_Wait_Dis          (0<<6)
//  Bit[5:0] : Command Index
#define C_SDC_CMD_Index             0x3F            // Mapping Range of Bit


//====================================================================
//  P_SDC_ARG (0xF0E004)
//  Argument of Command Register
//====================================================================
//  Bit[31:0] : Argument of Command
#define C_SDC_CMD_Arg               0xFFFFFFFF      // Mapping Range of Bit


//====================================================================
//  P_SDC_RSP_Status0 (0xF0E008)
//  P_SDC_RSP_Status1 (0xF0E00C)
//  P_SDC_RSP_Status2 (0xF0E010)
//  P_SDC_RSP_Status3 (0xF0E014)
//  Card Response Information       ; Long Reponse 136-bit      ; Short Reponse 48-bit
//====================================================================
//  Bit[31:0] : Card Status Response
#define C_SDC_Card_Status           0xFFFFFFFF      // Mapping Range of Bit


//====================================================================
//  P_SDC_RSP_CMD (0xF0E018)
//  Card Response Command Index
//====================================================================
//  Bit[31:7] : Reserved
//  Bit[6] : Reponse Application Command
#define C_SDC_Rep_APP_CMD_Flag      (1<<6)
//  Bit[5:0] : Reponse Command Index
#define C_SDC_Rep_CMD_Index         0x3F


//====================================================================
//  P_SDC_RW_Ctrl (0xF0E01C)
//  Data Read/Write Control
//====================================================================
//  Bit[31:11] : Reserved
//  Bit[10] : FIFO Reset
#define C_SDC_FIFO_Reset            (1<<10)
//  Bit[9:8] : Reserved
//  Bit[7] : FIFO Threshlod(Level) Setting
#define C_SDC_FIFO_Level_Half       (1<<7)          // When FIFO Half Full/Empty, assert to Flag
#define C_SDC_FIFO_Level_Full       (0<<7)          // When FIFO Full/Empty, assert to Flag
//  Bit[6] : Data Transfer Enable
#define C_SDC_Data_En               (1<<6)
#define C_SDC_Data_Dis              (0<<6)
//  Bit[5] : Reserved
//  Bit[4] : Data Read/Write Start
#define C_SDC_Data_Write            (1<<4)
#define C_SDC_Data_Read             (0<<4)
//  Bit[3:0] : Data Size per Block (2^N-Bytes)
#define C_SDC_Block_Size            0xF
#define C_SDC_Block_Size_1          0x0
#define C_SDC_Block_Size_2          0x1
#define C_SDC_Block_Size_4          0x2
#define C_SDC_Block_Size_8          0x3
#define C_SDC_Block_Size_16         0x4
#define C_SDC_Block_Size_32         0x5
#define C_SDC_Block_Size_64         0x6
#define C_SDC_Block_Size_128        0x7
#define C_SDC_Block_Size_256        0x8
#define C_SDC_Block_Size_512        0x9
#define C_SDC_Block_Size_1024       0xA
#define C_SDC_Block_Size_2048       0xB
#define C_SDC_Block_Reserved1       0xC
#define C_SDC_Block_Reserved2       0xD
#define C_SDC_Block_Reserved3       0xE
#define C_SDC_Block_Reserved4       0xF


//====================================================================
//  P_SDC_TimeOut (0xF0E020)
//  Data TimeOut Period
//====================================================================
//  Bit[31:0] : Data Timeout Period
#define C_SDC_Timeout               0xFFFFFFFF      // Mapping Range of Bit


//====================================================================
//  P_SDC_LEN (0xF0E024)
//  Data Length (Byes) to be Transferred
//====================================================================
//  Bit[31:0] : Data Length (Byes), Maximum 128MB
#define C_SDC_Data_Len              0xFFFFFFFF      // Mapping Range of Bit


//====================================================================
//  P_SDC_Status (0xF0E028)
//  P_SDC_CLR (0xF0E02C)
//  P_SDC_INT_Mask (0xF0E030)
//  Status Control Register and Status Register
//====================================================================
//  Bit[31:18] : Reserved                           // Status with Function as below
//  Bit[17] : IO0 Status
#define C_SDC_IO0_Status                (1<<17)     // Mask
//  Bit[16] : SDIO Card Interrupt Status
#define C_SDC_SDIO_Card_INT_Status      (1<<16)     // Mask, Clear
//  Bit[15:13] : Reserved
//  Bit[12] : Card Write Protected Status
#define C_SDC_Card_WP_Status            (1<<12)
//  Bit[11] : Card Detect Status
#define C_SDC_Card_DT_Removed_Status    (1<<11)
#define C_SDC_Card_DT_Inserted_Status   (0<<11)
//  Bit[10] : Card Detect Status Change, (Removed or Inserted)
#define C_SDC_Card_DT_Change_Status     (1<<10)     // Mask, Clear
//  Bit[9] : FIFO Available to Read Status
#define C_SDC_FIFO_Ava_Read_Status      (1<<9)      // Mask
//  Bit[8] : FIFO Available to Write Status
#define C_SDC_FIFO_Ava_Write_Status     (1<<8)      // Mask
//  Bit[7] : Data Transfer Finished
#define C_SDC_Trans_Finish_Status       (1<<7)      // Mask, Clear
//  Bit[6] : Command Sent
#define C_SDC_CMD_Sent_Status           (1<<6)      // Mask, Clear
//  Bit[5] : Data Block Send/Received and CRC OK
#define C_SDC_Data_CRC_OK_Status        (1<<5)      // Mask, Clear
//  Bit[4] : Command Reponse and CRC OK
#define C_SDC_Rsp_CRC_OK_Status         (1<<4)      // Mask, Clear
//  Bit[3] : Data Read/Write Timeout
#define C_SDC_Data_Timeout_Status       (1<<3)      // Mask, Clear
//  Bit[2] : Command Reponse Timeout
#define C_SDC_Rsp_Timeout_Status        (1<<2)      // Mask, Clear
//  Bit[1] : Data Block Send/Received and CRC Fail
#define C_SDC_Data_CRC_Fail_Status      (1<<1)      // Mask, Clear
//  Bit[0] : Command Reponse and CRC Fail
#define C_SDC_Rsp_CRC_Fail_Status       (1<<0)      // Mask, Clear


//====================================================================
//  P_SDC_PWR_Ctrl (0xF0E034)
//  Power Control Register
//====================================================================
//  Bit[31:5] : Reserved
//  Bit[4] : External Power Supply On
#define C_SDC_EXT_PWR_On            (1<<4)
#define C_SDC_EXT_PWR_Off           (0<<4)
//  Bit[3:0] : External Power Supply Output Range (2.0V~3.6V)
#define C_SDC_EXT_PWR_Range         0xF             // Mapping Range of Bit


//====================================================================
//  P_SDC_CLK (0xF0E038)
//  Clock Source Divider
//====================================================================
//  Bit[31:9] : Reserved
//  Bit[8] : Clock Eanble/Disable
#define C_SDC_CLK_En                (0<<8)
#define C_SDC_CLK_Dis               (1<<8)
//  Bit[7] : SD Memory/MMC Memory Select
#define C_SDC_Card_SD               (1<<7)
#define C_SDC_Card_MMC              (0<<7)
//  Bit[6:0] : SDC Clock Divisor
#define C_SDC_CLK_Div               0x7F            // Mapping Range of Bit
#define C_SDC_Freq                  1000000         // *** User Can Change this Value
#define C_SDC_CLK_Div_Data          ((C_System_Clock / C_SDC_Freq / 2) -1)


//====================================================================
//  P_SDC_BUS_Mode (0xF0E03C)
//  Bus Width Mode Select
//====================================================================
//  Bit[31:6] : Reserved
//  Bit[5] : Card Detect through IO3/CD
#define C_SDC_CD_from_IO3           (1<<5)
#define C_SDC_CD_from_CD            (0<<5)
//  Bit[4:3] : Bus Width Select
#define C_SDC_Bus_Width             (3<<3)
#define C_SDC_Bus_Width_1IO         (0<<3)
#define C_SDC_Bus_Width_4IO         (1<<3)
#define C_SDC_Bus_Width_8IO         (2<<3)
//  Bit[2] : Bus Width 4 Enable
#define C_SDC_Bus_Width_4IO_En      (1<<2)
#define C_SDC_Bus_Width_4IO_Dis     (0<<2)
//  Bit[1] : Bus Width 8 Enable
#define C_SDC_Bus_Width_8IO_En      (1<<1)
#define C_SDC_Bus_Width_8IO_Dis     (0<<1)
//  Bit[0] : Bus Width 1 Enable
#define C_SDC_Bus_Width_1IO_En      (1<<0)
#define C_SDC_Bus_Width_1IO_Dis     (0<<0)


//====================================================================
//  P_SDC_Data (0xF0E040)
//  Data Register
//====================================================================
//  Bit[31:0] : Data
#define C_SDC_Data                  0xFFFFFFFF      // Mapping Range of Bit


//====================================================================
//  P_SDC_MMC_INT (0xF0E044)
//  MMC Interrupt Response Time (Default=42)
//====================================================================
//  Bit[31:8] : Reserved
//  Bit[7:0] : MMC Interrupt Reponse Time
#define C_SDC_MMC_INT_Time          0xFF            // Mapping Range of Bit


//====================================================================
//  P_SDC_GPIO_Out (0xF0E048)
//  GPIO Data Output Control Register (4-Data Pins)
//====================================================================
//  Bit[31:4] : Reserved
//  Bit[3:0] : Direct Drive IO3~IO0
#define C_SDC_IO3_Out               8
#define C_SDC_IO2_Out               4
#define C_SDC_IO1_Out               2
#define C_SDC_IO0_Out               1


//====================================================================
//  P_SDC_SDIO_Ctrl1 (0xF0E06C)
//  SDIO Mode Control Register 1
//====================================================================
//  Bit[31:15] : SDIO Total Block Number, Minimum 1 Maximum 131071
#define C_SDC_SDIO_Block_Num        0xFFFF          // Mapping Range of Bit
//  Bit[14] : SDIO Enable/Disable
#define C_SDC_SDIO_En               (1<<14)
#define C_SDC_SDIO_Dis              (0<<14)
//  Bit[13] : SDIO Read Wait Support
#define C_SDC_SDIO_Read_Wait_En     (1<<13)
#define C_SDC_SDIO_Read_Wait_Dis    (0<<13)
//  Bit[12] : SDIO Multi Block Transaction
#define C_SDC_SDIO_Multi_Block_En   (1<<12)
#define C_SDC_SDIO_Multi_Block_Dis  (0<<12)
//  Bit[11:0] : SDIO Data Size per Block (Bytes)
#define C_SDC_SDIO_Data_Size        0xFFF           // Mapping Range of Bit 


//====================================================================
//  P_SDC_SDIO_Ctrl2 (0xF0E070)
//  SDIO Mode Control Register 2
//====================================================================
//  Bit[31:2] : Reserved
//  Bit[1] : Abort Suspend Transaction
#define C_SDC_SDIO_Susp_CMD_Abort   (1<<1)
//  Bit[0] : Keep the Read Wait State, Set this bit before sending out the suspend command
#define C_SDC_SDIO_Susp_Read_Wait   (1<<0)


//====================================================================
//  P_SDC_SDIO_Status (0xF0E074)
//  SDIO Mode Status Register
//====================================================================
//  Bit[31:24] : Reserved
//  Bit[23:17] : The Number that the valid data(words) remained in the FIFO
#define C_SDC_SDIO_FIFO_Remain_Num  (0x7F<<17)      // Mapping Range of Bit
//  Bit[16:0] : The Number that the block remained in a data transfer
#define C_SDC_SDIO_FIFO_Block_Count 0x1FFFF         // Mapping Range of Bit


//====================================================================
//  P_SDC_FIFO_Ctrl (0xF0E09C)
//  FIFO Control
//====================================================================
//  Bit[31:9] : Reserved
//  Bit[8] : With CPRM Function
#define C_SDC_CPRM_Func             (1<<8)
//  Bit[7:0] : FIFO Depth
#define C_SDC_FIFO_Depth            (0xFF<<0)
#define C_SDC_FIFO_Depth_4          (0x04<<0)
#define C_SDC_FIFO_Depth_8          (0x08<<0)
#define C_SDC_FIFO_Depth_16         (0x10<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  IR_BASE (P_IR)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_IR_Ctrl                   0xF0F000        // Control Register

typedef struct{
    volatile U32 Ctrl;
}IR_TypeDef;


//====================================================================
//  P_IR_Ctrl (0xF0F000)
//  Control Register   
//  IR Frequency = System-Clock / Clock-Divisor / Count-Divisor / 2
//====================================================================
//  Bit[31:13] : Reserved
//  Bit[12:8] : IR Count Divisor Clock Source from IR Clock
#define C_IR_CNT_Div                (0x1F<<8)       // Mapping Range of Bit
#define C_IR_CNT_Div_2              (0x01<<8)
#define C_IR_CNT_Div_3              (0x02<<8)
#define C_IR_CNT_Div_4              (0x03<<8)
#define C_IR_CNT_Div_5              (0x04<<8)
#define C_IR_CNT_Div_6              (0x05<<8)
#define C_IR_CNT_Div_7              (0x06<<8)
#define C_IR_CNT_Div_8              (0x07<<8)
#define C_IR_CNT_Div_9              (0x08<<8)
#define C_IR_CNT_Div_10             (0x09<<8)
#define C_IR_CNT_Div_11             (0x0A<<8)
#define C_IR_CNT_Div_12             (0x0B<<8)
#define C_IR_CNT_Div_13             (0x0C<<8)
#define C_IR_CNT_Div_14             (0x0D<<8)
#define C_IR_CNT_Div_15             (0x0E<<8)
#define C_IR_CNT_Div_16             (0x0F<<8)
#define C_IR_CNT_Div_17             (0x10<<8)
#define C_IR_CNT_Div_18             (0x11<<8)
#define C_IR_CNT_Div_19             (0x12<<8)
#define C_IR_CNT_Div_20             (0x13<<8)
#define C_IR_CNT_Div_21             (0x14<<8)
#define C_IR_CNT_Div_22             (0x15<<8)
#define C_IR_CNT_Div_23             (0x16<<8)
#define C_IR_CNT_Div_24             (0x17<<8)
#define C_IR_CNT_Div_25             (0x18<<8)
#define C_IR_CNT_Div_26             (0x19<<8)
#define C_IR_CNT_Div_27             (0x1A<<8)
#define C_IR_CNT_Div_28             (0x1B<<8)
#define C_IR_CNT_Div_29             (0x1C<<8)
#define C_IR_CNT_Div_30             (0x1D<<8)
#define C_IR_CNT_Div_31             (0x1E<<8)
#define C_IR_CNT_Div_32             (0x1F<<8)
//  Bit[7:4] : IR Clock Divisor, Clock Source from System Clock
#define C_IR_CLK_Div                (0xF<<4)
#define C_IR_CLK_Div_1              (0x0<<4)
#define C_IR_CLK_Div_2              (0x1<<4)
#define C_IR_CLK_Div_3              (0x2<<4)
#define C_IR_CLK_Div_4              (0x3<<4)
#define C_IR_CLK_Div_5              (0x4<<4)
#define C_IR_CLK_Div_6              (0x5<<4)
#define C_IR_CLK_Div_7              (0x6<<4)
#define C_IR_CLK_Div_8              (0x7<<4)
#define C_IR_CLK_Div_9              (0x8<<4)
#define C_IR_CLK_Div_10             (0x9<<4)
#define C_IR_CLK_Div_11             (0xA<<4)
#define C_IR_CLK_Div_12             (0xB<<4)
#define C_IR_CLK_Div_13             (0xC<<4)
#define C_IR_CLK_Div_14             (0xD<<4)
#define C_IR_CLK_Div_15             (0xE<<4)
#define C_IR_CLK_Div_16             (0xF<<4)
//  Bit[3:2] : Reserved
//  Bit[1] : IR Idle Level
#define C_IR_Idle_High              (1<<1)
#define C_IR_Idle_Low               (0<<1)
//  Bit[0] : IR Run/Idle
#define C_IR_Run                    (1<<0)
#define C_IR_Idle                   (0<<0)





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  PWMA_BASE (P_PWMA)
//  PWMB_BASE (P_PWMB)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_PWMA_TMR_Ctrl             0xF11000        // Timer Control
#define P_PWMA_TMR_Data             0xF11004        // Data
#define P_PWMA_TMR_INT_Ctrl         0xF11008        // Interrupt Control
#define P_PWMA_TMR_Flag             0xF1100C        // Flag
#define P_PWMA0_Duty                0xF11010        // PWMA0 Duty
#define P_PWMA1_Duty                0xF11014        // PWMA1 Duty
#define P_PWMA2_Duty                0xF11018        // PWMA2 Duty
#define P_PWMA3_Duty                0xF1101C        // PWMA3 Duty

#define P_PWMA_CAP_Data             0xF11020        // Capture Mode, Data Register      (Timer shared from PWMA)
#define P_PWMA_CAP_INT_Ctrl         0xF11024        // Capture Mode, Interrupt Control  (Timer shared from PWMA)
#define P_PWMA_CAP_Flag             0xF11028        // Capture Mode, Flag               (Timer shared from PWMA)

#define P_PWMB_TMR_Ctrl             0xF11040        // Timer Control
#define P_PWMB_TMR_Data             0xF11044        // Data
#define P_PWMB_TMR_INT_Ctrl         0xF11048        // Interrupt Control
#define P_PWMB_TMR_Flag             0xF1104C        // Flag
#define P_PWMB0_Duty                0xF11050        // PWMB0 Duty
#define P_PWMB1_Duty                0xF11054        // PWMB1 Duty
#define P_PWMB2_Duty                0xF11058        // PWMB2 Duty
#define P_PWMB3_Duty                0xF1105C        // PWMB3 Duty

typedef struct{
    volatile U32 TMR_Ctrl;
    volatile U32 TMR_Data;
    volatile U32 TMR_INT_Ctrl;
    volatile U32 TMR_Flag;
    volatile U32 IO0_Duty;
    volatile U32 IO1_Duty;
    volatile U32 IO2_Duty;
    volatile U32 IO3_Duty;
    volatile U32 CAP_Data;          // Capture Function only for PWMA
    volatile U32 CAP_INT_Ctrl;      // Capture Function only for PWMA
    volatile U32 CAP_Flag;          // Capture Function only for PWMA
}PWM_TypeDef;


//====================================================================
//  P_PWMA_TMR_Ctrl (0xF11000)
//  P_PWMB_TMR_Ctrl (0xF11040)
//  Timer Control Register, Capture Function only for PWMA
//====================================================================
//  Bit[31:20] : Reserved
//  Bit[19:16] : Capture Pad Select of PortA[15:0], Only for PWMA and PortA
#define C_PWMA_CAP_Pad_Sel          (0xF<<16)
#define C_PWMA_CAP_Pad_Sel_PA0      (0x0<<16)
#define C_PWMA_CAP_Pad_Sel_PA1      (0x1<<16)
#define C_PWMA_CAP_Pad_Sel_PA2      (0x2<<16)
#define C_PWMA_CAP_Pad_Sel_PA3      (0x3<<16)
#define C_PWMA_CAP_Pad_Sel_PA4      (0x4<<16)
#define C_PWMA_CAP_Pad_Sel_PA5      (0x5<<16)
#define C_PWMA_CAP_Pad_Sel_PA6      (0x6<<16)
#define C_PWMA_CAP_Pad_Sel_PA7      (0x7<<16)
#define C_PWMA_CAP_Pad_Sel_PA8      (0x8<<16)
#define C_PWMA_CAP_Pad_Sel_PA9      (0x9<<16)
#define C_PWMA_CAP_Pad_Sel_PA10     (0xA<<16)
#define C_PWMA_CAP_Pad_Sel_PA11     (0xB<<16)
#define C_PWMA_CAP_Pad_Sel_PA12     (0xC<<16)
#define C_PWMA_CAP_Pad_Sel_PA13     (0xD<<16)
#define C_PWMA_CAP_Pad_Sel_PA14     (0xE<<16)
#define C_PWMA_CAP_Pad_Sel_PA15     (0xF<<16)
//  Bit[15:8] : PWM Dead Zone Cycle, Clock Source from PWM Clock
#define C_PWM_DeadZone_Cycles       (0xFF<<8)       // Mapping Range of Bit
//  Bit[7] : Reserved
//  Bit[6:4] : PWM Clock Divisor
#define C_PWM_CLK_Div               (7<<4)
#define C_PWM_CLK_Div_1             (7<<4)
#define C_PWM_CLK_Div_2             (6<<4)
#define C_PWM_CLK_Div_4             (5<<4)
#define C_PWM_CLK_Div_8             (4<<4)
#define C_PWM_CLK_Div_16            (3<<4)
#define C_PWM_CLK_Div_32            (2<<4)
#define C_PWM_CLK_Div_64            (1<<4)
#define C_PWM_CLK_Div_128           (0<<4)          // Default
//  Bit[3] : Capture Mode Enable/Disable, Only for PWMA
#define C_PWMA_CAP_En               (1<<3)
#define C_PWMA_CAP_Dis              (0<<3)
//  Bit[2] : PWMIO CH0/1 Complementary Mode Enable/Disable (CH0/1 With DeadZone Function)
#define C_PWM_CPLM_Mode_En          (1<<2)
#define C_PWM_CPLM_Mode_Dis         (0<<2)
//  Bit[1] : Data Reload Trigger, Write '1' to Reload Counter Data, Write only
#define C_PWM_Data_Reload_Start     (1<<1)
//  Bit[0] : PWM Function Enable/Disable
#define C_PWM_En                    (1<<0)
#define C_PWM_Dis                   (0<<0)


//====================================================================
//  P_PWMA_TMR_Data (0xF11004)
//  P_PWMB_TMR_Data (0xF11044)
//  Preload Data/Counter Port
//====================================================================
//  Bit[31:16] : Current Counter Value, Read only
#define C_PWM_Count                 (0xFFFF<<16)    // Mapping Range of Bit
//  Bit[15:0] : PWM Timer Data (Preload)
#define C_PWM_Data                  0xFFFF          // Mapping Range of Bit


//====================================================================
//  P_PWMA_TMR_INT_Ctrl (0xF11008)
//  P_PWMB_TMR_INT_Ctrl (0xF11048)
//  Interrupt Control
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : PWM Timer Underflow Interrupt Enable/Disable
#define C_PWM_TMR_IEn               1
#define C_PWM_TMR_IDis              0


//====================================================================
//  P_PWMA_TMR_Flag (0xF1100C)
//  P_PWMB_TMR_Flag (0xF1104C)
//  Flag
//====================================================================
//  Bit[31:1] : Reserved
//  Bit[0] : PWM Timer Underflow Flag
#define C_PWM_TMR_Flag              1


//====================================================================
//  P_PWMA0_Duty (0xF11010)
//  P_PWMA1_Duty (0xF11014)
//  P_PWMA2_Duty (0xF11018)
//  P_PWMA3_Duty (0xF1101C)
//  P_PWMB0_Duty (0xF11050)
//  P_PWMB1_Duty (0xF11054)
//  P_PWMB2_Duty (0xF11058)
//  P_PWMB3_Duty (0xF1105C)
//  PWM Output Duty
//====================================================================
//  Bit[31:16] : Duty Value
#define C_PWM_Duty                  (0xFFFF<<16)    // Mapping Range of Bit
//  Bit[15:2] : Reserved
//  Bit[1] : PWM Polarity Setting
#define C_PWM_Duty_Pol_High         (1<<1)          // Start '1'
#define C_PWM_Duty_Pol_Low          (0<<1)          // Start '0'
//  Bit[0] : PWM Polarity Setting
#define C_PWM_OUT_En                (1<<0)
#define C_PWM_OUT_Dis               (0<<0)


//====================================================================
//  P_CAP_Data (0xF11020)
//  Capture Mode, Data Register      (Timer shared from PWMA)
//====================================================================
//  Bit[31:16] : Capture Data that Rising Edge Latch
#define C_PWMA_CAP_Latch_Rising     (0xFFFF<<16)    // Mapping Range of Bit
//  Bit[15:0] : Capture Data that Falling Edge Latch
#define C_PWMA_CAP_Latch_Falling    (0xFFFF<<0)     // Mapping Range of Bit


//====================================================================
//  P_CAP_INT_Ctrl (0xF11024)
//  Capture Mode, Interrupt Control      (Timer shared from PWMA)
//====================================================================
//  Bit[31:6] : Reserved
//  Bit[5] : Capture Interuput Enable/Disable (Rising Edge Latch Occured)
#define C_PWMA_CAP_Rising_IEn       (1<<5)
#define C_PWMA_CAP_Rising_IDis      (0<<5)
//  Bit[4] : Capture Interuput Enable/Disable (Falling Edge Latch Occured)
#define C_PWMA_CAP_Falling_IEn      (1<<4)
#define C_PWMA_CAP_Falling_IDis     (0<<4)
//  Bit[3:0] : Reserved


//====================================================================
//  P_CAP_INT_Flag (0xF11028)
//  Capture Mode, Flag      (Timer shared from PWMA)
//====================================================================
//  Bit[31:6] : Reserved
//  Bit[5] : Capture Rising Edge Flag (Rising Edge Latch Occured)
#define C_PWMA_CAP_Rising_Flag      (1<<5)
//  Bit[4] : Capture Falling Edge Flag (Falling Edge Latch Occured)
#define C_PWMA_CAP_Falling_Flag     (1<<4)
//  Bit[3:0] : Reserved





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  ADC_BASE (P_ADC)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_ADC_Ctrl                  0xF14000        // Control Register
#define P_ADC_AGC_Ctrl              0xF14004        // AGC Control Register
#define P_ADC_FIFO_Ctrl             0xF14008        // FIFO Control
#define P_ADC_Flag                  0xF1400C        // Flag Register
#define P_ADC_Data_CH0              0xF14010        // Data Channel 0
#define P_ADC_Data_CH1              0xF14014        // Data Channel 1
#define P_ADC_Data_CH2              0xF14018        // Data Channel 2
#define P_ADC_Data_CH3              0xF1401C        // Data Channel 3
#define P_ADC_Data_CH4              0xF14020        // Data Channel 4
#define P_ADC_Data_CH5              0xF14024        // Data Channel 5
#define P_ADC_Data_CH6              0xF14028        // Data Channel 6
#define P_ADC_Data_CH7              0xF1402C        // Data Channel 7

typedef struct{
    volatile U32 Ctrl;
    volatile U32 AGC_Ctrl;
    volatile U32 FIFO_Ctrl;
    volatile U32 Flag;
    volatile U32 Data_CH0;
    volatile U32 Data_CH1;
    volatile U32 Data_CH2;
    volatile U32 Data_CH3;
    volatile U32 Data_CH4;
    volatile U32 Data_CH5;
    volatile U32 Data_CH6;
    volatile U32 Data_CH7;
}ADC_TypeDef;


//====================================================================
//  P_ADC_Ctrl (0xF14000)
//  12-bit ADC Control Register
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:14] : Sample Cycle Setting (Base on ADC Clock)
#define C_ADC_Sample_Cycle          (3<<14)
#define C_ADC_Sample_Cycle_8        (3<<14)
#define C_ADC_Sample_Cycle_16       (2<<14)
#define C_ADC_Sample_Cycle_32       (1<<14)
#define C_ADC_Sample_Cycle_64       (0<<14)
//  Bit[13] : Pre-Charge Sample&Hold Capacitor to 1/2 VDD
#define C_ADC_PreCharge_En          (1<<13)
#define C_ADC_PreCharge_Dis         (0<<13)
//  Bit[12] : ADC Enable/Disable
#define C_ADC_En                    (1<<12)
#define C_ADC_Dis                   (0<<12)
//  Bit[11] : Reserved
//  Bit[10] : ADC Convertion Trigger by Manual Mode (Software Trigger), Write Only
#define C_ADC_Manual_Start          (1<<10)
//  Bit[9:8] : Clock Divisor from System Clock
#define C_ADC_CLK_Div               (3<<8)
#define C_ADC_CLK_Div_8             (3<<8)
#define C_ADC_CLK_Div_16            (2<<8)
#define C_ADC_CLK_Div_32            (1<<8)
#define C_ADC_CLK_Div_64            (0<<8)
//  Bit[7:6] : Scan Mode Setting and Enable (Select Hardware Auto-Switch Channel Mode)
#define C_ADC_Scan_Mode             (3<<6)
#define C_ADC_Scan_Mode_CH3210      (3<<6)          // CH3 -> CH2 -> CH1 -> CH0/MIC
#define C_ADC_Scan_Mode_CH210       (2<<6)          // CH2 -> CH1 -> CH0/MIC
#define C_ADC_Scan_Mode_CH10        (1<<6)          // CH1 -> CH0/MIC
#define C_ADC_Single_Mode           (0<<6)
//  Bit[5:4] : ADC Trigger Source (Timer 0/1/2 or Manual)
#define C_ADC_Trig                  (3<<4)
#define C_ADC_Trig_Manual           (0<<4)
#define C_ADC_Trig_Timer0           (1<<4)
#define C_ADC_Trig_Timer1           (2<<4)
#define C_ADC_Trig_Timer2           (3<<4)
//  Bit[3] : Channel0 Input Source
#define C_ADC_CH0_From_MIC          (1<<3)
#define C_ADC_CH0_From_PA0          (0<<3)
//  Bit[2:0] : Line in Channel Select
#define C_ADC_CH                    (7<<0)
#define C_ADC_CH0                   (0<<0)          // CH0/MIC
#define C_ADC_CH1                   (1<<0)
#define C_ADC_CH2                   (2<<0)
#define C_ADC_CH3                   (3<<0)
#define C_ADC_CH4                   (4<<0)
#define C_ADC_CH5                   (5<<0)
#define C_ADC_CH6                   (6<<0)
#define C_ADC_CH7                   (7<<0)



//====================================================================
//  P_ADC_AGC_Ctrl (0xF14004)
//  AGC/PGA Control Register
//====================================================================
//  Bit[31:15] : Reserved
//  Bit[14:12] : AGC Clock Divisor from Lo-Clock(32768Hz), AGC_CLK = 32768Hz/(N+1)/2
#define C_ADC_AGC_CLK               (7<<12)         // Hz
#define C_ADC_AGC_CLK_16384         (0<<12)
#define C_ADC_AGC_CLK_8192          (1<<12)
#define C_ADC_AGC_CLK_5461          (2<<12)
#define C_ADC_AGC_CLK_4096          (3<<12)
#define C_ADC_AGC_CLK_3277          (4<<12)
#define C_ADC_AGC_CLK_2731          (5<<12)
#define C_ADC_AGC_CLK_2341          (6<<12)
#define C_ADC_AGC_CLK_2048          (7<<12)
//  Bit[11:9] : Reserved
//  Bit[8:4] : PGA Gain Control
#define C_ADC_PGA_Gain_Offset       4
#define C_ADC_PGA_Gain              (0x1F<<4)       // OPI=0ohm     ;OPI=5.1Kohm
#define C_ADC_PGA_Gain_0            (0x00<<4)       // 10dB         ;7.4dB
#define C_ADC_PGA_Gain_1            (0x01<<4)       // 11dB         ;7.8dB
#define C_ADC_PGA_Gain_2            (0x02<<4)       // 12dB         ;8.3dB
#define C_ADC_PGA_Gain_3            (0x03<<4)       // 13dB         ;8.7dB
#define C_ADC_PGA_Gain_4            (0x04<<4)       // 14dB         ;9.1dB
#define C_ADC_PGA_Gain_5            (0x05<<4)       // 15dB         ;9.5dB
#define C_ADC_PGA_Gain_6            (0x06<<4)       // 16dB         ;10dB
#define C_ADC_PGA_Gain_7            (0x07<<4)       // 17dB         ;10.4dB
#define C_ADC_PGA_Gain_8            (0x08<<4)       // 18dB         ;10.8dB
#define C_ADC_PGA_Gain_9            (0x09<<4)       // 19dB         ;11.3dB
#define C_ADC_PGA_Gain_10           (0x0A<<4)       // 20dB         ;11.7dB
#define C_ADC_PGA_Gain_11           (0x0B<<4)       // 21dB         ;12.1dB
#define C_ADC_PGA_Gain_12           (0x0C<<4)       // 22dB         ;12.5dB, Default
#define C_ADC_PGA_Gain_13           (0x0D<<4)       // 23dB         ;13dB
#define C_ADC_PGA_Gain_14           (0x0E<<4)       // 24dB         ;13.4dB
#define C_ADC_PGA_Gain_15           (0x0F<<4)       // 25dB         ;13.8dB
#define C_ADC_PGA_Gain_16           (0x10<<4)       // 26dB         ;14.3dB
#define C_ADC_PGA_Gain_17           (0x11<<4)       // 27dB         ;14.7dB
#define C_ADC_PGA_Gain_18           (0x12<<4)       // 28dB         ;15.1dB
#define C_ADC_PGA_Gain_19           (0x13<<4)       // 29dB         ;15.6dB
#define C_ADC_PGA_Gain_20           (0x14<<4)       // 30dB         ;16dB
#define C_ADC_PGA_Gain_21           (0x15<<4)       // 31dB         ;16.4dB
#define C_ADC_PGA_Gain_22           (0x16<<4)       // 32dB         ;16.8dB
#define C_ADC_PGA_Gain_23           (0x17<<4)       // 33dB         ;17.3dB
#define C_ADC_PGA_Gain_24           (0x18<<4)       // 34dB         ;17.7dB
#define C_ADC_PGA_Gain_25           (0x19<<4)       // 35dB         ;18.1dB
#define C_ADC_PGA_Gain_26           (0x1A<<4)       // 36dB         ;18.6dB
#define C_ADC_PGA_Gain_27           (0x1B<<4)       // 37dB         ;19dB
#define C_ADC_PGA_Gain_28           (0x1C<<4)       // 38dB         ;19.4dB
#define C_ADC_PGA_Gain_29           (0x1D<<4)       // 39dB         ;19.8dB
#define C_ADC_PGA_Gain_30           (0x1E<<4)       // 40dB         ;20.3dB
#define C_ADC_PGA_Gain_31           (0x1F<<4)       // 41dB         ;20.7dB
//  Bit[3] : AGC/PGA Enable/Disable
#define C_ADC_AGC_En                (1<<3)
#define C_ADC_PGA_En                (0<<3)
//  Bit[2] : Microphone Amplifier Gain Setting (First Stage Amplifier)
#define C_ADC_MIC_AMP_Gain_30dB     (1<<2)
#define C_ADC_MIC_AMP_Gain_15dB     (0<<2)          // Default
//  Bit[1] : Reserved
//  Bit[0] : Microphone Enable/Disable (V_MIC Enable/Disable)
#define C_ADC_MIC_En                (1<<0)
#define C_ADC_MIC_Dis               (0<<0)


//====================================================================
//  P_ADC_FIFO_Ctrl (0xF14008)
//  FIFO Control and Interruput Control Register
//====================================================================
//  Bit[31:10] : Reserved
//  Bit[9] : All Channel Data Store to FIFO or Channel Data Register
#define C_ADC_Data_Store_FIFO       (1<<9)
#define C_ADC_Data_Store_CH_Data    (0<<9)
//  Bit[8] : FIFO Reset
#define C_ADC_FIFO_Reset            (1<<8)          // Write only, write '1' to Clear FIFO
//  Bit[7] : Reserved
//  Bit[6:4] : ADC FIFO Level Setting
#define C_ADC_FIFO_Level_Offset     4
#define C_ADC_FIFO_Level            (7<<4)
#define C_ADC_FIFO_Level_0          (0<<4)
#define C_ADC_FIFO_Level_1          (1<<4)
#define C_ADC_FIFO_Level_2          (2<<4)
#define C_ADC_FIFO_Level_3          (3<<4)
#define C_ADC_FIFO_Level_4          (4<<4)          // Default
#define C_ADC_FIFO_Level_5          (5<<4)
#define C_ADC_FIFO_Level_6          (6<<4)
#define C_ADC_FIFO_Level_7          (7<<4)
//  Bit[3] : ADC Convert Finish Interrupt Enable/Disable
#define C_ADC_CNV_Done_IEn          (1<<3)
#define C_ADC_CNV_Done_IDis         (0<<3)
//  Bit[2] : FIFO Level Interrupt Enable/Disable
#define C_ADC_FIFO_Level_IEn        (1<<2)
#define C_ADC_FIFO_Level_IDis       (0<<2)
//  Bit[1] : FIFO Empty Interrupt Enable/Disable
#define C_ADC_FIFO_Empty_IEn        (1<<1)
#define C_ADC_FIFO_Empty_IDis       (0<<1)
//  Bit[0] : FIFO Full Interrupt Enable/Disable
#define C_ADC_FIFO_Full_IEn         (1<<0)
#define C_ADC_FIFO_Full_IDis        (0<<0)


//====================================================================
//  P_ADC_Flag (0xF1400C)
//  ADC and FIFO Flag
//====================================================================
//  Bit[31:5] : Reserved
//  Bit[4] : ADC Busy Flag
#define C_ADC_Busy_Flag             (1<<4)
//  Bit[3] : ADC Convert Finish Flag
#define C_ADC_CNV_Done_Flag         (1<<3)          // Write '1' to Clear Flag
//  Bit[2] : FIFO Level Flag
#define C_ADC_FIFO_Level_Flag       (1<<2)
//  Bit[1] : FIFO Empty Flag
#define C_ADC_FIFO_Empty_Flag       (1<<1)
//  Bit[0] : FIFO Full Flag
#define C_ADC_FIFO_Full_Flag        (1<<0)


//====================================================================
//  P_ADC_Data_CH0 (0xF14010)       // Share to FIFO Address
//  P_ADC_Data_CH1 (0xF14014)
//  P_ADC_Data_CH2 (0xF14018)
//  P_ADC_Data_CH3 (0xF1401C)
//  P_ADC_Data_CH4 (0xF14020)
//  P_ADC_Data_CH5 (0xF14024)
//  P_ADC_Data_CH6 (0xF14028)
//  P_ADC_Data_CH7 (0xF1402C)
//  12-bit ADC Data Register
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:4] : ADC Data
#define C_ADC_Data                  (0xFFF<<4)      // Mapping Range of Bit
//  Bit[3:0] : Reserved





//////////////////////////////////////////////////////////////////////////////////////
//====================================================================================
//  DAC_BASE (P_DAC)
//====================================================================================
//////////////////////////////////////////////////////////////////////////////////////
#define P_DAC_CH0_Ctrl              0xF15000        // Control Register
#define P_DAC_CH0_Data              0xF15004        // Data Register
#define P_DAC_CH0_FIFO              0xF15008        // FIFO Control
#define P_DAC_CH0_Flag              0xF1500C        // FIFO Flag

#define P_DAC_CH1_Ctrl              0xF15040        // Control Register
#define P_DAC_CH1_Data              0xF15044        // Data Register
#define P_DAC_CH1_FIFO              0xF15048        // FIFO Control
#define P_DAC_CH1_Flag              0xF1504C        // FIFO Flag

typedef struct
{
    volatile U32 CH_Ctrl;
    volatile U32 CH_Data;
    volatile U32 CH_FIFO;
    volatile U32 CH_Flag;
}CH_TypeDef;

typedef struct
{
    CH_TypeDef CH0;
    volatile U32 Reserved[12];
    CH_TypeDef CH1;
}DAC_TypeDef;


//====================================================================
//  P_DAC_CH0_Ctrl (0xF15000)
//  P_DAC_CH1_Ctrl (0xF15040)
//  14-bit DAC Control Register
//====================================================================
//  Bit[31:12] : Reserved
//  Bit[11:8] : DAC Gain Setting, *** Only Setting in Data-Ch0 Regsiter
#define C_DAC_PPGain_Offset         8
#define C_DAC_PPGain                (0xF<<8)
#define C_DAC_PPGain_0              (0x0<<8)
#define C_DAC_PPGain_1              (0x1<<8)
#define C_DAC_PPGain_2              (0x2<<8)
#define C_DAC_PPGain_3              (0x3<<8)
#define C_DAC_PPGain_4              (0x4<<8)
#define C_DAC_PPGain_5              (0x5<<8)
#define C_DAC_PPGain_6              (0x6<<8)
#define C_DAC_PPGain_7              (0x7<<8)
#define C_DAC_PPGain_8              (0x8<<8)
#define C_DAC_PPGain_9              (0x9<<8)
#define C_DAC_PPGain_10             (0xA<<8)
#define C_DAC_PPGain_11             (0xB<<8)
#define C_DAC_PPGain_12             (0xC<<8)
#define C_DAC_PPGain_13             (0xD<<8)
#define C_DAC_PPGain_14             (0xE<<8)
#define C_DAC_PPGain_15             (0xF<<8)
//  Bit[7] : Ch0/Ch1 Data Hardware Mixer Enable/Disable, *** Only Setting in Data-Ch1 Regsiter
#define C_DAC_Data_Mixer_En         (1<<7)
#define C_DAC_Data_Mixer_Dis        (0<<7)
//  Bit[6] : DAC Trigger by Timer0/1/2 Enable/Disable
#define C_DAC_Trig_Timer_En         (1<<6)
#define C_DAC_Trig_Timer_Dis        (0<<6)
//  Bit[5:4] : DAC Trigger by Timer0/1/2 or Manual
#define C_DAC_Trig                  (3<<4)
#define C_DAC_Trig_Manual           (0<<4)
#define C_DAC_Trig_Timer0           (1<<4)
#define C_DAC_Trig_Timer1           (2<<4)
#define C_DAC_Trig_Timer2           (3<<4)
//  Bit[3] : Sample Rate Up-Sample Enable/Disable
#define C_DAC_Up_Sample_En          (1<<3)
#define C_DAC_Up_Sample_Dis         (0<<3)
//  Bit[2] : External AUX Input Mixer
#define C_DAC_EXT_AUX_In_En         (1<<2)
#define C_DAC_EXT_AUX_In_Dis        (0<<2)
//  Bit[1] : Output Mode Select (Voltage DAC/PP)
#define C_DAC_Out_Mode              (1<<1)
#define C_DAC_Out_Mode_VolDAC       (1<<1)
#define C_DAC_Out_Mode_PushPull     (0<<1)
//  Bit[0] : DAC Enable/Disable
#define C_DAC_En                    (1<<0)
#define C_DAC_Dis                   (0<<0)


//====================================================================
//  P_DAC_CH0_Data (0xF15004)
//  P_DAC_CH1_Data (0xF15044)
//  14-bit DAC Data Register
//====================================================================
//  Bit[31:16] : Reserved
//  Bit[15:2] : DAC Data
#define C_DAC_Data                  (0x3FFF<<2)     // Mapping Range of Bit
//  Bit[1:0] : Reserved


//====================================================================
//  P_DAC_CH0_FIFO (0xF15008)
//  P_DAC_CH1_FIFO (0xF15048)
//  FIFO Control
//====================================================================
//  Bit[31:9] : Reserved
//  Bit[8] : FIFO Reset
#define C_DAC_FIFO_Reset            (1<<8)
//  Bit[7] : Reserved
//  Bit[6:4] : DAC FIFO Level Setting
#define C_DAC_FIFO_Level_Offset     4
#define C_DAC_FIFO_Level            (7<<4)
#define C_DAC_FIFO_Level_0          (0<<4)
#define C_DAC_FIFO_Level_1          (1<<4)
#define C_DAC_FIFO_Level_2          (2<<4)
#define C_DAC_FIFO_Level_3          (3<<4)
#define C_DAC_FIFO_Level_4          (4<<4)          // Default
#define C_DAC_FIFO_Level_5          (5<<4)
#define C_DAC_FIFO_Level_6          (6<<4)
#define C_DAC_FIFO_Level_7          (7<<4)
//  Bit[2] : FIFO Level Interrupt Enable/Disable
#define C_DAC_FIFO_Level_IEn        (1<<2)
#define C_DAC_FIFO_Level_IDis       (0<<2)
//  Bit[1] : FIFO Empty Interrupt Enable/Disable
#define C_DAC_FIFO_Empty_IEn        (1<<1)
#define C_DAC_FIFO_Empty_IDis       (0<<1)
//  Bit[0] : FIFO Full Interrupt Enable/Disable
#define C_DAC_FIFO_Full_IEn         (1<<0)
#define C_DAC_FIFO_Full_IDis        (0<<0)


//====================================================================
//  P_DAC_CH0_Flag (0xF1500C)
//  P_DAC_CH1_Flag (0xF1504C)
//  FIFO Flag
//====================================================================
//  Bit[31:3] : Reserved
//  Bit[2] : FIFO Level Flag
#define C_DAC_FIFO_Level_Flag       (1<<2)
//  Bit[1] : FIFO Empty Flag
#define C_DAC_FIFO_Empty_Flag       (1<<1)
//  Bit[0] : FIFO Full Flag
#define C_DAC_FIFO_Full_Flag        (1<<0)





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Interrupt Subroutine Define
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DAC_CH0_ISR                 HW0_ISR
#define DAC_CH1_ISR                 HW1_ISR
#define ADC_ISR                     HW2_ISR
#define TMR0_ISR                    HW4_ISR
#define TMR1_ISR                    HW5_ISR
#define TMR2_ISR                    HW6_ISR
#define GPIOA_ISR                   HW7_ISR
#define SPI0_ISR                    HW8_ISR
#define GPIOB_ISR                   HW9_ISR
#define RTC_ISR                     HW10_ISR
#define PWMA_ISR                    HW11_ISR
#define PWMB_ISR                    HW12_ISR
#define CAPTURE_ISR                 HW13_ISR
#define GPIOC_ISR                   HW16_ISR
#define I2C_ISR                     HW17_ISR
#define UART_ISR                    HW18_ISR
#define SPI1_ISR                    HW19_ISR
#define SDC_ISR                     HW20_ISR
#define SWI_ISR                     HW21_ISR
#define SWA_ISR                     HW22_ISR





#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
