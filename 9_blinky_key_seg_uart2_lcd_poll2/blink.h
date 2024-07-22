/*
 * blink.h
 *
 *  Created on: Jan 27, 2024
 *      Author: kunal
 */

#ifndef BLINK_H_
#define BLINK_H_

void debounce();

void switch_led_init();
void change_blink_speed(char []);
void change_color(char []);
void delayMs(int);
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
#define magenta 6
#define white 14
#define maxdelay 50

#define color_on(color) GPIO_PORTF_DATA_R = color
#define color_off GPIO_PORTF_DATA_R = 0x00

int color_code[7] = {green , blue , cyan , red , yellow , magenta , white};


int blink_delay =50;
 int blink_speed = 0;




#endif /* BLINK_H_ */
