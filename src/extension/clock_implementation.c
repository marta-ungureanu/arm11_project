/* ARM Project 2016
 *
 * clock_implementation.c contains the functions that make the clock work
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "clock_misc.h"

/* The core function that implements the clock
 *
 * PARAMS: void
 *
 * RETURN: void
 *
 * If the time is not set manually, the clock uses the system time. After the
 * first initialization, we delay the loop by 1 second in order to keep the
 * proper timing.
 */
void clock(void) {
  while(true) {
    handle_mode();
    if(actual_time.hours == UNDEFINED) {
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      actual_time.hours = timeinfo -> tm_hour;
      actual_time.mins = timeinfo -> tm_min;
      actual_time.secs = timeinfo -> tm_sec;
    }
    print_time(actual_time.secs, actual_time.mins, actual_time.hours);
    delay(ONE_SECOND_DELAY);
    handle_alarm();
    actual_time.secs ++;
    if(actual_time.secs == MAX_MINS_SECS) {
      actual_time.secs = 0;
      actual_time.mins ++;
    }
    if(actual_time.mins == MAX_MINS_SECS) {
      actual_time.mins = 0;
      actual_time.hours ++;
    }
    if(actual_time.hours == MAX_HOURS) {
      actual_time.hours = 0;
      actual_time.mins = 0;
      actual_time.secs = 0;
    }
  }
}

/* Function that handles the mode of the clock, which depends on the state of
 * the CHANGE_MODE pin (which corresponds to a button)
 *
 * PARAMS: void
 *
 * RETURN: void
 *
 * If the alarm is on (sound_on is true), pressing the CHANGE_MODE button turns
 * it off. Otherwise, by pressing the button the clock enters SET_TIME_MODE and
 * by pressing it again it enters the SET_ALARM_MODE. The third time it just
 * returns to the CLOCK_MODE, displaying the time. The time is set starting the
 * current time and the alarm is set starting to 00:00.
 */
void handle_mode(void) {
  if(digitalRead(CHANGE_MODE) == HIGH) {
    if(sound_on) {
      system("pkill omxplayer");
      sound_on = false;
    } else {
      while(digitalRead(CHANGE_MODE) == HIGH) {
        turnOnLeds(0, NUMBER_LEDS_FOR_MIN_SEC, SECS_DATA,
          SECS_LATCH, SECS_CLOCK);
        delay(SHORT_WAITING_TIME);
      }
      mode ++;
      switch(mode) {
        case SET_TIME_MODE:
          actual_time = set_time(actual_time.mins == UNDEFINED ?
            timeinfo -> tm_min : actual_time.mins,
            actual_time.hours == UNDEFINED ?
            timeinfo -> tm_hour : actual_time.hours);
          mode ++;
        case SET_ALARM_MODE:
          alarm_time = set_time(0, 0);
          if(alarm_time.hours != UNDEFINED) {
            alarm_is_set = true;
          }
          mode = CLOCK_MODE;
          break;
      }
    }
  }
}

/* Function that takes the time as an input from the users when they set the
 * time or the alarm
 *
 * PARAMS: int mins, int hours
 * mins: the current value of minutes
 * hours: the current value of hours
 *
 * RETURN: time_rep
 *
 * When the CHANGE_MINS pin is HIGH, which means that the button is pressed,
 * the minutes are incremented by 1 after every 0.3 seconds. It is the same for
 * the hours.
 */
time_rep set_time(int mins, int hours) {
  while(digitalRead(CHANGE_MODE) == LOW) {
    if(digitalRead(CHANGE_MINS) == HIGH) {
      while(digitalRead(CHANGE_MINS)) {
        mins = (mins + 1) % MAX_MINS_SECS;
        turnOnLeds(mins, NUMBER_LEDS_FOR_MIN_SEC, MINS_DATA, MINS_LATCH,
          MINS_CLOCK);
	      delay(LONG_WAITING_TIME);
      }
    }

    if(digitalRead(CHANGE_HOURS) == HIGH) {
      while(digitalRead(CHANGE_HOURS)) {
        hours = (hours + 1) % MAX_HOURS;
        turnOnLeds(hours, NUMBER_LEDS_FOR_HOUR, HOURS_DATA, HOURS_LATCH,
          HOURS_CLOCK);
        delay(LONG_WAITING_TIME);
      }
    }
  }

  while(digitalRead(CHANGE_MODE) == HIGH) {
    print_time(0, 0, 0);
    delay(SHORT_WAITING_TIME);
  }

  time_rep new_time = {hours - 1, mins - 1, 0};
  return new_time;
}

/* Function that prints the time using the LEDs
 *
 * PARAMS: int secs, int mins, int hours
 * secs: the value of the seconds
 * mins: the value of the minutes
 * hours: the value of the hour
 *
 * RETURN: void
 */
void print_time(int secs, int mins, int hours) {
  turnOnLeds(secs, NUMBER_LEDS_FOR_MIN_SEC, SECS_DATA, SECS_LATCH, SECS_CLOCK);
  turnOnLeds(mins, NUMBER_LEDS_FOR_MIN_SEC, MINS_DATA, MINS_LATCH, MINS_CLOCK);
  turnOnLeds(hours, NUMBER_LEDS_FOR_HOUR, HOURS_DATA, HOURS_LATCH, HOURS_CLOCK);
}

/* Function that plays a song when the alarm turns on
 *
 * PARAMS: void
 *
 * RETURN: void
 */
void handle_alarm(void) {
  if(alarm_is_set) {
    if(alarm_time.hours == actual_time.hours &&
        alarm_time.mins == actual_time.mins) {
      system("omxplayer /home/pi/Desktop/Android_tone.mp3 &");
      alarm_is_set = false;
      sound_on = true;
    }
  }
}
