
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"
/**
 * main.c
 */


// 7 segnemtn start

extern int segment_num[10] ;




// 7 segment end;


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
 int counter=0;

int blink_delay =2000;
 int blink_speed = 0;

 void init_led(){

     SYSCTL_RCGC2_R |= (1<<5);
        GPIO_PORTF_LOCK_R = 0x4C4F434B;
           GPIO_PORTF_CR_R = 0x01;
           GPIO_PORTF_LOCK_R = 0;



            GPIO_PORTF_DIR_R &= ~(1<<4)|~(1<<0);
            GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
            GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
            GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));

 }


extern int segment_frame[4];
int main(void)

{
    SYSCTL_RCGC2_R |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2, 1) as output */
    GPIO_PORTF_DIR_R = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIO_PORTF_DEN_R = 0x0E;

    unsigned int value;
   // init_led();
    SysTick_init();
    init_7segment();
    for(int i=0;i<10;i++){
    segment_frame[0] = i;
    segment_frame[1] = i;
    segment_frame[2] = i;
    segment_frame[3] = i;
    delayMs(1000);
    }





        // trying interrupts

//
//              GPIO_PORTF_IM_R  |= ((1<<0)| (1<<4)); // enable interrupt.
//              GPIO_PORTF_IS_R  &= (~(1<<0)| ~(1<<4)); //  edge triggered
//              GPIO_PORTF_IEV_R &= (~(1<<0)| ~(1<<4)); // falling edge
//
//
//               NVIC_EN0_R |= (1<<30);
//


              // end trying.


//        while(1){
//            color_on(selected_color);
//            delayMs(blink_delay);
//            color_off;
//            delayMs(blink_delay);
//        }

   // GPIO_PORTA_DATA_R =  (1<<4) | (1<<5) | (1<<6) | (1<<7);

    while(1){

    }



    return 0;
}

void segment_display_refresh(){

    static int segment_row=0 , segment_pin=0;

    segment_display(segment_pin,segment_frame[segment_row]);


    segment_row++;
    segment_pin++;

    if(segment_row==4)
        segment_row = 0;

    if(segment_pin==4)
          segment_pin = 0;

//       segment_display(1,2);
//       delayMs(50);
////       segment_display(2,8);
////       delayMs(10);
////       segment_display(3,2);
////       delayMs(10);


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
