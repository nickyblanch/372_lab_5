#include <avr/io.h>
#include "pwm.h"

void initPWMTimer1() {

    // set DDR for output pin
    // We are using PORTB7 (pin 13 on board) (this is the OCR1C output pin)
    //
    // NOTE: When using Mode 15, OCR1A is needed to set the top value. Therefore, we must
    // use OCR1C for out output compare, and out output pin must be PB7 (pin 13).
    DDRB |= (1 << DDB7);

    // We will use PWM mode 15 (so we can vary frequency)
    // WGM13 = 1, WGM12 = 1, WGM11 = 1, WGM10 = 1 (these bits are stored in the TCCR1A and TCCR1B registers)
    TCCR1A |= ((1 << WGM10) | (1 << WGM11));
    TCCR1B |= (1 << WGM13);
    TCCR1B |= (1 << WGM12);

    // Frequency calculation:
    // We will use a prescaler of 1. The 'top value' is OCR1A.
    // fpwm = fclk / (prescaler * (1 + top value)), we will use a prescaler of 1

    // Set prescaler to 1
    TCCR1B |= (1 << CS10);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);

    // Set to non-inverting mode - output starts high and then is low
    // See table 17-4
    // Non-inverting: COM1C1 = 1, COM1C0 = 0 (located in TCCR1A register)
    TCCR1A |= (1 << COM1C1);
    TCCR1A &= ~(1 << COM1C0);
    // NOTE: When using Mode 15 (variable frequency), we use OCR1C for output compare. Therefore, we
    // need to use COM1C1 and COM1C0 here. Otherwise, we would use COM1A1 and COM1A0.


    // NOTE: OCR1C and OCR1A are NOT set here. To determine the frequenc and duty cycle,
    // they must be set by the change_frequency function.
}

void change_frequency(unsigned int freq_val) {
    // To vary the frequency, we need to to change OCR1A (the TOP value) and OCR1C (the COMPARE value)
    OCR1A = 16000000/freq_val;  // (ignoring the 1 in the formula)
}