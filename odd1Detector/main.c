/*
 * main.c
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include "SysTick_Init.h"

#define even 0
#define odd 1

// port e initialization Prototype
void PORTE_INIT () ;

int main(void) {
    struct state {
        uint8_t output ;
        uint32_t delay ;
        uint8_t next [2] ;
    };

    typedef struct state Stype ;

    // construct the structure that represent the finite state machine

    Stype fsm [2] = {
                     {0 , 100 , {even , odd}},
                     {1 , 100 , {odd , even}}
    } ;

    uint8_t Cstate = even ; // representing the current state
    uint32_t input ;

    PORTE_INIT() ;
    PLL_Init() ;




    while (1){
                /*GPIO_PORTE_DATA_R = fsm[Cstate].output << 2 ;
                SysTick_Delay_inSecond (2) ;
                input = (GPIO_PORTE_DATA_R & 0x01 ) ;
                Cstate = fsm[Cstate].next[input] ;*/
        GPIO_PORTE_DATA_R = 0x006 ;
        SysTick_Delay_10ms(50) ;
        GPIO_PORTE_DATA_R = 0x000 ;
        SysTick_Delay_10ms(50) ;


    }



	
	return 0;
}


void PORTE_INIT () {
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

void PLL_Init(void){
  // 0) Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USERCC2
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;
  // 4) set the desired system divider
  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (4<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
}
