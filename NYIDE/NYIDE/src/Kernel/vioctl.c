/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "nx1_gpio.h"
#if _ACTION_MODULE
#include "IoTab.h"
#endif
/* Defines -------------------------------------------------------------------*/

#define E_VIO_INVALID_FILE_IDX          1
#define E_VIO_INVALID_ACTION_IDX        2
#define E_VIO_INVALID_CHANNEL           3

#define VIO_FLAG_REPEAT                 0x1
#define VIO_FLAG_HIGH_BUSY              0x2
#define VIO_FLAG_LOW_BUSY               0x4

#define VIO_STATE_HIGH                  0
#define VIO_STATE_LOW                   1
#define VIO_STATE_TOGGLE_HIGH           2
#define VIO_STATE_TOGGLE_LOW            3

#define VIO_HIGH_BUSY_OFFSET            0x1
#define VIO_LOW_BUSY_OFFSET             0x2

#define VIO_LOW     0xFD
#define VIO_HIGH    0xFE    
#define VIO_END     0xFF
//
//#define VIO_BUSY        BIT_RESET
//#define VIO_NOT_BUSY    BIT_SET

#define PIN_MASK    0x0F
#define PORT_MASK   0xF0

struct FILE_HEADER{
    U8   type;
    U8   rsvd[3];
    U32  tick;
};

struct CH_INFO{
    U32 id;
    U32 offset;
};
//struct IO_FRAME{
//    U16  index;
//    U8 data;
//};
struct IO_ACTION{
    U8 counter;
    U8 flag;
    U8 flag2;
    U8 state;
    U8 port;
    //U8 reset;
    U8 storage_mode;
    U32 offset;
    U32 start;
};    

/* Static Variables ----------------------------------------------------------*/
#if _ACTION_MODULE

static struct IO_ACTION IOActionCtl[_VIO_CH_NUM];
static U8 VioData[_VIO_CH_NUM][2];
static U8 g_storage_mode;

#if TEMPLATE_FORMAT==TEMPLATE_APP
static U8 IOOpTask[_VIO_CH_NUM];
#endif

#endif

/* Extern Tables ----------------------------------------------------------*/
extern const U32  IOActionList[ ];
//------------------------------------------------------------------//
// Check if any vio control event
// Parameter: 
//          NONE
// return value:
//          NONE
//------------------------------------------------------------------//      
void ChkVioCtl(void)
{
#if _ACTION_MODULE   
    U8 i,j,port,pin;
    struct IO_ACTION *ctl;
    GPIO_TypeDef *p_port;
//    GPIO_Write(GPIOA,1,BIT_SET);
   
    for(i=0;i<_VIO_CH_NUM;i++){
        ctl=IOActionCtl+i;
        ctl->counter--;
       
        if(ctl->counter==0){
            if(ctl->state==STATUS_PLAYING){
                if(ctl->storage_mode == OTP_MODE)
                {
                    ctl->offset++;                    
                    VioData[i][0] = *(U8 *)ctl->offset;
                    VioData[i][1] = *(U8 *)(ctl->offset+1);      
                    ctl->counter=VioData[i][0];
                }
                else //SPI_MODE
                {
                    U8  tempbuf[2];

                    ctl->offset++;
                    SPI_BurstRead(tempbuf,ctl->offset,2); //read data from SPI        
                    VioData[i][0] = tempbuf[0];
                    VioData[i][1] = tempbuf[1];
                    ctl->counter=VioData[i][0];
                }
                //dprintf("counter:%d\r\n",ctl->counter);
                
                port=ctl->port&PORT_MASK;
                pin=ctl->port&PIN_MASK;
                if(port==ACTION_PORTA){
                    p_port=GPIOA;
                }else if(port==ACTION_PORTB){
                    p_port=GPIOB;
                }else{
                    p_port=GPIOC;
                }   
                         
                if(ctl->counter==VIO_LOW){  
                    //ctl->reset=1; 
                    ctl->offset++;
                    ctl->counter=VioData[i][1];      
                    //GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_LOW_BUSY)>>VIO_LOW_BUSY_OFFSET);
                    if(ctl->flag&VIO_FLAG_LOW_BUSY){
                        p_port->Data|=(1<<pin);
                    }else{
                        p_port->Data&=(~(1<<pin));
                    }
                    ctl->flag2=VIO_STATE_LOW;
                }else if(ctl->counter==VIO_HIGH){
                    //ctl->reset=1;
                    ctl->offset++;
                    ctl->counter=VioData[i][1];  
                    //GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET);
                    if(ctl->flag&VIO_FLAG_HIGH_BUSY){
                        p_port->Data|=(1<<pin);
                    }else{
                        p_port->Data&=(~(1<<pin));
                    }
                    ctl->flag2=VIO_STATE_HIGH;
                }else{    
                    if(ctl->counter==VIO_END){
                        if(ctl->flag&VIO_FLAG_REPEAT){
                            if(ctl->storage_mode == OTP_MODE)
                            {
                                ctl->offset=ctl->start;
                                VioData[i][0] = *(U8 *)ctl->offset;
                                VioData[i][1] = *(U8 *)(ctl->offset+1);  
                                ctl->counter=VioData[i][0];
                            }
                            else //SPI_MODE
                            {
                                U8  tempbuf[2];

                                ctl->offset=ctl->start;
        
                                SPI_BurstRead(tempbuf,ctl->offset,2); //read data from SPI        
                                VioData[i][0] = tempbuf[0];
                                VioData[i][1] = tempbuf[1];
                                ctl->counter=VioData[i][0];
                            }
                            
                            if(ctl->counter==VIO_LOW){
                                //ctl->reset=1; 
                                ctl->offset++;
                                ctl->counter=VioData[i][1];
                                //GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_LOW_BUSY)>>VIO_LOW_BUSY_OFFSET);
                                if(ctl->flag&VIO_FLAG_LOW_BUSY){
                                    p_port->Data|=(1<<pin);
                                }else{
                                    p_port->Data&=(~(1<<pin));
                                }
                                ctl->flag2=VIO_STATE_LOW;
                            }else if(ctl->counter==VIO_HIGH){
                                //ctl->reset=1; 
                                ctl->offset++;
                                ctl->counter=VioData[i][1];
                                //GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET);
                                if(ctl->flag&VIO_FLAG_HIGH_BUSY){
                                    p_port->Data|=(1<<pin);
                                }else{
                                    p_port->Data&=(~(1<<pin));
                                }
                                ctl->flag2=VIO_STATE_HIGH;
                            }else{
                                //GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET);
                                if(ctl->flag&VIO_FLAG_HIGH_BUSY){
                                    p_port->Data|=(1<<pin);
                                }else{
                                    p_port->Data&=(~(1<<pin));
                                }
                                ctl->flag2=VIO_STATE_TOGGLE_HIGH;
                            }                
                        }else{
                            ctl->state=STATUS_STOP;
                        }        
//                        
//                        for(j=0;j<_VIO_CH_NUM;j++){
//                            if(IOActionCtl[j].state!=STATUS_STOP){
//                                break;
//                            }    
//                        }
//                        if(j==_VIO_CH_NUM){  
//                            ResetSysTick();    
//                        }    
                    }else{    
//                        if(ctl->reset){
//                            ctl->reset=0;  
//                            dprintf("r\r\n"); 
//                            GPIO_Write(p_port,pin,(ctl->flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET); 
//                        }else{    
                            //GPIO_Toggle(p_port,pin);
                          
                          if(ctl->flag2==VIO_STATE_TOGGLE_HIGH){
                            ctl->flag2=VIO_STATE_TOGGLE_LOW;
                          }else{
                            ctl->flag2=VIO_STATE_TOGGLE_HIGH;
                          }              
                          p_port->Data^=(1<<pin);  
//                        }
                    }    
                }        
            }else if(ctl->state==STATUS_PAUSE){
                if(ctl->flag2==VIO_STATE_HIGH){
                    ctl->counter=VioData[i][1];
                }else if(ctl->flag2==VIO_STATE_LOW){
                    ctl->counter=VioData[i][1];
                }else if(ctl->flag2==VIO_STATE_TOGGLE_LOW){
                     
                    port=ctl->port&PORT_MASK;
                    pin=ctl->port&PIN_MASK;
                    if(port==ACTION_PORTA){
                        p_port=GPIOA;
                    }else if(port==ACTION_PORTB){
                        p_port=GPIOB;
                    }else{
                        p_port=GPIOC;
                    }    
                    ctl->offset--;
                    ctl->counter=VioData[i][0];
                    p_port->Data^=(1<<pin);  
                    ctl->flag2=VIO_STATE_TOGGLE_HIGH;
                }else{    
                    port=ctl->port&PORT_MASK;
                    pin=ctl->port&PIN_MASK;
                    if(port==ACTION_PORTA){
                        p_port=GPIOA;
                    }else if(port==ACTION_PORTB){
                        p_port=GPIOB;
                    }else{
                        p_port=GPIOC;
                    }    
                    ctl->offset++;
                    ctl->counter=VioData[i][1];
                    p_port->Data^=(1<<pin);
                    ctl->flag2=VIO_STATE_TOGGLE_LOW;
                }     
            }        
        }        
    }
//    GPIO_Write(GPIOA,1,BIT_RESET);
#endif    
}    
//------------------------------------------------------------------//
// Play IO
// Parameter: 
//          ch:vio channel
//          file: io action file
//          port: assign gpio port, port|pin
//          repeat:1:repeat, 0:no_repeat
//          busy: 0:busy_low,1:busy_high
//          mode: OTP_MODE only
// return value:
//          0:ok, others:fail
//------------------------------------------------------------------//      
U8 ACTION_PlayIO(U8 ch,const U32 file,U8 port,U8 repeat,U8 busy,U8 mode)
{
#if _ACTION_MODULE     
    struct FILE_HEADER  *header;
    GPIO_TypeDef *p_port;
    U8 total_action;
    U8 i,pin;
    U32 offset=0;
    
    if(ch>=_VIO_CH_NUM){
        return E_VIO_INVALID_CHANNEL;
    }    

#if TEMPLATE_FORMAT==TEMPLATE_APP    
    IOOpTask[ch]=QCurrentTask|QSentencFlag;
#endif   

    IOActionCtl[ch].port=port;
    
    pin=port&PIN_MASK;
    port=port&PORT_MASK;
    
    if(port==ACTION_PORTA){
        p_port=GPIOA;
    }else if(port==ACTION_PORTB){
        p_port=GPIOB;
    }else{
        p_port=GPIOC;
    }            
    GPIO_Init(p_port,pin,GPIO_MODE_OUT);

    IOActionCtl[ch].flag=0;
    if(repeat){
        IOActionCtl[ch].flag|=VIO_FLAG_REPEAT;
    }
    if(busy){
        IOActionCtl[ch].flag|=VIO_FLAG_HIGH_BUSY;    
    }else{
        IOActionCtl[ch].flag|=VIO_FLAG_LOW_BUSY;
    }     

    IOActionCtl[ch].storage_mode = mode;
    if(mode==OTP_MODE)
    {
        header=(struct FILE_HEADER *)file;
        offset=8; //header offset
        IOActionCtl[ch].offset=(U32)file+offset;
        VioData[ch][0] = *(U8 *)IOActionCtl[ch].offset;
        VioData[ch][1]=*(U8 *)(IOActionCtl[ch].offset+1);
        
    }
    else //SPI_MODE
    {
        U8  tempbuf[2];
        
        SPI_BurstRead(tempbuf,IOActionCtl[ch].offset,2); //read data from SPI        
        VioData[ch][0] = tempbuf[0];
        VioData[ch][1] = tempbuf[1];
    }
    
    IOActionCtl[ch].start=IOActionCtl[ch].offset;
   // IOActionCtl[ch].reset=0;
    
    IOActionCtl[ch].counter=VioData[ch][0];
    if(IOActionCtl[ch].counter==VIO_LOW){
        //IOActionCtl[ch].reset=1;
        IOActionCtl[ch].offset++;
        IOActionCtl[ch].counter=VioData[ch][1];
        GPIO_Write(p_port,pin,(IOActionCtl[ch].flag&VIO_FLAG_LOW_BUSY)>>VIO_LOW_BUSY_OFFSET);
        
        IOActionCtl[ch].flag2=VIO_STATE_LOW;
    }else if(IOActionCtl[ch].counter==VIO_HIGH){
        //IOActionCtl[ch].reset=1;
        IOActionCtl[ch].offset++;
        IOActionCtl[ch].counter=VioData[ch][1];
        GPIO_Write(p_port,pin,(IOActionCtl[ch].flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET);
        
        IOActionCtl[ch].flag2=VIO_STATE_HIGH;
    }else{
        GPIO_Write(p_port,pin,(IOActionCtl[ch].flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET);
        
        IOActionCtl[ch].flag2=VIO_STATE_TOGGLE_HIGH;
    }       
    
	GIE_DISABLE();     
	
    for(i=0;i<_VIO_CH_NUM;i++){
        if(IOActionCtl[i].state!=STATUS_STOP){
            break;
        }    
    }
    
    IOActionCtl[ch].state=STATUS_PLAYING;
    
//    if(i==_VIO_CH_NUM){
//        SysTickInit(header->tick); 
//        //dprintf("header->tick:%d\r\n",header->tick);               
//    }    
    
	GIE_ENABLE();  
    return 0;
//    while(1){
//        GPIO_Toggle(GPIOA,1);
//    }    
//    IOActionCtl[0].state=STATUS_PLAYING;
//    IOActionCtl[1].state=STATUS_PLAYING;
//    IOActionCtl[2].state=STATUS_PLAYING;
//    IOActionCtl[3].state=STATUS_PLAYING;
//    IOActionCtl[4].state=STATUS_PLAYING;
//    IOActionCtl[5].state=STATUS_PLAYING;
//    IOActionCtl[6].state=STATUS_PLAYING;
//    IOActionCtl[7].state=STATUS_PLAYING;
    //dprintf("count:%d\r\n",IOActionCtl[port].count);
#endif
}    
//------------------------------------------------------------------//
// Pause IO
// Parameter: 
//          ch: vio channel, 0xff:stop all channel
// return value:
//          NONE
//------------------------------------------------------------------//      
void ACTION_PauseIO(U8 ch)
{
#if _ACTION_MODULE  
    U8 i;
    if(ch<_VIO_CH_NUM){
        if(IOActionCtl[ch].state==STATUS_PLAYING){
            IOActionCtl[ch].state=STATUS_PAUSE;
        }
    }else if(ch==0xFF){
        for(i=0;i<_VIO_CH_NUM;i++){
            if(IOActionCtl[i].state==STATUS_PLAYING){
                IOActionCtl[i].state=STATUS_PAUSE;        
            }
        }
    }                
#endif    
}   
//------------------------------------------------------------------//
// Resume IO
// Parameter: 
//          ch:vio channel,0xff:stop all channel
// return value:
//          NONE
//------------------------------------------------------------------//      
void ACTION_ResumeIO(U8 ch)
{
#if _ACTION_MODULE     
    U8 i;
    if(ch<_VIO_CH_NUM){
        if(IOActionCtl[ch].state==STATUS_PAUSE){
            IOActionCtl[ch].state=STATUS_PLAYING;
        }      
    }else if(ch==0xFF){
        for(i=0;i<_VIO_CH_NUM;i++){
            if(IOActionCtl[i].state==STATUS_PAUSE){
                IOActionCtl[i].state=STATUS_PLAYING;        
            }      
        }
    }          
#endif    
}   
//------------------------------------------------------------------//
// Stop IO
// Parameter: 
//          ch:vio channel,0xff:stop all channel
// return value:
//          NONE
//------------------------------------------------------------------//      
void ACTION_StopIO(U8 ch)
{
#if _ACTION_MODULE  
	U8 i,port,state;
    GPIO_TypeDef *p_port;
    
    if(ch<_VIO_CH_NUM){
        
        state=IOActionCtl[ch].state;
        IOActionCtl[ch].state=STATUS_STOP;
        //ClrWaitingAction(ch);
        
        port=IOActionCtl[ch].port&PORT_MASK;
        if(port==ACTION_PORTA){
            p_port=GPIOA;
        }else if(port==ACTION_PORTB){
            p_port=GPIOB;
        }else{
            p_port=GPIOC;
        }   
        if(state==STATUS_PLAYING||state==STATUS_PAUSE){
            GPIO_Write(p_port,IOActionCtl[ch].port&PIN_MASK,!((IOActionCtl[ch].flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET));
        }
    }else if(ch==0xFF){
        for(i=0;i<_VIO_CH_NUM;i++){
           state=IOActionCtl[i].state;
           IOActionCtl[i].state=STATUS_STOP;
           //ClrWaitingAction(i);
           
           port=IOActionCtl[i].port&PORT_MASK;
           if(port==ACTION_PORTA){
                p_port=GPIOA;
            }else if(port==ACTION_PORTB){
                p_port=GPIOB;
            }else{
                p_port=GPIOC;
            }   
            if(state==STATUS_PLAYING||state==STATUS_PAUSE){
                GPIO_Write(p_port,IOActionCtl[i].port&PIN_MASK,!((IOActionCtl[i].flag&VIO_FLAG_HIGH_BUSY)>>VIO_HIGH_BUSY_OFFSET));
            }
        }
    }           
    
//    for(i=0;i<_VIO_CH_NUM;i++){
//        if(IOActionCtl[i].state!=STATUS_STOP){
//            break;
//        }    
//    }
//    if(i==_VIO_CH_NUM){
//        ResetSysTick();                  
//    }    
#endif         
}   
//------------------------------------------------------------------//
// Get IO current status
// Parameter: 
//          ch:vio channel
// return value:
//          status:STATUS_STOP,STATUS_PLAYING,STATUS_PAUSE
//------------------------------------------------------------------// 
U8 ACTION_GetIOStatus(U8 ch){
#if _ACTION_MODULE 
    if(ch>=_VIO_CH_NUM){
        return 0;
    }       
    return IOActionCtl[ch].state;
#endif
    return 0;
}
//------------------------------------------------------------------//
// Get IO task pointer
// Parameter: 
//        *num:io port number
// return value:
//          operation task pointer
//------------------------------------------------------------------// 
U8* GetIOOpTask(U8 *num)
{
#if _ACTION_MODULE 
#if TEMPLATE_FORMAT==TEMPLATE_APP   
    *num= _VIO_CH_NUM;
    return IOOpTask;
#endif    
#endif
    return 0;
}    
