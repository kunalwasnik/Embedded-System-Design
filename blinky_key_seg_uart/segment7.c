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
int segment_frame[4] = {0,1,0,0};

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
    GPIO_PORTB_DATA_R = segment_num[num];


}

void SysTick_init(){



    /* Configure SysTick */

    NVIC_ST_RELOAD_R = (16000000)/500; /* reload with number of clocks per second */
    NVIC_ST_CTRL_R = 7; /* enable SysTick interrupt, use system clock */
    NVIC_SYS_PRI3_R = 0xFFFFFFFF;
    //NVIC_SYS_PRI3_R |= (7<< 5)<<24;
    enable_irq();


}



void SysTick_Handler(void)
{

   segment_display_refresh();
}


void inline enable_irq(void)
{
__asm (" CPSIE I\n");
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


}




