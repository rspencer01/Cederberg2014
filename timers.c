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
#include "gpio.h"

int timer_8ms_1s;

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
  OCR0A = 128;
  // Set up the scalars
  timer_8ms_1s = INI_8MS_1S;
}

ISR(TIMER0_COMPA_vect)
{
  timer_8ms_1s--;
  if (timer_8ms_1s==0)
  {
    // Occurs every second
    timer_8ms_1s = INI_8MS_1S;
    // Flash at us
    portD ^= 0x80;
    setPorts();
  }
}