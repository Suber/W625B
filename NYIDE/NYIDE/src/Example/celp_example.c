//==========================================================================
// File Name          : celp_example.c
// Description        : Example of CELP Module
// Version            : V0
// Last modified date : 2016/9/7
//==========================================================================
#include "nx1_lib.h"

#if _CELP_PLAY&&(!_UNIFIED_API)
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
                CELP_Play(0,SPI_MODE);
                break;
                
            case 0x02:
                CELP_Pause();
                break;
                
            case 0x04:
                CELP_Resume();
                break; 
                    
            case 0x08:
                CELP_Stop();
                break;   
                    
            case 0x10:
                volume++;
                if(volume>CH_VOL_15)
                {
                    volume=CH_VOL_0;    
                }    
                CELP_SetVolume(volume);
                break;  
                  
            case 0x20:

                break;   
            case 0x40:               
                break; 
                  
            case 0x80:               
                break;                                                     
                             
        }
        CELP_ServiceLoop();
        SYS_ServiceLoop();
    }    
}
#endif
