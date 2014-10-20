/// \file devices.c
/// Handles all the devices connected to the microcontroller
///
/// In this case this file handles the thermistors, the reading of the
/// same and the conversion/calibration.

/// Reads the indoor thermometer
///
/// Currently a dummy function, returning 123
unsigned char readIndoor()
{
  return 123;  
}

/// Reads the outdoor thermometer
///
/// Currently a dummy function, returning 213
unsigned char readOutdoor()
{
  return 213;  
}