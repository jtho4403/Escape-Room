#include "sequence.h"

char seq1[4] = "WASD";
char seq2[5] = "WWASD";
char seq3[6] = "WASDDD";
char seq4[7] = "WAASSDWS";

void CheckSequence(uint8_t *input, uint32_t count, uint32_t *flag){
//	uint8_t *seq[8];
//	Sequence(seq, count);

//	DeclareSequence();
//
//	switch(count){
//	case 1: {
//		if (input == seq1){
//			*flag = 1;
//		}
//
//	}
//	case 2: {
//		if (input == seq1){
//			*flag = 1;
//		}
//	}
//	default: {
//		*flag = 0;
//	}
//	}
	char k = (char) input[0];
	if (k == x[0]){
		flag = 1;
	}
}
