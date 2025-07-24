/* SPDX-License-Identifier: MIT */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference:
 *  - https://github.com/feilipu/Arduino_FreeRTOS_Library/blob/master/src/port.c
 */

#ifndef FREERTOSPORT_AVR_H
#define FREERTOSPORT_AVR_H

#include <avr/sleep.h>
#include <avr/wdt.h>

#define portUSE_TIMER_NUM    2

#define portWDTO_PERIOD      WDTO_15MS

#define portSLEEP_RESET()    _SLEEP_CONTROL_REG = 0;

#if !defined( portUSE_TIMER_NUM )
    #define portTICK_ISR          WDT_vect
    #define portTICK_PERIOD_MS    ( ( TickType_t ) _BV( portWDTO_PERIOD + 4 ) )

    #define configTICK_RATE_HZ    ( ( TickType_t ) ( ( uint32_t ) 128000 >> ( portWDTO_PERIOD + 11 ) ) )
#else
    #define __TIMERx_COMPA_vect( x )    TIMER ## x ## _COMPA_vect
    #define TIMERx_COMPA_vect( x )      __TIMERx_COMPA_vect( x )
    #define portTICK_ISR    TIMERx_COMPA_vect( portUSE_TIMER_NUM )
#endif /* if defined( portUSE_TIMER_NUM ) */


#ifdef __cplusplus
    extern "C" {
#endif

#if !defined( portUSE_TIMER_NUM )
    extern void vPortDelayMs( const uint64_t xTimeInMs );
#else
    #define vPortDelayMs( xTimeInMs ) \
            do { \
                vTaskDelay( pdMS_TO_TICKS( xTimeInMs ) ); \
            } while( 0 )
#endif /* if defined( portUSE_TIMER_NUM ) */

#ifdef __cplusplus
    }
#endif

#endif /* FREERTOSPORT_AVR_H  */
