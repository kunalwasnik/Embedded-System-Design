/*
 * segment7.h
 *
 *  Created on: Jan 31, 2024
 *      Author: kunal
 */

#ifndef SEGMENT7_H_
#define SEGMENT7_H_





void init_7segment();
void segment_display(int display_number , int num);
void segment_display_refresh(void);
void enable_irq();
void SysTick_Handler(void);
void SysTick_init();
// from left display number 0 ,1,2,3



#endif /* SEGMENT7_H_ */
