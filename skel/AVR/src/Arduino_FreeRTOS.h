/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * FreeRTOS library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#pragma once

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOSConfig.h"

#include "lib/FreeRTOS-Kernel-v<freertos_version>/FreeRTOS.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/croutine.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/event_groups.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/queue.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/semphr.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/task.h"
#include "lib/FreeRTOS-Kernel-v<freertos_version>/timers.h"
#include <stdbool.h>


// If you need to automatically start FREERTOS, declare either EARLY_AUTOSTART_FREERTOS or
// AUTOSTART_FREERTOS in your library or sketch code (.ino or .cpp file)
//
// EARLY_AUTOSTART_FREERTOS -> if you need the scheduler to be already running in setup()
// AUTOSTART_FREERTOS -> if you only declare the threads in setup() and use them in loop()

void _start_freertos_on_header_inclusion_impl(bool early_start);
void early_start_freertos_on_header_inclusion();
void start_freertos_on_header_inclusion();
#define EARLY_AUTOSTART_FREERTOS \
    void early_start_freertos_on_header_inclusion() { \
        _start_freertos_on_header_inclusion_impl(true); \
    }
#define AUTOSTART_FREERTOS \
    void start_freertos_on_header_inclusion() { \
        _start_freertos_on_header_inclusion_impl(false); \
    }


#ifdef __cplusplus
}
#endif
