#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "blink.h"
#include "uart.h"
#include "keypad.h"
#include "console.h"
#include "segment7.h"
#include "game.h"
#include "inc/tm4c123gh6pm.h"

#define spaces ( console_cmd_buffer[i] == ' ' || console_cmd_buffer[i] =='\r' || console_cmd_buffer[i]=='\t')
int selected_color;
int counter=0;
int cycles=0;

volatile int device_pause =0;
volatile int device_stop = 0;
int toggle_pause=0;
int toggle_stop=0;

// 7segment
extern int segment_num[10] ;
extern int segment_frame[4];


// end 7segment

char* msg = "\n\rBlinky Remote\n\r";

char (*status[3])[15]={"Timer Running..." , "Ready To Run" , "Timer Paused"};;
volatile int status_index = 0;

char console_cmd_buffer[30];
char cmd[30] , cmd_argument[30];

int ci=0; // console index

char color[10];
int  blink_rate=10;


// timer related

char str_timer[10];

extern volatile unsigned int stopwatch_timer;
// led , switch

// led_swed

// lcd related , tic tac

 char frame[3][3] = {{'.' , '.' , '.'},
                     {'.' , '.' , '.'},
                     {'.' , '.' , '.'}
                        };

int current_player = 0;

int game_row=-1 , game_col=-1;

char game_symbol[2] = {'X' , 'O'};

 int total_moves=0;
// lcd related end.



extern int blink_delay;
extern int blink_speed;
int main(void)
{



    switch_led_init();
    SysTick_init();
    UART_Init();
    keypad_init();
    keypad_interrupt();
    lcd_config();
    lcd_init();


    init_7segment();

    print("Starting\n\r");

    print_2d(frame);

    blink_delay = 1;
    lcd_print(status[1]);

    segment_frame[0] = 0;
    segment_frame[1] = 0;
    segment_frame[2] = 0;
    segment_frame[3] = 0;

    device_stop = 1;
    selected_color = green;

    while(1){


        color_on(selected_color);
        delayMs(1);

        while(device_stop==1){ delayMs(1);}
        color_off;
        delayMs(1);



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
    else{
        console_cmd_buffer[ci] = '\0';

        pre_process();

        if(mystringcompare(cmd , "color")==0){
            //print(cmd_argument);


            // check color
            if(device_pause !=1 ) {
                change_color(cmd_argument);

            }else{
                print("\nDevice Paused.");
            }




        }else if (mystringcompare(cmd , "blink")==0){

            change_blink_speed(cmd_argument);
        }

        else if (mystringcompare(cmd , "pause")==0){

                    console_pause();
                }

        else if (mystringcompare(cmd , "resume")==0){

                   console_resume();
                }


        else if (mystringcompare(cmd , "start")==0){

                    console_start();
                }

        else if (mystringcompare(cmd , "stop")==0){
                    console_stop();

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

    if(bspeed>0 && bspeed <=20){
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
               GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
               GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
               GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));


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
    blink_delay = 1;
    lcd_print(status[2]);
}
void console_resume(){
    selected_color = green;
       blink_delay = 0;

    status_index = 0;
    selected_color = green;
    device_pause = 0;
    device_stop = 0;

    lcd_print(status[0]);
}
void console_start(){

    status_index = 0;

    selected_color = green;
    blink_delay = 0;


    device_stop=0;
    device_pause=0;

    lcd_print(status[0]);



  //  ResetISR();
}

void console_stop(){

    selected_color = green;
    blink_delay = 1;

    status_index = 1;

    stopwatch_timer = 0;
    device_stop=1;
    device_pause=0;



      segment_frame[0] = 0;
      segment_frame[1] = 0;
      segment_frame[2] = 0;
      segment_frame[3] = 0;
      lcd_print(status[1]);

   // ResetISR();
}


// for keypad handling.

void gpioc_handler(void){
    check_keypad_sw();
    //GPIO_PORTC_ICR_R |= 0x01; // interrupt is cleared.

    GPIO_PORTC_ICR_R |= 0xFF; // interrupt is cleared.
}



int debounce_keypad =0 , keypad_pressed=0;


void check_keypad_sw(){


//    if(keypad_pressed==1){
//        debounce_keypad++;
//
//
//        if(debounce_keypad > (1000*100) ){
//            keypad_pressed=0;
//            debounce_keypad=0;
//            //print("not on");
//        }
//            return;
//    }
//
//    keypad_pressed=1;


     game_row=-1;
     game_col=-1;

    int rows[4] = {ROW1_PIN, ROW2_PIN , ROW3_PIN , ROW4_PIN};
    int cols[4] = {COL1_PIN, COL2_PIN , COL3_PIN , COL4_PIN};


   if( (GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0]) ){
               game_col = 0;
              // nothing is pressed.
              // make ith row 0 and all 1  and check column
              // switch 13

       GPIO_PORTE_DATA_R = ~(1<<rows[1]);
        if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){
           // print("Switch 5\n");
            game_row = 0;
            GPIO_PORTE_DATA_R = (1<<rows[1]);

         }

        GPIO_PORTE_DATA_R = ~(1<<rows[2]);
         if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){
             game_row = 1;

             GPIO_PORTE_DATA_R = (1<<rows[2]);

          }

              GPIO_PORTE_DATA_R = ~(1<<rows[3]);
              if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){
                  game_row = 2;

                  //print("Switch 13\n");

               }


          }else
              if( (GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1]) ){
                         // nothing is pressed.
                         // make ith row 0 and all 1  and check column
                         // switch 13
                  game_col = 1;

                  GPIO_PORTE_DATA_R = ~(1<<rows[1]);

                   if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){
                       //print("Switch 6\n");
                       game_row = 0;
                       GPIO_PORTE_DATA_R = (1<<rows[1]);

                    }

                   GPIO_PORTE_DATA_R = ~(1<<rows[2]);
                    if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){
                        //print("Switch 10\n");
                        game_row = 1;

                        GPIO_PORTE_DATA_R = (1<<rows[2]);

                     }

                         GPIO_PORTE_DATA_R = ~(1<<rows[3]);
                         if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){
                            // print("Switch 14\n");
                             game_row = 2;

                          }


            }else
                if( (GPIO_PORTC_DATA_R & (1<<cols[2])) !=(1<<cols[2]) ){
                           // nothing is pressed.
                           // make ith row 0 and all 1  and check column
                           // switch 13
                    game_col = 2;

                    GPIO_PORTE_DATA_R = ~(1<<rows[1]);
                     if((GPIO_PORTC_DATA_R & (1<<cols[2])) !=(1<<cols[2])){
                        // print("Switch 7\n");
                         game_row = 0;
                         GPIO_PORTE_DATA_R = (1<<rows[1]);

                      }

                     GPIO_PORTE_DATA_R = ~(1<<rows[2]);
                      if((GPIO_PORTC_DATA_R & (1<<cols[2])) !=(1<<cols[2])){
                          //print("Switch 11\n");
                          game_row = 1;

                          GPIO_PORTE_DATA_R = (1<<rows[2]);

                       }

                           GPIO_PORTE_DATA_R = ~(1<<rows[3]);
                           if((GPIO_PORTC_DATA_R & (1<<cols[2])) !=(1<<cols[2])){
                             //  print("Switch 15\n");
                               game_row = 2;

                            }


                       }










          GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));


          if(game_row==-1 || game_col==-1){
                 print("Invalid Move \n");
                 return;
             }


          if(frame[game_row][game_col] == '.'){
          frame[game_row][game_col] = game_symbol[current_player];
          }else{
              print("Already Played Move. Try Again \n");
                              return;

          }


          if(current_player == 0){

              current_player=1;
          }
          else{
              current_player=0;
          }
          print_2d(frame);




          GPIO_PORTE_ICR_R = 255;
          total_moves++;

          if(total_moves<3) return;

          if(check_win(game_symbol[0]) == 1) {
              print("Player 1 won \n");
              reset_game();

          }else if(check_win(game_symbol[1]) == 1){

              print("Player 2 won \n");
              reset_game();

          }
return;

}





void delay_timer(int n)
{


int i, j;

for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {

} /* do nothing for 1 ms */

}



void delayMs(int n)
{


int i, j;

for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {



   segment_display_refresh();



} /* do nothing for 1 ms */

}

