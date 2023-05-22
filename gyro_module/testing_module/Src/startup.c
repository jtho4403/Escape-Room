/*
 * startup.c
 *
 *  Created on: 29 Mar. 2023
 *      Author: David
 */
#include "startup.h"
#include "stm32f303xc.h"

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {

	// enable the clocks for peripherals (GPIOA, C and E)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;

	// store a 1 in bit for the TIM2 enable flag
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

// initialise the discovery board I/O (just outputs: inputs are selected by default)
void initialise_board() {
	// get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_output_registers = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_output_registers = 0x5555;
}


