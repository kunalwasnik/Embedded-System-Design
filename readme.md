

## Assignments done as part of the course.


## BLINKY
    • Test the toolchain with the Tiva board given to you
    • Implement the colour wheel shown in the attachment using SW1 switch that is present on the TIVA board.
    • With each key (SW1) press the LED cycles through different colours starting from Green in the following order
    • Order: Green, Blue, Cyan, Red, Yellow, Magenta, White
    • Use macros to define colours and GPIO memory address mapping. For example COLOR_GREEN_ON, COLOR_CYAN_ON
    • Use SW2 to increase the blink speed starting from once in two seconds to 16 times in a second. Doubling the rate with every key press (SW2)
    • Once the max blink speed is reached and the next key (SW2) press occurs, the LED will switch to "continuously ON" state and wait for the next key press (SW2)
    • The blink cycle will repeat again from the beginning on the next key (SW2) press 
    • Use software delay loop to implement the blink delay. Use macro to compute the delay value.
    • Bonus points: You might encounter the key debouncing problem that you need to address using software solution

    


## Blinky with console



    • Use the virtual UART port to establish the serial communication link with the Tiva board.
    • 
    • Initialize the console port with the pre-defined baud rate
    • Provide the echo for the character received from the host (PC/laptop)
    • Store the received ASCII characters in console_cmd_buffer (max 30 characters)
    • The command should be processed once the "Enter" character is received
    • Pre-process the received command string to remove extra characters (space, tab etc.) and convert the string to the lowercase (bonus points)
    • Support backspace functionality (bonus points)
    • Write the command parser to support the following commands. Any wrong or unsupported command string should print "command help" on the console
    • > color [color_name]
    • > blink [ blink_rate] (number of times led blinks in a minute, zero means glow continuously)
    • Maintain global state of the program that can be defined as [Color, Blink_rate]
      



## 9_blinky_key_seg_uart2_lcd_poll2

    • Peek and Poke with lcd
    • Extend the UART console program to support read and write to arbitrary memory address:
    • peek memory_address in hex number_of_bytes_to_read. Display in Hex as well as ASCII string format)
    • poke memory_address_in_hex number_of_bytes_write Content_in_hex_or_string
    • Define string "Version 0.01" in the program
    • Add a symbol/variable in Linker command file. This variable will point to the above string to be displayed 16x2 LCD display. 
    • The message we should be able to change from command line using poke command

## 12_stopwatch_motor 

(DC motor speed control using ADC and PWM)


    • Develop a down-counting timer using a ADC and PWM peripherals
    • UART commands that needs to be supported
    • > timer set xxx (sec)
    • > timer start/pause/resume/reset
    • Further, SW1 and SW2 should provide functionality to start/stop/pause/resume the timer 
    • Implement DC motor speed control using potentiometer on the EduARM addon board. Min value on potentiometer should make the motor speed 'zero'. 
    • LED blink speed depends on the potentiometer value. Maximum deviation will keep LED on all the time and min value on potentiometer switches off LED (Bonus points)
    • 7-segment display is used to show timer value by default.
    • Whenever ADC value is changed, the seven segment shows updated ADC value for some time before displaying the Timer value (Bonus points)
    • 7-segment display should not flicker


## Seven segment Display and 4x4 keypad (blinky_key_seg_uart)

    • Extend the UART console assignment to support the following features
    • Use 1st two keys of the 4x4 keypad to pause, resume, stop, and restart functionality
    • Seven segment display is used to display the current cycle number that system is playing and the present colour code (Green #1 .. Whilte #8) of the LED
    • In the class it was mentioned that first two blocks are used to count the number of colour changes. However, if you want to use it display the number colour change cycles that machine has gone through then that is also fine.
    • Note: Machine always moves in forward direction. Colour code wraps around and adds the cycle. whenever necessary.
    • In addition to the earlier console commands please add functionality to support following commands
    • color color_name
    • pause 
    • resume
    • stop 
    • start
    • In 4x4 keypad, first key is used to toggle between pause and resume state. In the pause state the system holds the present state and continues to blink as soon as same key is pressed to resume the activity
    • 




## Tic tac 

    • Stopwatch Timer
    • Develop a stopwatch timer that works on Tiva board.
    • The timer uses four modules of seven segment LED display to show the timer value.
    • First three modules are used for seconds field and last one for milliseconds field.
    • Use onboard SW1 and SW2 switches to control the timer.
    • SW1 - Stop/Start
    • SW2 - Pause/Resume
    • Above functionality is also supported using UART console commands "timer stop/start/pause/resume".
    • SysTick timer is used to keep time
    • If the timer is ready (STOP state and timer value reset to 0.0) then it glows Green LED continuously and displays "Timer Ready" on two line LCD display module.
    • If the timer is in running state then it is indicated by blinking Green LED and displays "Timer Running" on the the LCD module.
    • If the timer is in pause state then it is indicated by blinking Blue LED and displaying "Timer Paused" on the LCD module.
    • Tic-Tac-Toe
    • The system also supports playing Tic-Tac-Toe game simultaneously. It uses first 3 rows and columns of the 4x4 keypad to play the game. 
    • Every alternate move is assigned to the same player ( Player 1 or 2) represented using different symbols ( 'X' and 'O').
    • After every move, updated status of the game is displayed on the UART console. The program should be able to checks validity of the move and detect the "game over" condition. UART console prints appropriate message during and after the game. For example, "Invalid move", "Game won by Player 1/2" or "Game draw" .
