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

extern int outdoorLow;
extern int outdoorHigh;
extern int indoorLow;
extern int indoorHigh;

#endif /* DEVICES_H_ */