#include <stdio.h>
#include <stdint.h>

#include "blink.h"
#include "uart.h"
#include "inc/tm4c123gh6pm.h"

#define spaces ( console_cmd_buffer[i] == ' ' || console_cmd_buffer[i] =='\r' || console_cmd_buffer[i]=='\t')


char* msg = "\n\rBlinky Remote\n\r";

char console_cmd_buffer[30];
char cmd[30] , cmd_argument[30];

int ci=0; // console index

char color[10];
int  blink_rate=0;


// led , switch

// led_swed

int main(void)
{
    UART_Init();

    unsigned int value;
        SYSCTL_RCGC2_R |= (1<<5);
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
           GPIO_PORTF_CR_R = 0x01;
           GPIO_PORTF_LOCK_R = 0;



            GPIO_PORTF_DIR_R &= ~(1<<4)|~(1<<0);
            GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
            GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
            GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));


            // trying interrupts


                  GPIO_PORTF_IM_R  |= ((1<<0)| (1<<4)); // enable interrupt.
                  GPIO_PORTF_IS_R  &= (~(1<<0)| ~(1<<4)); //  edge triggered
                  GPIO_PORTF_IEV_R &= (~(1<<0)| ~(1<<4)); // falling edge


                   NVIC_EN0_R |= (1<<30);



          // end trying.





    while(1){

        color_on(selected_color);
        delayMs(blink_delay);
        color_off;
        delayMs(blink_delay);

    char c;
    ci=0;

        print(msg);

    do {
        c= UART_InChar();   /* receive char */



        if(c >= 'A' && c <= 'Z') {
              c =c + 32;
           }


        console_cmd_buffer[ci++]= c;

        UART_OutChar(c);     /* echo received char */

    }while(c!='\r');

    console_cmd_buffer[ci] = '\0';


    // pre process string stored in cmd , cmd_argument;
    pre_process();

//   print("\n\rAfter preprocessing command : \n\r");
//
//   print(cmd);
//
//   print("\n\rAfter preprocessing command argument : \n\r");
//   print(cmd_argument);
//   //printf("\n");

    if(mystringcompare(cmd , "color")==0){

        print("GOOD");
    }else if (mystringcompare(cmd , "blink")==0){
        print(cmd_argument);
    }else{
        print("command help");

    }





}

}



void pre_process(){

    // remove start spaces;
    int j=0,i=0;
    int fc=0; // first character
    // remove initial spaces
    while(spaces) i++;
    for( j=0;j<ci;j++){

        if(spaces){

            break;
        }
        cmd[j] = console_cmd_buffer[i++];
    }

    cmd[j]= '\0';

    // middle spaces
  while(spaces) i++;

    for( j=0;j<ci;j++){

        if(spaces){
            break;
        }

        cmd_argument[j] = console_cmd_buffer[i++];

    }
cmd_argument[j]= '\0';
}




/* UART_Init
* Initialize the UART for 115,200 baud rate (assuming 16 MHz bus clock),
* 8 bit word length, no parity bits, one stop bit, FIFOs enabled
* Input: none
* Output: none
*/


void myHandler(void){

     if (GPIO_PORTF_MIS_R & 0x10){
                         // switch 1

                 counter = counter + 1;
                   if(counter==7){
                       counter=0;
                   }

                   selected_color = color_code[counter];



                         //GPIO_PORTF_DATA_R =0;

                          GPIO_PORTF_ICR_R = 0x10;


                     }


     else if(GPIO_PORTF_MIS_R & 0x01){

                         if(blink_speed==7){
                             blink_delay = 0;
                             blink_speed++;

                         }

                         else if(blink_speed >=7){
                                        blink_speed = 1;
                                        blink_delay = maxdelay;

                                  }else{
                                      blink_speed++;
                                      blink_delay = blink_delay/2;


                                  }

       // GPIO_PORTF_DATA_R = (1<<3);
            // switch 2

                 GPIO_PORTF_ICR_R = 0x01;

                 }


}


void delayMs(int n)
{

int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {} /* do nothing for 1 ms */
}

