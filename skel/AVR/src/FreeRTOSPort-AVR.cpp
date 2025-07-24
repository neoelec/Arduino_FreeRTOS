/* SPDX-License-Identifier: MIT */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 *
 * Reference:
 *  - https://github.com/feilipu/Arduino_FreeRTOS_Library/blob/master/src/port.c
 */

#include <Arduino_FreeRTOS.h>

#include <util/delay.h>

void initVariant( void )
{
    /* As the Task stacks are on heap before Task allocated heap variables,
     * the library default __malloc_heap_end = 0 doesn't work. */
    __malloc_heap_end = ( char * ) ( RAMEND - __malloc_margin );
}

#if !defined( portUSE_TIMER_NUM )
    void vPortDelayMs( const uint64_t xTimeInMs )
    {
        if( xTimeInMs < portTICK_PERIOD_MS )
        {
            _delay_ms( ( double ) xTimeInMs );
            portYIELD();
        }
        else
        {
            _delay_ms( ( double ) ( ( xTimeInMs - portTICK_PERIOD_MS ) % portTICK_PERIOD_MS ) );
            vTaskDelay( pdMS_TO_TICKS( xTimeInMs ) );
        }
    }
#endif /* if !defined( portUSE_TIMER_NUM ) */
