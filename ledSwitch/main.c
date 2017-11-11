/*
 * main.c
 */


#include <stdint.h>
#include <tm4c123gh6pm.h>
#include "macros.h"
#include <PORTF_Init.h>
#define pf1 (*((volatile uint32_t *)0x40025008))

//functions prototypes
//--------------------
void PORTE_INIT ();

void Systick_Wait(uint32_t delay) ;
void Systick_Wait_10ms(uint32_t delay ) ;
void SWITCH_TOGGLE () ;
void delay( uint32_t ms ) ;




int main(void) {
    PORTE_INIT() ;
    SysTick_Wait(800000) ;
    //PORTF_Init() ;
    //pf1 = 1 ;
    //pf1 = (1<<1) ;


    while(1){
        if(PUSH_BUTTON == 0x1){
        SWITCH_TOGGLE(); // toggle the 2 switchs
    }
    }



	
	return 0;
}


//-----------------------
//PORT E INITIALIZATION
//-----------------------
void PORTE_INIT (){
       uint32_t delay ;
       SYSCTL_RCGC2_R |= 0x10 ;
       delay = SYSCTL_RCGC2_R ;
       GPIO_PORTE_DIR_R |= 0x06 ;      // bit 1 and 2 are output for leds
       GPIO_PORTE_DIR_R &= ~0x08 ;     // bit 0 is input for the switch .. bit 3 input for the second sw
       GPIO_PORTE_AMSEL_R &= ~0x0f ;   // no analog
       GPIO_PORTE_AFSEL_R &= ~0x0f ;   // no alt functions are needed
       GPIO_PORTE_PCTL_R &= ~0x0f ; // ???
       GPIO_PORTE_DEN_R |= 0x0f ;      // digital enable for the bits used

}


//----------------
//SWITCH TOGGLE
//----------------
void SWITCH_TOGGLE (){
    int i ;
    for (i = 0 ; i < 3 ; i++){
        RLED = 0x02 ;
        delay (100);
        RLED = 0x00 ;
        YLED = 0x04 ;
        delay (100);
        YLED = 0x00;
        delay (100);
    }

}

//--------------
// delay function
//----------------
void delay (uint32_t ms ){
    unsigned long count ;
        while (ms > 0)
        {
            count = 16000 ;
            while (count > 0 ){
            count-- ;
            }
            ms-- ;
        }

}


//--------------------------------------------
// Systick wait
//--------------------------------------------
void Systick_Wait (uint32_t delay){
    NVIC_ST_CTRL_R = 0x00  ; // disable during the setup
    NVIC_ST_RELOAD_R = delay -1 ; // the no of count to wait
    NVIC_ST_CURRENT_R = 0 ; // write any value to start counting
    NVIC_ST_CTRL_R = 0x05  ; // 0101 enable system tick with the system clk, no interrupts, enable the Systek

    while (NVIC_ST_CTRL_R & 0x010000 == 0){
        // wait for the flag to be set ;
    }
}

//-------------------------------------------
// Systick wait in ms ;
//-------------------------------------------
void Systick_Wait_10ms (uint32_t ms){
    uint32_t i ;
    for (i = 0 ; i<ms ; i++){
        Systick_Wait(800000) ; // wait for 10 ms
    }
}

