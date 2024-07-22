/*
 * uart.h
 *
 *  Created on: Jan 27, 2024
 *      Author: kunal
 */

#ifndef UART_H_
#define UART_H_
char UART_InChar(void);
int mystringcompare(char [] , char []);
void print(char []);
void error();
void UART0_Transmitter(unsigned char );


// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

/* U0Rx receive connected to PA0 */
/* U0Tx transmit connected to PA1 */

#define UART_FR_TXFF            0x00000020  /* UART Transmit FIFO Full */
#define UART_FR_RXFE            0x00000010  /* UART Receive FIFO Empty */
#define UART_LCRH_WLEN_8        0x00000060  /* 8 bit word length */
#define UART_LCRH_FEN           0x00000010  /* UART Enable FIFOs */
#define UART_CTL_UARTEN         0x00000001  /* UART Enable */
#define SYSCTL_RCGC1_UART0      0x00000001  /* UART0 Clock Gating Control */
#define SYSCTL_RCGC2_GPIOA      0x00000001  /* port A Clock Gating Control */

void UART_Init(void);
char UART_InChar(void);
void UART_OutChar(char data);



#endif /* UART_H_ */
