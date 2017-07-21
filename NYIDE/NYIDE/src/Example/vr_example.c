//==========================================================================
// File Name          : vr_example.c
// Description        : Example of Voice Recognition
// Version            : V0
// Last modified date : 2016/11/30
//==========================================================================
#include "nx1_lib.h"
#include "VRCmdTab.h"
int main(void)
{ 
    S8 id;
    
    SYS_Init();
  
    
    VR_Start(VR_CMD_CYBASE,VR_CMD_GROUP_1);
    while(1)
    {
        id=VR_ServiceLoop();
        
        if(id!=-1){
            //parse command id here
            
            //AUDIO_Play(AUDIO_CH0,0,SPI_MODE);
        }

        AUDIO_ServiceLoop();
        SYS_ServiceLoop();
    }    
}
