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
/// value, and reads the thermometer in order to set them to their
/// actual value.
void resetMinMax(int therm)
{
  // Reset the right values
  if (therm == INDOOR_THERMOMETER)
  {
    indoorLow = 9999;
    indoorHigh = -5999;
  }
  else
  {
    outdoorLow = 9999;
    outdoorHigh = -5999;
  }
  // Read in the thermometer, just to get the values set to the
  // current temperature.
  readThermometer(therm);
}

/// Reads the given thermometer
///
/// Reads the thermometer and updates the relevant indoor/outdoor
/// min/max temperatures
int readThermometer(int thermometer)
{
  int actual = readADC(ADC_CHANNEL_OUTDOOR);  
  int reference = readADC(ADC_CHANNEL_REFERENCE);
  // Thousand times fraction of voltage
  long f = ((long)actual *1000) / reference;
  // Resistance of thermistor
  long R = f*SERIES_RESISTOR_VALUE/(1000-f);
  // 100 / r
  long reciprinf100 = 843;
  // Paramter.
  long B = 3380;
  // Temperature calculation
  long T = (100*100*B) / hunlogthou(((R*reciprinf100) / 100) / 1000) - 27300;
    
  // Update the min/max
  if (thermometer == INDOOR_THERMOMETER)
  {
    indoorLow  = min(indoorLow,(int)T);
    indoorHigh = max(indoorHigh,(int)T);
  }
  if (thermometer == OUTDOOR_THERMOMETER)
  {
    outdoorLow  = min(outdoorLow,(int)T);
    outdoorHigh = max(outdoorHigh,(int)T);
  }  
  return T;
}