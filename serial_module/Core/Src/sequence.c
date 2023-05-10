#include "sequence.h"

//sequence as static variable
static char seq1[4] = "WASD";
static char seq2[5] = "WWASD";
static char seq3[6] = "WASDDD";
static char seq4[7] = "WAASSDW";
volatile uint8_t pass_state;
volatile uint8_t count;

void CheckSequence(uint8_t *input){
	pass_state = 0;

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
	int fail = 0;
	for (i = 0; i < count+3; i++){
		current_char = (char)input[i];
		if (current_char != seq[i]){
			SerialInitialise(BAUD_115200, &USART1_PORT);
			SerialOutputString(fail_message, &USART1_PORT);
			fail = 1;

			break;
		}
	}

	if (fail == 0){
		SerialInitialise(BAUD_115200, &USART1_PORT);
		SerialOutputString(pass_message, &USART1_PORT);
	}

}
