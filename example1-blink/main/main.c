/*
Website: www.pcbartists.com
Copyright (C) 2020, PCB Artists OPC Pvt Ltd, all right reserved.
E-mail: hello@pcbartists.com

The code referencing this license is open source software. Redistribution
and use of the code in source and binary forms, with or without modification,
are permitted provided that the following condition is met:

    Redistribution of original and modified source code must retain the 
    above copyright notice, this condition and the following disclaimer.

This software is provided by the copyright holder and contributors "AS IS"
and any warranties related to this software are DISCLAIMED. The copyright 
owner or contributors are NOT LIABLE for any damages caused by use of this 
software.
*/

// System includes
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// Application includes
#include "main.h"

const char *TAG = "main.c";

void app_main()
{
    // Configure LED GPIO for output
    gpio_pad_select_gpio (AS32_LED_GPIO);
    gpio_set_direction (AS32_LED_GPIO, GPIO_MODE_OUTPUT);

    ESP_LOGI (TAG, "Begin blinking LED...");
    
    while(1)
    {
        // Turn LED on
        gpio_set_level(AS32_LED_GPIO, 0);
        vTaskDelay (500 / portTICK_PERIOD_MS);

        // Turn LED off
        gpio_set_level(AS32_LED_GPIO, 1);
        vTaskDelay (500 / portTICK_PERIOD_MS);
    }
}
