/*
 * Thermometer.h
 *
 * Created: 2014/09/17 09:29:32 PM
 *  Author: Robert Spencer
 */ 

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#define STATE_DISPLAY_MIN         0b000100
#define STATE_DISPLAY_MAX         0b001000

#define STATE_SLEEP               0b000000

#define STATE_INDOOR_DISPLAY      0b000001
#define STATE_INDOOR_MIN_WORD     0b000110
#define STATE_INDOOR_MIN_DISPLAY  0b000010
#define STATE_INDOOR_MAX_WORD     0b001011
#define STATE_INDOOR_MAX_DISPLAY  0b000011

#define STATE_OUTDOOR_DISPLAY     0b010001
#define STATE_OUTDOOR_MIN_WORD    0b010110
#define STATE_OUTDOOR_MIN_DISPLAY 0b010010
#define STATE_OUTDOOR_MAX_WORD    0b011011
#define STATE_OUTDOOR_MAX_DISPLAY 0b010011

// What state are we in?
extern int state;

// How long should we be alive for?
extern unsigned int stateChangeTics;

// If we should go to sleep.
extern volatile char goToSleep;

// Set the display etc to the relevant state
void setState();

#endif /* COMPASS_H_ */