/*
 * Compass.c
 *
 * Created: 2014/03/21 10:56:00 AM
 *  Author: Robert Spencer
 */ 

#include "Thermometer.h"
#include "utils.h"
#include "timers.h"
#include "gpio.h"

int main(void)
{
  // Initialise all the things.
  initMicro();
  initPorts();
  initTimers();
  // Set all ports to zero (all segments active), except one display enable.
  portB = 0x0;
  portC = 0x0;
  portD = 0b00010000;
  setPorts();
  // Loop forever
  while(1);
}