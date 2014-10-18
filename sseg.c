/*
 * sseg.c
 *
 * Created: 2014/03/21 12:19:36 PM
 *  Author: Robert
 */ 
#include "sseg.h"
#include "gpio.h"

const unsigned char SSEG_SELECT[3] = {0b01000000,0b00010000,0b00000010};
int currentDisplay = 0;

// Strobes the display.
void updateDisplay()
{
  SSEG_SELECT_PORT &= ~SSEG_SELECT[currentDisplay];
  currentDisplay++;
  currentDisplay%=3;
  SSEG_SELECT_PORT |= SSEG_SELECT[currentDisplay];
  setPorts();
}