/// \file gpio.c
/// Handles all the general purpose IO for the device.
///
/// Sets up the registers to handle input/output correctly
/// as well as the writing to the digital ports.

#include "sseg.h"
#include "gpio.h"
#include "utils.h"

/// An array of the pins that activate each of the three displays
const unsigned char SSEG_SELECT[3] = {0b01000000,0b00010000,0b00000010};

/// An array consisting of the digits represented in seven segment displays
const unsigned char SSEG_NUMBERS[10] = {SSEG_ZERO,SSEG_ONE,SSEG_TWO,SSEG_THREE,SSEG_FOUR,SSEG_FIVE,SSEG_SIX,SSEG_SEVEN,SSEG_EIGHT,SSEG_NINE};

/// The actual values in each display
int displays[3] = {0,0,0};

/// The currently strobed display number
int currentDisplay = 0;

/// Writes the correct values to the correct ports. 
/// 
/// Due to the nature of the circuitry, different seven
/// segment pins are in different ports, on weirdly
/// placed pins.
///
/// This function performs all the shifting and setting
/// of bits required, but _does not write the port_.
/// setPorts must be called after this if the value is to
/// actually be displayed.
///
/// \warning Don't mess with this unless you know 
/// exactly what you are doing.
void writeDisplay(int segmentNumber)
{
  portB = setNthBit(portB,(displays[segmentNumber] & 0x02) << 5, 6);
  portB = setNthBit(portB,(displays[segmentNumber] & 0x04) >> 2, 0);
  portB = setNthBit(portB,(displays[segmentNumber] & 0x10) << 3, 7);

  portC = setNthBit(portC,(displays[segmentNumber] & 0x20)     , 5);
  portC = setNthBit(portC,(displays[segmentNumber] & 0x80) >> 3, 4);

  portD = setNthBit(portD,(displays[segmentNumber] & 0x01)     , 0);
  portD = setNthBit(portD,(displays[segmentNumber] & 0x08) << 2, 5);
  portD = setNthBit(portD,(displays[segmentNumber] & 0x40) << 1, 7);
}

/// Strobes the display.
///
/// Activates the next display and writes its value out.
void updateDisplay()
{
  SSEG_SELECT_PORT &= ~SSEG_SELECT[currentDisplay];
  currentDisplay++;
  currentDisplay%=3;
  SSEG_SELECT_PORT |= SSEG_SELECT[currentDisplay];
  writeDisplay(currentDisplay);
  setPorts();
}

/// Writes a three digit integer to the display.
///
/// Any number of more than 3 digits will be truncated
/// to 3.
void writeNumber(int n)
{
  displays[0] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[1] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[2] = SSEG_NUMBERS[n%10];
}

/// Writes a message to the display.
/// 
/// Writes either `CLR`, `HI` or `LO` to the display.
void writeMessage(int msg)
{
  if (msg==SSEG_MSG_CLR)
  {
    displays[2] = SSEG_C;
    displays[1] = SSEG_L;
    displays[0] = SSEG_R;
  }
  if (msg==SSEG_MSG_HI)
  {
    displays[2] = 0xFF;
    displays[1] = SSEG_H;
    displays[0] = SSEG_I;
  }
  if (msg==SSEG_MSG_LO)
  {
    displays[2] = 0xFF;
    displays[1] = SSEG_L;
    displays[0] = SSEG_O;
  }
}

/// Switches off the display
///
/// Simply turns off all the control lines without changing
/// the values of the display.
/// 
/// To turn the display back on, simply call `updateDisplay`
/// repeatedly.
void clearDisplay()
{
  SSEG_SELECT_PORT &= ~ SSEG_SELECT_MASK;  
  setPorts();
}