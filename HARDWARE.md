# Hardware Setup Guide

## Complete Parts List

### Required Components
1. ESP32 Development Board (e.g., ESP32-DevKitC, ESP32-WROOM-32)
2. DFPlayer Mini MP3 Player Module
3. Micro SD Card (4GB-32GB, FAT32 formatted)
4. Push Button (momentary, normally open)
5. Speaker (3W, 4-8Ω impedance)
6. Micro SD Card adapter (if ESP32 board doesn't have built-in slot)
7. Resistor 1kΩ (for DFPlayer RX line - optional but recommended)

### Optional Components
- Breadboard and jumper wires (for prototyping)
- Enclosure/case for final installation
- Capacitor 100µF (for power stabilization)
- Capacitor 0.1µF (for button debouncing)
- LED + 220Ω resistor (for status indication)

## Detailed Wiring Instructions

### Step 1: DFPlayer Mini Module

The DFPlayer Mini is the audio playback module that reads MP3 files from its own SD card.

```
DFPlayer Pin    ESP32 Pin       Notes
------------    ---------       -----
VCC             5V              Power supply
GND             GND             Ground
TX              GPIO 16         Serial transmit (connects to ESP32 RX)
RX              GPIO 17         Serial receive (connects to ESP32 TX)
                                (use 1kΩ resistor in series recommended)
SPK1            Speaker +       Speaker positive terminal
SPK2            Speaker -       Speaker negative terminal
BUSY            (not used)      Can be used to detect playback status
IO1             (not used)      Advanced features
IO2             (not used)      Advanced features
ADKEY1          (not used)      For button control
ADKEY2          (not used)      For button control
```

**Important Notes:**
- DFPlayer Mini RX pin is 3.3V tolerant, but using a 1kΩ resistor is safer
- Insert SD card into DFPlayer Mini's SD card slot
- The speaker connects directly to SPK1 and SPK2 (no amplifier needed)

### Step 2: SD Card Reader (on ESP32)

If using external SD card reader module for ESP32:

```
SD Card Pin     ESP32 Pin       SPI Function
-----------     ---------       ------------
CS              GPIO 5          Chip Select
MOSI            GPIO 23         Master Out Slave In
MISO            GPIO 19         Master In Slave Out
SCK             GPIO 18         Serial Clock
VCC             3.3V            Power (NOT 5V!)
GND             GND             Ground
```

**Important:** Use 3.3V, not 5V, for SD card modules!

### Step 3: Doorbell Button

```
Button Pin      ESP32 Pin       Notes
----------      ---------       -----
Pin 1           GPIO 33         Must be RTC-capable GPIO for deep sleep wake
Pin 2           GND             Ground connection
```

**Why GPIO 33?**
- GPIO 33 is an RTC GPIO, which means it can wake the ESP32 from deep sleep
- Other RTC GPIOs: 0, 2, 4, 12, 13, 14, 15, 25, 26, 27, 32, 33, 34, 35, 36, 39

**Button Debouncing (Optional):**
Add a 0.1µF ceramic capacitor between GPIO 33 and GND to reduce noise.

### Step 4: Power Supply

```
Power Source    ESP32           Notes
------------    -----           -----
5V              5V/VIN          Main power input
GND             GND             Ground
```

**Power Requirements:**
- ESP32 active: ~80-160mA
- DFPlayer Mini: ~20-200mA (depends on volume)
- Total: 1A power supply recommended
- USB power is sufficient for testing

## Physical Layout Recommendations

### Breadboard Prototype Layout
```
┌─────────────────────────────────────────┐
│                                         │
│  [Button]  [ESP32]  [DFPlayer]         │
│     |         |         |               │
│     └─GPIO33──┤         |               │
│               |         |               │
│            5V─┴────────VCC              │
│           GND─┬────────GND              │
│               |                         │
│         GPIO16────────TX                │
│         GPIO17────────RX                │
│                       |                 │
│                   SPK1/SPK2             │
│                       |                 │
│                   [Speaker]             │
│                                         │
└─────────────────────────────────────────┘
```

### PCB Installation Tips

For permanent installation:
1. Use a PCB prototype board or design a custom PCB
2. Add mounting holes for secure installation
3. Include a 100µF capacitor near the ESP32 power pins
4. Route speaker wires carefully to avoid interference
5. Use terminal blocks for easy speaker and button connections

## SD Card Preparation

### DFPlayer Mini SD Card
1. Format as FAT32 (max 32GB)
2. Create folder: `/mp3/`
3. Add audio files: `0001.mp3`, `0002.mp3`, etc.
4. File naming is strict: 4 digits + .mp3 extension
5. Files should be in the `mp3` folder, not root

**Supported Audio Formats:**
- MP3: 8kbps-320kbps
- WAV: 16-bit
- Sample rates: 8kHz-48kHz

**Recommended Settings:**
- Format: MP3
- Bitrate: 128kbps
- Sample rate: 44.1kHz
- Mono or stereo (stereo is fine, speaker is mono)

### ESP32 SD Card (Optional)
The current implementation uses SD card primarily through DFPlayer Mini.
The ESP32's SD card functionality is initialized for future expansion.

## Testing Procedure

### Step 1: Hardware Check
1. Verify all connections match the wiring diagram
2. Check for short circuits (especially power lines)
3. Ensure SD card is properly formatted and has audio files

### Step 2: Upload Code
1. Connect ESP32 to computer via USB
2. Upload the Covenant.ino sketch
3. Open Serial Monitor (115200 baud)

### Step 3: Initial Test
1. Press the ESP32 reset button
2. Watch Serial Monitor for startup messages
3. You should hear the doorbell sound play
4. System will enter deep sleep

### Step 4: Wake Test
1. Press the doorbell button
2. ESP32 should wake up
3. Sound should play again
4. Check Serial Monitor for wake reason

### Expected Serial Output
```
========================================
ESP32 Doorbell System
========================================
Boot count: 1
Wakeup reason: Not deep sleep wakeup (0)

Initializing SD card...
SD card initialized successfully

Initializing DFPlayer Mini...
DFPlayer Mini online
Total files on DFPlayer SD: 1

Playing track 1...
Playback complete. Entering deep sleep mode...
```

## Common Issues and Solutions

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| No sound | Speaker not connected | Check SPK1/SPK2 connections |
| | Volume too low | Increase VOLUME in code |
| | No audio files | Check SD card has mp3/0001.mp3 |
| DFPlayer init fails | Wrong RX/TX | Swap GPIO 16 and 17 |
| | No power | Check VCC/GND connections |
| | Bad SD card | Try different SD card |
| Won't wake from sleep | Wrong GPIO | Use RTC GPIO (33 recommended) |
| | Button not connected | Check button to GND |
| SD card error | Wrong voltage | Use 3.3V, not 5V |
| | Not FAT32 | Reformat as FAT32 |
| Random resets | Power issues | Add 100µF capacitor |

## Safety Notes

⚠️ **Important Safety Information:**
- Never connect SD card reader to 5V (use 3.3V only)
- Keep speaker wires away from power supply wires
- Do not exceed 30 volume level on DFPlayer (can damage speaker)
- Use appropriate wire gauge for power connections
- Ensure proper grounding to prevent static damage
- Do not hot-swap SD cards while powered on

## Customization Ideas

Once basic functionality is working, consider these enhancements:
- Multiple doorbell sounds (random selection)
- Volume adjustment based on time of day
- WiFi notification when doorbell is pressed
- Battery operation with solar charging
- MQTT integration for smart home
- Camera trigger on doorbell press
- Multiple buttons for different sounds
