/*
 * Compass.c
 *
 * Created: 2014/03/21 10:56:00 AM
 *  Author: Robert Spencer
 */ 


#include <avr/io.h>
#include "Thermometer.h"
#include "utils.h"
#include "timers.h"

int main(void)
{
  // Initialise all the things.
  initMicro();
  initPorts();
  initTimers();
  // Set all ports to zero (all segments active), except one display enable.
  PORTB = 0x0;
  PORTC = 0x0;
  PORTD = 0b00010000;
  // Loop forever
  while(1);
}