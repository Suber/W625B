/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "nx1_pwm.h"
#include "nx1_timer.h"
/* Defines -------------------------------------------------------------------*/

#ifdef SUPPORT_NYQ
U8 QIOPort[QIO_PORT_NUM];
#pragma weak QIOPort
#endif
#ifdef SUPPORT_PLAYIO
U8  VIOPort[QIO_PORT_NUM];
#pragma weak VIOPort
#endif

#ifdef SUPPORT_WAVE_MARK
void (*WaveMarkFunc[WAVEMARK_FUNC_NUM])(void);
#pragma weak WaveMarkFunc
#endif
//void func1(void){
//    dprintf("f1\r\n");
//}    
//void func2(void){
//    dprintf("f2\r\n");
//}    
//void func3(void){
//    dprintf("f3\r\n");
//}    
#ifdef SUPPORT_MIDI_MARK
void (*MidiMarkFunc[MIDIMARK_FUNC_NUM])(void);
#pragma weak MidiMarkFunc
#endif

extern void SetMidiMark(U8 id);
extern void SetNyqMark(U8 id);
//------------------------------------------------------------------//
// Callback fucntion when NYQ event happens
// Parameter: 
//          io: io channel
//          data: io data
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_NYQEvent(U8 io,U8 data){
#ifdef SUPPORT_NYQ
    U8 port;
    
    //dprintf("%d,%d\r\n",io,data);
    if(io>=QIO_PORT_NUM){
        return;
    }    
        
    port=QIOPort[io];

    if(port==0xFF){
        return;
    }    
    if(port>=PWM_PORT_NUM){
        port-=PWM_PORT_NUM;    
        PWM_SetDuty(PWMB,port,data);
    }else{
        PWM_SetDuty(PWMA,port,data);
    }      
#endif 
//    if(io==0){
//        dprintf("%d,%d\r\n",io,data);
//    }  
} 
//------------------------------------------------------------------//
// Callback fucntion when vio event happens
// Parameter: 
//          io: io channel
//          data: io data
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_VioEvent(U8 io,U8 data){
#ifdef SUPPORT_PLAYIO    
    U8 port;
    if(io>=_VIO_CH_NUM){
        return;
    }        
    port=VIOPort[io];
    
    if(port==0xFF){
        return;
    }    
    //dprintf("%d,%d\r\n",io,data);

    if(port>=PWM_PORT_NUM){
        port-=PWM_PORT_NUM;    
        PWM_SetDuty(PWMB,port,data);
    }else{
        PWM_SetDuty(PWMA,port,data);
    }           
//    if(io==0){
//        dprintf("%d,%d\r\n",io,data);
//    }    
#endif
} 
//------------------------------------------------------------------//
// Callback fucntion when NYQ Mark happens
// Parameter: 
//          id: mark id
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_NYQMark(U8 id){
#ifdef SUPPORT_WAVE_MARK    
    void (*func)(void);
    
#if TEMPLATE_FORMAT==TEMPLATE_APP    
    SetNyqMark(id);
    return;
#endif
    
    if(id>=WAVEMARK_FUNC_NUM){
        return;
    }    
    func=WaveMarkFunc[id];
    
    if(func==0){
        return;
    }       
    func();
#endif    
//        dprintf("%d\r\n",id); 
}  
//------------------------------------------------------------------//
// Callback fucntion when Midi Mark happens
// Parameter: 
//          id: mark id
// return value:
//          NONE
//------------------------------------------------------------------// 
void CB_MidiMark(U8 id){
#ifdef SUPPORT_MIDI_MARK
    void (*func)(void);  
    
#if TEMPLATE_FORMAT==TEMPLATE_APP   
    SetMidiMark(id);
    return;
#endif
    if(id>=MIDIMARK_FUNC_NUM){
        return;
    }    
    func=MidiMarkFunc[id];
    
    if(func==0){
        return;
    }       
    func();
#endif    
//        dprintf("%d\r\n",id);
}  
