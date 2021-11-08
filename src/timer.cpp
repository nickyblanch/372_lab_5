#include <avr/io.h>

void initTimer1(){

// make timer for 1 second 

//set waverform generations bits to CTC mode
//WGM13=0; WGM12=1; WGM11=0; WGM10=0;
TCCR1A&=~(1<<WGM10);
TCCR1A&=~(1<<WGM11);
TCCR1A|=(1<<WGM12);
TCCR1B&=~(1<<WGM13);

//OCR1A=1*40000000/1024=39062.5
//set prescaler to 1024
//CS12=1, CS11=0 CS10=1
TCCR1B|=(1<<CS10)|(1<<CS12);
TCCR1B&=~(1<<CS11);

OCR1A=39062;

}



void delay_Ms(unsigned int delay){

    unsigned int delayCnt= 0;TCNT0 = 0;
    //starting the timer at 0 instead of some random junk number
    TIFR0 |= (1 << OCF0A);// set compare flag to start timer
    while (delayCnt< delay) {
        if (TIFR0 & (1 << OCF0A)) {
            //increment only while the flag is set (timer reached maxval= OCR0A)
            delayCnt++;
            TIFR0 |= (1 << OCF0A);
            TCNT0 = 0;
            //re-start timer. will go to 0 before reaching the if statement above
        }
    }
}