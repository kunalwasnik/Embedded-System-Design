#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "blink.h"
#include "uart.h"
#include "keypad.h"
#include "console.h"
#include "segment7.h"
#include "adc_pwm.h"

#include "inc/tm4c123gh6pm.h"

#define spaces ( console_cmd_buffer[i] == ' ' || console_cmd_buffer[i] =='\r' || console_cmd_buffer[i]=='\t')
int selected_color= 8;
int counter=0;
int cycles=0;

volatile int device_pause = 0;
volatile int device_stop = 0;
volatile  int count_down=0;
int toggle_pause=0;
int toggle_stop=0;
volatile int value_set_count_down = 0;


// 7segment
extern int segment_num[10] ;
extern int segment_frame[4];

// end 7segment

char* msg = "\n\rBlinky Remote\n\r";

char (*status[3])[15]={"Running..." , "Stopped" , "Paused"};;
volatile int status_index = 0;

char console_cmd_buffer[30];
char cmd[30] , cmd_argument[30] , cmd_argument2[30];

int ci=0; // console index

char color[10];
int  blink_rate=0;


// timer related

char str_timer[10];
volatile int adc_value = Vref*1000-1;
volatile float mvolt;
volatile int mvolt_int;
volatile int pot_value , pot_display_value;

volatile int x = 0 , prev_adc=0 , adc_change=0;


extern volatile  int stopwatch_timer;

// led , switch

// led_swed


int main(void)
{



    switch_led_init();
    SysTick_init();
    UART_Init();
   // keypad_init();
   //keypad_interrupt();

    //lcd_config();
   // lcd_init();




  init_adc();

  init_7segment();
  init_pwm();

    print("Starting\n\r");
    print("Options:timer set value, timer stop , timer pause , timer start , timer resume \n");
    blink_delay = 100;


    // adc pwm variables

    // adc pwm variables end.
    char str[80];

  //  GPIO_PORTF_DATA_R = (1<<green_pin);


    while(1){


                ADC0_PSSI_R |= 8; /* start a conversion sequence 3 */
                while((ADC0_RIS_R & 8) == 0)
                ; /* wait for conversion complete */
                pot_value = ADC0_SSFIFO3_R; /* read conversion result */
                ADC0_ISC_R = 8; /* clear completion flag */



                mvolt=(((pot_value)/4095.0)* (4999.0));

                mvolt_int=(int)mvolt;
                x=(5*1000-1)-mvolt_int;

                int error = prev_adc - pot_value;
                if(error<0) error = error*-1;

                if(error<20){
                    adc_change = 0;
                    pot_display_value = prev_adc;

                }else{
                    adc_change=1;
                    prev_adc = pot_value;

                }

                if(mvolt_int<1500){
                                   PWM1_3_CMPA_R=4999;
                                   //GPIO_PORTF_DATA_R=0;
//                                   sprintf(str, "Motor is Stopped.  %d milivolt\n", mvolt_int);
//                                   print(str);

                              }
                                  else{
                                   GPIO_PORTF_DATA_R =  14;
                                   PWM1_3_CMPA_R=x;
//                                   sprintf(str, "Current mvolt :  %d\n", mvolt_int);
//                                   print(str);
//                                   sprintf(str, "Potentiometer Value : %d\n", pot_value);
//                                   print(str);
//                                   sprintf(str, "x: %d\n", x);
//                                   print(str);
                               }




//        sprintf(str, "potval: %d\n", pot_value);
//        print(str);
        segment_display_refresh();




    }

}



void pre_process(){

    // remove start spaces;
    int j=0,i=0;

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

// for second argument.
while(spaces) i++;

  for( j=0;j<ci;j++){

      if(spaces){
          break;
      }

      cmd_argument2[j] = console_cmd_buffer[i++];

  }
cmd_argument2[j]= '\0';






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





                         if(device_pause==1){
                         console_resume();
                         }
                         else{
                             console_pause();
                         }


                   // end segment display code.


                         //GPIO_PORTF_DATA_R =0;

                          GPIO_PORTF_ICR_R = 0x10;


                     }


     else if(GPIO_PORTF_MIS_R & 0x01){


         if(device_stop==0){
         console_stop();

         }
         else{
             console_start();
         }


                 GPIO_PORTF_ICR_R = 0x01;

     }


}

void myUartHandler( void )
{
    char temp_str[30];

    UART0_ICR_R &= ~(0x010); // Clear receive interrupt
    unsigned char c = 0;
    c = UART0_DR_R; // get the received data byte


    // send data that is received
    UART0_Transmitter(c);
    // backspace
    if(c==127){
                //print("-1");
                //print("\n\r Check \n\r");
                //print(console_cmd_buffer);

                ci--;
                if(ci==-1)
                    ci=0;


            }

    // backspace end.


    else if((c!='\r')){

        if(c >= 'A' && c <= 'Z') {
                     c =c + 32;
                  }


    console_cmd_buffer[ci++]= c;
    }

    // enter press.
    else{
            console_cmd_buffer[ci] = '\0';

            pre_process();

            if(mystringcompare(cmd , "color")==0){

                // check color
                if(device_pause !=1 ) {
                    change_color(cmd_argument);

                }else{
                    print("\nDevice Paused.");
                }


            }else if(mystringcompare(cmd , "timer")==0) {

                if (mystringcompare(cmd_argument , "set")==0){

                    count_down =atoi(cmd_argument2);
                    // convert to millisecond
                    value_set_count_down = 1;
                    count_down = count_down * 1000;

//
//                    sprintf(temp_str , "Timer set value : %d \n" , count_down);
//                    print(temp_str);
                    }




            else if (mystringcompare(cmd_argument , "pause")==0){

                            console_pause();
                        }

                else if (mystringcompare(cmd_argument , "resume")==0){

                           console_resume();
                        }


                else if (mystringcompare(cmd_argument , "start")==0){

                            console_start();
                        }

                else if (mystringcompare(cmd_argument , "stop")==0){
                            console_stop();

                        }

                else{

                        print("command help");

                       }

            }

            else{
                print("command help");

            }


            ci = 0;
            print("\n\r");

    }
}

void change_color(char color[]){

    if(mystringcompare(color , "blue")==0){
        selected_color = blue;
        counter=1;
    }
    else if(mystringcompare(color , "red")==0){
        selected_color = red;
        counter = 3;
    }

    else if(mystringcompare(color , "cyan")==0){
           selected_color = cyan;
           counter = 2;
    }
    else if(mystringcompare(color , "yellow")==0){
           selected_color = yellow;
           counter = 4;
    }
    else if(mystringcompare(color , "magenta")==0){
           selected_color = magenta;
           counter =5;
    }
    else if(mystringcompare(color , "white")==0){
               selected_color = white;
               counter =7;
    }
    else if(mystringcompare(color , "green")==0){
               selected_color = green;
               counter =0;
    }
    else
        print("Enter valid color\n\r");




}

void change_blink_speed(char speed[]){


    int bspeed = 0;
        for (int i = 0; speed[i] != '\0'; i++) {
            bspeed = bspeed * 10 + (speed[i] - '0');
        }

    if(bspeed>=0 && bspeed <=20){
    blink_delay = 1000/bspeed;
    }else
        print("enter valid speed!\n\r");

    return;

}
void switch_led_init(){

              SYSCTL_RCGC2_R |= (1<<5);
              GPIO_PORTF_LOCK_R = 0x4C4F434B;
              GPIO_PORTF_CR_R = 0x01;
              GPIO_PORTF_LOCK_R = 0;



               GPIO_PORTF_DIR_R &= ~(1<<4)|~(1<<0);
               GPIO_PORTF_DIR_R |= ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
               GPIO_PORTF_DEN_R |= ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
               GPIO_PORTF_PUR_R |= ((1<<switch1) | (1<<switch2));


               // trying interrupts


                GPIO_PORTF_IM_R  |= ((1<<0)| (1<<4)); // enable interrupt.
                GPIO_PORTF_IS_R  &= (~(1<<0)| ~(1<<4)); //  edge triggered
                GPIO_PORTF_IEV_R &= (~(1<<0)| ~(1<<4)); // falling edge


                NVIC_EN0_R |= (1<<30);
}




void console_pause(){


    status_index = 1;

    device_pause = 1;
    selected_color = blue;
    // lcd_print(status[1]);
}
void console_resume(){

    status_index = 0;
    selected_color = green;
    device_pause = 0;
    // lcd_print(status[0]);
}
void console_start(){

    status_index = 0;

    selected_color = green;


    device_stop=0;
    device_pause=0;

    // lcd_print(status[0]);



  //  ResetISR();
}

void console_stop(){

    status_index = 1;
    selected_color = blue;

    stopwatch_timer = 0;
    device_stop=1;
    device_pause=1;


    segment_frame[0] = 0;
      segment_frame[1] = 0;
      segment_frame[2] = 0;
      segment_frame[3] = 0;
      lcd_print(status[1]);

   // ResetISR();
}


// for keypad handling.

void gpioc_handler(void){
    //check_keypad_sw();
    //GPIO_PORTC_ICR_R |= 0x01; // interrupt is cleared.

    GPIO_PORTC_ICR_R |= 0xFF; // interrupt is cleared.
}






//

void delayMs(int n)
{


int i, j;

for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {



    //segment_display_refresh();



} /* do nothing for 1 ms */

}

