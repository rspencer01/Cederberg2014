/// \file devices.c
/// Handles all the devices connected to the microcontroller
///
/// In this case this file handles the thermistors, the reading of the
/// same and the conversion/calibration.
#include "devices.h"
#include "gpio.h"
#include "utils.h"

/// The low of the outdoor thermometer
int outdoorLow = 999;
/// The high of the outdoor thermometer
int outdoorHigh = 0;

/// The low of the indoor thermometer
int indoorLow = 999;
/// The high of the indoor thermometer
/// \todo Make this initially some negative value
int indoorHigh = 0;

/// Reads the given thermometer
///
/// Reads the thermometer and updates the relevant indoor/outdoor
/// min/max temperatures
/// 
/// \todo Do all calculations for temperature
int readThermometer(int thermometer)
{
  int actual = readADC(ADC_CHANNEL_OUTDOOR);  
  int reference = readADC(ADC_CHANNEL_REFERENCE);
  long long calc = ((long long)actual * 1024) / reference;
  
  // Update the min/max
  if (thermometer == INDOOR_THERMOMETER)
  {
    indoorLow  = min(indoorLow,(int)calc);
    indoorHigh = max(indoorLow,(int)calc);
  }
  if (thermometer == OUTDOOR_THERMOMETER)
  {
    outdoorLow  = min(outdoorLow,(int)calc);
    outdoorHigh = max(outdoorLow,(int)calc);
  }  
  
  return (int) calc;
}