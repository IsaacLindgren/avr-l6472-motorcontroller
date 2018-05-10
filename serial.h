/******************************************************************
 * Contains functions for accessing UART and SPI ports
 ******************************************************************/
#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>
#include <stdint.h>

void initUSART(void);
void txUSART( uint8_t byte );
uint8_t rxUSART(void);
void printHex( uint8_t byte );
void print32Hex( uint32_t integer );
void printString( const char *string );

void initSPI(void);
uint8_t txrxSPI( uint8_t *buffer, uint8_t size );

#endif
