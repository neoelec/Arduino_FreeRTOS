/* SPDX-License-Identifier: MIT */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#ifndef PORTTICKSRC_H
#define PORTTICKSRC_H

#include <avr/io.h>

#include "../../Arduino_FreeRTOS.h"

#ifndef ARRAY_SIZE
    #define ARRAY_SIZE( arr )    ( sizeof( arr ) / sizeof( ( arr )[ 0 ] ) )
#endif

#define __TIMER_REG( n ) \
        { \
            .uTimerNum = n, \
            .TCCRnA = &( TCCR ## n ## A ), \
            .TCCRnB = &( TCCR ## n ## B ), \
            .OCRnA = &( OCR ## n ## A ), \
            .TIMSKn = &( TIMSK ## n ), \
            .WGMn1 = ( WGM ## n ## 1 ), \
            .WGMn2 = ( WGM ## n ## 2 ), \
            .OCIEnA = ( OCIE ## n ## A ), \
        }
#define TIMER_REG( n )    __TIMER_REG( n )

typedef struct
{
    uint8_t uTimerNum;
    volatile void * TCCRnA;
    volatile void * TCCRnB;
    volatile void * OCRnA;
    volatile void * TIMSKn;
    uint8_t WGMn1;
    uint8_t WGMn2;
    uint8_t OCIEnA;
} TimerCfg_t;

static const TimerCfg_t *prvxTimerCfg( const TimerCfg_t * pxTimerCfg,
                                       uint8_t uNrTimerCfg,
                                       uint8_t uTimerNum )
{
    for(uint8_t i = 0; i < uNrTimerCfg; i++)
    {
        if( pxTimerCfg[ i ].uTimerNum == uTimerNum )
        {
            return &pxTimerCfg[ i ];
        }
    }

    return NULL;
}

typedef struct
{
    uint8_t uShift;
    uint8_t uClockSelect;
} PrescalerCfg_t;

static const PrescalerCfg_t *prvxPrescale( const PrescalerCfg_t * pxPrescalerCfg,
                                           uint8_t uNrPrescalerCfg,
                                           uint32_t * pulTicks,
                                           uint32_t ulMaxTicks )
{
    uint32_t ulTicks = *pulTicks;

    for(uint8_t i = 0; i < uNrPrescalerCfg; i++)
    {
        if( ( ulTicks >> ( uint32_t ) pxPrescalerCfg[ i ].uShift ) <= ulMaxTicks )
        {
            ulTicks >>= ( uint32_t ) pxPrescalerCfg[ i ].uShift;
            ulTicks -= 1;
            *pulTicks = ulTicks;
            return &pxPrescalerCfg[ i ];
        }
    }

    return NULL;
}

static const PrescalerCfg_t xPrescalerCfg5[] =
{
    { .uShift = 0,  .uClockSelect = _BV( CS00 ),},
    { .uShift = 3,  .uClockSelect = _BV( CS01 ),},
    { .uShift = 6,  .uClockSelect = _BV( CS01 ) | _BV( CS00 ),},
    { .uShift = 8,  .uClockSelect = _BV( CS02 ),},
    { .uShift = 10, .uClockSelect = _BV( CS02 ) | _BV( CS00 ),},
};

static const PrescalerCfg_t xPrescalerCfg7[] =
{
    { .uShift = 0,  .uClockSelect = _BV( CS00 ),},
    { .uShift = 3,  .uClockSelect = _BV( CS01 ),},
    { .uShift = 5,  .uClockSelect = _BV( CS01 ) | _BV( CS00 ),},
    { .uShift = 6,  .uClockSelect = _BV( CS02 ),},
    { .uShift = 7,  .uClockSelect = _BV( CS02 ) | _BV( CS00 ),},
    { .uShift = 8,  .uClockSelect = _BV( CS02 ) | _BV( CS01 ),},
    { .uShift = 10, .uClockSelect = _BV( CS02 ) | _BV( CS01 ) | _BV( CS00 ),},
};

/*
 * Setup timer 0/2 compare match A to generate a tick interrupt.
 */
#if !defined( TIMSK )
    #define uTimerNum5    0
#else
    #define uTimerNum5    2
#endif

static inline void prvSetupTimerInterrupt8bit( uint8_t uTimerNum )
{
    #if defined( TIMSK )
        volatile uint8_t TCCRnB;
    #endif
    const TimerCfg_t xTimerCfg[] = {
        #if !defined( TIMSK )
            #if defined( TIMSK0 )
                TIMER_REG( 0 ),
            #endif
            #if defined( TIMSK2 )
                TIMER_REG( 2 ),
            #endif
        #else /* if !defined( TIMSK ) */
            #if defined( OCIE0 )
                {
                    uTimerNum = 0,
                    TCCRnA = &( TCCR0 ),
                    TCCRnB = &TCCRnB,
                    OCRnA = &( OCR0 ),
                    TIMSKn = &( TIMSK );
                    WGMn1 = WGM01,
                    OCIEnA = OCIE0,
                },
            #elif defined( OCIE2 )
                {
                    uTimerNum = 2,
                    TCCRnA = &( TCCR2 ),
                    TCCRnB = &TCCRnB,
                    OCRnA = &( OCR2 ),
                    TIMSKn = &( TIMSK );
                    WGMn1 = WGM21,
                    OCIEnA = OCIE2,
                },
            #endif /* if defined( OCIE0 ) */
        #endif /* if !defined( TIMSK ) */
    }, * pxTimerCfg;
    const PrescalerCfg_t * pxPrescalerCfg;
    uint32_t ulCompareMatch;

    ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

    if( uTimerNum == uTimerNum5 )
    {
        pxPrescalerCfg = prvxPrescale( xPrescalerCfg5, ARRAY_SIZE( xPrescalerCfg5 ), &ulCompareMatch, 0x100 );
    }
    else
    {
        pxPrescalerCfg = prvxPrescale( xPrescalerCfg7, ARRAY_SIZE( xPrescalerCfg7 ), &ulCompareMatch, 0x100 );
    }

    pxTimerCfg = prvxTimerCfg( xTimerCfg, ARRAY_SIZE( xTimerCfg ), uTimerNum );

    /* Initialize timer/counter control register */
    _MMIO_BYTE( pxTimerCfg->TCCRnA ) = 0x0;
    _MMIO_BYTE( pxTimerCfg->TCCRnB ) = 0x0;

    /* Setup compare match value for compare match A. */
    _MMIO_BYTE( pxTimerCfg->OCRnA ) = ( uint8_t ) ulCompareMatch;

    /* Setup clock source and compare match behaviour. */
    _MMIO_BYTE( pxTimerCfg->TCCRnA ) = _BV( pxTimerCfg->WGMn1 );
    _MMIO_BYTE( pxTimerCfg->TCCRnB ) = pxPrescalerCfg->uClockSelect;

    /* Enable the interrupt - this is okay as interrupt are currently globally
     * disabled. */
    _MMIO_BYTE( pxTimerCfg->TIMSKn ) = _BV( pxTimerCfg->OCIEnA );
}

/*
 * Setup timer 1/3/4/5 compare match A to generate a tick interrupt.
 */
#if defined( TIMSK )
    #define TIMSK1    TIMSK
#endif
#if defined( ETIMSK )
    #define TIMSK3    ETIMSK
#endif

static inline void prvSetupTimerInterrupt16bit( uint8_t uTimerNum )
{
    const TimerCfg_t xTimerCfg[] = {
        #if defined( TIMSK1 )
            TIMER_REG( 1 ),
        #endif
        #if defined( TIMSK3 )
            TIMER_REG( 3 ),
        #endif
        #if defined( TIMSK4 )
            TIMER_REG( 4 ),
        #endif
        #if defined( TIMSK5 )
            TIMER_REG( 5 ),
        #endif
    }, * pxTimerCfg;
    const PrescalerCfg_t * pxPrescalerCfg;
    uint32_t ulCompareMatch;

    ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
    pxPrescalerCfg = prvxPrescale( xPrescalerCfg5, ARRAY_SIZE( xPrescalerCfg5 ), &ulCompareMatch, 0x10000 );

    pxTimerCfg = prvxTimerCfg( xTimerCfg, ARRAY_SIZE( xTimerCfg ), uTimerNum );

    /* Initialize timer/counter control register */
    _MMIO_BYTE( pxTimerCfg->TCCRnA ) = 0x0;
    _MMIO_BYTE( pxTimerCfg->TCCRnB ) = 0x0;

    /* Setup compare match value for compare match A. */
    _MMIO_WORD( pxTimerCfg->OCRnA ) = ( uint16_t ) ulCompareMatch;

    /* Setup clock source and compare match behaviour. */
    _MMIO_BYTE( pxTimerCfg->TCCRnB ) = _BV( pxTimerCfg->WGMn2 ) | pxPrescalerCfg->uClockSelect;

    /* Enable the interrupt - this is okay as interrupt are currently globally
     * disabled. */
    _MMIO_BYTE( pxTimerCfg->TIMSKn ) = _BV( pxTimerCfg->OCIEnA );
}

#if defined( TIMSK )
    #undef TIMSK1 TIMSK
#endif
#if defined( ETIMSK )
    #undef TIMSK3 ETIMSK
#endif

static inline void prvSetupTimerInterrupt( void )
{
    switch( portUSE_TIMER_NUM )
    {
        case 0:
        case 2:
            prvSetupTimerInterrupt8bit( portUSE_TIMER_NUM );
            break;

        default:
            prvSetupTimerInterrupt16bit( portUSE_TIMER_NUM );
            break;
    }
}

static inline void prvDisableTimerInterrupt( void )
{
}

#endif /* ifndef PORTTICKSRC_H */
