#include <tm4c123gh6pm.h>
#include "PORTF_Init.h"



void PORTF_Init(){
    // setting the configurations for port f
        int dummy ;
        SYSCTL_RCGC2_R |= 0x20 ;
        dummy = SYSCTL_RCGC2_R ;
        GPIO_PORTF_DIR_R |= 0x0e ;
        GPIO_PORTF_DEN_R |= 0x0e ;
}
