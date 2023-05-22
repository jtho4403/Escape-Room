#include <stdint.h>
#include "stm32f303xc.h"
#include <time.h>
#include <stdlib.h>

void init_timer(){
	__disable_irq();

    TIM2->PSC = 8; // 1 ms per tick
    TIM2->ARR = 5000000; // 30 seconds
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    //TIM2->CR1 |= TIM_CR1_CEN;

    // Re-enable all interrupts (now that we are finished)
    __enable_irq();
}

void start_timer() {
    TIM2->CNT = 0; // Reset the timer counter
    TIM2->CR1 |= TIM_CR1_CEN; // start timer
}


void TIM2_IRQHandler() {
    if ((TIM2->SR & TIM_SR_UIF) != 0) {
        TIM2->SR &= ~TIM_SR_UIF;
        //toggle_led();
    }
}

