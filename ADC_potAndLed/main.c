/*
 * this project is about monitoring the temp of the cpu
 * if the temperature of the processor is above 30 .. the led on the board will be red
 * otherwise it will be white
 *
 * main.c
 *
 */

#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <PORTF_Init.h>

void ADC_Init() ;



int main(void) {
    ADC_Init () ;
    PORTF_Init() ;
    uint32_t result ;
    uint32_t TEMP ;
    while (1){
        uint32_t counter = 0 ;
        while (counter++ != 800000) ;

        ADC0_PSSI_R |= 0x8 ;                   // start conversion
        while ((ADC0_RIS_R & 0x8) == 0) ;
        result = ADC0_SSFIFO3_R ;            // read the fifo
        ADC0_ISC_R &= ~(0x8) ;                 // clear the flag of completion
        TEMP = 147.5 - ( (75 * (3.3) * result) / 4096 ) ; // the  from the data sheet page 813



        if(TEMP > 30)
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R = 0 ) | 0x2 ;
        else
            GPIO_PORTF_DATA_R = 0xe  ;










    }
	
	return 0;
}


void ADC_Init(){
    int delay;
    // module initialization
    //----------------------
    //1. Enable the ADC clock using the RCGCADC register (see page 352).
    SYSCTL_RCGCADC_R  |= 0x01  ;         // enable adc0
    delay = SYSCTL_RCGCADC_R ;
    delay = SYSCTL_RCGCADC_R ;
    delay = SYSCTL_RCGCADC_R ;

    //2. Enable the clock to the appropriate GPIO modules via the RCGCGPIO register (see page 340).
    //To find out which GPIO ports to enable, refer to “Signal Description” on page 801.
    // no change since we use the internal temperature sensor

    //3. Set the GPIO AFSEL bits for the ADC input pins (see page 671). To determine which GPIOs to
    //configure, see Table 23-4 on page 1344.
    //4. Configure the AINx signals to be analog inputs by clearing the corresponding DEN bit in the
    //GPIO Digital Enable (GPIODEN) register (see page 682).
    //5. Disable the analog isolation circuit for all ADC input pins that are to be used by writing a 1 to
    //the appropriate bits of the GPIOAMSEL register (see page 687) in the associated GPIO block.

    // sample sequencer configuration
    //-------------------------------

    //1. Ensure that the sample sequencer is disabled by clearing the corresponding ASENn bit in the
    //ADCACTSS register.
    ADC0_ACTSS_R &= ~0x8 ;       // disable ss3

    //2. Configure the trigger event for the sample sequencer in the ADCEMUX register.
    ADC0_EMUX_R &= 0xf000 ;         // choosing the processor to trigger the adc

    //3. pwm setting if used as a trigger for adc

    //4. For each sample in the sample sequence, configure the corresponding input source in the
    //ADCSSMUXn register.

    ADC0_SSMUX3_R = 0 ;

    //5. For each sample in the sample sequence, configure the sample control bits in the corresponding
    //nibble in the ADCSSCTLn register.
    ADC0_SSCTL3_R |= 0xe ;           // temp sensor , raw interrupt asserted , this is the end of seq and no diff

    //7. Enable the sample sequencer logic by setting the corresponding ASENn bit in the ADCACTSS
    //register.

    ADC0_ACTSS_R |= 0x08 ;          // active the ss3


}
