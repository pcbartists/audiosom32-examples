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
#include "sounds.h"

static const char *TAG = "main.c";

void audio_play_task (void *pvParameter)
{
    const char *ptr = (const char *)audiobit_music;
    unsigned int offset = 0;
    signed short samples[256], i;
    size_t written;

    // Play array from signed, 16-bit stereo file
    while (offset<NUM_ELEMENTS*2)
    {
        memcpy (samples, ptr+offset, sizeof(samples));

        for (i=0; i<256; i++)
            samples[i] = samples[i] + 32768;
        i2s_write (AUDIOSOM32_I2S_NUM, (const char*) samples, 512, &written, portMAX_DELAY);
        offset = offset+512;
    }

    ESP_LOGW (TAG, "Done playing demo file. Reset to re-play!\n");

    // Silence here
    while (1)
    {
        for (i=0; i<256; i++)
            samples[i] = 0;
        i2s_write (AUDIOSOM32_I2S_NUM, (const char*) samples, 512, &written, portMAX_DELAY);
    }
}

void app_main()
{
    // Initialize the I2S clocks
    audiosom32_i2s_init ();
    // Wait for MCLK to stabilize
    ets_delay_us (1000);
    // Start communicating over I2C
    audiosom32_i2c_init();

    if (audiosom32_playback_init () == ESP_OK)
        ESP_LOGI (TAG, "I2S and I2C setup is completed for playback!\n");
    else
        ESP_LOGI (TAG, "Seems like AudioSOM32 is not connected configured!\n");

    // Create a task to play audio by loading DMA buffers
    // Not loading in time may cause muting or glitches
    xTaskCreate(&audio_play_task, "audio_play_task", 4096, NULL, 8, NULL);
}
