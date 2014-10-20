/// \file gpio.c
/// Handles all the general purpose IO for the device.
///
/// Sets up the registers to handle input/output correctly
/// as well as the writing to the digital ports.

#include <avr/io.h>
#include "gpio.h"

/// Internal buffer for port B
unsigned char portB;
/// Internal buffer for port C
unsigned char portC;
/// Internal buffer for port D
unsigned char portD;

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
/// |`2`        | Thermometer Drive     | `O`  |
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
/// |`1`        | `SSEG SELECT 0`    |      |
/// |`2`        | Outdoor Pushbutton |      |
/// |`3`        | Indoor Pushbutton  |      |
/// |`4`        | `SSEG SELECT 1`    |      |
/// |`5`        | `SSEG 3`           |      |
/// |`6`        | `SSEG SELECT 0`    | `O`  |
/// |`7`        | `SSEG 6`           | `O`  |
void initPorts()
{
  DDRB = 0b11111111;
  
  DDRC = 0b1110000;
  
  DDRD = 0b11110011;
}

/// Sets all the ports to be the values stored in the buffers.
///
/// We use buffers for the ports as operations such as `PORTD^=0x10` are
/// not well defined (it implies a read of an output pin, which may be
/// sensitive on the circuitry).
///
/// In addition, this allows us to keep the pull up resistors on the
/// push buttons by masking `PORTD`.
void setPorts()
{
  PORTB = portB;
  PORTC = portC;
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
