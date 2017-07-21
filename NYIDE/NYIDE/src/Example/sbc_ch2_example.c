//==========================================================================
// File Name          : sbc_ch2_example.c
// Description        : Example of SBC Module
// Version            : V0
// Last modified date : 2016/9/7
//==========================================================================
#include "nx1_lib.h"

#if _SBC_CH2_PLAY&&(_SBC_PLAY)&&(!_UNIFIED_API)
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
                SBC_CH2_Play(0,SPI_MODE);
                break;
                
            case 0x02:
                SBC_CH2_Pause();
                break;
                
            case 0x04:
                SBC_CH2_Resume();
                break; 
                    
            case 0x08:
                SBC_CH2_Stop();
                break;   
                    
            case 0x10:
                volume++;
                if(volume>CH_VOL_15)
                {
                    volume=CH_VOL_0;    
                }    
                SBC_CH2_SetVolume(volume);
                break;  
                  
            case 0x20:
                SBC_Play(1,SPI_MODE);
                break;   
            case 0x40:               

                break; 
                  
            case 0x80:               

                break;                                                     
                             
        }
        SBC_ServiceLoop();
        SBC_CH2_ServiceLoop();
        SYS_ServiceLoop();
    }    
}
#endif
