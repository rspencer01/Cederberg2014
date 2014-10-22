/// \file Thermometer.c
/// Handles the main loop of the device

#include "Thermometer.h"
#include "utils.h"
#include "timers.h"
#include "gpio.h"
#include "devices.h"

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
  // Reset the thermometer
  resetMinMax();
  
  goToSleep = 1;
  // Loop forever
  while(1)
  {
    if (goToSleep)
    {
      // If either push button is down, don't go to sleep!  This is
      // because we wake on a pushbutton low, via an interrupt, and
      // setting interrupts will immediately vector there.
      //
      // Do this here because we will want to try again.
      if (readPushButton(0) || readPushButton(1))
        continue;
      goToSleep = 0;
      sleep();    
    }
  }
}