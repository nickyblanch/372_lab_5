
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "spi.h"
#include "i2c.h"



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
volatile LEDstate AccelerationState=frown;    // Tnitialize the state to frown 

//----------------------------------------------------------------------//
// Main function
int main(void) {

  // VARIABLES
  int freq_count = 500;

  // HARDWARE INITIALIZATIONS
  initSwitchPD0();      // Initialize the switch
  SPI_MASTER_Init();    //Initialize the SPI module
  initTimer1();         // Initialize timer 1 (for millisecond delay)
  Serial.begin(9600);   // Serial (for debugging)
  sei();                // Enable global interrupts

  // SETUP DISPLAY
  write_execute(0x0A,0x08); //Brightness control
  write_execute(0x0B,0x07); //scan all rows and collumns
  write_execute(0x0C,0x01); //set shutdown register to normal operation
  write_execute(0x0F,0x00); //display test register set to normal operation
  AccelerationState=smile;  // Begin in the smile state

  while(1) {
    
    // BUZZER:
    change_frequency(freq_count);
    freq_count++;
    if(freq_count > 10000){
      freq_count = 500;
    }
    else{
      freq_count = freq_count;
    }

    // READ ACCELEROMETEROR
    

    // DISPLAY STATE MACHINE:      
    switch(AccelerationState){
        case smile:
          OCR1C = 0;
          write_execute(0x01,0b00000000);
          write_execute(0x02,0b00000000);
          write_execute(0x03,0b00100100);
          write_execute(0x04,0b00000000);
          write_execute(0x05,0b10000001);
          write_execute(0x06,0b01100110);
          write_execute(0x07,0b00111100);
          write_execute(0x08,0b00000000);
          break;
        case frown:
          OCR1C = OCR1A * 0.25;
          write_execute(0x01,0b00000000);
          write_execute(0x02,0b00000000);
          write_execute(0x03,0b00100100);
          write_execute(0x04,0b00000000);
          write_execute(0x05,0b00011000);
          write_execute(0x06,0b01100110);
          write_execute(0x07,0b10000001);
          write_execute(0x08,0b00000000);
          break;
    } // end display state machine

    // BUTTON STATE MACHINE:
    switch(button_state) {

      case wait_press:
        // Do nothing; we are waiting for the button to be pressed.
        break;
      case debounce_press:
        // Wait for the noisy 'debounce' state to pass. Then, we are awaiting release.
        delayMs(1);
        button_state = wait_release;
        break;
      case wait_release:
        // Do nothing; we are waiting for the button to be released.
        break;
      case debounce_release:
        // The button has been released.
        // Wait for the noisy 'debounce' state to pass. Then, we are awaiting press.
        delayMs(1);
        
        button_state = wait_press;
        break;

    } // end switch state machine  
  } // end while loop
} // end main



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

    // Buzzer:
    if (OCR1C) {
      OCR1C = 0;
    }
    else {
      OCR1C = OCR1A*.25;
    } // end if/else
  } // end if/else
  // If the flag triggers while the button is 1 in one of the noisy debounce states, we do nothing.
} // end ISR
