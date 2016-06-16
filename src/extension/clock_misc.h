/* ARM Project 2016
 *
 * clock_misc.c contains all the macros used in implementing the clock, as well
 * as the headers of all the functions used
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#ifndef CLOCK_MISC_H_
#define CLOCK_MISC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <wiringPi.h>
#include <sys/types.h>
#include <unistd.h>


#define NUMBER_LEDS_FOR_HOUR 5
#define NUMBER_LEDS_FOR_MIN_SEC 6
#define HOURS_CLOCK 2
#define HOURS_LATCH 3
#define HOURS_DATA 7
#define MINS_CLOCK 6
#define MINS_LATCH 10
#define MINS_DATA 11
#define SECS_CLOCK 15
#define SECS_LATCH 16
#define SECS_DATA 1
#define CHANGE_MODE 12
#define CHANGE_MINS 13
#define CHANGE_HOURS 14
#define SET_TIME_MODE 1
#define SET_ALARM_MODE 2
#define CLOCK_MODE 0

#define BORDERLINE_OUTPUT_INPUT 9
#define NUMBER_OF_USED_PINS 12
#define NO_OF_PINS_TO_SEND_TO_REGISTER 7

#define UNDEFINED -1
#define MAX_MINS_SECS 60
#define MAX_HOURS 24
#define ONE_SECOND_DELAY 1000
#define SHORT_WAITING_TIME 1
#define LONG_WAITING_TIME 300
#define SECS_IN_AN_HOUR 3600

static int used_pins[] = {2, 3, 7, 6, 10, 11, 15, 16, 1, 12, 13, 14};

typedef struct time {
  int hours, mins, secs;
} time_rep;

static time_rep actual_time = {UNDEFINED, UNDEFINED, UNDEFINED};
static time_rep alarm_time = {UNDEFINED, UNDEFINED, UNDEFINED};

static time_t rawtime;
static time_t rawtime_after_alarm;
static struct tm *timeinfo;
static int mode = CLOCK_MODE;
static bool alarm_is_set = false;
static bool sound_on = false;

void turnOnLeds(int number, int times, int dataPin, int latchPin, int clockPin);
void initializePins(void);
void digitalWrite_t(int pin, int status, int clockPin);
time_rep set_time(int mins, int hours);
void binary_clock(void);
void handle_mode(void);
void print_time(int secs, int mins, int hours);
void handle_alarm(void);

#endif
