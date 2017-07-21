//==========================================================================
// File Name          : pcm_dvr_example.c
// Description        : Example of PCM Module
// Version            : V0
// Last modified date : 2016/9/7
//==========================================================================
#include "nx1_lib.h"

#if _PCM_PLAY&&_PCM_RECORD&&(!_UNIFIED_API)
int main(void)
{ 
    U8 volume=0;
    U8 key;
    
    SYS_Init();

    while(1)
    {
        key=DIRECTKEY_GetKey();
        switch (key) 
        {
            case 0x01:
                PCM_Play(0x80000|ADDRESS_MODE,SPI_MODE);
                break;
                
            case 0x02:
                PCM_Pause();
                break;
                
            case 0x04:
                PCM_Resume();
                break; 
                    
            case 0x08:
                PCM_Stop();
                break;   
                    
            case 0x10:
                volume++;
                if(volume>CH_VOL_15)
                {
                    volume=CH_VOL_0;    
                }    
                PCM_SetVolume(volume);
                break;  
                  
            case 0x20:
                SPI_BurstErase(0x80000,0x30000);
                PCM_Record(0x80000|ADDRESS_MODE,SPI_MODE,SAMPLE_RATE_16000,0x30000);
                break;   
                
            case 0x40: 

                break; 
                  
            case 0x80:               

                break;                                                     
                             
        }
        PCM_ServiceLoop();
        SYS_ServiceLoop();
    }    
}
#endif
