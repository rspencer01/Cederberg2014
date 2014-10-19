/*
 * timers.c
 *
 * Created: 2014/03/23 05:44:31 PM
 *  Author: Robert Spencer
 */ 

#include <avr/interrupt.h>
#include "sseg.h"
#include "timers.h"
#include "Thermometer.h"
#include "sseg.h"
#include "gpio.h"

int timer_4ms_1s;
// Number of seconds to remain alive before falling asleep again
int alive = 0;

void initTimers()
{
  // Enable Interrupt TimerCounter0 Compare Match A (TIMER0_COMPA_vect)
  TIMSK0 = _BV(OCIE0A);
  // Mode = CTC (Clear timer on compare match)
  TCCR0A = _BV(WGM01);
  // The CKDIV8 fuse should be set.  On an internal clock of 8MHz, that
  // gives (with a prescaler of 64)
  // Clock/8 = 1MHz/64 = 64µs per tick
  TCCR0B = _BV(CS01) | _BV(CS00);
  // Set compare match to be 128 (64µs*128 = 8ms)
  OCR0A = 64;
  // Set up the scalars
  timer_4ms_1s = INI_4MS_1S;
}

ISR(TIMER0_COMPA_vect)
{
  // Update the display strobing
  updateDisplay();
    
  timer_4ms_1s--;
  if (timer_4ms_1s==0)
  {
    // Occurs every second
    timer_4ms_1s = INI_4MS_1S;
    // Decrease the alive timer
    if (alive>0)
    {
      // Go to sleep if we need to
      alive--;
      if (alive==0)
        goToSleep = 1;
    }
  }
}

ISR(WDT_vect)
{
  // Just go to sleep immediately.
  goToSleep = 1;
}

ISR(INT0_vect)
{
  // Change interrupts to be only on rising and falling
  // edges (by default they are triggered as long as the
  // pin is held low, which might be for a while).
  // This will be changed back in sleep()
  SREG &= ~_BV(SREG_I);
  EICRA = _BV(ISC10) | _BV(ISC00);  
  SREG |= _BV(SREG_I);
  // Write a dummy number out
  writeNumber(readOutdoor());
  // Stay alive for 3 seconds
  alive = 3;
}  

ISR(INT1_vect)
{
  // Change interrupts to be only on rising and falling
  // edges (by default they are triggered as long as the
  // pin is held low, which might be for a while).
  // This will be changed back in sleep()
  SREG &= ~_BV(SREG_I);
  EICRA = _BV(ISC10) | _BV(ISC00);
  SREG |= _BV(SREG_I);
  // Write a dummy number out
  writeNumber(readIndoor());
  // Stay alive for 3 seconds 
  alive = 3;
}
