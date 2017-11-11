/*
 * main.c
 */

#include<tm4c123gh6pm.h>
#include<stdint.h>
#include<SysTick_Init.h>




int main(void) {
    //asm(" cpsid i ") ;

    // setting the configurations for port f
    int dummy ;
    SYSCTL_RCGC2_R |= 0x20 ;
    dummy = SYSCTL_RCGC2_R ;
    GPIO_PORTF_DIR_R |= 0x0e ;
    GPIO_PORTF_DEN_R |= 0x0e ;

    // setting configurations for SysTick and interrupts
    NVIC_ST_CTRL_R = 0x00 ;             // disable during the set up
    NVIC_ST_RELOAD_R = 16000000 ;       // if 16M clk .. meaning 1 sec
    NVIC_ST_CURRENT_R = 0  ;            // write any value to start counting
    NVIC_ST_CTRL_R = 0x07 ;             // enable counter interrupt and choose the sys clk


    while (1){

        GPIO_PORTF_DATA_R |= 0x04 ; // blue

    }
	
	return 0;
}

void SysTick_Handler(){
    GPIO_PORTF_DATA_R ^= 0x02 ; // toggle the red led
}
