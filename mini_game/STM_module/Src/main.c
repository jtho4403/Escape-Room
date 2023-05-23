#include "serial.h"
#include "stm32f303xc.h"

// enable the clocks for desired peripherals (GPIOA, C and E)
void enable_clocks() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}

volatile uint8_t buffer[64] = "RUN_PYTHON\r\n";

int main() {
	enable_clocks();
	SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);

	setup_transmission(buffer,  &USART1_PORT);

	while(1){

	}

}
