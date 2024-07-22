/*
 * lcd.h
 *
 *  Created on: Feb 9, 2024
 *      Author: kunal
 */

#ifndef LCD_H_
#define LCD_H_

void lcd_config();
void lcd_init();
void lcd_print(char []);

void lcd_command(char);
void lcd_delay(int);

#endif /* LCD_H_ */
