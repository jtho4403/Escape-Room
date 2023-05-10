#include "sequence.h"

//sequence as static variable
static char seq1[4] = "WASD";
static char seq2[5] = "WASDW";
static char seq3[6] = "WASDWA";
static char seq4[7] = "WASDWAS";
volatile int substage_state; //0 for fail, 1 for pass
volatile int count;

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
	for (i = 0; i < count+3; i++){
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
	}
}

//void CheckStage2(int state){
//	if (state == 1){
//		uint8_t progress_message[32] = "You've passed Stage 2\r\n";
//		SerialInitialise(BAUD_115200, &USART1_PORT);
//		SerialOutputString(progress_message, &USART1_PORT);
//		//call next stage
//	}
//	else if (state == 0){
//		uint8_t restart_message[32] = "Restarting Stage 2\r\n";
//		SerialInitialise(BAUD_115200, &USART1_PORT);
//		SerialOutputString(restart_message, &USART1_PORT);
//
//		Stage2();
//	}
//	else{
//		uint8_t error_message[32] = "Error\n";
//		SerialOutputString(error_message, &USART1_PORT);
//	}
//
//}

void Stage2(){
	count = 1;
	substage_state = 1;
	int cnt;
	int message_sent = 0;

	while (count < 5){
		cnt = count;

		//put LED function here

		//enable timer polling here

		//enable serial receive interrupt
		SerialInitialise(BAUD_115200, &USART1_PORT);
		EnableSerialInterrupt();

		//add timer polling condition when integrating
		while(count == cnt && substage_state == 1){
		}

		if(substage_state == 0){
			uint8_t restart_message[32] = "Restarting Stage 2\r\n";
			SerialOutputString(restart_message, &USART1_PORT);

			Stage2();
			break;
		}
	}

	uint8_t progress_message[32] = "You've passed Stage 2\r\n";
	SerialOutputString(progress_message, &USART1_PORT);
	message_sent = 1;
	//call next stage
}
