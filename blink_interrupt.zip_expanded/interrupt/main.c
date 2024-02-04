
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
/**
 * main.c
 */


#define switch1 4
#define switch2 0

// led pins.
#define red_pin 1
#define blue_pin 2
#define green_pin 3

//color codes
//Green, Blue, Cyan, Red, Yellow, Magenta, White

#define green 8
#define blue 4
#define cyan 12
#define red 2
#define yellow 10
#define mangenta 6
#define white 14
#define maxdelay 2000

#define color_on(color) GPIO_PORTF_DATA_R = color
#define color_off GPIO_PORTF_DATA_R = 0x00

int color_code[7] = {green , blue , cyan , red , yellow , mangenta , white};
 int selected_color= 8;
 int counter=-1;

 int blink_delay =2000;
 int blink_speed = 0;



int main(void)

{


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

        }

    return 0;
}


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


                         if(blink_speed >=7){
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
