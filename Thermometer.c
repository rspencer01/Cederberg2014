/// \file Thermometer.c
/// Handles the main loop of the device

#include "Thermometer.h"
#include "utils.h"
#include "timers.h"
#include "gpio.h"

volatile char goToSleep=0;

/// The main entry point of the project.
///
/// Performs all initialisation and then hangs in an
/// infinite loop, repeatedly going to sleep when 
/// requested via the `goToSleep` flag.
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