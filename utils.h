/*
 * utils.h
 *
 * Created: 2014/03/21 11:36:55 AM
 *  Author Robert Spencer
 */ 


#ifndef UTILS_H_
#define UTILS_H_

// Typical minimum
#define min(_a,_b) (_a)<(_b)?(_a):(_b)
// Typical maximum
#define max(_a,_b) (_a)>(_b)?(_a):(_b)

// Initialises all the flags for the watchdog and interrupts etc
void initMicro();

// Sets up interrupts and puts the microcontroller in sleep mode
void sleep();

// Sets the nth bit of the first argument to the nth bit of the second and returns the result
unsigned char setNthBit(unsigned char, unsigned char, int);

// Delays for approximately that many milliseconds
void delay(int);

// Calculates the logarithm of the given number
int thouloghundredth (int);

#endif /* UTILS_H_ */