/*
 * Thermometer.h
 *
 * Created: 2014/09/17 09:29:32 PM
 *  Author: Robert Spencer
 */ 

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#define STATE_DISPLAY_MIN         0b00100
#define STATE_DISPLAY_MAX         0b01000

#define STATE_INDOOR_DISPLAY      0b00000
#define STATE_INDOOR_MIN_WORD     0b00110
#define STATE_INDOOR_MIN_DISPLAY  0b00010
#define STATE_INDOOR_MAX_WORD     0b01011
#define STATE_INDOOR_MAX_DISPLAY  0b00011

#define STATE_OUTDOOR_DISPLAY     0b10000
#define STATE_OUTDOOR_MIN_WORD    0b10110
#define STATE_OUTDOOR_MIN_DISPLAY 0b10010
#define STATE_OUTDOOR_MAX_WORD    0b11011
#define STATE_OUTDOOR_MAX_DISPLAY 0b10011

// What state are we in?
extern int state;

// How long should we be alive for?
extern int alive;

// If we should go to sleep.
extern volatile char goToSleep;

// Set the display etc to the relevant state
void setState();

#endif /* COMPASS_H_ */