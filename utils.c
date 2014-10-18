/*
 * utils.c
 *
 * Created: 2014/03/21 11:36:43 AM
 *  Author: Robert Spencer
 */ 

#include <avr/io.h>

void initMicro()
{
  // Disable all interrupts
  SREG &= ~_BV(SREG_I);
  
  // Clear these bits, in this order, on startup/restart, as advised in datasheet (pg51)
  MCUSR &= ~_BV(WDRF);
  WDTCSR &= ~_BV(WDE);
  
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