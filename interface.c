/******************************************************************
 * Contains functions for creating the user interface
 ******************************************************************/

#include "interface.h"
#include "serial.h"
#include "L6472.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE     30

char buffer[BUFFER_SIZE];

void initInterface(void) {
    printString( "\x1b[0;0H" );
    printString( "\x1b[2J" );
}

void printStatus(void) {
    printString( "\x1b[4;0H" );
    printString( "\x1b[J" );
    printString( "\n------------------------------------------------\r\n" );

    //Direction
    printString( "Direction: " );
    if( GetStatus() & 0x0010 ) {
        printString( "Forward\r\n" );
    } else {
        printString( "Reverse\r\n" );
    }
    //Speed
    printString( "Speed: " );
    printString( itoa( GetParam( SPEED ) * 100 / 0x0000cccc, buffer, 10 ) );
    printString( "%\r\n" );
    //Max speed
    printString( "Max Speed: " );
    //print32Hex( GetParam( MAX_SPEED ) );
    printString( itoa( GetParam( MAX_SPEED ) * 100 / 0x3c, buffer, 10 ) );
    printString( "%\r\n" );

    printString( "------------------------------------------------\r\n" );
}

char *getCommand(void) {
    uint8_t i;

    for( i = 0; i < BUFFER_SIZE; i++ ) {
        buffer[i] = rxUSART();
        if( buffer[i] == '\r' ) {
            buffer[i] = '\0';
            txUSART( '\n' );
            return buffer;
        }
    }
    return 0;
}

uint32_t convert( uint8_t in, uint8_t in_max, uint32_t max ) {
    if( in > in_max )
        in = in_max;
    return (uint32_t)in * (uint32_t)max / in_max;
}

uint32_t degToTick( uint16_t in, uint16_t in_max, uint32_t max ) {
    return (uint32_t)in * (uint32_t)max / in_max;
}

void handleCommand( char *string ) {
    char *subString;
    uint32_t status;

    printString( "\x1b[K" );
    printString( string );
    printString( "\r\n" );
    printString( "\x1b[K" );

    subString = strtok( string, " " );
    switch( *subString ) {
        // Stop
        case 'h':
            printString( "Hault Motor\r\n" );
            SoftHiZ();
            whileBusy();
            break;
        // Forward
        case 'f':
            subString = strtok( NULL, " " );
            if( subString != NULL ) {
                printString( "Forward: " );
                printString( subString );
                printString( "%" );
                Run( FORWARD, convert( atoi( subString ), 100, 0x00cccc ) );
            } else
                printString( "Error: wrong argument type\r\n" );
            break;
        // Reverse
        case 'r':
            subString = strtok( NULL, " " );
            if( subString != NULL ) {
                printString( "Reverse: " );
                printString( subString );
                printString( "%" );
                Run( REVERSE, convert( atoi( subString ), 100, 0x00cccc ) );
            } else
                printString( "Error: wrong argument type\r\n" );
            break;
        // Get Status
        case 'g':
            status = GetStatus();
            printString( "Get Status: 0x" );
            printHex( (status >> 24) & 0xff );
            printHex( (status >> 16) & 0xff );
            printHex( (status >> 8) & 0xff );
            printHex( (status >> 0) & 0xff );
            printString( "\r\n" );
            break;
        // Break
        case 'b':
            printString( "Break Motor\r\n" );
            SoftStop();
            whileBusy();
            break;
        // Move
        case 'm':
            subString = strtok( NULL, " " );
            if( *subString == 'f' ) {
                subString = strtok( NULL, " " );
                printString( "Move Forward: " );
                printString( subString );
                printString( " degrees" );
                Move( FORWARD, degToTick( atoi( subString ), 360, 0x6414 ) );
            } else if( *subString == 'r' ) {
                subString = strtok( NULL, " " );
                printString( "Move Reverse: " );
                printString( subString );
                printString( " degrees" );
                Move( REVERSE, degToTick( atoi( subString ), 360, 0x6414 ) );
            } else {
                printString( "Error: wrong argument type\r\n" );
            }
            break;
        // Status
        case 's':
            printString( "Motor Status" );
            printStatus();
            return;
            break;
        // maX speed
        case 'x':
            subString = strtok( NULL, " " );
            printString( "Max Speed: " );
            printString( subString );
            printString( "%" );
            SetParam( MAX_SPEED, convert( atoi( subString ), 100, 0x3c ) );
            break;
        // Error
        case '\0':
        default:
            printString( "Command not valid\r\n" );
            break;
    }
    printString( "\r\n\x1b[J" );
}

