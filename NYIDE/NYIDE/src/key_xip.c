/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "nx1_gpio.h"


#define    C_T5S        10000     //Baseon 0.5ms

#define    C_T1S        1000      //Baseon 1ms

//User Var
extern volatile U16 R_Test1KHz;        //For PA15 Ouput 1KHz Tone
extern volatile U8  R_T500us;          //For 1KHz baseon 250us 
extern volatile U8  R_ScanLine;        //For Control scan Matrix   
extern volatile U8  R_ScanEnter;       //For scan key in main loop
extern volatile U16 R_Debouncecnt;     //For Keydebounce        
extern volatile U8  R_KeyValue;        //For Key Value  
extern volatile U8  R_KeyData;         //For KeyIO input data 
extern void Delayms(U32 ms);


/* Defines -------------------------------------------------------------------*/
/*
#define DEBOUNCE_TIME               _DIRECTKEY_PRESS_DB
#define KEY_SCAN_TICK               _DIRECTKEY_SCAN_TICK
#define GPIOA_MASK                  ((_DIRECTKEY_PA7<<7)|(_DIRECTKEY_PA6<<6)|(_DIRECTKEY_PA5<<5)|(_DIRECTKEY_PA4<<4)  \
                                     |(_DIRECTKEY_PA3<<3)|(_DIRECTKEY_PA2<<2)|(_DIRECTKEY_PA1<<1)|_DIRECTKEY_PA0)
*/

/* Static Variables ----------------------------------------------------------*/
/*
static volatile struct{
    U8 debounce_cnt;
    U8 prev;
    U8 buf;     
    U8 toggle;  
}KeyHandle1,KeyHandle2,KeyHandle3,KeyHandle4;   
*/
/* Static Variables ----------------------------------------------------------*/
static volatile struct{
	U8 buf; 	
	U8 flag;  
}KeyHandle0,KeyHandle1,KeyHandle2,KeyHandle3;/*KeyHandle4;*/   

/* Defines -------------------------------------------------------------------*/
#define DEBOUNCE_TIME_RERESET       _DIRECTKEY_PRESS_RESET
#define DEBOUNCE_TIME_OK            _DIRECTKEY_PRESS_OK
#define GPIOA_MASK                  0x00FE //PA1~PA7
#define GPIOA_MASK2                 0x0070                                     



extern void XIP_Standby(void);


//------------------------------------------------------------------//
// Init KeyScan variables
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//
void DIRECTKEY_Init(void){

#if _DIRECTKEY_PA0    
    GPIO_Init(GPIOA,0,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA1    
    GPIO_Init(GPIOA,1,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA2    
    GPIO_Init(GPIOA,2,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA3    
    GPIO_Init(GPIOA,3,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA4    
    GPIO_Init(GPIOA,4,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA5    
    GPIO_Init(GPIOA,5,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA6    
    GPIO_Init(GPIOA,6,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA7    
    GPIO_Init(GPIOA,7,GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA12  
		GPIO_Init(GPIOA, 12, GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA13 
		GPIO_Init(GPIOA, 13, GPIO_MODE_IN_PU);
#endif
#if _DIRECTKEY_PA14 
		GPIO_Init(GPIOA, 14, GPIO_MODE_IN_PU);
#endif


	R_Debouncecnt=DEBOUNCE_TIME_RERESET;
    R_KeyValue=0;
    R_ScanLine=10;
    /*
    KeyHandle1.debounce_cnt=(DEBOUNCE_TIME/KEY_SCAN_TICK)+1;
    KeyHandle1.prev=0;
    KeyHandle1.toggle=1;

	KeyHandle2.debounce_cnt=(DEBOUNCE_TIME/KEY_SCAN_TICK)+1;
    KeyHandle2.prev=0;
    KeyHandle2.toggle=1;

	KeyHandle3.debounce_cnt=(DEBOUNCE_TIME/KEY_SCAN_TICK)+1;
    KeyHandle3.prev=0;
    KeyHandle3.toggle=1;

	KeyHandle4.debounce_cnt=(DEBOUNCE_TIME/KEY_SCAN_TICK)+1;
    KeyHandle4.prev=0;
    KeyHandle4.toggle=1;
    */
}   

//------------------------------------------------------------------//
// Scan Matrix keys
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//  
void MatrixDischange(void){
    P_PORTA->DIR &=0xFFFFF8FF;         //PA10~PA8 Output Low  1,input,0,output
    P_PORTA->Data|=0x00000700;         //Data output 
}
void SetMatrix_SetLine0(void){
    MatrixDischange();
    //PA8 Ouput Low
    GPIO_Init(GPIOA,8,GPIO_OUTPUT_NORMAL);
    GPIO_Write(GPIOA,8,BIT_RESET);             
    //other IO input H
    GPIO_Init(GPIOA,9,GPIO_INPUT_PULLHIGH);
    GPIO_Init(GPIOA,10,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,11,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,12,GPIO_INPUT_PULLHIGH);
}
void SetMatrix_SetLine1(void){
    MatrixDischange();
    //PA9 Ouput Low
    GPIO_Init(GPIOA,9,GPIO_OUTPUT_NORMAL);
    GPIO_Write(GPIOA,9,BIT_RESET);             
    //other IO input H
    GPIO_Init(GPIOA,8,GPIO_INPUT_PULLHIGH);
    GPIO_Init(GPIOA,10,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,11,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,12,GPIO_INPUT_PULLHIGH);
}
void SetMatrix_SetLine2(void){
    MatrixDischange();
    //PA10 Ouput Low
    GPIO_Init(GPIOA,10,GPIO_OUTPUT_NORMAL);
    GPIO_Write(GPIOA,10,BIT_RESET);             
    //other IO input H
    GPIO_Init(GPIOA,8,GPIO_INPUT_PULLHIGH);
    GPIO_Init(GPIOA,9,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,11,GPIO_INPUT_PULLHIGH);
    ////GPIO_Init(GPIOA,12,GPIO_INPUT_PULLHIGH);
}

void SetMatrix_SetLine3(void){
    MatrixDischange();
    //PA11 Ouput Low
    //GPIO_Init(GPIOA,11,GPIO_OUTPUT_NORMAL);
    //GPIO_Write(GPIOA,11,BIT_RESET);             
    //other IO input H
    GPIO_Init(GPIOA,8,GPIO_INPUT_PULLHIGH);
    GPIO_Init(GPIOA,9,GPIO_INPUT_PULLHIGH);
    GPIO_Init(GPIOA,10,GPIO_INPUT_PULLHIGH);
    //GPIO_Init(GPIOA,12,GPIO_INPUT_PULLHIGH);
}

void PoweronKeyScan(void){
    U8 i;  
    //PowerOn Read Line0
    SetMatrix_SetLine0();
    Delayms(2);
    i=255;
    while(i){
          R_KeyData=GPIO_ReadPort(GPIOA);
          R_KeyData^=GPIOA_MASK;
          R_KeyData&=GPIOA_MASK;
          if(KeyHandle0.flag!=R_KeyData){
              KeyHandle0.flag=R_KeyData;
              i=255;
          }
          else{
              i--;
          }
    }
    KeyHandle0.buf=R_KeyData;
    
    //PowerOn Read Line1
    SetMatrix_SetLine1();
    Delayms(2);
    i=255;
    while(i){
          R_KeyData=GPIO_ReadPort(GPIOA);
          R_KeyData^=GPIOA_MASK;
          R_KeyData&=GPIOA_MASK;
          if(KeyHandle1.flag!=R_KeyData){
              KeyHandle1.flag=R_KeyData;
              i=255;
          }
          else{
              i--;
          }
    } 
    KeyHandle1.buf=R_KeyData;
    
    //PowerOn Read Line2
    SetMatrix_SetLine2();
    Delayms(2);
    i=255;
    while(i){
          R_KeyData=GPIO_ReadPort(GPIOA);
          R_KeyData^=GPIOA_MASK;
          R_KeyData&=GPIOA_MASK;
          if(KeyHandle2.flag!=R_KeyData){
              KeyHandle2.flag=R_KeyData;
              i=255;
          }
          else{
              i--;
          }
    }
    KeyHandle2.buf=R_KeyData;
    /*
    //PowerOn Read Line3
    SetMatrix_SetLine3();
    Delayms(2);
    i=255;
    while(i){
          R_KeyData=GPIO_ReadPort(GPIOA);
          R_KeyData^=GPIOA_MASK;
          R_KeyData&=GPIOA_MASK;
          if(KeyHandle3.flag!=R_KeyData){
              KeyHandle3.flag=R_KeyData;
              i=255;
          }
          else{
              i--;
          }
    }
    KeyHandle3.buf=R_KeyData;
    
   //PowerOn Read Line4
    SetMatrix_SetLine4();
    Delayms(2);
    i=255;
    while(i){
          R_KeyData=GPIO_ReadPort(GPIOA);
          R_KeyData^=GPIOA_MASK;
          R_KeyData&=GPIOA_MASK;
          if(KeyHandle4.flag!=R_KeyData){
              KeyHandle4.flag=R_KeyData;
              i=255;
          }
          else{
              i--;
          }
    } 
    KeyHandle4.buf=R_KeyData;
    */
    //Check Test Mode
    //MIDI MODE +KEY 4,Enter 1KHz mode
    if((KeyHandle0.flag==0x90)&&(KeyHandle1.flag==0)&&(KeyHandle2.flag==0)/*&&(KeyHandle3.flag==0)&&(KeyHandle4.flag==0)*/){
          R_Test1KHz=C_T5S;
          R_T500us=0;
    }
    //SONG MODE +KEY 3,Enter Standby mode
    else if((KeyHandle0.flag==0x60)&&(KeyHandle1.flag==0)&&(KeyHandle2.flag==0)/*&&(KeyHandle3.flag==0)&&(KeyHandle4.flag==0)*/){
        ////SYS_Standby();
    } else {
    	PCM_CH2_Play(50, SPI_MODE);
    }
    R_Debouncecnt=DEBOUNCE_TIME_RERESET;
    R_KeyValue=0;
    R_ScanLine=10;
    
}     

//------------------------------------------------------------------//
// Scan 5 keys
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//  
void KeyScan(void){
    if (R_ScanEnter==0){
         return;
    }
    else{
         R_ScanEnter=0;
    }
    
    R_ScanLine++;
    if(R_ScanLine>=8){
         R_ScanLine=0;
    }
    switch(R_ScanLine)
    {
         case 0:  //Set Scan Line0
               SetMatrix_SetLine0();
               break;
         case 1:  //Scan Line0
               R_KeyData=GPIO_ReadPort(GPIOA);
               R_KeyData^=GPIOA_MASK;
               R_KeyData&=GPIOA_MASK;
               
               if(KeyHandle0.flag==R_KeyData){
                  //Key No change
                  KeyHandle0.buf=R_KeyData;
                  return;
               }
               if(KeyHandle0.buf!=R_KeyData){
                  //IO Debounce
                  KeyHandle0.buf=R_KeyData;
                  R_Debouncecnt=DEBOUNCE_TIME_RERESET;
                  return;
               }
               if(R_Debouncecnt>DEBOUNCE_TIME_OK){
                  //wait Debounce timer OK
                  return;
               }
               R_KeyData^=KeyHandle0.flag;
               R_KeyData&=KeyHandle0.buf;
               KeyHandle0.flag=KeyHandle0.buf;
               switch(R_KeyData)
               {
                     case 0x04:
                     R_KeyValue=6;    //Key: F6
                     break;

					 case 0x08:
					 R_KeyValue=5;    //Key: F5
					 break;

					 case 0x10:
					 R_KeyValue=4;    //Key: F4
					 break;

					 case 0x20:
					 R_KeyValue=3;    //Key: F3
					 break;

					 case 0x40:
					 R_KeyValue=2;    //Key: F2
					 break;

					 case 0x80:
					 R_KeyValue=1;    //Key: F1
					 break;
					 
                     default :
                     break;
               }
               break;
         case 2:  //Set Scan Line1
               SetMatrix_SetLine1();
               break;
         case 3:  //Scan Line1
               R_KeyData=GPIO_ReadPort(GPIOA);
               R_KeyData^=GPIOA_MASK;
               R_KeyData&=GPIOA_MASK;
               
               if(KeyHandle1.flag==R_KeyData){
                  //Key No change
                  KeyHandle1.buf=R_KeyData;
                  return;
               }
               if(KeyHandle1.buf!=R_KeyData){
                  //IO Debounce
                  KeyHandle1.buf=R_KeyData;
                  R_Debouncecnt=DEBOUNCE_TIME_RERESET;
                  return;
               }
               if(R_Debouncecnt>DEBOUNCE_TIME_OK){
                  //wait Debounce timer OK
                  return;
               }
               R_KeyData^=KeyHandle1.flag;
               R_KeyData&=KeyHandle1.buf;
               KeyHandle1.flag=KeyHandle1.buf;
               switch(R_KeyData)
               {
                      case 0x02 :
                      R_KeyValue=17;    //Key:M7
                      break;
                      case 0x04 :
                      R_KeyValue=16;    //Key:M6
                      break;
                      case 0x08 :
                      R_KeyValue=15;    //Key:M5
                      break;
                      case 0x10 :
                      R_KeyValue=14;    //Key:M4
                      break;
                      case 0x20 :
                      R_KeyValue=13;    //Key:M3
                      break;
                      case 0x40 :
                      R_KeyValue=12;    //Key:M2
                      break;
					  case 0x80 :
                      R_KeyValue=11;    //Key:M1
                      break;
                      default :
                      break;
              }
              break;
         case 4:  //Set Scan Line2
               SetMatrix_SetLine2();
               break;
         case 5:  //Scan Line2
               R_KeyData=GPIO_ReadPort(GPIOA);
               R_KeyData^=GPIOA_MASK;
               R_KeyData&=GPIOA_MASK;
               
               if(KeyHandle2.flag==R_KeyData){
                  //Key No change
                  KeyHandle2.buf=R_KeyData;
                  return;
               }
               if(KeyHandle2.buf!=R_KeyData){
                  //IO Debounce
                  KeyHandle2.buf=R_KeyData;
                  R_Debouncecnt=DEBOUNCE_TIME_RERESET;
                  return;
               }
               if(R_Debouncecnt>DEBOUNCE_TIME_OK)
               {
                  //wait Debounce timer OK
                  return;
               }
               R_KeyData^=KeyHandle2.flag;
               R_KeyData&=KeyHandle2.buf;
               KeyHandle2.flag=KeyHandle2.buf;
               switch(R_KeyData)
               {
                      case 0x02 :
                      R_KeyValue=24;    //Key: M14
                      break;
                      case 0x04 :
                      R_KeyValue=23;    //Key: M13
                      break;
                      case 0x08 :
                      R_KeyValue=22;    //Key: M12
                      break;
                      case 0x10 :
                      R_KeyValue=21;    //Key: M11
                      break;
                      case 0x20 :
                      R_KeyValue=20;    //Key: M10
                      break;
                      case 0x40 :
                      R_KeyValue=19;    //Key: M9
                      break;
					  case 0x80 :
                      R_KeyValue=18;    //Key: M8
                      break;
                      default :
                      break;
              }
              break; 
	     
         case 6:  //Set Scan Line3
               SetMatrix_SetLine3();
               break;
         case 7:  //Scan Line3
               R_KeyData= (GPIO_ReadPort(GPIOA) >> 8);
               R_KeyData^=GPIOA_MASK2;
               R_KeyData&=GPIOA_MASK2;
               
               if(KeyHandle3.flag==R_KeyData){
                  //Key No change
                  KeyHandle3.buf=R_KeyData;
                  return;
               }
               if(KeyHandle3.buf!=R_KeyData){
                  //IO Debounce
                  KeyHandle3.buf=R_KeyData;
                  R_Debouncecnt=DEBOUNCE_TIME_RERESET;
                  return;
               }
               if(R_Debouncecnt>DEBOUNCE_TIME_OK)
               {
                  //wait Debounce timer OK
                  return;
               }
               R_KeyData^=KeyHandle3.flag;
               R_KeyData&=KeyHandle3.buf;
               KeyHandle3.flag=KeyHandle3.buf;
               switch(R_KeyData)
               {
                      case 0x10:
                      R_KeyValue=31;    //Key:A
                      break;
                      case 0x20:
                      R_KeyValue=32;    //Key:B
                      break;
                      case 0x40:
                      R_KeyValue=33;    //Key:C
                      break;
                      case 0x50:
					  R_KeyValue=34;   //Key:A+C
                      break;
                      default :
                      break;
              }
              break;   
	    /*
        case 8:  //Set Scan Line4
               SetMatrix_SetLine4();
               break;
         case 9:  //Scan Line4
               R_KeyData=GPIO_ReadPort(GPIOA);
               R_KeyData^=GPIOA_MASK;
               R_KeyData&=GPIOA_MASK;
               
               if(KeyHandle4.flag==R_KeyData){
                  //Key No change
                  return;
               }
               if(KeyHandle4.buf!=R_KeyData){
                  //IO Debounce
                  KeyHandle4.buf=R_KeyData;
                  R_Debouncecnt=DEBOUNCE_TIME_RERESET;
                  return;
               }
               if(R_Debouncecnt>DEBOUNCE_TIME_OK)
               {
                  //wait Debounce timer OK
                  return;
               }
               R_KeyData^=KeyHandle4.flag;
               R_KeyData&=KeyHandle4.buf;
               KeyHandle4.flag=KeyHandle4.buf;
               switch(R_KeyData)
               {
                      case 0x02 :
                      R_KeyValue=2;    //Key2
                      break;
                      case 0x04 :
                      R_KeyValue=6;    //Key:6
                      break;
                      case 0x08 :
                      R_KeyValue=10;    //Key:10
                      break;
                      case 0x10 :
                      R_KeyValue=14;    //Key:14
                      break;
                      case 0x20 :
                      R_KeyValue=18;    //Key:18
                      break;
                      case 0x40 :
                      R_KeyValue=22;    //Key:22
                      break;
                      default :
                      break;
              }
              break;  
              */
         default :
              break; 
    }  
}  

//------------------------------------------------------------------//
// Get KeyCode
// Parameter: 
//          NONE
// return value:
//          0: no key 
//          others: keycode
//------------------------------------------------------------------//    
U8 Matrix_GetKey(void){
    R_KeyData=R_KeyValue;
    R_KeyValue=0;
    return R_KeyData;    
}   

