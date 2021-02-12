## Buy an AudioSOM32 module or dev kit
https://pcbartists.com/audiosom32

## What this example does
- Waits for an SD card to be plugged in, sets it up when plugged in
- Initializes the I2S and I2C for the AudioSOM32 module in recording mode (Line in -> I2S and Line in -> HP)
- Waits for any button to be pressed on the AudioSOM32 Carrier rev.3.0.
- Creates a file REC.WAV and starts recording audio into it.
- Saves the file when any button is pressed again.
- Further button presses will restart recording and stop recording. However, REC.WAV will be overwritten with the latest recording.
- Audio is recorded at 48kHz sampling rate, 16 bpp stereo

## How to build
- Within an ESP-IDF terminal, cd into this directory to build and flash
```sh
cd YOUR_PATH/audiosom32-examples/audio-recording
idf.py build flash -b 921600 monitor -p COMx
```
- COMx is whatever COM port is used to flash the ESP32, e.g. COM4.

## Development environment
This example was last tested with
- ESP-IDF v.4.0 (release version)