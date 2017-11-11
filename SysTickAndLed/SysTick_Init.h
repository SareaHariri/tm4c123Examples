/*
 * SysTick_Init.h
 *
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
}

#endif /* SYSTICK_INIT_H_ */
