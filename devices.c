/// \file devices.c
/// Handles all the devices connected to the microcontroller
///
/// In this case this file handles the thermistors, the reading of the
/// same and the conversion/calibration.
#include "devices.h"
#include "gpio.h"
#include "utils.h"

/// The low of the outdoor thermometer
int outdoorLow;
/// The high of the outdoor thermometer
int outdoorHigh;

/// The low of the indoor thermometer
int indoorLow;
/// The high of the indoor thermometer
int indoorHigh;

/// Resets the minima and maxima
///
/// Sets the minima to some large value and the maxima to a small
/// value, and reads the thermometers in order to set them to their
/// actual value.
/// 
/// \todo Make the high values initially negative
void resetMinMax()
{
  outdoorLow = indoorLow = 999;
  outdoorHigh = indoorHigh = 0;
  readThermometer(INDOOR_THERMOMETER);
  readThermometer(OUTDOOR_THERMOMETER);
}

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
    indoorHigh = max(indoorHigh,(int)calc);
  }
  if (thermometer == OUTDOOR_THERMOMETER)
  {
    outdoorLow  = min(outdoorLow,(int)calc);
    outdoorHigh = max(outdoorHigh,(int)calc);
  }  
  
  return (int) calc;
}