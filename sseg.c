/*
 * sseg.c
 *
 * Created: 2014/03/21 12:19:36 PM
 *  Author: Robert
 */ 
#include "sseg.h"
#include "gpio.h"
#include "utils.h"

// The bits for the display select
const unsigned char SSEG_SELECT[3] = {0b01000000,0b00010000,0b00000010};

// The numbers
const unsigned char SSEG_NUMBERS[10] = {SSEG_ZERO,SSEG_ONE,SSEG_TWO,SSEG_THREE,SSEG_FOUR,SSEG_FIVE,SSEG_SIX,SSEG_SEVEN,SSEG_EIGHT,SSEG_NINE};

// The values in each display
int displays[3] = {0,0,0};

// The currently strobed display number
int currentDisplay = 0;

// Writes the correct values to the correct ports.  Don't mess with this
// unless you know exactly what you are doing.
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

// Strobes the display.
void updateDisplay()
{
  SSEG_SELECT_PORT &= ~SSEG_SELECT[currentDisplay];
  currentDisplay++;
  currentDisplay%=3;
  SSEG_SELECT_PORT |= SSEG_SELECT[currentDisplay];
  writeDisplay(currentDisplay);
  setPorts();
}

// Writes a three digit integer to the display.
void writeNumber(int n)
{
  displays[0] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[1] = SSEG_NUMBERS[n%10];
  n/=10;
  displays[2] = SSEG_NUMBERS[n%10];
}

void clearDisplay()
{
  SSEG_SELECT_PORT &= ~ SSEG_SELECT_MASK;  
  setPorts();
}