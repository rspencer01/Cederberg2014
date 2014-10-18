/*
 * sseg.h
 *
 * Created: 2014/03/21 12:18:07 PM
 *  Author: Robert
 */ 


#ifndef SSEG_H_
#define SSEG_H_

// The port on which the sseg select pins reside
#define SSEG_SELECT_PORT portD
// A mask for which pins are sseg select pins
#define SSEG_SELECT_MASK 0b01010010
extern const unsigned char SSEG_SELECT[3];

void updateDisplay();

#endif /* SSEG_H_ */