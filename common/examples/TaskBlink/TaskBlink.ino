/* SPDX-License-Identifier: MIT */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <Arduino_FreeRTOS.h>

void TaskBlink( void * pvParameters )
{
    /* Initialize LED pin */
    pinMode( LED_BUILTIN, OUTPUT );

    for( ;; )
    {
        digitalWrite( LED_BUILTIN, HIGH );
        vPortDelayMs( 250 );  /* Delay for 250ms */
        digitalWrite( LED_BUILTIN, LOW );
        vPortDelayMs( 250 );  /* Delay for 250ms */
    }
}

void TaskSerialMonitor( void * pvParameters )
{
    for( ;; )
    {
        Serial.print( "Millis: " );
        Serial.println( millis() );
        vPortDelayMs( 1000 );  /* Delay for 1 second */
    }
}

void setup()
{
    Serial.begin( 9600 );
    xTaskCreate(
        TaskBlink,
        "Blink",
        128, /* Stack size */
        NULL,
        1,   /* Priority */
        NULL
        );
    xTaskCreate(
        TaskSerialMonitor,
        "Serial",
        128, /* Stack size */
        NULL,
        1,   /* Priority */
        NULL
        );
    vTaskStartScheduler();   /* Start the FreeRTOS scheduler */
}

void loop( void )
{
}
