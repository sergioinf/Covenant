# Covenant - ESP32 Smart Doorbell

ESP32-based doorbell system that plays audio from SD card and uses deep sleep mode for power efficiency.

## Features

- 🔔 Plays custom doorbell sounds from SD card
- 💾 SD card support for audio file storage
- 🔊 Audio playback through DFPlayer Mini module
- ⚡ Deep sleep mode for ultra-low power consumption
- 🔘 Wake-up on button press
- 📊 Debug output via Serial monitor

## Hardware Requirements

- **ESP32 Development Board** (any variant with RTC GPIOs)
- **DFPlayer Mini MP3 Player Module**
- **Micro SD Card** (FAT32 formatted, max 32GB)
- **Push Button** (normally open)
- **Speaker** (3W, 4-8Ω recommended)
- **Power Supply** (5V/1A minimum)
- Jumper wires and breadboard (for prototyping)

## Wiring Diagram

### DFPlayer Mini Connections
```
DFPlayer Mini     ESP32
-------------     -----
TX            ->  GPIO 16 (RX2)
RX            ->  GPIO 17 (TX2)
VCC           ->  5V
GND           ->  GND
SPK1          ->  Speaker +
SPK2          ->  Speaker -
```

### SD Card Connections (on ESP32)
```
SD Card       ESP32
-------       -----
CS            GPIO 5
MOSI          GPIO 23
MISO          GPIO 19
SCK           GPIO 18
VCC           3.3V
GND           GND
```

### Button Connection
```
Button        ESP32
------        -----
Pin 1         GPIO 33
Pin 2         GND
```
*Note: Internal pullup resistor is enabled in code*

## Software Setup

### Arduino IDE Setup

1. Install Arduino IDE (1.8.13 or newer)
2. Add ESP32 board support:
   - Go to File -> Preferences
   - Add to "Additional Board Manager URLs": 
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to Tools -> Board -> Boards Manager
   - Search for "ESP32" and install "esp32 by Espressif Systems"

3. Install required libraries:
   - **DFRobotDFPlayerMini** by DFRobot (via Library Manager)
   - **SD** (built-in with ESP32 board package)
   - **SPI** (built-in with ESP32 board package)

### Preparing the SD Card

1. Format SD card as FAT32
2. Create a folder named `mp3` in the root directory (required by DFPlayer Mini)
3. Copy your doorbell sound files:
   - Files must be named: `0001.mp3`, `0002.mp3`, etc.
   - Default track is `0001.mp3` (Track 1)
   - Supported formats: MP3, WAV
   - Recommended: 128-320 kbps MP3 files

### Uploading the Code

1. Open `Covenant/Covenant.ino` in Arduino IDE
2. Select your ESP32 board: Tools -> Board -> ESP32 Arduino -> Your ESP32 Board
3. Select the correct COM port: Tools -> Port
4. Click Upload

## Configuration

You can modify these parameters in the code:

```cpp
#define BUTTON_PIN          33    // GPIO pin for doorbell button
#define SD_CS_PIN           5     // SD card chip select pin
#define DFPLAYER_RX         16    // Connect to DFPlayer TX
#define DFPLAYER_TX         17    // Connect to DFPlayer RX

#define VOLUME              25    // Volume level (0-30)
#define TRACK_NUMBER        1     // Track number to play (1 = 0001.mp3)
#define PLAYBACK_DELAY      5000  // Delay after playback before sleep (ms)
```

## How It Works

1. **Startup**: ESP32 wakes up from deep sleep or initial power-on
2. **Initialization**: Initializes SD card and DFPlayer Mini
3. **Playback**: Plays the configured doorbell sound
4. **Sleep Mode**: Enters deep sleep mode to save power
5. **Wake-up**: Button press triggers wake-up and repeats cycle

### Power Consumption

- **Active Mode** (during playback): ~80-160mA
- **Deep Sleep Mode**: ~10µA (0.01mA)
- **Wake-up Time**: ~100-300ms

## Troubleshooting

### DFPlayer Mini Issues
- **"Unable to begin DFPlayer"**: Check wiring, ensure TX/RX are not swapped
- **No sound**: Check speaker connections, verify SD card has `mp3` folder with audio files
- **"No files found"**: Ensure files are named `0001.mp3`, `0002.mp3`, etc. in the `mp3` folder

### SD Card Issues
- **"SD card initialization failed"**: Check wiring, ensure SD card is FAT32 formatted
- **"No SD card attached"**: Ensure SD card is properly inserted

### Wake-up Issues
- **Won't wake up**: Verify button is connected to GPIO 33 and GND
- **Wakes up randomly**: Add debounce capacitor (0.1µF) across button

## Serial Monitor Output

Example output when system runs correctly:
```
========================================
ESP32 Doorbell System
========================================
Boot count: 1
Wakeup reason: Not deep sleep wakeup (0)

Initializing SD card...
SD Card Type: SDHC
SD Card Size: 8192MB
SD card initialized successfully

Initializing DFPlayer Mini...
DFPlayer Mini online
Total files on DFPlayer SD: 5
DFPlayer initialized successfully
Volume set to: 25

Playing track 1...
Waiting for playback to complete...
DFPlayer state: 1
Playback wait complete

Playback complete. Entering deep sleep mode...
----------------------------------------
Configuring deep sleep mode...
Configured GPIO 33 as wakeup source (active LOW)
Press the doorbell button to wake up
Entering deep sleep now...
```

## License

This project is open source and available for educational and personal use.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

For issues and questions, please open an issue on GitHub.
