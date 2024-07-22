/*
 * lcd.c
 *
 *  Created on: Feb 9, 2024
 *      Author: kunal
 */
#include <stdio.h>
#include <stdint.h>

#include "lcd.h"

#include "inc/tm4c123gh6pm.h"
#define rs_pin 6

#define lcd_en 7


void lcd_config(){

// port b data pin;
         SYSCTL_RCGC2_R |= (1<<1);
         // turning on 0-7  (4--5 is data pins) for output
         GPIO_PORTB_DIR_R = (0xFF);
         GPIO_PORTB_DEN_R = (0xFF);
         // rs pin a6 , a7 enable pin

         SYSCTL_RCGC2_R |= (1<<0);

         GPIO_PORTA_DIR_R |= (1<<rs_pin);
         GPIO_PORTA_DEN_R |= (1<<rs_pin);

         // set enable as output and set den
         GPIO_PORTA_DIR_R |= (1<<lcd_en);
         GPIO_PORTA_DEN_R |= (1<<lcd_en);



}

void lcd_init(){

// flow chart steps.
    // step 1
   lcd_delay(50);
   GPIO_PORTA_DATA_R &= ~(1<<rs_pin);
   GPIO_PORTB_DATA_R = (1<<5 || 1<<4);
   GPIO_PORTA_DATA_R = (1<<lcd_en);
   lcd_delay(1);
   GPIO_PORTA_DATA_R &= ~(1<<lcd_en);

   lcd_delay(5);

   // step 2

      GPIO_PORTA_DATA_R &= ~(1<<rs_pin);
      GPIO_PORTB_DATA_R = (1<<5 || 1<<4);
      GPIO_PORTA_DATA_R |= (1<<lcd_en);
      lcd_delay(1);
      GPIO_PORTA_DATA_R &= ~(1<<lcd_en);

      lcd_delay(1);

      GPIO_PORTA_DATA_R &= ~(1<<rs_pin);
      GPIO_PORTB_DATA_R = (1<<5 || 1<<4);
      GPIO_PORTA_DATA_R | (1<<lcd_en);
      lcd_delay(1);
      GPIO_PORTA_DATA_R &= ~(1<<lcd_en);




      lcd_delay(100);

      lcd_command(0x38); // 8 bit mode
      lcd_delay(1);
      lcd_command(0x01); // clear display
      lcd_delay(1);
      lcd_command(0x02);  // return home
      lcd_delay(1);
      lcd_command(0x0E);    // display on no cursor.






}

void lcd_command(char command){




    lcd_delay(50);

    GPIO_PORTA_DATA_R &= ~(1<<rs_pin);
    GPIO_PORTB_DATA_R = (command);

    GPIO_PORTA_DATA_R |= (1<<lcd_en);
    lcd_delay(50);
    GPIO_PORTA_DATA_R &= ~(1<<lcd_en);
    lcd_delay(50);


}

void lcd_print(char msg[]){



// check busy flag;
//while ( (GPIO_PORTA_DATA_R && (1<<7)) == (1<<7));

 // commenting for testing out single character.
//int col=0;

    lcd_delay(5);
    lcd_command(0x01); // clear display
    lcd_delay(5);
//    lcd_command(0x02);  // return home
//    lcd_delay(5);
   lcd_command(0xC0);
    lcd_delay(5);

 for(int i=0;msg[i]!='\0';i++){

         GPIO_PORTB_DATA_R = msg[i];
         GPIO_PORTA_DATA_R |= (1<<rs_pin);
         GPIO_PORTA_DATA_R |= (1<<lcd_en);
         lcd_delay(10);
         GPIO_PORTA_DATA_R &= ~(1<<lcd_en);
         lcd_delay(10);


         // extra commands
//         if(i>=17){
//         col++;
//        lcd_command( (0xC0 +col ));
//         lcd_delay(100);
//         }
}


}

void lcd_addr_addr(char *loc , int n){



// check busy flag;
//while ( (GPIO_PORTA_DATA_R && (1<<7)) == (1<<7));

 // commenting for testing out single character.
//int col=0;

    lcd_delay(50);
    lcd_command(0x01); // clear display
    lcd_delay(50);
    lcd_command(0x02);  // return home
    lcd_delay(50);

 for(int i=0;i<n;i++){

         GPIO_PORTB_DATA_R = *(loc + i);
         GPIO_PORTA_DATA_R |= (1<<rs_pin);
         GPIO_PORTA_DATA_R |= (1<<lcd_en);
         lcd_delay(10);
         GPIO_PORTA_DATA_R &= ~(1<<lcd_en);
         lcd_delay(10);


         // extra commands
//         if(i>=17){
//         col++;
//        lcd_command( (0xC0 +col ));
//         lcd_delay(100);
//         }
}


}



void lcd_delay(int n)
{

int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 1500; j++) {
    // change j <1500 to j<1000

} /* do nothing for 1 ms */

}
