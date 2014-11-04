/*
 * eeprom.c
 *
 * Created: 2014/11/04 11:36:57 AM
 *  Author: Robert Spencer
 */ 

#include <avr/io.h>

/// Writes a value to the EEPROM
/// 
/// During the operation all interrupts will be disabled.
void writeEEPROM(unsigned int addr, unsigned char data)
{
  while (EECR & _BV(EEPE));
  // Disable interrupts
  SREG &= ~_BV(SREG_I);
  EEAR = addr;
  EEDR = data;
  EECR = _BV(EEMPE);
  EECR |= _BV(EEPE);
  // Enable interrupts
  SREG |= _BV(SREG_I);
}

/// Reads a value from the EEPROM
/// 
/// During the operation, all interrupts will be disabled.
unsigned char readEEPROM(unsigned int addr)
{
  // Disable interrupts
  SREG &= ~_BV(SREG_I);
  EEAR = addr;
  EECR += _BV(EERE);
  // Enable interrupts
  SREG |= _BV(SREG_I);
  return EEDR;
}