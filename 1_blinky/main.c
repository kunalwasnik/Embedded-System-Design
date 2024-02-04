#include <stdint.h>
#include "inc/tm4c123gh6pm.h"


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
#define mangenta 6
#define white 14


//#define red_on GPIO_PORTF_DATA_R |= (1<<red)
//#define blue_on GPIO_PORTF_DATA_R |= (1<<blue)
//#define green_on GPIO_PORTF_DATA_R |= (1<<green)


#define color_on(color) GPIO_PORTF_DATA_R = color
#define color_off GPIO_PORTF_DATA_R = 0x00

/*
Green, Blue, Cyan, Red, Yellow, Magenta, White

8 4 2
g b r
1 0 0
0 1 0
1 1 0
0 0 1
1 0 1
0 1 1
1 1 1

8 , 4 , 12 , 2 , 10 , 6 , 14
 *
 *
 */






void delayMs(int n);

int color_code[7] = {green , blue , cyan , red , yellow , mangenta , white};
 int selected_color= 10;
 int counter=-1;
 int blink_delay =2000;
 int blink_speed = 0;

int main(void)
{






    SYSCTL_RCGC2_R |= 0x00000020;
/* enable clock to GPIOF at clock gating
control register */
    int unsigned value;

    // unblocking for sw2
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x01;



     GPIO_PORTF_DIR_R = ( (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) ) ;
     GPIO_PORTF_DEN_R = ((1<<switch1) | (1<<red_pin) | (1<<green_pin) | (1<<blue_pin) | (1<<switch2) ) ;
     GPIO_PORTF_PUR_R = ((1<<switch1) | (1<<switch2));

     // trying interrupts

      NVIC_EN0_R |= (1<<30);
      GPIO_PORTF_IM_R  = ((1<<0)| (1<<4)); // enable interrupt.
      GPIO_PORTF_IS_R = (~(1<<0)| ~(1<<4)); //  edge triggered
      GPIO_PORTF_IEV_R = (~(1<<0)| ~(1<<4)); // falling edge


      // end trying.
      GPIO_PORTF_DATA_R=0;

while(1) {
 //   GPIO_PORTF_DATA_R=0;
    value = GPIO_PORTF_DATA_R;



// check switch 1 click.
    /*
if( (value & (1<<switch1))==0){

    counter = counter + 1;
    if(counter==7){
        counter=0;
    }

    selected_color = color_code[counter];
}


*/

//blink

//color_on(selected_color);
//delayMs(blink_delay/2);
//color_off;


// check switch 2 click.
/*
if( (value & (1<<switch2))==0){

    color_on(color_code[3]);
    blink_speed++;
    if(blink_speed==6){
           blink_speed = 1;
           blink_delay = 2000;
    }
    for(int i=1;i<=blink_speed;i++)
    blink_delay = blink_delay/2;

}
*/
//delayMs(blink_delay/2);


}

}
/* delay n milliseconds (16 MHz CPU clock) */
void DisableInterrupts(void){__asm ("CPSID  I\n");}
void EnableInterrupts(void){__asm  ("CPSIE  I\n");}

void myHandler(void){



    if(GPIO_PORTF_MIS_R & 0x10){

       /* counter = counter + 1;
          if(counter==7){
              counter=0;
          }

          selected_color = color_code[counter];


        */
        GPIO_PORTF_DATA_R = 0x02;
        //blue
         GPIO_PORTF_ICR_R = 0x10;


         }

  else if(GPIO_PORTF_MIS_R & 0x01){

                /*
            color_on(color_code[3]);
             blink_speed++;
             if(blink_speed==6){
                    blink_speed = 1;
                    blink_delay = 2000;
             }
             for(int i=1;i<=blink_speed;i++)
             blink_delay = blink_delay/2;

             */
             GPIO_PORTF_DATA_R = 0x02;

             GPIO_PORTF_ICR_R = 0x01;

             }

}

void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
}
