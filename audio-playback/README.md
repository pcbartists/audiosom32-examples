## Buy an AudioSOM32 module or dev kit
https://pcbartists.com/audiosom32

## What this example does
- Initializes the I2S and I2C for the AudioSOM32 module
- Plays raw stereo effect audio file stored in sounds.h for a few seconds via headphone
- Press the restart button to replay the audio clip

## How to build
- Within an ESP-IDF terminal, cd into this directory to build and flash
```sh
cd YOUR_PATH/audiosom32-examples/audio-playback
idf.py build flash -b 921600 monitor -p COMx
```
- Make sure partitions.csv is set in menuconfig as this example needs a custom partition table
- COMx is whatever COM port is used to flash the ESP32, e.g. COM4.

## Development environment
This example was last tested with
- ESP-IDF v.4.0 (release version)