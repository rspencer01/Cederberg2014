/*
 * devices.h
 *
 * Created: 2014/10/19 06:17:21 PM
 *  Author: Robert Spencer
 */ 


#ifndef DEVICES_H_
#define DEVICES_H_

#define INDOOR_THERMOMETER 0
#define OUTDOOR_THERMOMETER 1

// Reads the given thermometer
int readThermometer(int);

// Clears the minimum and maximums
void resetMinMax();

// The min outdoor temperature
extern int outdoorLow;
// The max outdoor temperature
extern int outdoorHigh;
// The min indoor temperature
extern int indoorLow;
// The max indoor temperature
extern int indoorHigh;

#endif /* DEVICES_H_ */