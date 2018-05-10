/******************************************************************
 * Contains functions for creating the user interface
 ******************************************************************/
#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

void initInterface(void);

void printStatus(void);

char *getCommand(void);

void handleCommand( char *string );

#endif
