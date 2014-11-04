/// \file gpio.c
/// Handles all the general purpose IO for the device.
///
/// Sets up the registers to handle input/output correctly
/// as well as the writing to the digital ports.

#include <avr/io.h>
#include "gpio.h"
#include "utils.h"
#include "Thermometer.h"

/// Internal buffer for port B
unsigned char portB = 0;
/// Internal buffer for port C
unsigned char portC = 0;
/// Internal buffer for port D
unsigned char portD = 0;

/// Initialises the input/output of all the ports
///
/// Sets the pins to be input or output according to the following
/// port scheme.  By default the directions are set to out, if they
/// are inputs or programmers or no connect.
/// 
/// __Port B__
/// |Pin Number | Usage      | I/O  |
/// |:---------:|:----------:|:----:|
/// |`0`        | `SSEG 2`   | `O`  |
/// |`1`        | no connect |      |
/// |`2`        | no connect |      |
/// |`3`        | programmer |      |
/// |`4`        | programmer |      |
/// |`5`        | programmer |      |
/// |`6`        | `SSEG 1`   | `O`  |
/// |`7`        | `SSEG 4`   | `O`  |
///
/// __Port C__
/// |Pin Number | Usage                 | I/O  |
/// |:---------:|:---------------------:|:----:|
/// |`0`        | Outdoor Thermometer   | `I`  |
/// |`1`        | Indoor Thermometer    | `I`  |
/// |`2`        | Thermometer Max Read  | `I`  |
/// |`3`        | Thermometer Drive     | `O`  |
/// |`4`        | `SSEG 7`              | `O`  |
/// |`5`        | `SSEG 5`              | `O`  |
/// |`6`        | programmer            |      |
/// |`7`        | no connect            |      |
///
/// __Port D__
/// |Pin Number | Usage              | I/O  |
/// |:---------:|:------------------:|:----:|
/// |`0`        | `SSEG 0`           | `O`  |
/// |`1`        | `SSEG SELECT 0`    | `O`  |
/// |`2`        | Outdoor Pushbutton | `I`  |
/// |`3`        | Indoor Pushbutton  | `I`  |
/// |`4`        | `SSEG SELECT 1`    | `O`  |
/// |`5`        | `SSEG 3`           | `O`  |
/// |`6`        | `SSEG SELECT 0`    | `O`  |
/// |`7`        | `SSEG 6`           | `O`  |
void initPorts()
{
  DDRB = 0b11111111;
  
  DDRC = 0b11111000;
  
  DDRD = 0b11110011;
}

/// Sets all the ports to be the values stored in the buffers.
///
/// We use buffers for the ports as operations such as `PORTD^=0x02` are
/// not well defined (it implies a read of an output pin, which may be
/// sensitive on the circuitry).
///
/// In addition, this allows us to keep the pull up resistors on the
/// push buttons by masking `PORTD`, and disabling them on `PORTC`.
void setPorts()
{
  PORTB = portB;
  PORTC = portC & PORTC_NO_PULL_UP;
  PORTD = portD | PORTD_PULL_UP;
}

/// Reads either pushbutton 0 or pushbutton 1
///
/// Returns 0 on pushbutton up and not zero on pushbutton
/// down.
char readPushButton(int id)
{
  return (PIND & (0x08 >> id) ) ^ (0x08 >> id);
}

/// Reads either the indoor or the outdoor thermistor voltages
///
/// Performs an ADC and blocks until the result comes in.  Returns
/// a value from 0 to 1023, where 0 is 0V and 1023 is VCC.
int readADC(int channel)
{
  // Drive the thermistors
  portC |= THERMISTOR_DRIVE_PIN;
  setPorts();
  // Delay a short while to allow capacitors to charge etc.
  /// \todo Check this delay against actual thermistors
  delay(20);
      
  // Enable the ADC
  PRR &= ~_BV(PRADC);
  ADCSRA = _BV(ADEN);
  // Input channel select
  ADMUX = _BV(REFS0) | channel;

  // Start conversion
  ADCSRA |= _BV(ADSC);
      
  // Block until the conversion is complete
  while (ADCSRA & _BV(ADSC));
      
  // Result all 10 bits, Low then High as per the datasheet
  unsigned char resultL = ADCL;
  unsigned char resultH = ADCH;
      
  // Disable the ADC
  ADCSRA &= ~_BV(ADEN);
  PRR |= _BV(PRADC);
  
  // Stop driving thermistors
  portC &= ~THERMISTOR_DRIVE_PIN;
  setPorts();
  
  return ((resultH&0x03) <<8) + resultL;
}

/// Handles the pushbutton press
///
/// Performs all the state changes etc regarding the indoor push
/// button press.  Instructs the main loop to immediately undergo
/// a state change, after setting the state variable correctly.
void indoorPushbuttonPress()
{
  // Set the new state
  switch (state)
  {
    // If displaying the indoor temperature, initiate the min/max
    // cycle
    case STATE_INDOOR_DISPLAY:
    {
      state = STATE_INDOOR_MIN_WORD;
      break;
    }
    // Fast track the min to max cycle
    case STATE_INDOOR_MIN_WORD:
    case STATE_INDOOR_MIN_DISPLAY:
    {
      state = STATE_INDOOR_MAX_WORD;
      break;
    }
    // If we are showing max word do nothing, to fast track
    case STATE_INDOOR_MAX_WORD:
    break;
    // If we are in calibration mode, do the indoor
    case STATE_CALIBRATE_WAIT:
    {
      state = STATE_CALIBRATE_INDOOR_PRE;
      break;
    }
    // In other cases, just start to display the temperature
    default:
    state = STATE_INDOOR_DISPLAY_PRE;
  }
  
  // Force an immediate change
  stateChangeTics = 0;
  changeState = 1;
}

/// Handles the pushbutton press
///
/// Performs all the state changes etc regarding the outdoor push
/// button press.  Instructs the main loop to immediately undergo
/// a state change, after setting the state variable correctly.
void outdoorPushbuttonPress()
{
  // Set the new state
  switch (state)
  {
    // If displaying the outdoor temperature, initiate the min/max
    // cycle
    case STATE_OUTDOOR_DISPLAY:
    {
      state = STATE_OUTDOOR_MIN_WORD;
      break;
    }
    // Fast track the min to max cycle
    case STATE_OUTDOOR_MIN_WORD:
    case STATE_OUTDOOR_MIN_DISPLAY:
    {
      state = STATE_OUTDOOR_MAX_WORD;
      break;
    }
    // If we are showing max word do nothing, to fast track
    case STATE_OUTDOOR_MAX_WORD:
    break;
    // If we are in calibration mode, do the outdoor
    case STATE_CALIBRATE_WAIT:
    {
      state = STATE_CALIBRATE_OUTDOOR_PRE;
      break;
    }
    // In other cases, just start to display the temperature
    default:
    state = STATE_OUTDOOR_DISPLAY_PRE;
  }
  
  // Force an immediate change
  stateChangeTics = 0;
  changeState = 1;
}