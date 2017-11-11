/*
 *
 * main.c
 */
#include <tm4c123gh6pm.h>
#include "SysTick_Init.h"

// preprocessor macros for the sensor PORTE0, PORTE1 and for the output light PORTB 0-5
#define sensor *((volatile uint32_t *) 0x4002400c)
#define light  *((volatile uint32_t *) 0x400050fc)

// prototype for the used functions
//---------------------------------

void PORTE_Init() ; // bit 0 and 1 are digital input (switches)
void PORTB_Init() ; // bit 5 to 0 are digital output
void PLL_Init() ;   // set the main osc and the 80 mega frequency

// constructing the linked data structure
struct state {
        uint16_t output ;
        uint32_t time ;
        uint32_t next [4] ;
        };

// define the new type (state type)
    typedef struct state Stype ;


// define a name for each state in the fsm
#define goN     0
#define waitN   1
#define goE     2
#define waitE   3



// array of states corresponding to the fsm
    Stype fsm [4] = {
                     {0x21, 200, {goN, waitN, goN, waitN}},
                     {0x22, 100, {goE, goE, goE, goE}},
                     {0x0c, 200, {goE, goE, waitE, waitE}},
                     {0x14, 100, {goN, goN, goN, goN}}
    };


int main(void) {
    PORTE_Init() ;
    PORTB_Init() ;
    PLL_Init() ;
    uint8_t CurrentState = goN; // the starting state for the first time
    uint32_t input ; // input variable to hold the data in (sensors)
    while(1){
        light = fsm[CurrentState].output ;
        SysTick_Delay_10ms(fsm[CurrentState].time) ;
        input = sensor ; // read sensors
        CurrentState = fsm[CurrentState].next[input] ; // state transition based on the input and C.state
    }
	
	return 0;
}


// PORTE INITIALIZATION
void PORTE_Init () {
    uint32_t delay ; // dummy delay for stable clk
    SYSCTL_RCGC2_R |= 0X10 ; // access the clock for port E
    delay = SYSCTL_RCGC2_R ; // wait for the clk to be stable
    GPIO_PORTE_DIR_R &= ~0X03 ; // bit 0 and 1 are output
    GPIO_PORTE_AMSEL_R &= ~0X03 ; // no analog input
    GPIO_PORTE_AFSEL_R &= ~0X03 ; // NO ALT FUNC
    GPIO_PORTE_PCTL_R &= ~0X0ff ; // no specific peripheral signal since AFSEL  is cleared
    GPIO_PORTE_DEN_R |= 0X03 ; // digital enable for the used bits

}

//PORTB INITIALIZATION
void PORTB_Init(){
    uint32_t delay ; // dummy delay for stable clk
    SYSCTL_RCGC2_R |= 0X2 ; // access the clock for port E
    delay = SYSCTL_RCGC2_R ; // wait for the clk to be stable
    GPIO_PORTB_DIR_R |= 0X03f ; // bit 0 and 1 are output
    GPIO_PORTB_AMSEL_R &= ~0X03f ; // no analog input
    GPIO_PORTB_AFSEL_R &= ~0X01f ; // NO ALT FUNC
    GPIO_PORTB_PCTL_R &= ~0X0ffffff ; // no specific peripheral signal since AFSEL  is cleared
    GPIO_PORTB_DEN_R |= 0X03f ; // digital enable for the used bits
}

//PLL initialization for 80 MEGA HERTZ
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
