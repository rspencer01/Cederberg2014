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
long logMesh      [81] =  {   20,   22,   24,   26,   28,   30,   33,   36,   39,   42,   45,   48,   50,
55,   60,   65,   70,   75,   80,   85,   90,   95,  100,  110,  120,  130,  140,
150,  160,  170,  185,  200,  220,  240,  260,  280,  300,  320,  340,  360,
390,  420,  450,  480,  510,  550,  590,  630,  680,  700,  750,  800,  850,
900,  950, 1000, 1050, 1100, 1150, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900,
2000, 2100, 2250, 2400, 2650, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200,
4500};
/// The function evaluation for the logarithm interpolation
long logValues1000 [81] = {-1609,-1514,-1427,-1347,-1273,-1204,-1109,-1022, -942, -868, -799, -734, -693, -
598, -511, -431, -357, -288, -223, -163, -105,  -51,    0,   95,  182,  262,  336,
  405,  470,  531,  615,  693,  788,  875,  956, 1030, 1099, 1163, 1224, 1281,
1361, 1435, 1504, 1569, 1629, 1705, 1775, 1841, 1917, 1946, 2015, 2079, 2140, 2197,
 2251, 2303, 2351, 2398, 2442, 2485, 2565, 2639, 2708, 2773, 2833, 2890, 2944,
  2996, 3045, 3114, 3178, 3277, 3332, 3401, 3466, 3526, 3584, 3638, 3689, 3738,
3807};

/// Performs a logarithm on the parameter.
/// 
/// Due to the limitations of integer arithmetic actually
/// returns `1000 * log (x/100)`, hence its name
int thouloghundredth (int x)
{
    if (x<20) return 0;
    if (x>4200) return 0;
    int i = 0;
    while (x>logMesh[i+1]) i++;
    long interp = logValues1000[i] * (logMesh[i+1]-x)/(logMesh[i+1]-logMesh[i]) + logValues1000[i+1] * (x-logMesh[i])/(logMesh[i+1]-logMesh[i]);
    return interp;
}