/* ARM Project 2016
 *
 * assemble.c contains the main function for the clock
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "clock_misc.h"

/* Function that initializes the wiringPi library and the pins and then calls
 * the function that implements the clock
 */
int main(void) {
  wiringPiSetup();
  initializePins();
  binary_clock();
  return EXIT_SUCCESS;
}
