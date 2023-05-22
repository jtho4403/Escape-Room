#include "serial.h" // Header file for serial function definitions.

#include "stm32f303xc.h" // STM32F definitions file.




// Mapping overhead for SeralPort Struct.
// These are stored as pointers because they are const values so we can't store them directly in the struct
struct _SerialPort {
	volatile uint32_t *BaudRate;
	volatile uint32_t *ControlRegister1;
	volatile uint32_t *StatusRegister;
	volatile uint32_t *InterruptClearRegister;
	volatile uint16_t *ReceiveDataregister;
	volatile uint16_t *DataOutputRegister;
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


// GPIO port enumeration for different serial ports.
// Only serial port 1 is used in this program.
enum {
	SERIAL_GPIO_A,
	SERIAL_GPIO_B,
	SERIAL_GPIO_C,
	SERIAL_GPIO_D,
	SERIAL_GPIO_E
};



// Instantiate the serial port base parameters. The complexity is hidden in the c file.
SerialPort USART1_PORT = {
	&(USART1->BRR), // Baud Rate Register
	&(USART1->CR1), // Control Register 1
	&(USART1->ISR), // Input status register
	&(USART1->ICR), // Interrupt clear register
	&(USART1->RDR), // Receive data register
	&(USART1->TDR), // Transmit data register.
	&(RCC->APB2ENR), // Enable register for advanced peripheral bus 2.
	RCC_APB2ENR_USART1EN,
	SERIAL_GPIO_C,
	&(GPIOC->MODER), // GPIO C mode register.
	0xA00,
	&(GPIOC->OSPEEDR),
	0xF00,
	((uint8_t*)&(GPIOC->AFR[0])) + 2,
	0x77
};

void (*callback_function2)() = 0x00;

// Initialise the serial port using baudRate from an enumerated set
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t)) {

	serial_port->completion_function = completion_function; // Assign function pointer to run when interrupt is called.


	// Enable clock power, system configuration clock, and GPIOC.
	// Common to all UARTs.
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Select & assign correct GPIO port.
	switch(serial_port->SerialPortGPIO) {
		case SERIAL_GPIO_C:
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
			break;
		default:
			break;
	}

	// Set pin mode for GPIO port.
	*(serial_port->SerialPinModeRegister) = serial_port->SerialPinModeValue;

	// Enable high speed clock for GPIOC.
	*(serial_port->SerialPinSpeedRegister) = serial_port->SerialPinSpeedValue;

	// Set alternate function to enable USART to an external pin.
	*(serial_port->SerialPinAlternatePinRegister) = serial_port->SerialPinAlternatePinValue;

	// Enable output regulation timer to control baud rate.
	*(serial_port->TimerEnableRegister) |= serial_port->TimerEnableMask;

	// Set bead Rate. Only 16 bits used.
	uint16_t *baud_rate_config = ((uint16_t*)serial_port->BaudRate);


	// Baud rate calculation:
	switch(baudRate) {
		case BAUD_9600:
			*baud_rate_config = 840;
			break;
		case BAUD_19200:
			*baud_rate_config = 420;
			break;
		case BAUD_38400:
			*baud_rate_config = 210;
			break;
		case BAUD_57600:
			*baud_rate_config = 140;  // 57600 at 8MHz
			break;
		case BAUD_115200:
			*baud_rate_config = 0x46*0x06;  // 115200 at 8MHz
			break;
	}


	// Enable serial port for tx and rx functionality.
	*(serial_port->ControlRegister1) |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

	callback_function2 = completion_function;

	 EnableInterrupt(); // Enable interrupts for serial port.
}


// Outputs a single character from the serial port
void SerialOutputChar(uint8_t data, SerialPort *serial_port) {

	while((*(serial_port->StatusRegister) & USART_ISR_TXE) == 0){
		; // Wait until the serial port is ready to transmit.
	}

	*(serial_port->DataOutputRegister) = data; // Write output data to the serial port.
}


// Output an entire string via the serial port.
// Pretty much just iterates over SerialOutputChar().
void SerialOutputString(uint8_t *pt, SerialPort *serial_port) {

	uint32_t counter = 0; // Holds number of characters transmitted.

	// Iterate over elements in string until null terminator is reached.
	while(*(pt+counter) != '\n') {

		// Output only printable characters. Ignore special characters.
		if((int)(*(pt+counter) > 32)) {
			SerialOutputChar(*(pt+counter), serial_port); // Output a single character.
		}

		counter++;
	}

	SerialOutputChar('\n', serial_port); // Output a terminating newline.

	//serial_port->completion_function(counter);
}


// Reads a single character from the serial port.
uint8_t SerialInputChar(SerialPort *serial_port) {

	while((*(serial_port->StatusRegister) & USART_ISR_RXNE) == 0){
		; // Wait until character is ready to read.
	}

	// Condense the base address of the USART port into a single variable.
	uint32_t USART_BASEADDR = (uint32_t)(serial_port->ControlRegister1); // 0x40013800 (Page 52)

	// Acquire the address of the read data register (RDR) for the USART port.
	uint32_t* USART1_RDR = (uint32_t*)(USART_BASEADDR + 0x24); // Offset of 0x24 from base address.

	// Reset the recieve frame request register, indicating that the data will be read and is being dealt with.
	*(serial_port->ControlRegister1) |= USART_RQR_RXFRQ;

	// Read the data from the lowest byte of the RDR.
	uint8_t data = (uint8_t)(*USART1_RDR & 0xFF); // 0xFF = 11111111

	*((uint32_t*)(USART_BASEADDR + 0x20)) |= 1 << 3; // Clear overrun error flag.

	return data; // Return a single character of received data.
}


// Read a string of characters from the serial port.
uint32_t SerialInputString(uint8_t *buffer, SerialPort *serial_port) {


	// Read the first character of the string.
	uint8_t rxd_char = SerialInputChar(&USART1_PORT);
	buffer[0] = rxd_char; // Store first character in position 0 of the passed buffer array.

	uint32_t index = 1; // Iteration will start from 2nd character.

	// Iterate over recieved characters until terminating newline or null terminator is reached.
	while (rxd_char != '\0' && rxd_char != '\n' && index < 256) {

		rxd_char = SerialInputChar(&USART1_PORT); // Read individual character from USART port input.

		buffer[index] = rxd_char; // Store received character in buffer.

		index += 1; // Increment index to move to next character in transmission.
	}

	buffer[index] = '\0'; // Add null terminator to string.

	return index; // Return total length of received string.
}

// take in received char
void recieve_data(uint8_t *rx_buffer, SerialPort *serial_port ){

	// Reset the recieve frame request register, indicating that the data will be read and is being dealt with.
	//*(serial_port->ControlRegister1) |= USART_RQR_RXFRQ;
	//volatile uint16_t num = *serial_port->ReceiveDataregister;
	//*rx_buffer = (uint8_t)(num & 0xff);
	// Read the data from the lowest byte of the RDR.
	*rx_buffer = ((uint8_t)((*serial_port->ReceiveDataregister) & 0xFF));
	//*rx_buffer = num;

	rx_buffer++;
			//(uint8_t)(*USART1_RDR & 0xFF); // 0xFF = 11111111

	//*(serial_port->ControlRegister1)

	//*(serial_port->InterruptClearRegister) |= 1 << 3; // Clear overrun error flag.
	//uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	// USART1->ISR |= USART_ISR_RXNE
	//*(serial_port->StatusRegister) |= USART_ISR_RXNE;
	*(serial_port->InterruptClearRegister) |= 1 << 3;
	*(serial_port->ReceiveDataregister);
}


uint8_t* inbuffer[256]; // Global in it. of buffer for interrupt triggered rx.

void (*callback_function)(uint8_t* data, SerialPort *serial_port) = 0x00;

volatile static uint8_t *buffer;
static uint8_t *start;
static SerialPort *Saved_UART;

void setup_transmission(volatile uint8_t* string, SerialPort *serial_port){
	Saved_UART = serial_port;
	buffer = string;
	*(Saved_UART->ControlRegister1) |= USART_CR1_TXEIE;
	*(Saved_UART->DataOutputRegister) = *buffer;
	buffer++;
	return;
}

void enable_USART_interrupts(SerialPort *serial_port){
	*(serial_port->DataOutputRegister) = *buffer;
	buffer++;
	return;
}

void USART1_IRQHandler(){
	*(Saved_UART->StatusRegister) ^= USART_ISR_TC;
	if(*buffer){
		*(Saved_UART->DataOutputRegister) = *buffer;
		buffer++;
		return;
	}
	*(Saved_UART->ControlRegister1) ^= USART_CR1_TXEIE;
	buffer = 0x00;
}



	//USART1->ISR |= USART_ISR_RXNE; // Clear RXNE interrupt flag by writing to ICR register

//	uint32_t len = SerialInputString(inbuffer, &USART1_PORT);
//
//	// Handle received strings, ignoring noise and errenous readings.
//	// All strings should be at least of length 1 as they will have a terminating newline if read was successful.
//	if (len > 1) {
//		// Call function from main.c to handle received data.
//		(&USART1_PORT)->completion_function((uint32_t)inbuffer);
//	}



// Enable interrupts for USART1 rx functionality.
void EnableInterrupt() {

	__disable_irq(); // Disable the interrupts while editing settings.

	// Enable the system configuration controller (SYSCFG in RCC).
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//////USART1->CR1 |= USART_CR1_RXNEIE; // Enable RXNE (Receive Data Register Not Empty) interrupt.

    // Configure EXTI line for USART1.
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_Msk; // Clear EXTI5, remove previous settings.
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PA; // Set EXTI5 bits to PA for  (USART1)

    // Enable EXTI5 interrupt (corresponding to USART1).
    EXTI->IMR |= EXTI_IMR_MR5; // Enable interrupt on EXTI line 5 (corresponding to PA5/USART1).

	// Tell the NVIC module that EXTI0 interrupts should be handled.
	// NVIC_SetPriority(USART1_IRQn, 3); // Set priority to 5, fairly low.
	NVIC_EnableIRQ(USART1_IRQn);

	__enable_irq(); // Re-enable all interrupts now that we are finished editing settings.
}
