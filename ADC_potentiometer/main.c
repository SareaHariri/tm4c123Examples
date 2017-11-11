/*
 * main.c
 *   
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <SysTick_Init.h>
#include <PORTF_Init.h>

void ADC_Init() ;

int main(void) {
    uint32_t result ;
    ADC_Init() ;
    PORTF_Init() ;
    while(1){
    ADC0_PSSI_R |= 0x08 ; // initiate the conversion
    while((ADC0_RIS_R & 0x8) == 0 ) ;
    result = ADC0_SSFIFO3_R ;
	ADC0_ISC_R = 0x0008; // dont forget this acknowledge please 
    if (result>2500)
        GPIO_PORTF_DATA_R |= 0x02 ; // red led output
    else
        GPIO_PORTF_DATA_R = 0 ; // clear
    }

	return 0;
}


void ADC_Init(){
    int delay  ;
            //1. Enable the ADC clock using the RCGCADC register (see page 352).
            SYSCTL_RCGCADC_R |= 0x01 ;
            delay = SYSCTL_RCGCADC_R ;
            //2. Enable the clock to the appropriate GPIO modules via the RCGCGPIO register (see page 340).
            //To find out which GPIO ports to enable, refer to “Signal Description” on page 801.
            SYSCTL_RCGCGPIO_R |= (1<<5) ; // EN port E to use pe2
            delay = SYSCTL_RCGCGPIO_R ;
            GPIO_PORTE_DIR_R &= ~0x02 ;
            //3. Set the GPIO AFSEL bits for the ADC input pins (see page 671). To determine which GPIOs to
            //configure, see Table 23-4 on page 1344.
            GPIO_PORTE_AFSEL_R |= (1<<2) ;
            //4. Configure the AINx signals to be analog inputs by clearing the corresponding DEN bit in the
            //GPIO Digital Enable (GPIODEN) register (see page 682).
            GPIO_PORTE_DEN_R &= ~(0x04) ;
            //5. Disable the analog isolation circuit for all ADC input pins that are to be used by writing a 1 to
            //the appropriate bits of the GPIOAMSEL register (see page 687) in the associated GPIO block.
            GPIO_PORTE_AMSEL_R |= (1<<2) ;
            //6. If required by the application, reconfigure the sample sequencer priorities in the ADCSSPRI
            //register. The default configuration has Sample Sequencer 0 with the highest priority and Sample
            //Sequencer 3 as the lowest priority.
            //ADC0_SSPRI_R &= ~(0x3000) ; // highest priority
            ADC0_SSPRI_R = 0x123 ;
            //Sample Sequencer Configuration
            //------------------------------
            //1. Ensure that the sample sequencer is disabled by clearing the corresponding ASENn bit in the
            //ADCACTSS register
            ADC0_ACTSS_R &= ~0x08 ;
            //2. Configure the trigger event for the sample sequencer in the ADCEMUX register.
            ADC0_EMUX_R &= ~(0xf000) ;
            //For each sample in the sample sequence, configure the corresponding input source in the
            //ADCSSMUXn register.
            ADC0_SSMUX3_R &= ~0x000f  ;
            ADC0_SSMUX3_R += 1   ;
            //For each sample in the sample sequence, configure the sample control bits in the corresponding
            //nibble in the ADCSSCTLn register
            ADC0_SSCTL3_R |= 0x06 ;
            //6. If interrupts are to be used, set the corresponding MASK bit in the ADCIM register.
            //ADC0_IM_R |= 0x08 ;
            //7. Enable the sample sequencer logic by setting the corresponding ASENn bit in the ADCACTSS
            //register.
            ADC0_ACTSS_R |= 0x08 ;
}
