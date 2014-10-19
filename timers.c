/*
 * timers.c
 *
 * Created: 2014/03/23 05:44:31 PM
 *  Author: Robert Spencer
 */ 

#include <avr/interrupt.h>
#include "sseg.h"
#include "timers.h"
#include "devices.h"
#include "Thermometer.h"
#include "sseg.h"

int timer_4ms_1s;
int count = 0;

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
    // Write a dummy number to the display for testing purposes.
    count++;
    writeNumber(count);
  }
}