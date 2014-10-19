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

volatile char goToSleep=0;

int main(void)
{
  // Initialise all the things.
  initMicro();
  initPorts();
  initTimers();
  
  goToSleep = 1;
  // Loop forever
  while(1)
  {
    if (goToSleep)
    {
      goToSleep = 0;
      sleep();    
    }
  }
}