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

#include <string.h>
#include "esp_log.h"
#include "soc/rtc.h"
#include "soc/soc.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

// Application includes
#include "audiosom32_codec.h"
#include "audiosom32_driver.h"
#include "audiosom32_carrier.h"

static const char *TAG = "audiosom32_carrier.c";

// Weak default button interrupt handler, declare this elsewhere to replace this function
__attribute__((weak)) void IRAM_ATTR as32_btn_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    // Handle button interrupt here
}

void audiosom32_carrier_init (void)
{
    // Configure LED GPIO for output
    gpio_pad_select_gpio (AS32_LED_GPIO);
    gpio_set_direction (AS32_LED_GPIO, GPIO_MODE_OUTPUT);
    // Turn LED off
    gpio_set_level(AS32_LED_GPIO, 1);

    // Configure analog button interrupt
    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    io_conf.pin_bit_mask = (1ULL<<AS32_BTN_GPIO);    
    gpio_config(&io_conf);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(AS32_BTN_GPIO, as32_btn_isr_handler, (void*) AS32_BTN_GPIO);
}

esp_err_t audiosom32_sd_init (void)
{
    esp_err_t ret;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    esp_vfs_fat_sdmmc_mount_config_t mount_config =
    {
        .format_if_mount_failed = false,
        .max_files = 3,
        .allocation_unit_size = 4 * 1024
    };

    sdmmc_card_t* card;
    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE (TAG, "Failed to mount filesystem. Please format SD card.");
            return ret;
        }
        else
        {
            ESP_LOGE (TAG, "Failed to initialize the card (CAUSE: %s)", esp_err_to_name(ret));
            return ret;
        }
    }
    else
    {
        // Everything good, able to read SD card
        // Print some info about the card
        ESP_LOGI (TAG, "SD card ready!!!");
        sdmmc_card_print_info(stdout, card);
        return ESP_OK;
    }
}
