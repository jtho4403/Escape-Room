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
		*baud_rate_config = 0x46;  // 115200 at 8MHz
		break;
	}


	// enable serial port for tx and rx
	*(serial_port->ControlRegister1) |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

	__disable_irq();
	NVIC_EnableIRQ(USART1_IRQn);
	__enable_irq();
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

void SerialInputString(SerialPort *serial_port) {
	while (rx_index < 7) {
		// wait for data to be received
		while((*(serial_port->StatusRegister) & USART_ISR_RXNE) == 0){
		}

		uint8_t rx_data = *(serial_port->DataInputRegister);

		// exit if new line (\n) is detected
		if (rx_data == CARRIAGE_RETURN || rx_data == LINE_FEED) {
			break;
		}

		// store byte/ character in string buffer
		rx_buffer[rx_index]= rx_data;
		rx_index++;
	}

	CheckSequence(rx_buffer);
}

void USART1_EXTI25_IRQHandler(void)
{
	SerialInputString(&USART1_PORT);
}

#endif
