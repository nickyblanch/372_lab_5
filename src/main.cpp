
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.cpp"
#include "timer.cpp"
#include "pwm.cpp"
#include "spi.cpp"
#include "i2c.cpp"



//----------------------------------------------------------------------//
// Global variables

//----------------------------------------------------------------------//
// State machines

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum stateEnum {wait_press, debounce_press, wait_release, debounce_release} stateEnum;
typedef enum LEDstate {smile, frown} LEDstate;
// Initialize states. Remember to use volatile
volatile stateEnum button_state = wait_press; // ASSUMING WE BEGIN IN A STATE WHERE THE BUTTON IS NOT BEING PRESSED
volatile LEDstate AccelerationState=frown;//initialize the state to frown 
//----------------------------------------------------------------------//
// Main function
int main(void) {

  // VARIABLES

Serial.begin(9600);
  // HARDWARE INITIALIZATIONS
  initSwitchPD0();
  
  
  initTimer1();
  sei();
  

  while(1){


    switch(AccelerationState){
        case smile:
          break;
        case frown:
         break;
    }
    // Check the state
    switch(button_state) {
      case wait_press:
        // Do nothing; we are waiting for the button to be pressed.
        break;
      case debounce_press:
        // Wait for the noisy 'debounce' state to pass. Then, we are awaiting release.
        delay_Ms(1);
        button_state = wait_release;
        break;
      case wait_release:
        // Do nothing; we are waiting for the button to be released.
        break;
      case debounce_release:
        // The button has been released.
        // Wait for the noisy 'debounce' state to pass. Then, we are awaiting press.
        delay_Ms(1);
        
        button_state = wait_press;
        break;
    }

    
  }

}

//----------------------------------------------------------------------//
// Interrupt

ISR (INT0_vect) {
  // When the interrupt flag is triggered:

  if (button_state == wait_press) {
    // The button was pushed while we were waiting for it to be pressed. Enter the first debounce state.
    button_state = debounce_press;
  }
  else if (button_state == wait_release) {
    // The button was released while we were waiting for it to be released. Enter the second debounce state.
    button_state = debounce_release;
    
    // TODO
    
  }
  // If the flag triggers while the button is 1 in one of the noisy debounce states, we do nothing.
}