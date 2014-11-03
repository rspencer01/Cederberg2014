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

/// Set to 1 when the main loop is to change the state
volatile char changeState=0;

/// The left or right tics to be displayed.  Not to be confused with ticks
/// which are units of time.
int tics = 0;
/// The message to be displayed inbetween the tics
int tics_message = 0;

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
  resetMinMax(INDOOR_THERMOMETER);
  resetMinMax(OUTDOOR_THERMOMETER);
  
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
    // If we are to change state, do so
    if (changeState)
    {
      changeState = 0;
      setState();
    }      
  }
}

/// Sets the display and updates the state
/// 
/// This function is to be called every second from the timer.
/// It displays things like min/max and temperatures, and advances
/// us through the display cycles.
/// 
/// \todo Call this something else.  This name is not clear
void setState()
{
  // If we are to change something, do so
  if (stateChangeTics == 0)
  {
    // Special case is either of the states that display Lo.
    // They have bit structure such that testing for these states
    // is very easy.
    if (state & STATE_DISPLAY_MIN)
    {
      // Write out the message
      writeMessage(SSEG_MSG_LO);
      // Kill the display min bit, leaving the others
      state &= ~STATE_DISPLAY_MIN;
      // Keep for 1 second
      stateChangeTics = 1;
      // We are done here
      return;
    }
    // Similarly for the Hi message
    if (state & STATE_DISPLAY_MAX)
    {
      // Write out the message
      writeMessage(SSEG_MSG_HI);
      // Kill the display max bit, leaving the others
      state &= ~STATE_DISPLAY_MAX;
      // Keep for 1 second
      stateChangeTics = 1;
      // We are done here
      return;
    }    
    // Otherwise a switch statement is cleaner
    switch (state)
    {
      // We are to go to sleep.  Tell the main loop
      case STATE_SLEEP:
      {
        tics = 0;
        goToSleep = 1;
        break;
      }
      // Someone (an ISR) has asked us to display the
      // indoor temperature.
      case STATE_INDOOR_DISPLAY_PRE:
      {
        // Switch off the display, momentarily.
        // Display the temperature (dummy for now)
        writeNumber(readThermometer(INDOOR_THERMOMETER));
        // Change the state to say that we have done it.
        state=STATE_INDOOR_DISPLAY;
        // Keep for 3 seconds
        stateChangeTics = 3;
        break;
      }
      // ... and the same with the oudoor temperature
      case STATE_OUTDOOR_DISPLAY_PRE:
      {
        // Display the temperature (dummy for now)
        writeNumber(readThermometer(OUTDOOR_THERMOMETER));
        // Change the state to say that we have done it.
        state=STATE_OUTDOOR_DISPLAY;
        // Keep for 3 seconds
        stateChangeTics = 3;
        break;
      }
      // If we timeout on either display of timer ticks,
      // go to sleep
      case STATE_INDOOR_DISPLAY:
      case STATE_OUTDOOR_DISPLAY:
      {
          state=STATE_SLEEP;
          stateChangeTics = 0;
          break;
      }
      // Display the minimum for indoors
      case STATE_INDOOR_MIN_DISPLAY:
      {
        // Display the minimum (dummy for now)
        writeNumber(indoorLow);
        // Move to the next state in 3 ticks
        state = STATE_INDOOR_MAX_WORD;
        stateChangeTics = 3;
        break; 
      }
      // ... and the same for outdoors
      case STATE_OUTDOOR_MIN_DISPLAY:
      {
        // Display the minimum (dummy for now)
        writeNumber(outdoorLow);
        // Move to the next state in 3 ticks
        state = STATE_OUTDOOR_MAX_WORD;
        stateChangeTics = 3;
        break;
      }
      // ... and the same for the two maximums
      // except this time go to sleep afterwards.
      case STATE_INDOOR_MAX_DISPLAY:
      {
        writeNumber(indoorHigh);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }
      case STATE_OUTDOOR_MAX_DISPLAY:
      {
        writeNumber(outdoorHigh);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }      
      // Reset the indoor thermometer
      case STATE_INDOOR_RESET:
      {
        writeMessage(SSEG_MSG_CLR);
        resetMinMax(INDOOR_THERMOMETER);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }
      // Reset the outdoor thermometer
      case STATE_OUTDOOR_RESET:
      {
        writeMessage(SSEG_MSG_CLR);
        resetMinMax(OUTDOOR_THERMOMETER);
        state = STATE_SLEEP;
        stateChangeTics = 3;
        break;
      }      
      // Initialise the flashing CAL and left right ticks
      case STATE_CALIBRATE_INIT:
      {
        state = STATE_CALIBRATE_WAIT;
        writeMessage(SSEG_MSG_CAL);
        tics_message = SSEG_MSG_CAL;
        tics = LEFT|RIGHT;
        stateChangeTics = 15;
        break;
      }
      // On timeout, we go to sleep
      case STATE_CALIBRATE_WAIT:
      {
        state = STATE_SLEEP;
        stateChangeTics = 0;
        break;        
      }
      // Write out the equality message
      case STATE_CALIBRATE_INDOOR_PRE:
      {
        state = STATE_CALIBRATE_INDOOR;
        writeMessage(SSEG_MSG_EQU);
        tics_message = SSEG_MSG_EQU;
        tics = LEFT;
        stateChangeTics = 7;
        break;        
      }
      // Write out the ice message thingy
      case STATE_CALIBRATE_OUTDOOR_PRE:
      {
        state = STATE_CALIBRATE_OUTDOOR;
        writeMessage(SSEG_MSG_ICE);
        tics_message = SSEG_MSG_ICE;
        tics = RIGHT;
        stateChangeTics = 7;
        break;
      }      
      // Do nothing for now, except remove the tics and go to sleep.
      case STATE_CALIBRATE_INDOOR:
      {
        calibrateIndoor();
        tics = 0;
        state = STATE_SLEEP;
        goToSleep = 1;
      }
      case STATE_CALIBRATE_OUTDOOR:
      {
        calibrateOutdoor();
        tics = 0;
        state = STATE_SLEEP;
        goToSleep = 1;
      }
      // Something went wrong.  Just go to sleep.
      default:
      {
        state=STATE_SLEEP;
        goToSleep = 1;
      }        
    }      
  }
  // Decrease the state ticks if it is not zero already
  else
    stateChangeTics --;
  // Some ops require inter state screen changes
  if (tics)
  {
    if (stateChangeTics%2==0)
      writeTick(tics);
    else
      writeMessage(tics_message);
  }
}   