//==========================================================================
// File Name          : adpcm_dvr_example.c
// Description        : Example of ADPCM Module
// Version            : V0
// Last modified date : 2016/9/7
//==========================================================================
#include "nx1_lib.h"

#if _ADPCM_PLAY&&_ADPCM_RECORD&&(!_UNIFIED_API)
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
                ADPCM_Play(0x80000|ADDRESS_MODE,SPI_MODE);
                break;
                
            case 0x02:
                ADPCM_Pause();
                break;
                
            case 0x04:
                ADPCM_Resume();
                break; 
                    
            case 0x08:
                ADPCM_Stop();
                break;   
                    
            case 0x10:
                volume++;
                if(volume>CH_VOL_15)
                {
                    volume=CH_VOL_0;    
                }    
                ADPCM_SetVolume(volume);
                break;  
                  
            case 0x20:
                SPI_BurstErase(0x80000,0x30000);
                ADPCM_Record(0x80000|ADDRESS_MODE,SPI_MODE,SAMPLE_RATE_8000,0x30000);
                break;   
            case 0x40:               

                break; 
                  
            case 0x80:               

                break;                                                     
                             
        }
        ADPCM_ServiceLoop();
        SYS_ServiceLoop();
    }    
}
#endif
