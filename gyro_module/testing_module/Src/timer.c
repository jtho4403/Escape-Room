/*
 * timer.c
 *
 *  Created on: 5 Apr. 2023
 *      Author: David
 */

#include "stm32f303xc.h"
#include "timer.h"


// Function pointer set up
// make the function pointer = NULL

// Function pointer set up
void (*function_pointer)() = 0x00;
void (*oneshot_function_pointer)() = 0x00;
void (*interrupt_pointer)() = 0x00;

void up_count_TIM3(uint32_t time, void (*desired_function)()){
	// Set function pointer to given function
	interrupt_pointer = desired_function;

	uint32_t prescaler = 0;

	// Enable timer 2 in PWM mode
	config_TIM3(time, prescaler, PWM_MODE);

}

void config_TIM3(uint32_t time, uint32_t prescaler, uint32_t mode){
	__disable_irq();
	// enable the TIM3 Peripherial through the RCC registers
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// set the prescaler so that 1 count is 1 milisecond
	// 8MHz = 0.000000125, 1 microsecond is 0.000001,
	// prescaler 0.000001/0.000000125 = 8
	TIM3->PSC = 4800-1;  // timer at 48MHz/1KHz sysclock/desiredfreq
	TIM3->ARR = time-1; // 1 second before reset
	// Make the TIM3 trigger an interrupt when there is an overflow
	TIM3->DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM3_IRQn, 4);
	// Tell the NVIC module that timer2 interrupts should be handled
	NVIC_EnableIRQ(TIM3_IRQn); //TIM_CR1_CMS

	// Finally, enable the timer 3
	TIM3->CR1 |= TIM_CR1_CEN;

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void TIM3_IRQHandler(void){
	interrupt_pointer();
	TIM3->SR &= ~TIM_SR_UIF;
}
