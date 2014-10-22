/// \file Thermometer.c
/// Handles the main loop of the device

#include "Thermometer.h"
#include "utils.h"
#include "timers.h"
#include "gpio.h"
#include "devices.h"

/// The current state of the system
int state;

/// Number of state ticks for which to remain alive before falling asleep
int alive =0;

/// Set to 1 when the main loop is to put the microcontroller to sleep
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

/// Sets the display and updates the state
/// 
/// This function is to be called every second from the timer.
/// It displays things like min/max and temperatures.
/// 
/// \todo This function is very time heavy.  Don't call it from ISR.
/// 
/// \todo Call this something else.  This name is not clear
void setState()
{
  // Check if we should be going to sleep
  if (alive==0)
  {
    goToSleep = 1;
    return;
  }     
  // Decrease our alive counter
  alive--;
  
  // Check the states and do the right thing.
  if (state==STATE_INDOOR_DISPLAY)
    writeNumber(readThermometer(INDOOR_THERMOMETER));
  if (state==STATE_OUTDOOR_DISPLAY)
    writeNumber(readThermometer(OUTDOOR_THERMOMETER));
}   