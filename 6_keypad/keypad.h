/*
 * keypad.h
 *
 *  Created on: Feb 2, 2024
 *      Author: kunal
 */


#include <stdint.h>

#include "inc/tm4c123gh6pm.h"

#ifndef KEYPAD_H_
#define KEYPAD_H_

void keypad_init();
void keypad_interrupt();

#define ROW1_PIN  0
#define ROW2_PIN  1
#define ROW3_PIN  2
#define ROW4_PIN  3


#define COL1_PIN 4
#define COL2_PIN 5
#define COL3_PIN 6
#define COL4_PIN 7




#endif /* KEYPAD_H_ */
