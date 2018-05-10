/******************************************************************
 * Contains functions controlling the L6472 motor controller
 ******************************************************************/
#include "L6472.h"
#include "serial.h"

// Lookup table for register size
const uint8_t regSize[0x1c] = { 0,
                                3, 2, 3, 3, 2,
                                2, 2, 2, 1, 1,
                                1, 1, 0, 1, 1,
                                1, 0, 1, 1, 0,
                                2, 1, 1, 2, 2,
                                0, 0 };

uint8_t buffer[10];


void initL6472(void) {
    // Set Max and Min speeds
    SetParam( MAX_SPEED, 0x3c );
    SetParam( MIN_SPEED, 0 );

    // Set motor torque currents 0x00 = 31.25mA, 0x3f = 4A
    SetParam( TVAL_HOLD, 0x0f );
    SetParam( TVAL_RUN, 0x0f );
    SetParam( TVAL_ACC, 0x0f );
    SetParam( TVAL_DEC, 0x0f );
}

void SetParam( uint8_t param, uint32_t value ) {
    // param doesn't exist
    if( param >= 0x1c )
        return;

    // build and send packet
    switch( regSize[param] ) {
        case 3:
            buffer[0] = param;
            buffer[1] = (value & 0x00ff0000) >> 16;
            buffer[2] = (value & 0x0000ff00) >> 8;
            buffer[3] = (value & 0x000000ff) >> 0;
            txrxSPI( buffer, 4 );
            break;
        case 2:
            buffer[0] = param;
            buffer[1] = (value & 0x0000ff00) >> 8;
            buffer[2] = (value & 0x000000ff) >> 0;
            txrxSPI( buffer, 3 );
            break;
        case 1:
            buffer[0] = param;
            buffer[1] = (value & 0x000000ff) >> 0;
            txrxSPI( buffer, 2 );
            break;
        case 0:
        default:
            return;
            break;
    }
    return;
}

uint32_t GetParam( uint8_t param ) {
    // return value
    uint32_t paramValue = 0;

    // param doesn't exist
    if( param >= 0x1c )
        return 0;

    buffer[0] = param | 0x20;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;

    // read parameter
    switch( regSize[param] ) {
        case 3:
            txrxSPI( buffer, 4 );
            paramValue += buffer[1];
            paramValue <<= 16;
            paramValue += buffer[2];
            paramValue <<= 8;
            paramValue += buffer[3];
            break;
        case 2:
            txrxSPI( buffer, 3 );
            paramValue += buffer[1];
            paramValue <<= 8;
            paramValue += buffer[2];
            break;
        case 1:
            txrxSPI( buffer, 2 );
            paramValue += buffer[1];
            break;
        case 0:
        default:
            return 0;
            break;
    }
    return paramValue;
}

uint32_t GetStatus(void) {
    uint32_t status = 0;

    buffer[0] = 0xd0;
    buffer[1] = 0x00;
    buffer[2] = 0x00;

    txrxSPI( buffer, 3 );
    status += buffer[1];
    status <<= 8;
    status += buffer[2];

    return status;
}

void Run( direction_t dir, uint32_t speed ) {
    // Set run parameter
    if( dir == FORWARD )
        buffer[0] = 0x51;
    else
        buffer[0] = 0x50;

    // Set speed parameter
    buffer[1] = (speed >> 16) & 0xff;
    buffer[2] = (speed >> 8) & 0xff;
    buffer[3] = (speed >> 0) & 0xff;

    // execute
    txrxSPI( buffer, 4 );

    return;
}

void Move( direction_t dir, uint32_t steps ) {
    // Set run parameter
    if( dir == FORWARD )
        buffer[0] = 0x41;
    else
        buffer[0] = 0x40;

    // Set speed parameter
    buffer[1] = (steps >> 16) & 0xff;
    buffer[2] = (steps >> 8) & 0xff;
    buffer[3] = (steps >> 0) & 0xff;

    // execute
    txrxSPI( buffer, 4 );
    whileBusy();

    return;
}

void SoftStop(void) {
    buffer[0] = 0xb0;
    txrxSPI( buffer, 1 );
    return;
}

void SoftHiZ(void) {
    buffer[0] = 0xa0;
    txrxSPI( buffer, 1 );
    return;
}

void whileBusy(void) {
    while( checkBusy() );
}

bool checkBusy(void) {
    // Busy is set if pin is low
    if( (GetStatus() & 0x02) == 0 )
        return true;
    else
        return false;
}

