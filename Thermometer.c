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
  // Go to sleep
  sleep();
  // Loop forever
  while(1);
}