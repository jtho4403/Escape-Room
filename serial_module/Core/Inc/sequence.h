#ifndef SEQUENCE_HEADER
#define SEQUENCE_HEADER


#include <stdint.h>
#include "stm32f303xc.h"
#include "serial.h"

void CheckSequence(uint8_t *input, uint32_t count);

#endif
