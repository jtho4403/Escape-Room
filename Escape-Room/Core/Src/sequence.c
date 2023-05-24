#include "sequence.h"

//sequence as static variable
static char seq1[4] = "WASD";
static char seq2[5] = "WASDW";
static char seq3[6] = "WASDWA";
static char seq4[7] = "WASDWAS";
volatile int substage_state; //0 for fail, 1 for pass
volatile int count;
volatile int next_stage;
volatile int nest = 0;
volatile int timer_expired = 0; // Timer expiration flag
volatile int first_timer = 0;
volatile int timer_initialised = 0;

void CheckSequence(uint8_t *input){
	char seq[7];
	int i;
	if (count == 1){
		strcpy(seq, seq1);
	}
	else if (count == 2){
		strcpy(seq, seq2);
	}
	else if (count == 3){
		strcpy(seq, seq3);
	}
	else if (count == 4){
		strcpy(seq, seq4);
	}

	uint8_t fail_message[32] = "Fail\n";
	uint8_t pass_message[32] = "Success\n";

	char current_char;
	for (i = 0; i < count; i++){
		current_char = (char)input[i];
		if (current_char != seq[i]){
			SerialOutputString(fail_message, &USART1_PORT);
			substage_state = 0;
			break;
		}
	}

	if (substage_state == 1){
		SerialOutputString(pass_message, &USART1_PORT);
		count ++;
		next_stage = 1;
	}
}

void LED_Stage(){
	substage_state = 1;
	count = 1;

	while (count < 5){
		next_stage = 0;

		//put LED function here
		Display_LED();

		//enable serial receive interrupt
		SerialInitialise(BAUD_115200, &USART1_PORT);
		EnableSerialInterrupt(&USART1_PORT);

		//enable timer polling here
		init_timer();

		//add timer polling condition when integrating
		while(next_stage == 0 && substage_state == 1 && timer_expired == 0){
		}

		if(substage_state == 0 ){
			uint8_t restart_message[32] = "Restarting Stage 2\n";
			SerialOutputString(restart_message, &USART1_PORT);

			nest ++;
			LED_Stage();
			break;
		}

		else if (timer_expired != 0){
			timer_expired = 0;
			uint8_t message[32] = "Time's up, restarting stage 2\n";
			SerialOutputString(message, &USART1_PORT);

			nest ++;
			LED_Stage();
			break;
		}
	}

	if (nest == 0){
		uint8_t progress_message[32] = "You've passed Stage 2\n";
		SerialOutputString(progress_message, &USART1_PORT);
		//call next stage
	}
	else{
		nest --;
	}
}

void Current_LED(uint8_t current){
	// turn on NWSE LED based on WASD
	// Function to control LEDs based on the input direction (current)
	// This implementation assumes there are four LEDs (N, W, S, E)
	// Replace the following code with the appropriate hardware-specific calls or API functions

	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	if (current == 'W') {
		// Turn on North LED
		*led_register = 0b00000010; // Set the bit for the desired LED
	}
	else if (current == 'A') {
		// Turn on West LED
		// Example: SetWestLEDState(LED_ON);
		*led_register = 0b10000000; // Set the bit for the desired LED
	}
	else if (current == 'S') {
		// Turn on South LED
		*led_register = 0b00100000; // Set the bit for the desired LED
	}
	else if (current == 'D') {
		// Turn on East LED
		*led_register = 0b00001000; // Set the bit for the desired LED
	}

}

void Display_LED(){
	char seq[7];
	int i;
	if (count == 1){
		strcpy(seq, seq1);
	}
	else if (count == 2){
		strcpy(seq, seq2);
	}
	else if (count == 3){
		strcpy(seq, seq3);
	}
	else if (count == 4){
		strcpy(seq, seq4);
	}

	uint8_t current_char;
	for (i = 0; i < count+3; i++){
		current_char =  seq[i];
		Current_LED(current_char);

		//delay
		for (int i = 0; i < 5000000; ++i) {
			// Do nothing
		}
	}

	//turn off LED
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0b00000000;

    // Reset the timer counter
    TIM3->CNT = 0;
}

void init_timer(){
    if (timer_initialised == 0) { // Check if the timer is not already initialized
        __disable_irq();
        TIM3->PSC = 8000;
        TIM3->ARR = 65000;
        TIM3->DIER |= TIM_DIER_UIE;
        TIM3->CR1 |= TIM_CR1_CEN;
        NVIC_EnableIRQ(TIM3_IRQn);
        timer_initialised = 1; // Mark the timer as initialized
        __enable_irq();
    }
    else{
    	TIM3->DIER |= TIM_DIER_UIE;
    	TIM3->CR1 |= TIM_CR1_CEN;
    }

}

void TIM3_IRQHandler() {
	if (first_timer == 0) {
		first_timer = 1;
		TIM3->SR &= ~TIM_SR_UIF; // Put down the overflow flag
	}
	else if ((TIM3->SR & TIM_SR_UIF) != 0) {
		TIM3->SR &= ~TIM_SR_UIF; // Put down the overflow flag
		TIM3->CR1 &= ~TIM_CR1_CEN; // Disable the timer
		TIM3->CNT = 0; // Reset the timer counter
		timer_expired = 1;
	}
}

