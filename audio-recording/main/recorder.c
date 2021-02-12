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

static const char *TAG = "recorder.c";
static bool button_pressed = false;

// Executed every time any button is pressed
void IRAM_ATTR as32_btn_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;

    // Some button was pressed!
    button_pressed = true;
    // ets_printf ("#");
}

void audio_rec_task (void *pvParameter)
{
    FILE *f;
    size_t written;
    short *samples;

    // Fill WAV file header with necessary values. File size is left to 0.
    // Most players like Audacity and VLC will ignore wave data size descriptor anyway
    wav_header wav_hdr = 
    {
        .riff_header = "RIFF",
        .wav_size = 0,
        .wave_header = "WAVE",

        .fmt_header = "fmt ",
        .fmt_chunk_size = 16,
        .audio_format = 1,
        .num_channels = 2,
        .sample_rate = 48000,
        .byte_rate = 48000*2*2,
        .sample_alignment = 4,
        .bit_depth = 16, 

        .data_header = "data"
    };

    // Allocate buffer for recording data into and then dumping into SD card
    samples = malloc (2048);
    if (samples == NULL)
        goto end_recording;

    // Set up default recording mode:
    // Line in -> ADC -> I2S out
    // Line in -> HP
    if (audiosom32_record_init () == ESP_OK)
        ESP_LOGI (TAG, "I2S and I2C setup is completed for recording!\n");
    else
        ESP_LOGI (TAG, "Seems like AudioSOM32 is not connected configured!\n");

    while (1)
    {
        // Wait for button press event before recording to SD card
        while (button_pressed == false)
            vTaskDelay (100);
        // Debounce any accidental presses within 100ms
        vTaskDelay (100);
        button_pressed = false;
        ESP_LOGW (TAG, "Button pressed, started recording...");

        // Write WAV into a file, overwrite existing one
        f = fopen ("/sdcard/REC.WAV", "w");
        if (f == NULL)
        {
            ESP_LOGE (TAG, "Failed to create REC.WAV...");
            break;
        }

        // Write WAV header into file before recording
        written = fwrite (&wav_hdr, 1, sizeof (wav_hdr), f);

        while (1)
        {            
            // Wait for 2048 samples to arrive
            gpio_set_level(AS32_LED_GPIO, 0);       // LED on
            i2s_read (AUDIOSOM32_I2S_NUM, samples, 2048, &written, portMAX_DELAY);
            gpio_set_level(AS32_LED_GPIO, 1);       // LED off
            // Dump it into the file
            written = fwrite (samples, 1, 2048, f);

            // Stop recording?
            if (button_pressed == true)
                break;
        }

        // Debounce
        vTaskDelay (500/portTICK_RATE_MS);
        button_pressed = false;

        // Save recording
        fclose (f);
        ESP_LOGW (TAG, "Saved REC.WAV!");
    }
    free (samples);

    end_recording:
    ESP_LOGW (TAG, "IDLE, only reaches here on error!");

    // Hang up, must not exit task function!
    while (1)
        vTaskDelay (100);
}
