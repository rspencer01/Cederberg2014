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

// Representation of the numbers in the above sseg display
#define SSEG_ZERO    0b11000000
#define SSEG_ONE     0b11111001
#define SSEG_TWO     0b10100100
#define SSEG_THREE   0b10110000
#define SSEG_FOUR    0b10011001
#define SSEG_FIVE    0b10010010
#define SSEG_SIX     0b10000010
#define SSEG_SEVEN   0b11111000
#define SSEG_EIGHT   0b10000000
#define SSEG_NINE    0b10010000

// Symbols
#define SSEG_DECIMAL 0b01111111
#define SSEG_MINUS   0b10111111

// Representation of some letters
#define SSEG_A 0b10001000
#define SSEG_C 0b11000110
#define SSEG_E 0b10000110
#define SSEG_H 0b10001001
#define SSEG_I 0b11001111
#define SSEG_L 0b11000111
#define SSEG_O 0b11000000
#define SSEG_Q 0b10011000
#define SSEG_R 0b10101111
#define SSEG_U 0b11000001

// Some messages that will be used
#define SSEG_MSG_CLR 0
#define SSEG_MSG_HI  1
#define SSEG_MSG_LO  2
#define SSEG_MSG_EQU 3
#define SSEG_MSG_CAL 4
#define SSEG_MSG_ICE 5

// Performs the strobing
void updateDisplay();

// Writes a number to the displays
void writeNumber(int);

// Writes a message to the displays
void writeMessage(int msg);

// Clears the display so it shows nothing
void writeClear();

// Clears the display so it shows nothing via the control lines
void clearDisplay();

#endif /* SSEG_H_ */