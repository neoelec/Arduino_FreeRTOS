#ifndef FREERTOSPORT_FSP_H
#define FREERTOSPORT_FSP_H

#define vPortDelayMs( xTimeInMs ) \
        do { \
            vTaskDelay( pdMS_TO_TICKS( xTimeInMs ) ); \
        } while( 0 )

#endif /* FREERTOSPORT_FSP_H */
