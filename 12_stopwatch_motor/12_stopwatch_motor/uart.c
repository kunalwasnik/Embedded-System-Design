/*
 * blink.c
 *
 *  Created on: Jan 27, 2024
 *      Author: kunal
 */
#include <stdio.h>
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"



void UART_Init(void)
{

      SYSCTL_RCGCUART_R |= 0x01;            /* activate UART0 */
      SYSCTL_RCGCGPIO_R |= 0x01;            /* activate port A */

      while((SYSCTL_PRGPIO_R&0x0001) == 0){}; /* ready? */

      UART0_CTL_R &= ~UART_CTL_UARTEN;      /* disable UART */

      UART0_CC_R = 0;
      UART0_IBRD_R = 8;        /* IBRD = int(16,000,000 / (16 * 115,200)) = int(8.680) */
      UART0_FBRD_R = 44;       /* FBRD = round(0.5104 * 64 ) = 44 */
                               /* 8 bit word length (no parity bits, one stop bit, FIFOs) */
      UART0_LCRH_R = (UART_LCRH_WLEN_8);
      UART0_CTL_R |= UART_CTL_UARTEN;       /* enable UART */

      GPIO_PORTA_AFSEL_R |= 0x03;           /* enable alt funct on PA1-0 */
      GPIO_PORTA_DEN_R |= 0x03;             /* enable digital I/O on PA1-0 */
      GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011; /* configure PA1-0 as UART */
      GPIO_PORTA_AMSEL_R &= ~0x03;          /* disable analog functionality on PA */

      // interrupts
      UART0_IM_R=(1<<4); //enable UART Rx interrupt
      UART0_ICR_R &= ~(0x010); // Clear receive interrupt

      NVIC_EN0_R |= (1<<5);
      //NVIC_SYS_PRI1_R |= (4<<5)<<8;
      //NVIC_SYS_PRI1_R = 0xFFFF00FF;


      //
}

/* UART_InChar
* Wait for new serial port input
* Input: none
* Output: ASCII code for key typed
*/
char UART_InChar(void)
{
      while( (UART0_FR_R & UART_FR_RXFE) != 0)
          ;
      return((char)(UART0_DR_R & 0xFF));
}



void UART0_Transmitter(unsigned char data)
{
    while((UART0_FR_R & UART_FR_TXFF) != 0); /* wait until Tx buffer not full */
    UART0_DR_R = data;                  /* before giving it another byte */
}








/* UART_OutChar
* Output 8-bit to serial port
* Input: letter is an 8-bit ASCII character to be transferred
* Output: none
*/
void UART_OutChar(char data)
{
      while((UART0_FR_R & UART_FR_TXFF) != 0)
          ;
      UART0_DR_R = data;
}



void error(){
    char *errormsg = "\n\rcommand help\n\r";
    while( *errormsg)
            UART_OutChar(*errormsg++);

}

void print(char word[]){
    int i=0;
    while(word[i]!='\0'){
        UART_OutChar(word[i++]);
    }



}
void print_num(int n){

char a[20];
sprintf(a,"%d\n\0" , n);
print(a);


}

int mystringcompare(char a[] , char b[]){

// flag 0 means equal.
int i=0 , j=0;
int flag=0;
int max=0;

while(a[i]!='\0'){
    i++;
}
max = i;
i=0;
while(b[i]!='\0'){
    i++;
}

if(i>max) max =i;

i=0;

while(a[i]!='\0' && b[i]!='\0'){
    if(a[i]!=b[i])
        return 1;
    i++;


}

if(i!=max) return 1;

return 0;
}


