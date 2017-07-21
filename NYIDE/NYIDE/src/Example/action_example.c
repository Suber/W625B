#include "nx1_lib.h"
#include "IoTab.h"

int main(void)
{ 
    U8 key;
    U8 ch=0;
    U8 ch1=1,ch2=2;
    
    
    SYS_Init();
          
    while(1)
    {
        key=DIRECTKEY_GetKey();      
        switch (key) 
        {
            case 0x01:
                //Action1 play out from PC.0.
                //IOActionList is from Table/IoTab.c.
                //IOActionList[0] is Action1.
                ACTION_PlayIO(ch,IOActionList[0],ACTION_PORTC|0x0,ACTION_REPEAT,ACTION_BUSY_HIGH,OTP_MODE);
                //IOActionList[1] is Action2.   IOActionList[2] is Action3. 
                //ACTION_PlayIO(ch1,IOActionList[1],ACTION_PORTC|0x1,ACTION_REPEAT,ACTION_BUSY_HIGH,OTP_MODE);
                //ACTION_PlayIO(ch2,IOActionList[2],ACTION_PORTC|0x2,ACTION_REPEAT,ACTION_BUSY_HIGH,OTP_MODE);
                break;
                
            case 0x02:
                ACTION_PauseIO(ch);
                break;
                
            case 0x04:
                ACTION_ResumeIO(ch);
                break; 
                    
            case 0x08:
                ACTION_StopIO(ch);
                break;   
                    
            case 0x10:
                ACTION_GetIOStatus(ch);
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
    }    
}
