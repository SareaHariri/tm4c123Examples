/*
 * main.c
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include "SysTick_Init.h"


#define RLED                *(uint32_t *) 0x40024008 // PE1 for the Red led
#define YLED                *(uint32_t *) 0x40024010 // PE2 for the yellow led



void PORTE_INIT ();
void PLL_Init(void);
void SysTick_delay (uint32_t count );
void SysTick_delay_10ms (uint32_t ms);



int main(void) {
    PORTE_INIT() ;
    PLL_Init() ;

    while (1) {
        RLED = 0x02 ;
        SysTick_delay_10ms(10000);
        RLED = 0x00 ;
        SysTick_delay_10ms(10000);
        YLED = 0x04 ;
        SysTick_delay_10ms(10000) ;
        YLED = 0x00 ;
        SysTick_delay_10ms(10000) ;


    }
	return 0;
}


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

void SysTick_delay(uint32_t count){
        SysTick_Init(count);

        while (NVIC_ST_CTRL_R & 0x010000 == 0){
            // wait for the flag to be set ;
        }

}

void SysTick_delay_10ms (uint32_t ms){
    uint32_t i ;
    for (i = 0 ; i < ms ; i++){
        SysTick_delay (800000);
    }
}

void PLL_Init(void){
  // 0) Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USE RCC2
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
