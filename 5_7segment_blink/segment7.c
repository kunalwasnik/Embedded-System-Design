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

void init_7segment(){


    // port b
    SYSCTL_RCGC2_R |= (1<<1); // enable port b clock;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTB_DEN_R = 0xFF;
    GPIO_PORTB_DATA_R=0;

    // port A;
    SYSCTL_RCGC2_R |= (1<<0); // enable port a clock;
    GPIO_PORTA_DIR_R = 0xFF;
    GPIO_PORTA_DEN_R = 0xF0;

    GPIO_PORTA_DATA_R= 0;

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

