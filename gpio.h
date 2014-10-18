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

// Updates all the output ports.
void setPorts();

#endif /* GPIO_H_ */