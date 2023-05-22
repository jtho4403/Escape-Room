/*
 * output.c
 *
 *  Created on: 5 Apr. 2023
 *      Author: David
 */

#include "output.h"
#include "stm32f303xc.h"

void LED_switcher(void)
{
	// Branch based on the flag
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	int static count = 0;
	// check if first instance of LED_reg call
	if(count == 0){
		// set the LED state to be a specific value
		*led_register = 0x0f;
		count++;
		return;
	}

	// Alternate the LED register
	if(*led_register == 0x0f){
		// Left shift by 4 to get 0xf0
		//*led_register = *led_register << 4;
		*led_register = 0xf0;
	}
	// Right shift by 4 to get 0x0f again
	else{
		//*led_register = *led_register >> 4;
		*led_register = 0x0f;
	}
}

void LED_ON(void){
	// Branch based on the flag
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0xff;

}
void LED_OFF(void){
	// Branch based on the flag
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = 0x00;

}
