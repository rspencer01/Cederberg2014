/// \file timers.c
/// Handles all the timers and other ISRs
///
/// Sets up the timer0 and handles the ISR for the
/// timer0, watchdog timer and pin interrupts, `INT0`
/// and `INT1`

#include <avr/interrupt.h>
#include "sseg.h"
#include "timers.h"
#include "Thermometer.h"
#include "sseg.h"
#include "devices.h"
#include "gpio.h"

/// A countdown to divide the 4ms timer into 40ms
int timer_4ms_40ms;
/// A countdown to divide the 40ms timer into 1s
int timer_40ms_1s;

/// A counter for the watchdog to read the thermometers.
int watchdogCount=INI_8S_64S;
/// A counter for minimum time between pushbutton ISR0s in multiples of 4ms
int dead_isr0_4ms;
/// A counter for minimum time between pushbutton ISR1s in multiples of 4ms
int dead_isr1_4ms;
/// A counter for how long the button has been held down for reset
int indoorHold;
/// A counter for how long the button has been held down for reset
int outdoorHold;

/// Handles the pushbutton press
///
/// Performs all the state changes etc regarding the indoor push
/// button press.  Instructs the main loop to immediately undergo
/// a state change, after setting the state variable correctly.
///
/// \todo Put this in another file!
void indoorPushbuttonPress()
{
  // Set the new state
  switch (state)
  {
    // If displaying the indoor temperature, initiate the min/max
    // cycle
    case STATE_INDOOR_DISPLAY:
    {
      state = STATE_INDOOR_MIN_WORD;
      break;
    }
    // Fast track the min to max cycle
    case STATE_INDOOR_MIN_WORD:
    case STATE_INDOOR_MIN_DISPLAY:
    {
      state = STATE_INDOOR_MAX_WORD;
      break;
    }
    // If we are showing max word do nothing, to fast track
    case STATE_INDOOR_MAX_WORD:
      break;
    // In other cases, just start to display the temperature
    default:
      state = STATE_INDOOR_DISPLAY_PRE;
  }
    
  // Force an immediate change
  stateChangeTics = 0;
  changeState = 1;
}

/// Handles the pushbutton press
/// 
/// Performs all the state changes etc regarding the outdoor push
/// button press.  Instructs the main loop to immediately undergo
/// a state change, after setting the state variable correctly.
///
/// \todo Put this in another file!
void outdoorPushbuttonPress()
{
  // Set the new state
  switch (state)
  {
    // If displaying the outdoor temperature, initiate the min/max
    // cycle
    case STATE_OUTDOOR_DISPLAY:
    {
      state = STATE_OUTDOOR_MIN_WORD;
      break;
    }
    // Fast track the min to max cycle
    case STATE_OUTDOOR_MIN_WORD:
    case STATE_OUTDOOR_MIN_DISPLAY:
    {
      state = STATE_OUTDOOR_MAX_WORD;
      break;
    }
    // If we are showing max word do nothing, to fast track    
    case STATE_OUTDOOR_MAX_WORD:
      break;    
    // In other cases, just start to display the temperature
    default:
    state = STATE_OUTDOOR_DISPLAY_PRE;
  }
  
  // Force an immediate change
  stateChangeTics = 0;
  changeState = 1;    
}

/// Initialises the timer module.
///
/// Sets up the timers to operate on a 4ms and a 1s basis.
void initTimers()
{
  // Enable Interrupt TimerCounter0 Compare Match A (TIMER0_COMPA_vect)
  TIMSK0 = _BV(OCIE0A);
  // Mode = CTC (Clear timer on compare match)
  TCCR0A = _BV(WGM01);
  // The CKDIV8 fuse should be set.  On an internal clock of 8MHz, that
  // gives (with a prescaler of 64)
  // Clock/8 = 1MHz/64 = 64 micro s per tick
  TCCR0B = _BV(CS01) | _BV(CS00);
  // Set compare match to be 128 (64micros*128 = 8ms)
  OCR0A = 64;
  // Set up the scalars
  timer_4ms_40ms = INI_4MS_40MS;
  timer_40ms_1s = INI_40MS_1S;
}

/// The interrupt service routine for the timer0
///
/// Updates the display strobe on a 4ms basis.  Also counts
/// down the time to be alive and sets the sleep flag if the
/// timeout is reached.
///
/// Finally, decreases counters for the holding down of buttons
/// 
ISR(TIMER0_COMPA_vect)
{
  // Update the display strobing
  updateDisplay();
  
  // Decrease dead times on the buttons
  if (dead_isr0_4ms>0)dead_isr0_4ms--;
  if (dead_isr1_4ms>0)dead_isr1_4ms--;
    
  timer_4ms_40ms--;
  if (timer_4ms_40ms==0)
  {
    // Occurs every 40ms
    timer_4ms_40ms = INI_4MS_40MS;
    timer_40ms_1s--;
    if (timer_40ms_1s==0)
    {
      // Occurs every second
      timer_40ms_1s = INI_40MS_1S;      
      // Decrease the hold timer if the button is down, else reset it.
      if (readPushButton(INDOOR_PUSHBUTTON))
        indoorHold--;
      else
        indoorHold = HOLD_TIME;
      // If we have reached the timeout, reset the min/max
      if (indoorHold==0)
      {
        // Perhaps we have been told to go to sleep.  Don't.
        goToSleep = 0;
        state = STATE_INDOOR_RESET;
        // Ignore everything to do with pushbuttons.
        EIMSK = 0;
      }      
    
      // Decrease the hold timer if the button is down, else reset it.
      if (readPushButton(OUTDOOR_PUSHBUTTON))
        outdoorHold--;
      else
        outdoorHold = HOLD_TIME;
      // If we have reached the timeout, reset the min/max
      if (outdoorHold==0)
      {
        // Perhaps we have been told to go to sleep.  Don't.
        goToSleep = 0;
        state = STATE_OUTDOOR_RESET;
        // Ignore everything to do with pushbuttons.
        EIMSK = 0;
      }
      // Update displays etc
      changeState = 1;      
    }      
  }
}

/// The watchdog interrupt vector
///
/// Called whenever the watchdog times out (once every 8s)
/// Reads the thermometers in order to keep a minimum/maximum
/// that is updated every 64s.
ISR(WDT_vect)
{
  if (state!=STATE_SLEEP)
    return;
  watchdogCount--;
  if (watchdogCount==0)
  {
    // Simply read the temperatures.  This will update min/max
    // automatically.
    readThermometer(INDOOR_THERMOMETER);
    readThermometer(OUTDOOR_THERMOMETER);
    // Reset the counter
    watchdogCount = INI_8S_64S;    
  }
  // Just go to sleep immediately, if the timer won't do it
  if (state==STATE_SLEEP)
    goToSleep = 1;
}

/// The INT0 vector
/// 
/// Called by the depression of pushbutton 1.  In general use
/// (ie, when sleeping) this is called on a low.  However, this,
/// if continued, would result in the interrupt firing every time
/// the interrupt enable flag is set. Thus the interrupt is 
/// immediately changed to only trigger on falling edges for both
/// pushbuttons.
/// 
/// In addition, to debounce the input, a minimum time between triggers
/// is enforced by the `dead_isr0_4ms` counter.
ISR(INT0_vect)
{
  // Change interrupts to be only on falling edges (pushbutton pressed)
  EICRA = _BV(ISC11) | _BV(ISC01);
  
  // Prevent bouncing
  if (dead_isr0_4ms>0)
    return;
  dead_isr0_4ms = DEBOUNCE_TIMOUT_4MS;  
  
  // Do the indoor stuff
  indoorPushbuttonPress();
}    

/// The INT1 vector
///
/// Called by the depression of pushbutton 2.  In general use
/// (ie, when sleeping) this is called on a low.  However, this,
/// if continued, would result in the interrupt firing every time
/// the interrupt enable flag is set. Thus the interrupt is
/// immediately changed to only trigger on falling edges for both
/// pushbuttons.
ISR(INT1_vect)
{
  // Change interrupts to be only on falling edges (pushbutton pressed)
  EICRA = _BV(ISC11) | _BV(ISC01);
  
  // Prevent bouncing
  if (dead_isr1_4ms>0)
    return;
  dead_isr1_4ms = DEBOUNCE_TIMOUT_4MS;  
  
  // Do the outdoor stuff.
  outdoorPushbuttonPress();
}
