/*
 * sseg.h
 *
 * Created: 2014/03/21 12:18:07 PM
 *  Author: Robert
 */ 
#ifndef SSEG_H_
#define SSEG_H_

// The seven segment displays are set out as follows:
//
//        0
//      5   1
//        6
//      4   2
//        3   7
//
// We only show one at a time.  The rest are strobed
// one after another fast enough to appear static to
// the human eye.

// The port on which the sseg select pins reside
#define SSEG_SELECT_PORT portD
// A mask for which pins are sseg select pins
#define SSEG_SELECT_MASK 0b01010010

// Numbers
#define SSEG_ZERO  0b11000000
#define SSEG_ONE   0b11111001
#define SSEG_TWO   0b10100100
#define SSEG_THREE 0b10110000
#define SSEG_FOUR  0b10011001
#define SSEG_FIVE  0b10010010
#define SSEG_SIX   0b10000010
#define SSEG_SEVEN 0b11111000
#define SSEG_EIGHT 0b10000000
#define SSEG_NINE  0b10010000

// Performs the strobing
void updateDisplay();

// Writes a number to the displays
void writeNumber(int);

#endif /* SSEG_H_ */