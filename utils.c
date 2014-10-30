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
  writeClear();
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

/// Delays for a given number of milliseconds
/// 
/// Simply enters a loop for a calculated number of iterations to 
/// block for the desired time.
/// 
/// \note This function is very approximate and is only correct
///       to within an order of magnitude.  Higher precision timing
///       should be done with the `timer0` interrupts.
void delay(int ms)
{ 
  unsigned int n;
  
  while (ms!=0)
  {
    // Gotten from some old code for a 4MHz processor and doubled
    n = 495;
    while (--n);
    --ms;
  }
}

///\todo Make sure these are reasonable ranges
/// The mesh values for the logarithm interpolation
long logMesh   [15] = {   20,   40,   60,  100,  200,  300,  500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500};
/// The function evaluation for the logarithm interpolation
long logValues [15] = {  990, 1060, 1100, 1151, 1221, 1261, 1312, 1382, 1422, 1451, 1473, 1491, 1507, 1520, 1532};

/// Performs a logarithm on the parameter.
/// 
/// Due to the limitations of integer arithmetic actually
/// returns `100 * log (x*1000)`, hence its name
int hunlogthou (int x)
{
    if (x<10) return 0;
    if (x>4500) return 0;
    int i = 0;
    while (x>logMesh[i]) i++;
    long interp = logValues[i] * (logMesh[i+1]-x)/(logMesh[i+1]-logMesh[i]) + logValues[i+1] * (x-logMesh[i])/(logMesh[i+1]-logMesh[i]);
    return interp;
}