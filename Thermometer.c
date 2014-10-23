/// \file Thermometer.c
/// Handles the main loop of the device

#include "Thermometer.h"
#include "utils.h"
#include "timers.h"
#include "gpio.h"
#include "sseg.h"
#include "devices.h"

/// The current state of the system
int state;

/// The number of timer ticks until the next state change
unsigned int stateChangeTics;

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
/// It displays things like min/max and temperatures, and advances
/// us through the display cycles.
/// 
/// \todo This function is very time heavy.  Don't call it from ISR.
/// 
/// \todo Call this something else.  This name is not clear
void setState()
{
  if (stateChangeTics == 0)
  {
    if (state & STATE_DISPLAY_MIN)
    {
      writeMessage(SSEG_MSG_LO);
      state &= ~STATE_DISPLAY_MIN;
      stateChangeTics = 1;
      return;
    }
    if (state & STATE_DISPLAY_MAX)
    {
      writeMessage(SSEG_MSG_HI);
      state &= ~STATE_DISPLAY_MAX;
      stateChangeTics = 1;
      return;
    }    
    switch (state)
    {
      case STATE_SLEEP:
      {
        goToSleep = 1;
        break;
      }
      case STATE_INDOOR_DISPLAY_PRE:
      {
        writeNumber(100);
        state=STATE_INDOOR_DISPLAY;
        stateChangeTics = 3;
        break;
      }
      case STATE_OUTDOOR_DISPLAY_PRE:
      {
        writeNumber(200);
        state=STATE_OUTDOOR_DISPLAY;
        stateChangeTics = 3;
        break;
      }
      case STATE_INDOOR_DISPLAY:
      case STATE_OUTDOOR_DISPLAY:
      {
          state=STATE_SLEEP;
          goToSleep = 1;
          break;
      }
      case STATE_INDOOR_MIN_DISPLAY:
      {
        writeNumber(1);
        state = STATE_INDOOR_MAX_WORD;
        stateChangeTics = 3;
        break; 
      }
      case STATE_OUTDOOR_MIN_DISPLAY:
      {
        writeNumber(2);
        state = STATE_OUTDOOR_MAX_WORD;
        stateChangeTics = 3;
        break;
      }
      case STATE_INDOOR_MAX_DISPLAY:
      {
        writeNumber(3);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }
      case STATE_OUTDOOR_MAX_DISPLAY:
      {
        writeNumber(4);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }      
      default:
      {
        state=STATE_SLEEP;
        goToSleep = 1;
      }        
    }      
  }
  else
    stateChangeTics --;
}   