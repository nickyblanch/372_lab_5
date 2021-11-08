
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */
void initSwitchPD0(){

    // Initialize PD0 (pin 21) for input (set DDR to input). Enable pullup resistor.
    // NOTE: To set the port to input, we must 'clear' the bit by setting it to zero
    // while simultaneously preserving the other bits in the DDR register.

    DDRD &= ~( 1 << DDD0);  // Clear the bit in DDRD0
    PORTD |= (1 << PORTD0); // Enable pullup resistor by writing 1 to the port.

    // Pin-Change Interrupts must be enabled for pin PD0
    // We basically want to use INT0 as a pin change interrupt
    EICRA |= (1 << ISC00); EICRA &= ~(1 << ISC01); // Set INT0 to trigger asynchronously on any edge (high or low)

    EIMSK |= (1 << INT0);   // Enable INT0 in the EIMSK register
}
