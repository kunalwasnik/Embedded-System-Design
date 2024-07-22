#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "blink.h"
#include "uart.h"
#include "keypad.h"
#include "console.h"
#include "segment7.h"

#include "inc/tm4c123gh6pm.h"

#define spaces ( console_cmd_buffer[i] == ' ' || console_cmd_buffer[i] =='\r' || console_cmd_buffer[i]=='\t')

int debounce_button = 0;
int button_pressed=0;

int debounce_keypad = 0;
int button_keypad=0;

 int selected_color= 8;

 int counter=0;


 int cycles=0;

int device_pause = 0;
int device_stop = 0;
 int toggle_pause=0;
 int toggle_stop=0;

// 7segment
extern int segment_num[10] ;
extern int segment_frame[4];

// end 7segment

char* msg = "\n\rBlinky Remote\n\r";

char console_cmd_buffer[30];
char cmd[30] , cmd_argument[30] , cmd_argument2[30];;

int ci=0; // console index

char color[10];
int  blink_rate=0;


char lcd_msg[] = "Version 0.01! "; // 0x2000002c
char lcd_msg2[] = " Wake Up Neo!"; // //2000003b wake up neo string ,

char *address_for_lcd = 0x2000002c;

char *msg_ptr = lcd_msg;

unsigned long ul , num_bytes;
char *change_addr;
int temp=0;


// led , switch

// led_swed

int main(void)
{



    switch_led_init();
    UART_Init();
    keypad_init();

    lcd_config();
    lcd_init();
    //lcd_print(msg_ptr);
    lcd_addr_addr(address_for_lcd  , 12);


    init_7segment();






    print("Starting\n\r");






    while(1){






        while(device_pause ==1 || device_stop==1){
            delayMs(0);
        }


        color_on(selected_color);

        delayMs(blink_delay);



        while(device_pause ==1 || device_stop==1){
            delayMs(0);
        }

        color_off;
        delayMs(blink_delay);








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

    // debouncing logic
    if(button_pressed==1){
        debounce_button++;


        if(debounce_button > (1000*100) ){
            button_pressed=0;
            debounce_button=0;
            //print("not on");
        }
return;
    }

    // debouncing end;


        if ( (GPIO_PORTF_DATA_R & 0x10) !=0x10){
            button_pressed=1;

           // print("color change button pressed!\n");
            counter = counter + 1;
                              if(counter==7){
                                  counter=0;
                                  cycles++;

//                                  if(cycles>99)
//                                      cycles=0;
                              }


                              selected_color = color_code[counter];


                               //change segment display
                                  segment_frame[0] = 0;
                                  segment_frame[1] = counter+1;

                                  // for cycles
                                  int cycles_ones = cycles %10;
                                  int cycles_tens = cycles/10;

                                  segment_frame[2] = cycles_tens;
                                  segment_frame[3] = cycles_ones;

                               //end segment display code.

                                  // extra


                                    //GPIO_PORTF_DATA_R =0;

                     }


        else  if ( (GPIO_PORTF_DATA_R & 0x01) !=0x01){
            button_pressed=1;

            //print("Blink speed pressed\n");
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


                 }


}

char UART_InChar(void)
{
      while( (UART0_FR_R & UART_FR_RXFE) != 0)
          ;
      return((char)(UART0_DR_R & 0xFF));
}



void myUartHandler( void )
    {

unsigned char c = 0;
//c = UART0_DR_R; // get the received data byte
if( (UART0_FR_R & UART_FR_RXFE) != 0) return;
c = UART0_DR_R & 0xFF;

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
                //print("\nDevice Paused.");
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

        // lcd command
        else if (mystringcompare(cmd , "peek")==0){


                         sscanf(cmd_argument,"%lx", &ul);
                         sscanf(cmd_argument2,"%lx", &num_bytes);
                         msg_ptr = ul;
                          //2000002c
                         lcd_addr_addr(msg_ptr , num_bytes);
                         print("\n");
                         print(cmd_argument2);

                      }

        else if (mystringcompare(cmd , "poke")==0){
                            sscanf(cmd_argument,"%lx", &ul);
                           // sscanf(cmd_argument2,"%lx", &num_bytes);
                            change_addr = ul;

                            for( temp=0;cmd_argument2[temp]!='\0';temp++){
                                *(change_addr+temp) = cmd_argument2[temp];
                            }
                            *(change_addr+temp) = '\0';

                            //0x2000002c
                            lcd_addr_addr(address_for_lcd  , temp);

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


    segment_frame[1] = counter+1;





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
               GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
               GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
               GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));


               // trying interrupts


              /*  GPIO_PORTF_IM_R  |= ((1<<0)| (1<<4)); // enable interrupt.
                GPIO_PORTF_IS_R  &= (~(1<<0)| ~(1<<4)); //  edge triggered
                GPIO_PORTF_IEV_R &= (~(1<<0)| ~(1<<4)); // falling edge


                      NVIC_EN0_R |= (1<<30);*/
}




void console_pause(){
    device_pause = 1;
}
void console_resume(){
    device_pause = 0;
}
void console_start(){
    segment_frame[1] = 1;
    device_stop=0;
    device_pause=0;





  //  ResetISR();
}

void console_stop(){
    color_off;
    device_stop=1;
    device_pause=1;
    segment_frame[0] = 0;
      segment_frame[1] = 0;
      segment_frame[2] = 0;
      segment_frame[3] = 0;
      counter=1;
      selected_color=8;
      blink_delay =50;;
      blink_speed=0;
      cycles=0;
   // ResetISR();
}


// for keypad handling.

void gpioc_handler(void){
    check_keypad_sw();
    //GPIO_PORTC_ICR_R |= 0x01; // interrupt is cleared.

    GPIO_PORTC_ICR_R |= 0xFF; // interrupt is cleared.
}

void check_keypad_sw(){

    // debouncing logic
    if(button_keypad==1){
        debounce_keypad++;


        if(debounce_keypad > (1000*100) ){
            button_keypad=0;
            debounce_keypad=0;
            print("not on");
        }
return;
    }

    // debouncing end;


    int rows[4] = {ROW1_PIN, ROW2_PIN , ROW3_PIN , ROW4_PIN};

            int cols[4] = {COL1_PIN, COL2_PIN , COL3_PIN , COL4_PIN};


          if( (GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0]) ){

              // nothing is pressed.
              // make ith row 0 and all 1  and check column

              // switch 13

              GPIO_PORTE_DATA_R = ~(1<<rows[3]);

              if((GPIO_PORTC_DATA_R & (1<<cols[0])) !=(1<<cols[0])){
                  button_keypad=1;
                  print("Pressed Keypad Pause!");
                  if(toggle_pause==0){
                  toggle_pause=1;
                  console_pause();
                  //GPIO_PORTF_DATA_R = (1<<blue_pin);
                  }else {
                      toggle_pause=0;
                      console_resume();
                     // GPIO_PORTF_DATA_R = (1<<red_pin);
                  }

                  // pause switch
                // GPIO_PORTF_DATA_R = (1<<blue_pin);
              }



          }else if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1])){

                 // switch 14.


                          GPIO_PORTE_DATA_R = ~(1<<rows[3]);

                          if((GPIO_PORTC_DATA_R & (1<<cols[1])) !=(1<<cols[1]) ){
                              // reset switch.
                              button_keypad=1;
                              print("Pressed Keypad Reset!");
                              if(toggle_stop==0){
                                            console_stop();
                                            toggle_stop=1;
                                            }else {
                                                console_start();
                                                toggle_stop=0;
                                            }

                              //GPIO_PORTF_DATA_R = (1<<red_pin) | (1<<blue_pin) |(1<<green_pin);
                          }


          }

          GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));




}




//

void delayMs(int n)
{

    if(n==0){
        myUartHandler();
         myHandler();
         segment_display_refresh();
         check_keypad_sw();
    }
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {
    // change j <1500 to j<1000

    myUartHandler();
    myHandler();
    segment_display_refresh();
    check_keypad_sw();

    //debounce(5);


} /* do nothing for 1 ms */

}
// debounce delay.

void debounce(int n)
{

int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {
    // change j <1500 to j<1000

} /* do nothing for 1 ms */

}
