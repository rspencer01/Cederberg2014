/*
 * Thermometer.h
 *
 * Created: 2014/09/17 09:29:32 PM
 *  Author: Robert Spencer
 */ 

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

// The masses of different allowed states.
#define STATE_DISPLAY_MIN         0b0000100
#define STATE_DISPLAY_MAX         0b0001000

#define STATE_SLEEP               0b0000000

#define STATE_INDOOR_DISPLAY_PRE  0b0100000
#define STATE_INDOOR_DISPLAY      0b0000001
#define STATE_INDOOR_MIN_WORD     0b0000110
#define STATE_INDOOR_MIN_DISPLAY  0b0000010
#define STATE_INDOOR_MAX_WORD     0b0001011
#define STATE_INDOOR_MAX_DISPLAY  0b0000011
#define STATE_INDOOR_RESET        0b1100000

#define STATE_OUTDOOR_DISPLAY_PRE 0b0110000
#define STATE_OUTDOOR_DISPLAY     0b0010001
#define STATE_OUTDOOR_MIN_WORD    0b0010110
#define STATE_OUTDOOR_MIN_DISPLAY 0b0010010
#define STATE_OUTDOOR_MAX_WORD    0b0011011
#define STATE_OUTDOOR_MAX_DISPLAY 0b0010011
#define STATE_OUTDOOR_RESET       0b1110000

// What state are we in?
extern int state;

// How long should we be alive for?
extern unsigned int stateChangeTics;

// If we should go to sleep.
extern volatile char goToSleep;

// If the state should be changed
extern volatile char changeState;

// Set the display etc to the relevant state
void setState();

#endif /* COMPASS_H_ */