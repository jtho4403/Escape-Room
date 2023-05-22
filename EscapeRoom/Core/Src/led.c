#include <stdint.h>
#include "stm32f303xc.h"
#include <time.h>
#include <stdlib.h>

void delay_ms(uint32_t milliseconds) {
    // Function to introduce a delay in milliseconds
    // This implementation assumes a 1 ms delay for each iteration
    // Adjust it based on the system clock frequency and desired accuracy
    for (volatile uint32_t i = 0; i < (milliseconds * 8000); ++i) {
        // Do nothing
    }
}

void toggle_led() {
    static uint8_t current_led = 0; // Keep track of the current LED being displayed
    static uint8_t iteration = 0;  // Keep track of the number of iterations
    uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

    // Character-to-binary mappings for first LED sequence
    const uint8_t FIRST_LED_CHARACTERS[] = {
        0b00000010, // 'w'
		0b10000000, // 'a'
        0b00100000, // 's'
        0b00001000, // 'd'
    };

    // Character-to-binary mappings for the second LED sequence
    const uint8_t SECOND_LED_CHARACTERS[] = {
    	0b00000010, // 'w'
    	0b10000000, // 'a'
		0b00100000, // 's'
		0b00001000, // 'd'
		0b00000010, // 'w'
    };

    // Character-to-binary mappings for the third LED sequence
    const uint8_t THIRD_LED_CHARACTERS[] = {
        0b00000010, // 'w'
        0b10000000, // 'a'
    	0b00100000, // 's'
    	0b00001000, // 'd'
    	0b00000010, // 'w'
		0b10000000, // 'a'
    };

    // Check if all LEDs have been displayed once
    if (iteration == 1) {
    	display_second_led_sequence();
    }
    // Check if the second sequence has been displayed
    else if (iteration == 2) {
    	display_third_led_sequence();
    }

    // If none of the sequences have been displayed, display the first sequence
    else {
        // Turn off all LEDs
        *led_register &= ~(0xFF);

        // Turn on the current LED from the first sequence
        *led_register |= FIRST_LED_CHARACTERS[current_led];

        // Delay for 1 second (1000 milliseconds)
        delay_ms(100);

        // Move to the next LED in the sequence
        current_led = (current_led + 1) % 4;

        // Check if the last LED in the sequence has been displayed
        if (current_led == 0) {
            iteration++;  // Increment the iteration counter
        }
    }
}

void display_second_led_sequence() {
	static uint8_t current_led = 0; // Keep track of the current LED being displayed
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	// Character-to-binary mappings for the second LED sequence
	const uint8_t SECOND_LED_CHARACTERS[] = {
		0b00000010, // 'w'
		0b10000000, // 'a'
		0b00100000, // 's'
		0b00001000, // 'd'
		0b00000010, // 'w'
	};

	// Turn off all LEDs
	*led_register &= ~(0xFF);

	// Turn on the current LED from the second sequence
	*led_register |= SECOND_LED_CHARACTERS[current_led];

	// Delay for 1 second (1000 milliseconds)
	delay_ms(100);

	// Move to the next LED in the sequence
	current_led = (current_led + 1) % 4;
}

void display_third_led_sequence() {
	static uint8_t current_led = 0; // Keep track of the current LED being displayed
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	// Character-to-binary mappings for the third LED sequence
	const uint8_t THIRD_LED_CHARACTERS[] = {
		0b00000010, // 'w'
        0b10000000, // 'a'
    	0b00100000, // 's'
    	0b00001000, // 'd'
    	0b00000010, // 'w'
		0b10000000, // 'a'
	};

	// Turn off all LEDs
	*led_register &= ~(0xFF);

	// Turn on the current LED from the third sequence
	*led_register |= THIRD_LED_CHARACTERS[current_led];

	// Delay for 1 second (1000 milliseconds)
	delay_ms(100);

	// Move to the next LED in the sequence
	current_led = (current_led + 1) % 4;
}

