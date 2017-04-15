/*
 * Created: 10/03/2017 15:53:21
 * Author : Alisson Pereira, Rafael Goncalves
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

// Define the wanted sentence variables
#define GPS_SENTENCE "$GPGLL"
#define GPS_SENTENCE_LENGTH 7

// Functions and methods
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);

int main(void){
	// More one (header + data fields)
	unsigned char sentence[GPS_SENTENCE_LENGTH + 1][10];
	// Initializes initial configuration
	USART_init();

	while(1) {
		int i = 1;

		// Checks if the first character is the beginning of a message
		// and if it is the expected message
		do {
			sentence[0][0] = USART_receive();
		} while (sentence[0][0] != '$');
		do {
			sentence[0][i] = USART_receive();
			i++;
		} while (i <= 6);
		sentence[0][6] = '\0';

		// Receive the remaining chars of the message
		if (strcmp(sentence[0], GPS_SENTENCE) == 0) {

			int charLine = 1;
			int charIndex = 0;

			// Split the message string
			do {
				sentence [charLine][charIndex] = USART_receive();
				if(sentence[charLine][charIndex] == ','){
					sentence [charLine][charIndex] = '\0';
					charLine++;
					charIndex = 0;
					USART_send('\n');
					continue;
				}
				USART_send(sentence [charLine][charIndex]);
				charIndex++;
			} while (sentence[charLine][charIndex-1] != '\n');
		}
	}
	return 0;
}

// USART Initial Settings
void USART_init (void) {
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

// Receives character from USART
unsigned char USART_receive (void) {
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

// Send character to USART
void USART_send ( unsigned char data) {
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}
