/*
 * 7segment.c
 *
 *  Created on: Jan 31, 2024
 *      Author: kunal
 */

#include <stdio.h>
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"
#include "segment7.h"

int segment_num[10] = {0x3F , 0x06  , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D,0x07,0x7F , 0x6F};
int segment_frame[4] = {0,0,0,0};
volatile unsigned int stopwatch_timer=0;
extern volatile int device_pause , device_stop;

volatile static int timer_value_1ms=(1600000 - 1);

void init_7segment(){


    // port b
    SYSCTL_RCGC2_R |= (1<<1); // enable port b clock;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTB_DEN_R = 0xFF;
    GPIO_PORTB_DATA_R=0;

    // port A;
    SYSCTL_RCGC2_R |= (1<<0); // enable port a clock;
    GPIO_PORTA_DIR_R |= 0xF0;
    GPIO_PORTA_DEN_R |= 0xF0;

   // GPIO_PORTA_DATA_R= 0;

}

void segment_display(int display_number , int num){

    int dpin[4] = {7,6,5,4};
    int display_pin = dpin[display_number];
    GPIO_PORTA_DATA_R =  (1<<display_pin);

    if(display_number ==2){
        GPIO_PORTB_DATA_R = (segment_num[num]| (1<<7));
    }else{


    GPIO_PORTB_DATA_R = segment_num[num];
    }



}

void SysTick_init(){



    /* Configure SysTick */

    //NVIC_ST_RELOAD_R = (16000-1); /* reload with number of clocks per second */
    NVIC_ST_RELOAD_R = (1600000 - 1);
    NVIC_ST_CTRL_R = 7; /* enable SysTick interrupt, use system clock */
    NVIC_SYS_PRI3_R = 0xFFFFFFFF;
    //NVIC_SYS_PRI3_R |= (7<< 5)<<24;
    enable_irq();


}



void SysTick_Handler(void)
{




    if(device_pause==1){
        return;
    }
    if(device_stop==1){

        return;
    }

    int temp;
    stopwatch_timer++;
    temp = stopwatch_timer;
    if(segment_frame[0]== 9 && segment_frame[1]== 9 && segment_frame[2]== 9 && segment_frame[3]== 9 ){
        stopwatch_timer=0;
    }

    segment_frame[3] = temp%10;
    temp = temp/10;
    segment_frame[2] = temp%10;
    temp = temp/10;
    segment_frame[1] = temp%10;
    temp = temp/10;
    segment_frame[0] = temp%10;




    //if(NVIC_ST_CURRENT_R == )


}


void inline enable_irq(void)
{
__asm (" CPSIE I\n");
}

void segment_display_refresh(){

    // update frame:


    // end update frame :
    static int segment_row=0 , segment_pin=0;

    segment_display(segment_pin,segment_frame[segment_row]);


    segment_row++;
    segment_pin++;

    if(segment_row==4)
        segment_row = 0;

    if(segment_pin==4)
          segment_pin = 0;

    delayMs_time(2);
}

void delayMs_time(int n)
{


int i, j;

for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {

} /* do nothing for 1 ms */
}


