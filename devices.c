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

/// The parameter 1/r_inf multiplied by 1000
//long reciprinf1000 = 8431;

/// The B parameter of the indoor thermometer
long Bin = 3380;
/// The B parameter of the outdoor thermometer
long Bout = 3380;

/// Calibrates the outdoor thermometer
/// 
/// The outdoor thermometer is calibrated in ice, at 273K exactly.
/// The stored value (for both the indoor and outdoor thermistors)
/// is the coefficient B.
void calibrateOutdoor()
{
  // Read the resistor
  int actual = readADC(ADC_CHANNEL_OUTDOOR);
  int reference = readADC(ADC_CHANNEL_REFERENCE);
  // Thousand times fraction of voltage
  long f = ((long)actual *1000) / reference;
  // Resistance of thermistor
  volatile long R = f*SERIES_RESISTOR_VALUE/(1000-f);
  // Actual temperature and R_0
  long Tact = 2732;
  long R0 = 10000;
  // Split the calculation up to get maximum accuracy (ie
  // keep the number as big as possible for as long as 
  // possible)
  Bout = (2982*Tact)/100;
  Bout *= thouloghundredth((100*R)/R0);
  Bout /=100*(2982-Tact);
}

/// Calibrates the indoor thermometer
///
/// The in thermometer is calibrated _from the outdoor thermometer_
/// The stored value (for both the indoor and outdoor thermistors)
/// is the coefficient B.
void calibrateIndoor()
{
  // Read the resistor
  int actual = readADC(ADC_CHANNEL_INDOOR);
  int reference = readADC(ADC_CHANNEL_REFERENCE);
  // Thousand times fraction of voltage
  long f = ((long)actual *1000) / reference;
  // Resistance of thermistor
  volatile long R = f*SERIES_RESISTOR_VALUE/(1000-f);
  // Read the actual temperature and R_0
  long Tact = (readThermometer(OUTDOOR_THERMOMETER)+27315)/10;
  long R0 = 10000;
  // Split the calculation up to get maximum accuracy (ie
  // keep the number as big as possible for as long as
  // possible)
  Bin = (2982*Tact)/100;
  Bin *= thouloghundredth((100*R)/R0);
  Bin /=100*(2982-Tact);
}


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
/// min/max temperatures.  Returns a value 100 times larger than
/// the actuall temperature
int readThermometer(int thermometer)
{
  int actual = readADC(ADC_CHANNEL_OUTDOOR);  
  int reference = readADC(ADC_CHANNEL_REFERENCE);
  // Thousand times fraction of voltage
  long f = ((long)actual *1000) / reference;
  // Resistance of thermistor
  long R = f*SERIES_RESISTOR_VALUE/(1000-f);
  // Temperature calculation.  Start with the R_0 and T_0 which are known
  long R0 = 10000;
  long T0 = 298;
  // Reserve space
  long T;
  long B;
  // Select the correct coefficient
  if (thermometer==INDOOR_THERMOMETER)
    B = Bin;
  else
    B = Bout;
  // Do the temperature calculation.  Remember, we want a value 100 times
  // to big!
  T = (100*B*T0)/(B+T0*thouloghundredth((100*R)/R0)/1000)-27315;
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