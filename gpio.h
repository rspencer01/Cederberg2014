/*
 * gpio.h
 *
 * Created: 2014/10/18 09:32:13 PM
 *  Author: Robert Spencer
 */ 


#ifndef GPIO_H_
#define GPIO_H_

// The buffers for the io ports.  Used for digital
// output only.
extern unsigned char portB;
extern unsigned char portC;
extern unsigned char portD;

// The push buttons must have pull up resistors activated.
// Thus the following pins must be high
#define PORTD_PULL_UP 0b00001100

// Initialises the ports
void initPorts();

// Updates all the output ports.
void setPorts();

// Reads the push button (0 or 1)
char readPushButton(int);

#endif /* GPIO_H_ */