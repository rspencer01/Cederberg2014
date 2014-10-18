/*
 * Compass.c
 *
 * Created: 2014/03/21 10:56:00 AM
 *  Author: Robert
 */ 


#include <avr/io.h>
#include "Thermometer.h"
#include "utils.h"

int main(void)
{
	initMicro();
	initPorts();
	PORTB = 0x0;
	PORTC = 0x0;
	PORTD = 0b00010000;
	while(1);
}