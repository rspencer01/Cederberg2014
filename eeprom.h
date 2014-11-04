/*
 * eeprom.h
 *
 * Created: 2014/11/04 11:37:30 AM
 *  Author: Robert Spencer
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

// Writes data to the EEPROM
void writeEEPROM(unsigned int, unsigned char);
// Reads data from the EEPROM
unsigned char readEEPROM(unsigned int);

#endif /* EEPROM_H_ */