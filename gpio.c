/*
 * gpio.c
 *
 * Created: 2014/10/18 09:31:02 PM
 *  Author: Robert Spencer
 */ 

#include <avr/io.h>

unsigned char portB;
unsigned char portC;
unsigned char portD;

// We use buffers for the ports as operations such as PORTD^=0x10 are
// not well defined (it implies a read of an output pin).
void setPorts()
{
  PORTB = portB;
  PORTC = portC;
  PORTD = portD;
}
