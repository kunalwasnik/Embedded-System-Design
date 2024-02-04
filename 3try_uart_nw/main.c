

#include "inc/tm4c123gh6pm.h"

/**
 * main.c
 */

void uart_init(){

    SYSCTL_RCGCUART_R |= 0x01;            /* activate UART0 */
    SYSCTL_RCGCGPIO_R |= 0x01;            /* activate port A */

    while((SYSCTL_PRGPIO_R&0x0001) == 0){}; /* ready? */


    // disable uart by setting uarten bit to 0;
    UART0_CTL_R &= ~(1<<0);
    UART0_IBRD_R = 8;
    UART0_FBRD_R = 44;
    UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);

    UART0_CTL_R |= UART_CTL_UARTEN;
    GPIO_PORTA_AFSEL_R |= 0x03;           /* enable alt funct on PA1-0 */
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_PCTL_R = 0x00000011;



}


int main(void)
{

    uart_init();




	return 0;
}
