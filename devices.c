/// \file devices.c
/// Handles all the devices connected to the microcontroller
///
/// In this case this file handles the thermistors, the reading of the
/// same and the conversion/calibration.
#include "gpio.h"

/// Reads the indoor thermometer
///
/// Currently a dummy function, returning 123
int readIndoor()
{
  return 123;  
}

/// Reads the outdoor thermometer
///
/// Currently reads the ADC and returns that.
int readOutdoor()
{
  return readADC(0); 
}