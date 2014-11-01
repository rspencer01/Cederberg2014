/*
 * timers.h
 *
 * Created: 2014/03/23 05:44:44 PM
 *  Author: Robert Spencer
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

// Scalars to change 4ms to 1 second via 40ms
#define INI_4MS_40MS 10
#define INI_40MS_1S 25
// Scalar to change watchdog 8s to 64s
#define INI_8S_64S 8
// Scalar to wait in 4ms for debouncing
#define DEBOUNCE_TIMOUT_4MS 80
// Number of seconds to hold the button down for before reseting
#define HOLD_TIME 5

// Sets up all the timer stuff
void initTimers();

#endif /* TIMERS_H_ */