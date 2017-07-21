//==========================================================================
// File Name          : main.c
// Description        : Example of AUDIO Play
// Version            : V0
// Last modified date : 2017/05/11
//==========================================================================
#include "nx1_lib.h"
#include "nx1_smu.h"
#include "nx1_gpio.h"

extern void XIP_main(void);

int main(void)
{ 
    U8 volume=0;
    U8 key;
    U8 song_idx=0;
    
    
    SYS_Init();
    
    XIP_main();
	
}

