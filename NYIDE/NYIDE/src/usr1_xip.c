#include "nx1_lib.h"
#include "nx1_gpio.h"
#include "nx1_rtc.h"
#include "nx1_smu.h"

extern void PoweronKeyScan(void);
extern void SYS_Standby(void);


//static U8 table[8]={1,2,4,8,0x10,0x20,0x40,0x80};
////static U32 dfata=9;
////static U8 table[4]={1,2,4,8};
//U8 hello_xip2(void)
//{ 
//    static int a=0;
//        
//    a++;
//    if(a==8) a=0;
//    return table[a];
//}

//User Var

volatile U16 R_Test1KHz=0;        //For PA15 Ouput 1KHz Tone
volatile U8  R_T500us=0;          //For 1KHz baseon 250us 

volatile U8  R_ScanLine=0;        //For Control scan Matrix   
volatile U8  R_ScanEnter=0;       //For scan key in main loop
volatile U16 R_Debouncecnt=0;     //For Keydebounce     
volatile U16 R_Dec_bak = 0;

volatile U8  R_KeyValue=0;        //For Key Value  
volatile U8  R_KeyData=0;         //For KeyIO input data 
volatile U8  Mute_Flag = 0;
volatile U32 Mute_time = 0;


volatile U8 LoopPlayFlag = 0;
volatile U8 LoopPlayCnt = 0;
volatile U8 LoopPlayIndex = 0;
volatile U8 LoopFlag = 0;
volatile U8 Mkey = 0;
volatile U8 key = 0;
volatile U8 key_bak = 0;
volatile U8 OneSecFlag = 0;
volatile U32 OneSecTime = 0;
volatile U8 IntervalFlag = 0;
volatile U8 OneSecFlag2 = 0;
volatile U32 OneSecTime2 = 0;
volatile U8 IntervalFlag2 = 0;

volatile U8 PlayStateBak = 0;
volatile U8 PlayState = 0;
volatile U8 PlayCheckFlag = 0;
volatile U8 B_no_Song_index = 0;


const U8 Sp_up[3] = {54, 55, 57};
const U8 Suc_P[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const U8 Suc_PP[14] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const U8 Suc_PP2[14] = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
const U8 Suc_PP3[17] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

const U8 SP_P[3] = {28, 33, 29};
const U8 SD_P[3] = {30, 31, 32};
const U8 SB_P[14] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46}; 
const U8 SV_P[14][6] = {{33, 33, 33, 33, 33, 33}, {34, 34, 34, 34, 34, 34}, {35, 35, 35, 35, 35, 35},\
	                    {36, 36, 36, 36, 36, 36}, {37, 37, 37, 37, 37, 37}, {38, 38, 38, 38, 38, 38},\
	                    {39, 39, 40, 39, 39, 40}, {41, 41, 41, 41, 41, 41}, {42, 43, 42, 43, 42, 43},\
	                    {44, 44, 44, 44, 44, 44}, {45, 46, 45, 46, 45, 46}, {47, 47, 47, 47, 47, 47},\
	                    {48, 48, 48, 48, 48, 48}, {56, 56, 56, 56, 56, 56}};

const U8 SP_B[17] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 56};
const S8 Spd[3] = {SPEED_P0, SPEED_P3, SPEED_P7};

const U8 B_Key_NO_Song[17][2] = {{33, 34}, {34, 35}, {35, 36},  {36, 37}, {37, 38}, {38, 39}, {39, 40}, {40, 41}, {41, 42}, {42, 43},\
	                      {43, 44}, {44, 45}, {45, 46}, {46, 47}, {47, 48}, {48, 49}, {49, 33}};


const U8 Vol_T[3] = {CH_VOL_4, CH_VOL_10, CH_VOL_15};

const U8 Rng_tb[27][3] = {{2, 0, 2}, {2, 2, 1}, {2, 1, 1}, {0, 0, 2}, {1, 0, 0}, {0, 2, 2}, {0, 0, 1},\
	                      {2, 0, 0}, {2, 2, 2}, {0, 1, 2}, {2, 2, 0}, {1, 0, 2}, {0, 1, 0}, {0, 0, 2},\
	                      {1, 0, 1}, {1, 2, 0}, {2, 1, 2}, {1, 2, 1}, {1, 2, 2}, {0, 0, 0}, {0, 2, 1},\
	                      {0, 1, 1}, {1, 1, 2}, {0, 2, 0}, {2, 1, 0}, {1, 1, 0}, {1, 1, 1}};

U8 (*audi_Play)(U32 index, U8 mode);
void (*audi_Stop)(void);
U8 (*audi_GetStatus)(void);
const U8 *Lp;

U8 (*lp_audi_Play)(U32 index, U8 mode);
void (*lp_audi_Stop)(void);
U8 (*lp_audi_GetStatus)(void);




void XIP_stop_midi(void)
{
	 MIDI_Stop();
     while(MIDI_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for MIDI
     AUDIO_ServiceLoop();
}

void XIP_stop_sbc(void)
{
	SBC_Stop();
    while(SBC_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for SBC
    AUDIO_ServiceLoop();
}

void XIP_stop_pcm(void)
{
	PCM_Stop();
    while(PCM_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for SBC
    AUDIO_ServiceLoop();
}

void XIP_stop_pcm2(void)
{
	PCM_CH2_Stop();
    while(PCM_CH2_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for SBC
    AUDIO_ServiceLoop();
}

//void XIP_stop_pcm3(void)
//{
//	PCM_CH3_Stop();
//    while(PCM_CH3_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for SBC
//    AUDIO_ServiceLoop();
//}

void XIP_set_vol(U8 vol)
{
	MIDI_SetVolume(vol);
    SBC_SetVolume(vol);
	////SBC_CH2_SetVolume(vol);
    PCM_SetVolume(vol);
    PCM_CH2_SetVolume(vol);
//    PCM_CH3_SetVolume(vol);
}

U8 XIP_GetPlayStatus(void)
{
	if ((MIDI_GetStatus() != STATUS_STOP) || (SBC_GetStatus() != STATUS_STOP)\
	||  (PCM_GetStatus() != STATUS_STOP) || (PCM_CH2_GetStatus() != STATUS_STOP)\
	/*||   (PCM_CH3_GetStatus() != STATUS_STOP)*/) {
		return 1;
	} else {
		return 0;
	}
}

void XIP_Standby(void)
{
	if ((XIP_GetPlayStatus() == STATUS_STOP)\
	    && (R_Debouncecnt==0) && (key==0) && (R_Test1KHz==0) && (LoopPlayFlag == 0)\
	    && (Mute_Flag == 0) && (PlayCheckFlag == 0) && (OneSecFlag == 0) && (OneSecFlag2 == 0)) {
		
        
		////AUDIO_Stop(0);
		////AUDIO_Stop(1);
		////AUDIO_Stop(2);
	   
		//No Audio Playing&& No Key&& No 1KHz ouput
		GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(0xB9);                //Spi flash Deep_Powerdown
        GPIO_Write(GPIOB,0,1);
		
        //GPIOA->Wakeup_Mask |= 0X007E; ;   //PA6~PA1 Wakeup Enable 1,have wakeup,0,no wakeup
           
        P_PORTA->DIR =0x000078FF;         //PA15~PA8 Output Low  1,input,0,output
        P_PORTA->Data=0x000078FF;         //Data output
        //////DelayLoop(100);
           
        P_PORTA->Data=P_PORTA->PAD;
        P_PORTB->Data=P_PORTB->PAD;
        P_PORTC->Data=P_PORTC->PAD;
        RTC_IntCmd(RTC_4KHZ,DISABLE);
        WDT_Clear();
		SMU_LDO33Cmd(DISABLE);
		
        SMU_Sleep(SMU_HALT_MODE);
        //Wakeup 
        ////SYS_Init2();  
		////SYS_GPIOInit();//will reset key debounce 
		SMU_LDO33Cmd(ENABLE);

		GPIO_Write(GPIOB,0,0);
        SPI_SendCmd(0xAB);                //Spi flash Release Deep_Powerdown
        GPIO_Write(GPIOB,0,1);
        
		DIRECTKEY_Init();
		
		
		
        RTC_IntCmd(RTC_4KHZ,ENABLE);      //RTC Enable
	}
}

void StopLoopPlay(void)
{
	XIP_stop_midi();
	XIP_stop_sbc();
	XIP_stop_pcm();
	XIP_stop_pcm2();
//	XIP_stop_pcm3();

	LoopPlayFlag = 0;
	LoopPlayCnt = 0;
	LoopPlayIndex = 0;
	LoopFlag = 0;
	Mute_Flag = 0;
    Mute_time = 0;
}

void StopLoopPlay2(void)
{

	XIP_stop_pcm2();
//	XIP_stop_pcm3();
	
	LoopPlayFlag = 0;
	LoopPlayCnt = 0;
	LoopPlayIndex = 0;
	LoopFlag = 0;
	Mute_Flag = 0;
    Mute_time = 0;
}


void StartLoopPlay(U8 idx, U8 Cnt)
{ 
	LoopPlayFlag = 1;
	LoopPlayCnt = Cnt;
	Mute_Flag = 0;
		
	LoopPlayIndex = idx;
	Mkey = LoopPlayIndex;
	lp_audi_Play(Lp[LoopPlayIndex], SPI_MODE);
	while(lp_audi_GetStatus() == STATUS_STOP)  AUDIO_ServiceLoop();
}

void DealMutePlay(void)
{
	if (Mute_Flag == 1) {
		if ((SysTick - Mute_time) >= 800) {
			Mute_Flag = 0;
			lp_audi_Play(Lp[LoopPlayIndex], SPI_MODE);
			while(lp_audi_GetStatus() == STATUS_STOP)  AUDIO_ServiceLoop();
		}
	}
}

void DealLoopPlay(void)
{
	if (LoopPlayFlag == 1) {
		if ((lp_audi_GetStatus() == STATUS_STOP) && (Mute_Flag == 0)) {
			LoopPlayCnt--;
			LoopPlayIndex++;
		    Mkey++;
			if (LoopPlayCnt == 0) {
				LoopPlayFlag = 0;
			} else {
			    if (Mute_Flag == 0) {
					Mute_Flag = 1;
					Mute_time = SysTick;
			    } 
				////audi_Play(Lp[LoopPlayIndex], SPI_MODE);
				////while(audi_GetStatus() == STATUS_STOP)  AUDIO_ServiceLoop();
			}
		}
	}
}

void Disable1Counter(void)
{
	OneSecFlag = 0;
	OneSecTime = 0;
	IntervalFlag = 0;
}

void Disable2Counter(void)
{
	OneSecFlag2 = 0;
	OneSecTime2 = 0;
	IntervalFlag2 = 0;
}

void Start1Counter(void)
{
    OneSecFlag = 1;
	OneSecTime = SysTick;
	IntervalFlag = 0;
}

void Start2Counter(void)
{
    OneSecFlag2 = 1;
	OneSecTime2 = SysTick;
	IntervalFlag2 = 0;
}


void DealLoop1Cnt(void)
{
	if (OneSecFlag == 1) {
		if ((SysTick - OneSecTime) > 1000) {
			OneSecFlag = 0;
			IntervalFlag = 1;
		}
	}
}

void DealLoop2Cnt(void)
{
	if (OneSecFlag2 == 1) {
		if ((SysTick - OneSecTime2) > 1000) {
			OneSecFlag2 = 0;
			IntervalFlag2 = 1;
		}
	}
}

void DealSbc_Midi(void)
{
    if (PlayCheckFlag == 1) {
    	if ((MIDI_GetStatus() == STATUS_PLAYING)  || (SBC_GetStatus() == STATUS_PLAYING)) {
			PlayState = STATUS_PLAYING;
			Disable2Counter();
		    PlayStateBak = STATUS_PLAYING;
    	} else {
    		PlayState = STATUS_STOP;
			if (PlayStateBak != PlayState) {
				PlayStateBak = PlayState;
				Start2Counter();
			}

			if (IntervalFlag2 == 1) {
				Disable2Counter();
				PlayCheckFlag = 0;
			    IntervalFlag = 0;
				OneSecFlag = 0;
				B_no_Song_index = 0;
			}
    	}
    }
}


void XIP_main(void)
{ 
    U8 vol=0;
    
    U8 song_idx=0;
    U8 mod =0;
	U8 SpCnt = 0;
	U8 B_Cnt = 0;
	U8 AC_Key = 0;
	U8 R32 = 0;
	U8 R33 = 0;
	U8 RR = 0;
    PoweronKeyScan();
	audi_Play = 0;
	audi_Stop = 0;
	audi_GetStatus = 0;
	SPI_SendCmd(0xAB);            //Spi flash Release Deep_Powerdown
	GPIOA->Wakeup_Mask |= 0X70FE;   //PA12 PA13 PA14 PA7~PA1 Wakeup Enable 1,have wakeup,0,no wakeup
	vol = 1;

	XIP_set_vol(Vol_T[vol]);
  
    while(1)
    {
        KeyScan();
        key=Matrix_GetKey();
		
		if((key>=1)&&(key<=6)){
		////XIP_stop_midi();
		////XIP_stop_sbc();
		XIP_stop_pcm();
		////XIP_stop_pcm2();
		////XIP_stop_pcm3();
		audi_Play = PCM_CH2_Play; 
		audi_Stop = XIP_stop_pcm2;
        audi_GetStatus = PCM_CH2_GetStatus;
		audi_Stop();
		////StopLoopPlay();
        switch (key) 
            {
            case 1:
				if (vol >= 2) {
					vol = 2;
				} else {
					vol++;
					audi_Play(51, SPI_MODE);
				}
                XIP_set_vol(Vol_T[vol]);
            	break;

			case 2:
				if (vol <= 0) {
					vol = 0;
				} else {
					vol--;
					audi_Play(51, SPI_MODE);
				}
				XIP_set_vol(Vol_T[vol]);
            	break;

			case 3:
				if (MIDI_GetStatus() == STATUS_PLAYING) {
					XIP_stop_midi();
					
					////SBC_Play(Suc_P[Mkey], SPI_MODE);
				} else if (SBC_GetStatus() == STATUS_PLAYING) {
				    XIP_stop_sbc();
					////audi_Play(52, SPI_MODE);
				}
				if (LoopPlayFlag == 1) {
				    StopLoopPlay();
				}
				audi_Play(52, SPI_MODE);
				mod = 0;
				break;

			case 4:
				if (SBC_GetStatus() == STATUS_PLAYING) {
					XIP_stop_sbc();
					////MIDI_Play(Suc_P[Mkey] + 14, SPI_MODE);
				} else if (MIDI_GetStatus() == STATUS_PLAYING) {
				    XIP_stop_midi();
					////audi_Play(53, SPI_MODE);
				}
				if (LoopPlayFlag == 1) {
				    StopLoopPlay();
				}
				audi_Play(53, SPI_MODE);
				mod = 1;
				break;

			case 5:
				if (LoopPlayFlag == 1) {
					StopLoopPlay();
					audi_Play(Sp_up[2], SPI_MODE);
				} else {
				    if ((MIDI_GetStatus() == STATUS_STOP) && (SBC_GetStatus() == STATUS_STOP)) {
					    audi_Play(Sp_up[2], SPI_MODE);
				    } else {
					    audi_Play(Sp_up[SpCnt], SPI_MODE);
					    SpCnt++;
					    if (SpCnt >= 3) {
						    SpCnt = 0;
					    }
					    SBC_SetSpeed(Spd[SpCnt]);
					    MIDI_SetSpeedRatio(Spd[SpCnt]);
				    }
				}
				break;

			case 6:
				if (LoopPlayFlag == 0) {
				    ////if (mod == 0) {
				        lp_audi_Play = SBC_Play; 
				        lp_audi_Stop = XIP_stop_sbc;
        		        lp_audi_GetStatus = SBC_GetStatus;
					    Lp = Suc_PP;
				    ////} else { 
					    ////lp_audi_Play = MIDI_Play; 
				        ////lp_audi_Stop = XIP_stop_midi;
        		        ////lp_audi_GetStatus = MIDI_GetStatus;
					    ////Lp = Suc_PP2;
				    ////}
					LoopFlag = 1;
					B_Cnt = 0;
				    StopLoopPlay();

					if ((key_bak == key) && (lp_audi_GetStatus() == STATUS_PLAYING)) {
					    LoopFlag = 1;
				        StopLoopPlay();
					} else {
				        StartLoopPlay(0, 14);
					}
				}else {
        			////if (key_bak == key) {
						StopLoopPlay();
        			////}
				}
				
				
				break;
            default:   
                break;                                                                 
            }
            ////key_bak = key;
		    ////Disable1Counter();
        }else if ((key>=11)&&(key<=24)) {
            StopLoopPlay2();
			
            if (mod == 0) {
				XIP_stop_midi();
				XIP_stop_pcm();
				XIP_stop_pcm2();
//				XIP_stop_pcm3();
				if ((SBC_GetStatus() == STATUS_PLAYING) && ((Mkey + 11) == key)) {
					XIP_stop_sbc();
					PlayCheckFlag = 0;
					Disable2Counter();
				} else {
					XIP_set_vol(Vol_T[vol]);
			 	    SBC_Play(Suc_P[key-11],SPI_MODE);
					PlayCheckFlag = 1;
					Disable2Counter();
				}
            } else {
			 	XIP_stop_sbc();
				XIP_stop_pcm();
				XIP_stop_pcm2();
//				XIP_stop_pcm3();
				if ((MIDI_GetStatus() == STATUS_PLAYING) && ((Mkey + 11) == key)) {
					XIP_stop_midi();
					PlayCheckFlag = 0;
					Disable2Counter();
				} else {
					XIP_set_vol(Vol_T[vol]);
                	MIDI_Play(Suc_P[key-11] + 14,SPI_MODE);
					PlayCheckFlag = 1;
					Disable2Counter();
				}
             	
            }
			
			Mkey = key - 11;
            ////key_bak = key;
			SpCnt = 0;
			B_Cnt = 0;
        } else if ((key>=31)&&(key<=34)) {
        	audi_Play = PCM_CH2_Play;
			audi_Stop = XIP_stop_pcm2;
			audi_GetStatus = PCM_CH2_GetStatus;
			audi_Stop();
			switch(key) {
			case 34:
				if ((LoopPlayFlag == 1) && (LoopFlag == 2)) {
					StopLoopPlay2();
				}
				R33++;
				R33 %= 3;
				if (R33 == 0) {
					RR = R32;
				}
			 	audi_Play(SD_P[Rng_tb[RR][R33]], SPI_MODE);
				/*
				SpCnt++;
			 	if (SpCnt >= 3) {
					SpCnt = 0;
			 	}
			 	*/
				Disable1Counter();
			 	break;
			case 32:
			 	if (LoopPlayFlag == 0) {
					if (PlayCheckFlag == 1) {
						audi_Play(SV_P[Mkey][B_Cnt], SPI_MODE);
						B_Cnt++;
					    B_Cnt %= 6;
						Disable2Counter();
						Start2Counter();
					} else {
						
				        if (IntervalFlag == 1) {
						    ////B_no_Song_index = 0;
							
							B_no_Song_index++;
							if (B_no_Song_index > 16) {
							    B_no_Song_index = 0;
							}
							
						} else {
						    ////B_no_Song_index = B_no_Song_index;
						    ////B_no_Song_index++;
						    ////if (B_no_Song_index > 16) {
							////B_no_Song_index = 0;
							////}

							////B_no_Song_index = 0;
						}
						audi_Play(SP_B[B_no_Song_index], SPI_MODE);
						////if (OneSecFlag == 0)
						Disable1Counter();
						Start1Counter();

						
				    }
			 	} else {
			 	    audi_Play(SV_P[Mkey][B_Cnt], SPI_MODE);
					B_Cnt++;
					B_Cnt %= 6;
			 	}
			 	break;
			case 31:
			case 33:
				AC_Key = (GPIO_ReadPort(GPIOA) >> 8);
				AC_Key ^= 0x50;
			    AC_Key &= 0x50;
				if (AC_Key == 0x50) {
					if ((LoopPlayFlag == 1) && (LoopFlag == 2)) {
						StopLoopPlay2();
					}
					R33++;
					R33 %= 3;
					if (R33 == 0) {
						RR = R32;
					}
			 		audi_Play(SD_P[Rng_tb[RR][R33]], SPI_MODE);
			 		
					/*
					SpCnt++;
			 		if (SpCnt >= 3) {
						SpCnt = 0;
			 		}
			 		*/
					Disable1Counter();
				} else {
				    if ((LoopPlayFlag == 1) && (LoopFlag == 2)) {
					    StopLoopPlay2();
				    }
			 	    audi_Play(SP_P[key-31], SPI_MODE);
				}
				////Disable1Counter();
			 	break;
			default:
			 	break;
			}
            ////key_bak = key;
        }

		key_bak = key;

		DealLoopPlay();
		DealMutePlay();
		DealLoop1Cnt();
		DealLoop2Cnt();
		DealSbc_Midi();
		R32++;
		R32 %= 27;
		AUDIO_ServiceLoop();
        SYS_ServiceLoop();
		
        SYS_Standby();
		/*
        switch (key) 
        {
            case 0x01:
                song_idx ^=0x01;
                if(song_idx)    
                {
                	SBC_Stop();
                    while(SBC_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for SBC
                    AUDIO_ServiceLoop();
                	MIDI_Play(0,SPI_MODE);
                }
                else
                {
                	MIDI_Stop();
                    while(MIDI_GetStatus()==STATUS_PLAYING)  AUDIO_ServiceLoop(); //wait for MIDI
                    AUDIO_ServiceLoop();
               		SBC_Play(1,SPI_MODE);
               	}
                break;
                
            case 0x02:
                if(song_idx)    MIDI_Stop();
                else    SBC_Stop();
                break;
                
            case 0x04:
                PCM_Play(2,SPI_MODE);
                break; 
                    
            case 0x08:
                PCM_CH2_Play(3,SPI_MODE);
                break;   
                    
            case 0x10:
                PCM_CH3_Play(4,SPI_MODE);
                break;  
                  
            case 0x20:


                break;   
            case 0x40:                              
                break; 
                  
            case 0x80:
                break;                                                     
                             
        }
        AUDIO_ServiceLoop();
        SYS_ServiceLoop();
        */
    }    
}
