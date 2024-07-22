/*
 * keypad.c
 *
 *  Created on: Feb 2, 2024
 *      Author: kunal
 */


#include <stdint.h>
#include "keypad.h"
#include "inc/tm4c123gh6pm.h"

void keypad_init(){

    // port e row , set pins as output 0 1 2 3

    SYSCTL_RCGC2_R |= (1<<4); // enable clock for c port

    GPIO_PORTE_DIR_R |= ((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));
    GPIO_PORTE_DEN_R |= ((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));
    GPIO_PORTE_DATA_R &= ~((1<<ROW1_PIN)|(1<<ROW2_PIN)|(1<<ROW3_PIN)|(1<<ROW4_PIN));


    // port c column , pins : 4,5,6,7 set as input and pull up.
    SYSCTL_RCGC2_R |= (1<<2); // enable clock for c port


    GPIO_PORTC_DIR_R &= ~((1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN));
    GPIO_PORTC_DEN_R = ((1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN));
    GPIO_PORTC_PUR_R = ((1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN));




}

void keypad_interrupt(){
    GPIO_PORTC_IM_R  |= ((1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN));// enable interrupt.
    GPIO_PORTC_IS_R  &= ~( (1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN)); // edge triggered
    GPIO_PORTC_IEV_R &= ~((1<<COL1_PIN)|(1<<COL2_PIN)|(1<<COL3_PIN)|(1<<COL4_PIN));// falling edge
    NVIC_EN0_R |= (1<<2);



}






