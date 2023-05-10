#ifndef SEQUENCE_HEADER
#define SEQUENCE_HEADER


#include <stdint.h>
#include "serial.h"
#include "stm32f303xc.h"

void CheckSequence(uint8_t *input);

void Stage2();

//void CheckStage2(int state);

#endif
