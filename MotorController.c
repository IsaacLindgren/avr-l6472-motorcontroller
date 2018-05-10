/*
 * MotorController.c
 *
 * Created: 11/30/2015 2:19:14 PM
 *  Author: PKI340
 */
#include <avr/io.h>
#include <stdint.h>
#include "serial.h"
#include "L6472.h"
#include "interface.h"

void startup(void);

void wasteTime(void);

#define FOSC	8000000
#define BAUD	9600

int main(void)
{
    startup();
    initUSART();
    initSPI();
    initInterface();

    rxUSART();
    printString( "\b \b" );
    initL6472();

    while(1)
    {
        printString( "\x1b[0;0H" );             // move home
        printString( "\x1b[K" );                // erase line
        printString( "Enter Command: " );
        handleCommand( getCommand() );
    }
}

void startup(void) {
    // LED pins
    DDRC |= (1 << PC3) | (1 << PC2);
    // Green On, Red Off
    PORTC &= ~(1 << PC2);
    PORTC |= (1 << PC3);

    // USART pins
    DDRD |= (1 << PD1); // TX out
    DDRD &= ~(1 << PD0); // RX in

    // SPI pins
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

    // Set STBY_RESET_ high
    DDRD |= 1 << PD6;
    PORTD |= 1 << PD6;

    // Flag and busy as inputs
    DDRB &= ~(1 << PB0);
    DDRB &= ~(1 << PB1);

    // Standby as output
    DDRD |= 1 << STBY_RES;
    PORTD |= 1 << STBY_RES;

	return;
}


void wasteTime(void) {
	unsigned int i;
	for( i = 65535; i > 0; i-- ) {
		asm("nop");
	}
}
