/*
 * utils.c
 *
 * Created: 2014/03/21 11:36:43 AM
 *  Author: Robert Spencer
 */ 

#include <avr/io.h>
#include "sseg.h"

/// Initialises the microcontroller
/// 
/// Sets up the watchdog timer, and the pushbutton interrupts
/// and the type of sleep in the registers.
void initMicro()
{
  // Disable all interrupts
  SREG &= ~_BV(SREG_I);
  
  // Clear these bits, in this order, on startup/restart, as advised in datasheet (pg51)
  MCUSR &= ~_BV(WDRF);
  // Set the timer bits that let us change things
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  // Quickly!  Within 4 clock cycles we must set up the watchdog
  WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);  
  // Set the watchdog to give an interrupt
  WDTCSR |= _BV(WDIE);
  
  // Set the external interrupts
  EIMSK = _BV(INT0) | _BV(INT1);
  // Read on all edges
  EICRA = _BV(ISC10) | _BV(ISC00);  
  
  // Set the sleep mode to "power-down"
  SMCR = _BV(SM1);
  
  // Enable the interrupts again  
  SREG |= _BV(SREG_I);	
}

/// Puts the microcontroller in a sleep.
/// 
/// Prepares the chip to sleep, drawing as little current as
/// possible; sets the push buttons interrupts; and actually
/// goes to sleep.
void sleep()
{
  // Make the screen blank
  clearDisplay();
  // Push button interrupts on lows.  This is so that
  // we actually wake up.  Must be changed in the ISR.
  // Remember to disable/re-enable interrupts
  SREG &= ~_BV(SREG_I);
  EIMSK = _BV(INT0) | _BV(INT1);
  EICRA = 0;  
  SREG |= _BV(SREG_I);
  // Enable sleeping
  SMCR |= _BV(SE);  
  // Actually sleep
  __asm__ __volatile__("sleep");
}

/// Copies the nth bit of one variable onto another
/// 
/// Returns the destination character with the nth bit set
/// to the nth bit of the source.
unsigned char setNthBit(unsigned char dest, unsigned char src, int n)
{
  return dest ^ ( (dest & (1<<n)) ^ (src &(1<<n)) );
}
