/*
 * utils.c
 *
 * Created: 2014/03/21 11:36:43 AM
 *  Author: Robert Spencer
 */ 

#include <avr/io.h>
#include "sseg.h"

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

void initPorts()
{
  //Port B:
  // Pin 0: SG2
  // Pin 1: nc
  // Pin 2: nc
  // Pin 3: PROG
  // Pin 4: PROG
  // Pin 5: PROG
  // Pin 6: SG1
  // Pin 7: SG4
  DDRB = 0b11111111;
  
  //Port C:
  // Pin 0: Therm Out
  // Pin 1: Therm In
  // Pin 2: Therm Mid
  // Pin 3: Therm Mid
  // Pin 4: SG7
  // Pin 5: SG5
  // Pin 6: PROG
  DDRC = 0b1110000;
  
  //Port D:
  // Pin 0: SG0
  // Pin 1: D2
  // Pin 2: PBout
  // Pin 3: PBin
  // Pin 4: D1
  // Pin 5: SG3
  // Pin 6: D0
  // Pin 7: SG6
  DDRD = 0b11110011;
}

// Returns the destination character with the nth bit set
// to the nth bit of the source.
unsigned char setNthBit(unsigned char dest, unsigned char src, int n)
{
  return dest ^ ( (dest & (1<<n)) ^ (src &(1<<n)) );
}

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