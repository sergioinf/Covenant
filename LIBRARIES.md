# Required Arduino Libraries

This document lists all the libraries needed to compile the ESP32 Doorbell project in Arduino IDE.

## Installation Instructions

1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries...**
3. Search for each library below and click **Install**

## Required Libraries

### 1. DFRobotDFPlayerMini
- **Author**: DFRobot
- **Version**: 1.0.6 or newer
- **Purpose**: Communication with DFPlayer Mini MP3 module
- **Installation**: Library Manager → Search "DFRobotDFPlayerMini"
- **Repository**: https://github.com/DFRobot/DFRobotDFPlayerMini

### 2. ESP32 Board Support
- **Package**: esp32 by Espressif Systems
- **Version**: 2.0.0 or newer (tested with 2.0.14)
- **Purpose**: Core ESP32 functionality, SD, SPI, etc.
- **Installation**: 
  1. File → Preferences → Additional Board Manager URLs
  2. Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
  3. Tools → Board → Boards Manager → Search "esp32" → Install

## Built-in Libraries (No Installation Required)

These libraries are included with the ESP32 board package:

- **SD** - SD card support
- **SPI** - SPI communication protocol
- **HardwareSerial** - Serial communication
- **Arduino** - Core Arduino functions

## Library Versions Tested

| Library | Version | Status |
|---------|---------|--------|
| DFRobotDFPlayerMini | 1.0.6 | ✅ Tested |
| esp32 | 2.0.14 | ✅ Tested |
| esp32 | 2.0.11 | ✅ Compatible |
| esp32 | 3.0.0 | ⚠️ Not tested |

## Troubleshooting

### "Library not found" errors

If you get compilation errors about missing libraries:

1. Verify all libraries are installed
2. Restart Arduino IDE
3. Check library versions match requirements
4. Try reinstalling the library

### DFRobotDFPlayerMini Issues

If the DFRobotDFPlayerMini library is not found in Library Manager:

**Manual Installation:**
1. Download from: https://github.com/DFRobot/DFRobotDFPlayerMini/archive/master.zip
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

### ESP32 Board Package Issues

If ESP32 boards don't appear in Tools → Board menu:

1. Verify the URL was added correctly in Preferences
2. Close and reopen Boards Manager
3. Search for "esp32" (lowercase)
4. Make sure to install "esp32 by Espressif Systems"
5. Restart Arduino IDE after installation

## Alternative Installation Method: Manual

For advanced users who prefer manual installation:

### DFRobotDFPlayerMini
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/DFRobot/DFRobotDFPlayerMini.git
```

### Library Locations

**Windows**: `C:\Users\<username>\Documents\Arduino\libraries\`
**macOS**: `~/Documents/Arduino/libraries/`
**Linux**: `~/Arduino/libraries/`

## Verifying Installation

To verify libraries are installed correctly:

1. Open Arduino IDE
2. Go to Sketch → Include Library
3. Check that "DFRobotDFPlayerMini" appears in the list
4. Go to Tools → Board → esp32
5. Verify ESP32 boards are listed

## Additional Resources

- [Arduino Library Installation Guide](https://www.arduino.cc/en/Guide/Libraries)
- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [DFPlayer Mini Wiki](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299)

## Support

If you encounter library-related issues:
1. Check library versions match requirements
2. Ensure ESP32 board package is properly installed
3. Try with a fresh Arduino IDE installation
4. Open an issue on GitHub with error details
