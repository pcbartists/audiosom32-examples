/*
Website: www.pcbartists.com/audiosom32

Copyright (C) 2021, PCB Artists OPC Pvt Ltd, all right reserved.
Author:     Pratik Panda
E-mail:     hello@pcbartists.com
Website:    pcbartists.com

The code referencing this license is open source software. Redistribution
and use of the code in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    Redistribution of original and modified source code must retain the 
    above copyright notice, this condition and the following disclaimer.

    This code (or modifications) in source or binary forms may NOT be used
    in a commercial application without obtaining permission from the Author.

This software is provided by the copyright holder and contributors "AS IS"
and any warranties related to this software are DISCLAIMED. The copyright 
owner or contributors are NOT LIABLE for any damages caused by use of this 
software.
*/

// System includes
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Application includes
#include "main.h"
#include "audiosom32_driver.h"
#include "audiosom32_carrier.h"
#include "recorder.h"

static const char *TAG = "main.c";

void app_main()
{
    // Initialize the I2S clocks
    audiosom32_i2s_init ();
    // Wait for MCLK to stabilize
    ets_delay_us (1000);
    // Start communicating over I2C
    audiosom32_i2c_init();

    audiosom32_carrier_init ();

    // SD card setup and init, required for recording!
    while (audiosom32_sd_init () != ESP_OK)
    {
        ESP_LOGW (TAG, "Waiting for a usable SD card...");
        vTaskDelay (5000/portTICK_RATE_MS);
    }

    // Create a task to record audio
    xTaskCreate(&audio_rec_task, "audio_rec_task", 4096, NULL, 8, NULL);
}
