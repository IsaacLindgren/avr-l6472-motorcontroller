/******************************************************************
 * Contains functions controlling the L6472 motor controller
 ******************************************************************/
#ifndef L6472_H
#define L6472_H

#include <stdint.h>
#include <stdbool.h>

#define ABS_POS         0x01
#define EL_POS          0x02
#define MARK            0x03
#define SPEED           0x04
#define ACC             0x05
#define DEC             0x06
#define MAX_SPEED       0x07
#define MIN_SPEED       0x08
#define TVAL_HOLD       0x09
#define TVAL_RUN        0x0a
#define TVAL_ACC        0x0b
#define TVAL_DEC        0x0c
#define T_FAST          0x0e
#define TON_MIN         0x0f
#define TOFF_MIN        0x10
#define ADC_OUT         0x12
#define OCD_TH          0x13
#define FS_SPD          0x15
#define STEP_MODE       0x16
#define ALARM_EN        0x17
#define CONFIG          0x18
#define STATUS          0x19

#define FLAG            PB0
#define BUSY            PB1
#define STBY_RES        PD6

typedef enum {
    FORWARD,
    REVERSE
} direction_t;

void initL6472(void);

void SetParam( uint8_t param, uint32_t value );

uint32_t GetParam( uint8_t param );
uint32_t GetStatus(void);

void Run( direction_t dir, uint32_t speed );
void Move( direction_t, uint32_t steps );

void SoftStop(void);
void SoftHiZ(void);

void whileBusy(void);
bool checkBusy(void);


#endif
