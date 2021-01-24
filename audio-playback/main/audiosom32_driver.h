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

#ifndef _AUDIOSOM32_DRIVER_H_
#define _AUDIOSOM32_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <esp_types.h>
#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/i2s.h"
#include "soc/soc.h"
#include "audiosom32_codec.h"

// ################ AudioSOM32-specific settings ################
// Power rails in millivolts
#define AUDIOSOM32_VDDD			    1800
#define AUDIOSOM32_EXT_VDDD		    1
#define AUDIOSOM32_VDDIO			3300
#define AUDIOSOM32_VDDA			    1800

// GPIO pads for the control bus (I2C)
#define AUDIOSOM32_I2C_SCL_IO		5
#define AUDIOSOM32_I2C_SDA_IO		18
#define AUDIOSOM32_I2C_ADDR  		0x0A		            // AudioSOM32 SGTL5000 slave address

// GPIO pads for the I2S connections
#define AUDIOSOM32_MCLK			    0			            // NOTE: Do not change MCLK pad
#define	AUDIOSOM32_LRCLK			21
#define AUDIOSOM32_BCK			    22
#define AUDIOSOM32_DOUT			    23
#define AUDIOSOM32_DIN			    19			            // Audio recording not supported in this example!


// ################ System settings (better not touch) ################
// Control I2C peripheral settings
#define AUDIOSOM32_I2C_NUM 		    1			            // I2C module number
#define AUDIOSOM32_I2C_FREQ_HZ      100000		            // Master clock frequency (Hz)

// NOTE: Do not change I2S num right now because it will affect MCLK output!
#define AUDIOSOM32_I2S_NUM          (0)
#define AUDIOSOM32_SAMPLERATE		48000
#define AUDIOSOM32_BITSPERSAMPLE	16

#define WRITE_BIT  				    I2C_MASTER_WRITE        /*!< I2C master write */
#define READ_BIT   				    I2C_MASTER_READ         /*!< I2C master read */
#define ACK_CHECK_EN   			    0x1                     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  			    0x0                     /*!< I2C master will not check ack from slave */
#define ACK_VAL    				    0x0                     /*!< I2C ack value */
#define NACK_VAL   				    0x1                     /*!< I2C nack value */

#define I2C_MASTER_TX_BUF_DISABLE   0                       /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                       /*!< I2C master do not need buffer */

// General system related APIs
//esp_err_t audiosom32_poweron_init (void);
esp_err_t audiosom32_write_reg (i2c_port_t i2c_num, uint16_t reg_addr, uint16_t reg_val);
esp_err_t audiosom32_read_reg (i2c_port_t i2c_num, uint16_t reg_addr, uint16_t *reg_val);
void audiosom32_i2c_init();
void audiosom32_i2s_init();
esp_err_t audiosom32_playback_init (void);

// AudioSOM32 APIs for SGTL5000 config
esp_err_t audiosom32_set_surround_sound (uint8_t surround);
esp_err_t audiosom32_set_mic_resistor (uint8_t bias);
esp_err_t audiosom32_set_mic_voltage (uint16_t voltage);
esp_err_t audiosom32_set_mic_gain (uint8_t gain);
esp_err_t audiosom32_mute_headphone (void);
esp_err_t audiosom32_unmute_headphone (void);
esp_err_t audiosom32_set_ref (uint16_t vag_voltage);
esp_err_t audiosom32_check_module (void);
esp_err_t audiosom32_set_digital_volume (int8_t left_vol, int8_t right_vol);
esp_err_t audiosom32_set_headphone_volume (int8_t left_vol, int8_t right_vol);
esp_err_t audiosom32_pin_drive_strength (uint8_t i2c_strength, uint8_t i2s_strength);

#ifdef __cplusplus
}
#endif
#endif
