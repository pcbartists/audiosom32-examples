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

#ifndef _AUDIOSOM32_CARRIER_H_
#define _AUDIOSOM32_CARRIER_H_

#define     AS32_LED_GPIO    25

#define     AS32_BTN_UP      2950
#define     AS32_BTN_DN      2330
#define     AS32_BTN_LT      2860
#define     AS32_BTN_RT      2660

#define     AS32_BTN_GPIO    33

#define     AS32_SD_IO0      2
#define     AS32_SD_IO1      4
#define     AS32_SD_IO2      12
#define     AS32_SD_IO3      13
#define     AS32_SD_CLK      14
#define     AS32_SD_CMD      15

void audiosom32_carrier_init (void);
esp_err_t audiosom32_sd_init (void);

#endif
