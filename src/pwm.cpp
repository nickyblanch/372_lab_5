#include <avr/io.h>


void initPWMTimer3() {
    //set data direction pins
    DDRE |= (1<<PORTE3);
    
 
    //set for non inverting timer

    TCCR3A |= (1<<COM3A1);
    TCCR3A &= ~(1<<COM3A0);

    //set the mode for the timer for  fast PWM 10 BIT
    TCCR3A |= (1<<WGM31);
    TCCR3A |= (1<<WGM30);
    TCCR3B |= (1<<WGM32);
    TCCR3B &= (1<< WGM33);

    //set prescaler to 1
    TCCR3B|= (1<<CS30);
    TCCR3B &= ~(1<<CS31);
    TCCR3B &= ~(1<<CS32);

    //initialize the output compare register to ****
    OCR3A =251;

}