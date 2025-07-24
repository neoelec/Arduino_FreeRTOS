/* SPDX-License-Identifier: MIT */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference:
 *  - https://github.com/feilipu/Arduino_FreeRTOS_Library/blob/master/src/port.c
 */

#ifndef PORTTICKSRC_H
#define PORTTICKSRC_H

#include <avr/io.h>
#include <avr/wdt.h>

#include "../../Arduino_FreeRTOS.h"

/*
 * Setup WDT to generate a tick interrupt.
 */
static void prvWdtEnable( const uint8_t value )
{
    if( _SFR_IO_REG_P( _WD_CONTROL_REG ) )
    {
        asm volatile (
            "in __tmp_reg__, __SREG__ \n\t"
            "cli \n\t"
            "wdr \n\t"
            "out %0, %1 \n\t"
            "out __SREG__, __tmp_reg__ \n\t"
            "out %0, %2 \n\t"
            :     /* no outputs */
            : "I" ( _SFR_IO_ADDR( _WD_CONTROL_REG ) ),
            "r" ( ( uint8_t ) ( _BV( _WD_CHANGE_BIT ) | _BV( WDE ) ) ),
            "r" ( ( uint8_t ) ( ( value & 0x08 ? _WD_PS3_MASK : 0x00 ) |
                                _BV( WDIF ) | _BV( WDIE ) | ( value & 0x07 ) ) )
            : "r0"
            );
    }
    else
    {
        asm volatile (
            "in __tmp_reg__, __SREG__ \n\t"
            "cli \n\t"
            "wdr \n\t"
            "sts %0, %1 \n\t"
            "out __SREG__, __tmp_reg__ \n\t"
            "sts %0, %2 \n\t"
            :     /* no outputs */
            : "n" ( _SFR_MEM_ADDR( _WD_CONTROL_REG ) ),
            "r" ( ( uint8_t ) ( _BV( _WD_CHANGE_BIT ) | _BV( WDE ) ) ),
            "r" ( ( uint8_t ) ( ( value & 0x08 ? _WD_PS3_MASK : 0x00 ) |
                                _BV( WDIF ) | _BV( WDIE ) | ( value & 0x07 ) ) )
            : "r0"
            );
    }
}

static inline void prvSetupTimerInterrupt( void )
{
    wdt_reset();
    prvWdtEnable( portWDTO_PERIOD );
}

static inline void prvDisableTimerInterrupt( void )
{
    wdt_disable();
}

#endif /* ifndef PORTTICKSRC_H */
