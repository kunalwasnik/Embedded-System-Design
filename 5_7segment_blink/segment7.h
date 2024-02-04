/*
 * segment7.h
 *
 *  Created on: Jan 31, 2024
 *      Author: kunal
 */

#ifndef SEGMENT7_H_
#define SEGMENT7_H_



int segment_num[10] = {0x3F , 0x06  , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D,0x07,0x7F , 0x6F};
int segment_frame[4] = {0,0,0,0};

void init_7segment();
void segment_display(int display_number , int num);
void segment_display_refresh(void);
void enable_irq();
void SysTick_Handler(void);
void SysTick_init();
// from left display number 0 ,1,2,3



#endif /* SEGMENT7_H_ */
