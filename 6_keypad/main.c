
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "keypad.h"
#include "blink.h"
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

void check_keypad_sw();


 int selected_color= 8;
 int counter=0;



 unsigned int value;

 int main(void)
 {

       SYSCTL_RCGC2_R |= (1<<5);
       GPIO_PORTF_LOCK_R = 0x4C4F434B;
       GPIO_PORTF_CR_R = 0x01;
       GPIO_PORTF_LOCK_R = 0;



           GPIO_PORTF_DIR_R &= ~(1<<4)|~(1<<0);
           GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
           GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
           GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));

           GPIO_PORTF_IM_R  |= ((1<<0)| (1<<4)); // enable interrupt.
           GPIO_PORTF_IS_R  &= (~(1<<0)| ~(1<<4)); //  edge triggered
           GPIO_PORTF_IEV_R &= (~(1<<0)| ~(1<<4)); // falling edge
           NVIC_EN0_R |= (1<<30);

           keypad_init();
           keypad_interrupt();
           switch_led_init();


     while(1) {
         color_off;
              delayMs(blink_delay);
              color_on(color_code[5]);
              delayMs(blink_delay);


         delayMs(100);

     }
 }

 void switch_led_init(){

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
 }




void myHandler(void){


//   if( (GPIO_PORTF_DATA_R & 0x10) != 0x10){
//       /* read data from PORTF */
//
//
//
//
//
//       GPIO_PORTF_DATA_R = (1<<red_pin);
//
//   }
//
//
//   if( (GPIO_PORTF_DATA_R & 0x01) != 0x01){
//        /* read data from PORTF */
//        GPIO_PORTF_DATA_R = 0;
//
//    }




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

//     GPIO_PORTF_ICR_R |= 0x10;
//     GPIO_PORTF_ICR_R |= 0x01;


}

void gpioc_handler(void){

    int rows[4] = {ROW1_PIN, ROW2_PIN , ROW3_PIN , ROW4_PIN};
               int cols[4] = {COL1_PIN, COL2_PIN , COL3_PIN , COL4_PIN};
       GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));

             if( (GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0]) ){
                 // nothing is pressed.
                 // make ith row 0 and all 1  and check column

                 // switch 13

                 GPIO_PORTE_DATA_R = ~(1<<rows[3]);
                 if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){

                     // pause switch
                     GPIO_PORTF_DATA_R = (1<<blue_pin);
                 }



             }else if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){

                    // switch 14.


                             GPIO_PORTE_DATA_R = ~(1<<rows[3]);

                             if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1]) ){
                                 // reset switch.
                                 GPIO_PORTF_DATA_R = (1<<red_pin) | (1<<blue_pin) |(1<<green_pin);
                             }


             }


             GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));
             GPIO_PORTC_ICR_R |= 0xFF; // interrupt is cleared.

}

#if 0
void check_keypad_sw(){
    int rows[4] = {ROW1_PIN, ROW2_PIN , ROW3_PIN , ROW4_PIN};
            int cols[4] = {COL1_PIN, COL2_PIN , COL3_PIN , COL4_PIN};
    GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));

          if( (GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0]) ){
              // nothing is pressed.
              // make ith row 0 and all 1  and check column

              // switch 13

              GPIO_PORTE_DATA_R = ~(1<<rows[3]);
              if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){

                  // pause switch
                  GPIO_PORTF_DATA_R = (1<<blue_pin);
              }



          }else if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){

                 // switch 14.


                          GPIO_PORTE_DATA_R = ~(1<<rows[3]);

                          if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1]) ){
                              // reset switch.
                              GPIO_PORTF_DATA_R = (1<<red_pin) | (1<<blue_pin) |(1<<green_pin);
                          }


          }

          else{
              GPIO_PORTF_DATA_R = (1<<green_pin);

          }
         // GPIO_PORTE_DATA_R = ((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));
}
#endif



void delayMs(int n)
{

int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {} /* do nothing for 1 ms */
}
