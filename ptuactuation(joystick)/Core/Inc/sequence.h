#ifndef SEQUENCE_HEADER
#define SEQUENCE_HEADER


#include <stdint.h>
#include <string.h>
#include "serial.h"
#include "stm32f303xc.h"

void CheckSequence(uint8_t *input);

void Stage2();

void Current_LED(uint8_t current);

void Display_LED();

void init_timer();

#endif
