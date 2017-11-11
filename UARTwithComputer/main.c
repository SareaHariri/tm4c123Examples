/*
 * main.c
 */

#include <stdint.h>
#include <tm4c123gh6pm.h>

#define PF1 (*((volatile uint32_t *)0x40025008))


// clock configuation
void clk_Init() ;

// UART AND PORT CONFIGURATION
void UART_Init(void) ;

// UART outChar
void UART_outChar(uint8_t  data ) ;

// UART inChar .. return 8 bit data
uint8_t UART_inChar(void) ;

void printText(char * text) ;




int main(void) {
    UART_Init() ;
    // port f initialization to use the led
    SYSCTL_RCGCGPIO_R |= (1<<5) ; // set bit 5 for port f clocking
    GPIO_PORTF_DIR_R |= (1<<1) | (1<<2) | (1<<3) ; // bit 1, 2 and 3 as output (led)
    GPIO_PORTF_DEN_R |= (1<<1) | (1<<2) | (1<<3) ; // digital enable
    GPIO_PORTF_DATA_R &= ~0x0e ; // all leds are off at the beginning

    uint8_t c ;
    /*while(1){
       // UART_outChar('g');
        c = UART_inChar() ;
        if (c == 'r')
            GPIO_PORTF_DATA_R |= 0x02 ;
    }*/
    PF1 = 1 ;
    PF1 = (1<<1) ;
    PF1 = 0 ;
    PF1 = (1<<1) ;
    PF1 = 0 ;
    PF1 = 1 ;


    while(1){
        printText("please enter : r or b or g \n\n") ;
        c = UART_inChar() ;
        UART_outChar(c) ;

        switch (c){
        case 'r' :
            GPIO_PORTF_DATA_R = (1<<1) ;// red color
            break;
        case 'b' :
            GPIO_PORTF_DATA_R = (1<<2) ;// BLUE color
            break;
        case 'g' :
            GPIO_PORTF_DATA_R = (1<<3) ;// green color
            break;
        default :
            GPIO_PORTF_DATA_R &= ~((1<<1) | (1<<2) |(1<<3)) ;// LEDS ARE OFF
            break ;


        }


    }
	
	return 0;
}


void UART_Init(void) {
    //Enable the UART module using the RCGCUART register (see page 344).
    SYSCTL_RCGCUART_R |= (1<<0) ;

    //Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
    //To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
    SYSCTL_RCGCGPIO_R |= (1<<0) ;

    //Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
    //configure, see Table 23-4 on page 1344.
    GPIO_PORTA_AFSEL_R |= (1<<1) | (1<<0) ;

    //Configure the GPIO current level and/or slew rate as specified for the mode selected (see
    //page 673 and page 681).
    // NO CHANGE HERE

    //Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
    //pins (see page 688 and Table 23-5 on page 1351).
    GPIO_PORTA_PCTL_R |= (1<<0) | (1<<4) ;

    GPIO_PORTA_DEN_R |= (1<<0) | (1<<1) ; // DIGITAL ENABLE ..

    // HERE we setting the UART baud rate and configure the format of serial communications 8 bit, no parity ...etc

    //Disable the UART by clearing the UARTEN bit in the UARTCTL register.
    UART0_CTL_R &= ~(1<<0) ;

    //Write the integer portion of the BRD to the UARTIBRD register.
    UART0_IBRD_R = 104 ; // integer portion 104 in dec

    //Write the fractional portion of the BRD to the UARTFBRD register.
    UART0_FBRD_R = 11 ; // fractional portion 11 in dec

    //Write the desired serial parameters to the UARTLCRH register (in this case, a value of
    //0x0000.0060).
    UART0_LCRH_R |= (0<<7) | (0x3<<5) | (1<<4) | (0<<3); // no parity , 8 bit data , fifo enabled and on stop bit

    //Configure the UART clock source by writing to the UARTCC register.
    UART0_CC_R = 0x0 ; // system clk

    //Optionally, configure the microDMA channel (see “Micro Direct Memory Access ” on page 585)
    //and enable the DMA option(s) in the UARTDMACTL register.
    // NO CHANGE HERE

    //Enable the UART by setting the UARTEN bit in the UARTCTL register.

    UART0_CTL_R |= (1<<0) | (1<<8) | (1<<9) ;



}

void printText (char * text){

    while (*text){
        UART_outChar(*text) ;
        text++ ;
    }
}








// UART inChar .. return 8 bit data
uint8_t UART_inChar(void) {
    while ((UART0_FR_R & 0x10) != 0) {} // wait for the RXFE to be zero .. meaning fifo not empty, then read
    return (uint8_t)(UART0_DR_R & 0xff) ; // read data

}


// UART outChar
void UART_outChar(uint8_t  data ) {
    while ((UART0_FR_R & 0x20) != 0 ){}; // wait till the TXFF to be 0 .. meaning fifo not full, then write
    UART0_DR_R = data ; // write data on the 8 bit data register


}



void clk_Init(){
    SYSCTL_RCC_R &= ~0x30 ; // oscsrc is 00 meaning the main osc is selected
    SYSCTL_RCC_R |= (1<<11) ; // bypass is 1 .. meaning the system clk based on the main osc
    SYSCTL_RCC_R &= ~0x7800000 ; // sysdiv is 00  meaning the system clk divisor is 1

}
