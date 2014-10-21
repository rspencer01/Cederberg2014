/// \file devices.c
/// Handles all the devices connected to the microcontroller
///
/// In this case this file handles the thermistors, the reading of the
/// same and the conversion/calibration.
#include "gpio.h"

/// Reads the given thermometer
///
/// Currently a dummy function, returning the raw voltage
/// scaled from 0 to 255.
/// 
/// \todo Do all calculations for temperature
int readThermometer(int thermometer)
{
  return readADC(thermometer);  
}