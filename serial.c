/******************************************************************
 * Contains functions for accessing UART and SPI ports
 ******************************************************************/
#include "serial.h"
#include <avr/io.h>
#include <stdint.h>

const char hex2ascii[] = {  '0','1','2','3','4','5','6','7','8','9',
                            'A','B','C','D','E','F' };

void initUSART(void) {
	// Set baud rate
	uint16_t ubrr = 103;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	// 2x speed
	UCSR0A = (1 << U2X0);

	// Enable rx and tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Frame: 8bit 2stop
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void txUSART( uint8_t data ) {
	while( !( UCSR0A & (1 << UDRE0) ) );

	UDR0 = data;
}

uint8_t rxUSART(void) {
        uint8_t byte;
	while( !(UCSR0A & (1 << RXC0) ) );
        byte = UDR0;
        txUSART( byte );
	return byte;
}

void printHex( uint8_t byte ) {
    txUSART( hex2ascii[byte >> 4] );
    txUSART( hex2ascii[byte & 0x0f] );
}

void print32Hex( uint32_t integer ) {
    printString( "0x" );
    printHex( (integer >> 24) & 0xff );
    printHex( (integer >> 16) & 0xff );
    printHex( (integer >> 8) & 0xff );
    printHex( (integer >> 0) & 0xff );
}

void printString( const char *string ) {
    while( *string != '\0' ) {
        txUSART( *string );
        string++;
    }
}

void initSPI(void) {
    // Config
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL);
    // Set slave select high
    PORTB |= 1 << PB2;
}

uint8_t txrxSPI( uint8_t *buffer, uint8_t size ) {
    int i;

    // Send and read bytes
    for( i = 0; i < size; i++ ) {
        PORTB &= !(1 << PB2);
        SPDR = buffer[i];
        while( !(SPSR & (1 << SPIF) ) );
        buffer[i] = SPDR;
        PORTB |= 1 << PB2;
    }

    return size;
}

