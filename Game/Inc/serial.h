#ifndef SERIAL_PORT_HEADER
#define SERIAL_PORT_HEADER


#include <stdint.h> // Library of standard integer types.


// Define the serial port struct, the definition is hidden in the file as no one really needs to know this.
struct _SerialPort;
typedef struct _SerialPort SerialPort;


// Make any number of instances of the serial port.
extern SerialPort USART1_PORT; // They are extern because they are fixed, unique values.


// Enumeration for user baud rate selection.
enum {
  BAUD_9600,
  BAUD_19200,
  BAUD_38400,
  BAUD_57600,
  BAUD_115200
};

/* SerialInitialise: Initialize the USART port.
Inputs:
 	baudrate: Baud rate as defined in the enumerator above.
 	serial_port: Struct of type SerialPort.
 	completion_function: A function to handle data received by USART1 rx interrupt.
*/
void SerialInitialise(uint32_t baudRate, SerialPort *serial_port, void (*completion_function)(uint32_t) );
 

/* SerialOutputChar: Output a character to the serial port
//  note: this version waits until the port is ready (not using interrupts)
Input:
	data: Single char to be transferred.
	serial_port: Struct of type SerialPort.
*/
void SerialOutputChar(uint8_t data, SerialPort *serial_port);
 

// SerialOutputString - output a NULL TERMINATED string to the serial port
// Input: pointer to a NULL-TERMINATED string (if not null terminated, there will be problems)
void SerialOutputString(uint8_t *pt, SerialPort *serial_port);
 


// SerialInputString -
// Input:
uint8_t SerialInputChar(SerialPort *serial_port);


// SerialInputString -
// Input:
uint32_t SerialInputString(uint8_t *target, SerialPort *serial_port);

void recieve_data(uint8_t *rx_buffer, SerialPort *serial_port );

void EnableInterrupt();

void setup_transmission(volatile uint8_t* string, SerialPort *serial_port);

void enable_USART_interrupts(SerialPort *serial_port);

#endif
