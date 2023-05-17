#ifndef SERIAL_H
#define SERIAL_H

#include "serial.h"
#include "stm32f303xc.h"

#define BUFFER_SIZE 64                   // Define buffer size
#define CARRIAGE_RETURN '\r'			 // new line for Mac OS
#define LINE_FEED '\n'					 // new line for UNIX

volatile uint8_t rx_buffer[BUFFER_SIZE]; // Buffer for receiving data
volatile uint8_t rx_index = 0;           // Buffer index for receiving data

// NOTE: these are stored as pointers because they
//       are const values so we can't store them directly
//       in the struct
struct _SerialPort {
	volatile uint32_t *BaudRate;
	volatile uint32_t *ControlRegister1;
	volatile uint32_t *StatusRegister;
	volatile uint32_t *RequestRegister;
	volatile uint16_t *DataOutputRegister;
	volatile uint16_t *DataInputRegister;
	volatile uint32_t *TimerEnableRegister;
	volatile uint32_t TimerEnableMask;
	volatile uint32_t SerialPortGPIO;
	volatile uint32_t *SerialPinModeRegister;
	volatile uint32_t SerialPinModeValue;
	volatile uint32_t *SerialPinSpeedRegister;
	volatile uint32_t SerialPinSpeedValue;
	volatile uint8_t *SerialPinAlternatePinRegister;
	volatile uint8_t SerialPinAlternatePinValue;
	void (*completion_function)(uint32_t);
};


// The different serial ports require different GPIO ports
enum {
	SERIAL_GPIO_A,
	SERIAL_GPIO_B,
	SERIAL_GPIO_C,
	SERIAL_GPIO_D,
	SERIAL_GPIO_E
};



// instantiate the serial port parameters
//   note: the complexity is hidden in the c file
SerialPort USART1_PORT = {&(USART1->BRR),
		&(USART1->CR1),
		&(USART1->ISR),
		&(USART1->RQR),
		&(USART1->TDR),
		&(USART1->RDR),
		&(RCC->APB2ENR),
		RCC_APB2ENR_USART1EN,
		SERIAL_GPIO_C,
		&(GPIOC->MODER),
		0xA00,
		&(GPIOC->OSPEEDR),
		0xF00,
		((uint8_t*)&(GPIOC->AFR[0])) + 2,
		0x77};

//PD5 and PD6 for TX and RX
SerialPort USART2_PORT = {&(USART2->BRR),
		&(USART2->CR1),
		&(USART2->ISR),
		&(USART2->RQR),
		&(USART2->TDR),
		&(USART2->RDR),
		&(RCC->APB1ENR),
		RCC_APB1ENR_USART3EN,
		SERIAL_GPIO_D,
		&(GPIOD->MODER),
		0x2800,
		&(GPIOC->OSPEEDR),
		0x3C00,
		((uint8_t*)&(GPIOC->AFR[0])) + 1,
		0x77};

//PC 10 and PC11 for TX and RX
SerialPort USART3_PORT = {&(USART3->BRR),
		&(USART3->CR1),
		&(USART3->ISR),
		&(USART3->RQR),
		&(USART3->TDR),
		&(USART3->RDR),
		&(RCC->APB1ENR),
		RCC_APB1ENR_USART2EN,
		SERIAL_GPIO_C,
		&(GPIOC->MODER),
		0xA00000,
		&(GPIOC->OSPEEDR),
		0xF00000,
		((uint8_t*)&(GPIOC->AFR[1])) + 4,
		0x77};



// InitialiseSerial - Initialise the serial port
// Input: baudRate is from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port) {

	// enable clock power, system configuration clock and GPIOC
	// common to all UARTs
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	switch(serial_port->SerialPortGPIO) {
	case SERIAL_GPIO_C:
		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
		break;
	default:
		break;
	}

	// set pin mode
	*(serial_port->SerialPinModeRegister) = serial_port->SerialPinModeValue;

	// enable high speed clock for GPIOC
	*(serial_port->SerialPinSpeedRegister) = serial_port->SerialPinSpeedValue;

	// set alternate function to enable USART to an external pin
	*(serial_port->SerialPinAlternatePinRegister) = serial_port->SerialPinAlternatePinValue;

	*(serial_port->TimerEnableRegister) |= serial_port->TimerEnableMask;

	uint16_t *baud_rate_config = ((uint16_t*)serial_port->BaudRate); // only 16 bits used!

	// Baud rate calculation from datasheet
	switch(baudRate){
	case BAUD_115200:
		*baud_rate_config = 0x46 * 0x06;  // 115200 at 8MHz
		break;
	}

	// enable serial port for tx and rx
	*(serial_port->ControlRegister1) |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void EnableSerialInterrupt(SerialPort *serial_port) {
	__disable_irq(); // Disable the interrupts while editing settings.

	*(serial_port->ControlRegister1) |= USART_CR1_RXNEIE;

    // Enable the system configuration controller (SYSCFG in RCC).
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Configure EXTI line for USART1.
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_Msk; // Clear EXTI5, remove previous settings.
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; // Set EXTI5 bits to PA for  (USART1)

	// Enable EXTI5 interrupt (corresponding to USART1).
	EXTI->IMR |= EXTI_IMR_MR5; // Enable interrupt on EXTI line 5 (corresponding to PA5/USART1).

    NVIC_EnableIRQ(USART1_IRQn);

    __enable_irq(); // Re-enable all interrupts now that we are finished editing settings.
}

void SerialOutputChar(uint8_t data, SerialPort *serial_port) {
	while((*(serial_port->StatusRegister) & USART_ISR_TXE) == 0){
	}

	*(serial_port->DataOutputRegister) = data;
}

void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0;
	while(*pt) {
		SerialOutputChar(*pt, serial_port);
		counter++;
		pt++;
	}

	if (serial_port->completion_function != 0x00)
		serial_port->completion_function(counter);
}



void SerialInputSequence(SerialPort *serial_port) {
		if (rx_index < 32) {
			uint8_t rx_data = *(serial_port->DataInputRegister);

			// exit if new line is detected
			if (rx_data == CARRIAGE_RETURN) {
				// disable receive interrupt
				*(serial_port->ControlRegister1) &= ~USART_CR1_RXNEIE;

				CheckSequence(rx_buffer);

				// reset index & buffer
				rx_index = 0;
				memset(rx_buffer, 0, sizeof(rx_buffer));
			}
			else if (rx_data == LINE_FEED){
			}

			else{
				// store byte/ character in string buffer
				rx_buffer[rx_index]= rx_data;
				rx_index++;
			}
		}

		else{
			// disable receive interrupt
			*(serial_port->ControlRegister1) &= ~USART_CR1_RXNEIE;

			CheckSequence(rx_buffer);

			// reset index & buffer
			rx_index = 0;
			memset(rx_buffer, 0, sizeof(rx_buffer));
		}
}

void read_joystick_x(SerialPort *serial_port){
	rx_index = 0;
	memset(rx_buffer, 0, sizeof(rx_buffer));

	uint8_t rx_data ;

	//wait for input in serial
	while ((*(serial_port->StatusRegister) & USART_ISR_RXNE) == 0){
		rx_data = *(serial_port->DataInputRegister);

		//terminate when receive \n
		if (rx_data == LINE_FEED){
			break;
		}

		rx_buffer[rx_index]= rx_data;
		rx_index++;
	}
}

void read_joystick_y(SerialPort *serial_port){
	rx_index = 0;
	memset(rx_buffer, 0, sizeof(rx_buffer));

	uint8_t rx_data ;

	//wait for input in serial
	while ((*(serial_port->StatusRegister) & USART_ISR_RXNE) == 0){
		rx_data = *(serial_port->DataInputRegister);

		//terminate when receive \n
		if (rx_data == LINE_FEED){
			break;
		}

		rx_buffer[rx_index]= rx_data;
		rx_index++;
	}
}

void USART1_IRQHandler(void)
{
	// disable timer
	SerialInputSequence(&USART1_PORT);
}

#endif
