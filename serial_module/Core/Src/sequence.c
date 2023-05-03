#include "sequence.h"

//sequence as static variable
static char seq1[4] = "WASD";
static char seq2[5] = "WWASD";
static char seq3[6] = "WASDDD";
static char seq4[7] = "WAASSDWS";

void CheckSequence(uint8_t *input, int count, int *flag){
//	convert input to string
	char string[7];
	int i = 0;

	while (i < 7){
		string[i] = (char)input[i];
		i++;
	}

	int test = 1;

	if (count == 1){
		if (string == seq1){
			*flag = 1;
		}
	}

}
