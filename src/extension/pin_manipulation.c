/* ARM Project 2016
 *
 * pin_manipulation.c contains the functions that manipulate the gpio pins
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "clock_misc.h"

/* Function that initializes all the pins we use
 *
 * PARAMS: void
 *
 * RETURN: void
 */
void initializePins(void) {
  /*pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);*/
  for(int i = 0; i < BORDERLINE_OUTPUT_INPUT; i++){
    pinMode(used_pins[i], OUTPUT);
  }
  for(int i = BORDERLINE_OUTPUT_INPUT; i < NUMBER_OF_USED_PINS; i++) {
    pinMode(used_pins[i], INPUT);
  }
}

/* Function that sends a value to the shift register
 *
 * PARAMS: int pin, int status, int clockPin
 * pin: the data pin
 * status: HIGH or LOW
 * clockPin: the clock pin
 *
 * RETURN: void
 */
void digitalWrite_t(int pin, int status, int clockPin) {
  digitalWrite(pin, status);
  digitalWrite(clockPin, LOW);
  digitalWrite(clockPin, HIGH);
}

/* Function that turns on the LEDs in order to print the time
 *
 * PARAMS: int number, int times, int dataPin, int latchPin, int clockPin
 * number: the number of seconds/minutes/hours
 * times: how many LEDs we need to represent the number in binary
 * dataPin: the data pin for shift register
 * latchPin: the latch pin for shift register
 * clockPin: the clock pin for shift register
 *
 * RETURN: void
 */
void turnOnLeds(int number, int times, int dataPin, int latchPin,
    int clockPin) {
  digitalWrite_t(latchPin, LOW, clockPin);
  digitalWrite(latchPin, HIGH);
  digitalWrite(dataPin, LOW);
  for(int i = 0; i < times; i++) {
    if(number % 2) {
      digitalWrite_t(dataPin, HIGH, clockPin);
    } else {
      digitalWrite_t(dataPin, LOW, clockPin);
    }
    number >>= 1;
  }
  for(int i = 0; i < NO_OF_PINS_TO_SEND_TO_REGISTER - times; i++) {
    digitalWrite(dataPin, LOW);
  }
}
