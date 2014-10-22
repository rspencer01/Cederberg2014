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

/// A countdown to divide the 4ms timer into 1s
int timer_4ms_1s;
/// A counter for the watchdog to read the thermometers.
int watchdogCount=INI_8S_64S;
/// Which pushbutton was pushed last?
int lastPushButton;

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
  timer_4ms_1s = INI_4MS_1S;
}

/// The interrupt service routine for the timer0
///
/// Updates the display strobe on a 4ms basis.  Also counts
/// down the time to be alive and sets the sleep flag if the
/// timeout is reached.
ISR(TIMER0_COMPA_vect)
{
  // Update the display strobing
  updateDisplay();
    
  timer_4ms_1s--;
  if (timer_4ms_1s==0)
  {
    // Occurs every second
    timer_4ms_1s = INI_4MS_1S;
    // Update displays etc
    setState(); 
  }
}

/// The watchdog interrupt vector
///
/// Called whenever the watchdog times out (once every 8s)
/// Reads the thermometers in order to keep a minimum/maximum
/// that is updated every 64s.
///
/// \todo Test that the reading actually works.
ISR(WDT_vect)
{
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
  if (alive==0)
    goToSleep = 1;
}

/// The INT0 vector
/// 
/// Called by the depression of pushbutton 1.  In general use
/// (ie, when sleeping) this is called on a low.  However, this,
/// if continued, would result in the interrupt firing every time
/// the interrupt enable flag is set.
/// 
/// Thus the interrupt is immediately changed to only trigger on the
/// other pushbutton.  When we go to sleep, we will enable both.
ISR(INT0_vect)
{
  // Change interrupts to be only on other pushbutton
  EIMSK = _BV(INT1);
  
  // Write out the thermometer value
  state = STATE_INDOOR_DISPLAY;
  
  // Who are we?
  lastPushButton = INDOOR_PUSHBUTTON;
  // Stay alive for 3 seconds
  alive = 3;
}  

/// The INT1 vector
///
/// Called by the depression of pushbutton 2.  In general use
/// (ie, when sleeping) this is called on a low.  However, this,
/// if continued, would result in the interrupt firing every time
/// the interrupt enable flag is set.
///
/// Thus the interrupt is immediately changed to only trigger on the
/// other pushbutton.  When we go to sleep, we will enable both.
ISR(INT1_vect)
{
  // Change interrupts to be only on other pushbutton
  EIMSK = _BV(INT0);
  
  // Write out the thermometer value
  state = STATE_OUTDOOR_DISPLAY;
  
  // Who are we?
  lastPushButton = OUTDOOR_PUSHBUTTON;
  // Stay alive for 3 seconds 
  alive = 3;
}
