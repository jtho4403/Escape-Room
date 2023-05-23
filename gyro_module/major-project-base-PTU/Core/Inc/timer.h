/*
 * timer.h
 *
 *  Created on: 5 Apr. 2023
 *      Author: David
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

// Mode for enable timer 2 function
#define DEFAULT_MODE 0
#define PWM_MODE 1
// Timer module with setup timer and 2 functions as interrupts
// Function pointer set up
//typedef void (*function_pointer)();


void up_count_TIM3(uint32_t time, void (*desired_function)());

// Runs a time delay for time milliseconds before running the call back function once
//void delay_function(uint32_t time, void (*function_pointer)());

// Set up timer 2 by taking in the end time prescaler value and mode
// Mode are # defined above
void config_TIM3(uint32_t max_time, uint32_t prescaler, uint32_t mode);

// Function that resets the interrupt status registers
void reset_PWM_interrupt_status(void);

// runs a given call back function once
void turn_off_interrupt(void);


#endif /* SRC_TIMER_H_ */
