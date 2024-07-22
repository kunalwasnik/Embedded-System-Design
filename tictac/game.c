/*
 * game.c
 *
 *  Created on: Mar 9, 2024
 *      Author: kunal
 */
#include "game.h"

extern char frame[3][3];
extern int total_moves;

int check_win(char player){

    int win=0;
    int same_moves=0;

   if(total_moves ==9){
       print("Game Draw \n");
       reset_game();
       print_2d(frame);
       return 0;
   }

   // check row equal. ----
   for(int i=0;i<3;i++){
       same_moves=0;

          for(int j=0;j<3;j++){

              if(frame[i][j] != player){
                  break;
              }else{

                  same_moves++;
              }

          }
          if(same_moves == 3){
                  win=1;
                  return win;
          }
   }


   // check columns
   for(int i=0;i<3;i++){
       same_moves=0;

          for(int j=0;j<3;j++){

              if(frame[j][i] != player){
                  break;
              }else{

                  same_moves++;
              }

          }
          if(same_moves == 3){
                  win=1;
          return win;
          }

   }

   // check diagonal
same_moves=0;

for(int i=0;i<3;i++){
    if(frame[i][i] != player)
        break;
    else{
        same_moves++;
    }

}
if(same_moves == 3){
    win=1;
    return win;
}


// check reverse diagonal.


for(int i=0;i<3;i++){
    if(frame[i][3-i-1] != player)
        break;
    else{
        same_moves++;
    }

}

if(same_moves == 3){
    win=1;
    return win;
}





return win;
}

void  reset_game(){



    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            frame[i][j] = '.';
        }
    }

    total_moves=0;



}

