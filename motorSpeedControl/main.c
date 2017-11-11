/*
 *
 * main.c
 * this program is about controlling the digital output on PA5
 * to form a pwm output increasing and decreasing by switches on the board
 *
 * Author: Sarea Al Hariri
 *
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <PLL_Init.h>


uint32_t H, L ;


void MotorInit(void) ;
void SwitchInit(void) ;




nt main(void) {
    asm(" cpsid i ") ;
    PLL_Init() ;
    MotorInit();
    SwitchInit();
    while (1){
        asm(" cpsie i ") ;
    }

	
	return 0;
}


void MotorInit(void) {
    SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
      L = H = 40000;                // 50%
      GPIO_PORTA_AMSEL_R &= ~0x20;      // disable analog functionality on PA5
      GPIO_PORTA_PCTL_R &= ~0x00F00000; // configure PA5 as GPIO
      GPIO_PORTA_DIR_R |= 0x20;     // make PA5 out
      GPIO_PORTA_DR8R_R |= 0x20;    // enable 8 mA drive on PA5
      GPIO_PORTA_AFSEL_R &= ~0x20;  // disable alt funct on PA5
      GPIO_PORTA_DEN_R |= 0x20;     // enable digital I/O on PA5
      GPIO_PORTA_DATA_R &= ~0x20;   // make PA5 low
      NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
      NVIC_ST_RELOAD_R = L-1;       // reload value for 500us
      NVIC_ST_CURRENT_R = 0;        // any write to current clears it
      NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
      NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
    //* configuring the PA5 as a digital output

    uint32_t delay ;
    H = L = 40000 ;
    SYSCTL_RCGCGPIO_R |= 0x01 ; // Enable and provide a clock to GPIO Port A in Run mode.
    delay = SYSCTL_RCGCGPIO_R ;
    GPIO_PORTA_DIR_R |= 0x020 ; // PA5 is output
    GPIO_PORTA_DR8R_R |= 0x20 ; // enable 8 mA drive on PA5
    GPIO_PORTA_DEN_R |= 0x020 ; // digital output

    // configuring the system tick timer

    NVIC_ST_CTRL_R = 0x00 ; // disable the Systick during configurations
    NVIC_ST_RELOAD_R = L - 1 ;
    NVIC_ST_CURRENT_R = 0x00 ; // any value to clear it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
    NVIC_ST_CTRL_R = 0x07 ; // system clk and interrupt enable*//

}

void SysTick_handler (void) {
    if ((GPIO_PORTA_DATA_R & 0x020) == 0x020 ){ // if high
        GPIO_PORTA_DATA_R &= ~0x020 ;       // turn off
        NVIC_ST_RELOAD_R = L -1;              // and reload the last value of L
    }
    else{
        GPIO_PORTA_DATA_R |= 0x020 ; // else .. turn it high
        NVIC_ST_RELOAD_R = H -1;       // and reload the last value of H
    }


}


void SwitchInit(void) {
    int delay ;
    SYSCTL_RCGCGPIO_R |= 0x20  ; // clock to port F
    delay = SYSCTL_RCGCGPIO_R ;
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
    GPIO_PORTF_DIR_R &= ~0x11 ; // PF0 and PF4 are input
    GPIO_PORTF_PUR_R |= 0x11 ; // pull up resistors for both
    GPIO_PORTF_DEN_R |= 0x11 ;  // digital out enable
    GPIO_PORTF_IS_R &= ~0x11 ;  // edge interrupt
    GPIO_PORTF_IBE_R &= ~0x11;  // not both edges
    GPIO_PORTF_IEV_R &= ~0x11;  // falling edges
    GPIO_PORTF_ICR_R |= 0x11 ;  // clear flages
    GPIO_PORTF_IM_R |= 0x011 ;  // enable
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xff00ffff) | 0x00400000 ; // set priority to 2
    NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC


}

void PortF_handler(void)
{
        if ((GPIO_PORTF_RIS_R & 0x10) == 0x10) {
            GPIO_PORTF_ICR_R = 0x10 ; // clear flag
            if (L < 72000) L= L + 8000 ;

        }

        if((GPIO_PORTF_RIS_R & 0x01 ) == 1) {
            GPIO_PORTF_ICR_R = 0x01 ; // clear flag
            if (L > 8000) L = L-8000 ;
        }



        H = 80000-L ;


}

