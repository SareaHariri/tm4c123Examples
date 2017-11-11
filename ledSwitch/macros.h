/*
 * macros.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: visitor
 */

#ifndef MACROS_H_
#define MACROS_H_




// #define for the 4 pins used
//--------------------------------
#define PUSH_BUTTON         *(uint32_t *) 0x40024004 // PE0 for the first push button
#define RLED                *(uint32_t *) 0x40024008 // PE1 for the Red led
#define YLED                *(uint32_t *) 0x40024010 // PE2 for the yellow led
#define PUSH_BUTTON2        *(uint32_t *) 0x40024020 // PE3 for the second pushButton





#endif /* MACROS_H_ */
