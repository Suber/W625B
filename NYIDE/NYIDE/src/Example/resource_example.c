#include "nx1_lib.h"
#include "nx1_spi.h"

int main()
{
	U32 addr;
	U8 buf[32];
	
	SYS_Init();
	    
    //read 1st resource file address and read 12 byte resource data
    addr = RESOURCE_GetAddress(0);
    SPI_BurstRead(buf, addr,12);
    SYS_ServiceLoop();
    //read 2nd resource file address and read 12 byte resource data    
    addr = RESOURCE_GetAddress(1);
    SPI_BurstRead(buf, addr,12);    
    while(1)
    {    	
    	SYS_ServiceLoop();
    }
}
