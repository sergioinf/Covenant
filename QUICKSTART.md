# Quick Start Guide

Get your ESP32 Doorbell up and running in 5 minutes!

## What You Need

Hardware:
- [ ] ESP32 board
- [ ] DFPlayer Mini module
- [ ] Micro SD card (formatted FAT32)
- [ ] Push button
- [ ] Speaker (3W)
- [ ] USB cable
- [ ] Jumper wires

Software:
- [ ] Arduino IDE installed
- [ ] USB driver for ESP32 (usually automatic)

## Step-by-Step Setup

### 1. Prepare SD Card (2 minutes)

1. Format SD card as FAT32
2. Create folder named `mp3` on SD card
3. Add doorbell MP3 file named `0001.mp3` in the `mp3` folder
4. Eject SD card safely

**Download free doorbell sounds:**
- https://freesound.org/search/?q=doorbell
- Or use any short MP3 file (2-5 seconds)

### 2. Install Software (3 minutes)

1. Open Arduino IDE
2. Install ESP32 board support:
   - File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager
   - Search "esp32" → Install

3. Install DFPlayer library:
   - Sketch → Include Library → Manage Libraries
   - Search "DFRobotDFPlayerMini" → Install

### 3. Wire Hardware (5 minutes)

**Minimal wiring (no SD card reader on ESP32):**

```
DFPlayer → ESP32       Button → ESP32
--------   -----       ------   -----
TX      →  GPIO 16    Pin 1 →  GPIO 33
RX      →  GPIO 17    Pin 2 →  GND
VCC     →  5V
GND     →  GND
SPK1    →  Speaker +
SPK2    →  Speaker -
```

**Insert SD card into DFPlayer Mini** (not into ESP32)

### 4. Upload Code (2 minutes)

1. Connect ESP32 to computer via USB
2. Open `Covenant/Covenant.ino` in Arduino IDE
3. Select board: Tools → Board → ESP32 Arduino → ESP32 Dev Module
4. Select port: Tools → Port → (your COM port)
5. Click Upload ⬆️

### 5. Test (1 minute)

1. Open Serial Monitor (115200 baud)
2. Press ESP32 reset button
3. You should see:
   ```
   ========================================
   ESP32 Doorbell System
   ========================================
   Boot count: 1
   ...
   Playing track 1...
   ```
4. Audio should play!
5. Press doorbell button to wake and play again

## Troubleshooting

### No Sound?

1. Check speaker connections (SPK1/SPK2)
2. Verify SD card has `mp3/0001.mp3` file
3. Increase volume in code: `#define VOLUME 30`

### DFPlayer Error?

1. Swap GPIO 16 and 17
2. Check 5V power to DFPlayer
3. Re-insert SD card into DFPlayer

### Upload Failed?

1. Try different USB cable
2. Press and hold BOOT button during upload
3. Check correct COM port selected

### Won't Wake from Sleep?

1. Verify button connected to GPIO 33 and GND
2. Try pressing reset instead (for testing)

## Next Steps

Once basic operation works:

1. **Adjust volume**: Change `VOLUME` value (0-30)
2. **Change sound**: Replace `0001.mp3` with your file
3. **Extend playback**: Increase `PLAYBACK_DELAY` for longer sounds
4. **Add multiple sounds**: Add `0002.mp3`, `0003.mp3`, etc.
5. **Disable sleep**: Set `ENABLE_DEEP_SLEEP false` for debugging

## Full Documentation

- [README.md](README.md) - Complete overview
- [HARDWARE.md](HARDWARE.md) - Detailed wiring diagrams
- [LIBRARIES.md](LIBRARIES.md) - Library installation
- [AUDIO_SETUP.md](AUDIO_SETUP.md) - Audio file preparation

## Need Help?

Common issues and solutions: [README.md#troubleshooting](README.md#troubleshooting)

GitHub Issues: https://github.com/sergioinf/Covenant/issues

## Success Checklist

After following this guide, you should have:

- [x] Audio plays when ESP32 starts
- [x] ESP32 enters deep sleep after playback
- [x] Button press wakes ESP32 and plays audio again
- [x] Serial monitor shows system status

**Congratulations! Your ESP32 Doorbell is working! 🎉**
