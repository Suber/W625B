/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "nx1_spi.h"



/* Defines -------------------------------------------------------------------*/
#define RESOURCE_DATA_TABLE_OFFSET              10

/* Static Variables ----------------------------------------------------------*/
 

//------------------------------------------------------------------//
// Get Resource Count
// Parameter: 
//          NONE
// return value:
//          return: the number of resource count
//------------------------------------------------------------------//
U8 RESOURCE_GetResourceCnt(void)
{
    U8 tempbuf[4];
    U32 addr;
    U8 cnt;
    
    SPI_BurstRead(tempbuf,1020,4); // 1024-4
    addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 

    SPI_BurstRead(tempbuf,addr+RESOURCE_DATA_TABLE_OFFSET,SPI_TAB_INFO_ENTRY_SIZE);
    addr=addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24)); //Resource data table address

    SPI_BurstRead(tempbuf,addr,2); //Get Resoure count
    cnt = tempbuf[0]|(tempbuf[1]<<8);
    return cnt;
}   
//------------------------------------------------------------------//
// Get Resource Address
// Parameter: 
//          idx: resource index, it is from 0.
// return value:
//          return: the address of resource count
//------------------------------------------------------------------//
U32 RESOURCE_GetAddress(U8 idx)
{
    U8 tempbuf[4];
    U32 addr;
    U32 base_addr;
    
    SPI_BurstRead(tempbuf,1020,4); // 1024-4
    base_addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 
    SPI_BurstRead(tempbuf,base_addr+RESOURCE_DATA_TABLE_OFFSET,SPI_TAB_INFO_ENTRY_SIZE);
    addr=base_addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24)); //Resource data table address
    SPI_BurstRead(tempbuf,addr+3+(5*idx),4); 
    addr=base_addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24)); //Get idx Resoure Data Address
    return addr;
} 

//------------------------------------------------------------------//
// Get Resource type
// Parameter: 
//          idx: resource index, it is from 0.
// return value:
//          return: resource type
//------------------------------------------------------------------//
U8 RESOURCE_GetType(U8 idx)
{
    U8 tempbuf[4];
    U32 addr;
    U8 type;
    
    SPI_BurstRead(tempbuf,1020,4); // 1024-4
    addr=tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24); 

    SPI_BurstRead(tempbuf,addr+RESOURCE_DATA_TABLE_OFFSET,SPI_TAB_INFO_ENTRY_SIZE);
    addr=addr+(tempbuf[0]|(tempbuf[1]<<8)|(tempbuf[2]<<16)|(tempbuf[3]<<24)); //Resource data table address

    SPI_BurstRead(tempbuf,addr+2+(5*idx),1); 
    type=tempbuf[0]; //Get idx Resoure Data Address
    return type;
}