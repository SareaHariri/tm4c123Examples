/*
 * SysTick_Init.h
 *      GPIO_PORTE_DATA_R = fsm[Cstate].output << 2 ;
        SysTick_Delay_inSecond (2) ;
        input = (GPIO_PORTE_DATA_R & 0x01 ) ;
        Cstate = fsm[Cstate].next[input] ;
 *  Created on: ??þ/??þ/????
 *      Author: visitor
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>


#ifndef SYSTICK_INIT_H_
#define SYSTICK_INIT_H_



void SysTick_Init(uint32_t count){
            NVIC_ST_CTRL_R = 0x00  ; // disable during the setup
            NVIC_ST_RELOAD_R = count -1 ; // the no of count to wait
            NVIC_ST_CURRENT_R = 0 ; // write any value to start counting
            NVIC_ST_CTRL_R = 0x05  ; // 0101 enable system tick with the system clk, no interrupts, enable the Systek
            while ((NVIC_ST_CTRL_R & 0x010000) == 0 ){}
}

// if the clk at 16 mega .. the 1600 loop means 1000 ms
void SysTick_Delay_10ms (uint32_t second){
    int i ;
    for (i = 0 ; i<second ; i++)
    SysTick_Init (800000);

}

#endif /* SYSTICK_INIT_H_ */
