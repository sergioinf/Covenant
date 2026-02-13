# Troubleshooting Guide

This guide helps you diagnose and fix common issues with the ESP32 Doorbell system.

## Issue Categories

- [Compilation Errors](#compilation-errors)
- [Upload Issues](#upload-issues)
- [DFPlayer Mini Problems](#dfplayer-mini-problems)
- [SD Card Issues](#sd-card-issues)
- [Audio Playback Problems](#audio-playback-problems)
- [Deep Sleep Issues](#deep-sleep-issues)
- [Power Problems](#power-problems)
- [Hardware Connection Issues](#hardware-connection-issues)

---

## Compilation Errors

### Error: "DFRobotDFPlayerMini.h: No such file or directory"

**Cause**: DFPlayer library not installed

**Solution**:
1. Arduino IDE → Sketch → Include Library → Manage Libraries
2. Search "DFRobotDFPlayerMini"
3. Click Install
4. Restart Arduino IDE

### Error: "SD.h: No such file or directory"

**Cause**: ESP32 board package not installed

**Solution**:
1. Tools → Board → Boards Manager
2. Search "esp32"
3. Install "esp32 by Espressif Systems"
4. Restart Arduino IDE

### Error: "esp_sleep_enable_ext0_wakeup was not declared"

**Cause**: Wrong board selected

**Solution**:
1. Tools → Board → esp32 → ESP32 Dev Module (or your specific board)
2. Recompile

---

## Upload Issues

### Error: "Serial port not found"

**Cause**: Driver not installed or wrong port

**Solution**:
1. Install CP210x or CH340 driver for your ESP32
2. Check Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac)
3. Tools → Port → Select correct port (COM3, /dev/ttyUSB0, etc.)

### Error: "Failed to connect to ESP32"

**Cause**: ESP32 not in upload mode

**Solution**:
1. Hold BOOT button on ESP32
2. Click Upload in Arduino IDE
3. Release BOOT button when "Connecting..." appears
4. Or try lower upload speed: Tools → Upload Speed → 115200

### Upload Stuck at "Connecting..."

**Cause**: ESP32 in wrong state

**Solution**:
1. Press and hold BOOT button
2. Press and release RESET button
3. Release BOOT button
4. Try upload again

---

## DFPlayer Mini Problems

### Error: "Unable to begin DFPlayer"

**Possible Causes & Solutions**:

1. **Wiring issue**:
   - Verify TX/RX connections
   - Try swapping GPIO 16 and 17
   - Check 5V and GND connections

2. **No SD card in DFPlayer**:
   - Ensure SD card is inserted into DFPlayer Mini
   - Check card is properly seated

3. **Faulty DFPlayer module**:
   - Test with multimeter (5V on VCC pin)
   - Try different DFPlayer Mini module

4. **Serial communication issue**:
   - Add delay before initialization:
     ```cpp
     delay(1000);
     dfPlayerSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
     delay(500);
     ```

### DFPlayer Initialization Successful But No Sound

**Possible Causes & Solutions**:

1. **Speaker not connected**:
   - Check SPK1 and SPK2 connections
   - Verify speaker is working (test with another device)

2. **Volume too low**:
   ```cpp
   #define VOLUME 30  // Maximum volume
   ```

3. **No audio files**:
   - Check SD card has `mp3/0001.mp3`
   - Verify file is valid MP3 format

4. **Wrong track number**:
   ```cpp
   #define TRACK_NUMBER 1  // Must match file number
   ```

### "Total files on DFPlayer SD: 0"

**Cause**: SD card not readable or no files

**Solution**:
1. Ensure `mp3` folder exists on SD card
2. Files must be named `0001.mp3`, `0002.mp3`, etc.
3. Try reformatting SD card as FAT32
4. Test SD card in computer first

---

## SD Card Issues

### "SD card initialization failed"

**Possible Causes & Solutions**:

1. **No SD card inserted** (in ESP32 slot):
   - Note: This is optional - audio comes from DFPlayer's SD card
   - Can comment out SD initialization if not needed

2. **Wrong CS pin**:
   - Verify CS is connected to GPIO 5
   - Check wiring

3. **Wrong voltage**:
   - **CRITICAL**: SD card needs 3.3V, NOT 5V!
   - Double-check power connections

4. **Corrupted SD card**:
   - Reformat as FAT32
   - Try different SD card
   - Check card with computer first

### "No SD card attached"

**Cause**: Card detection failed

**Solution**:
1. Re-insert SD card firmly
2. Clean SD card contacts
3. Try different SD card
4. Check wiring continuity with multimeter

---

## Audio Playback Problems

### Audio Plays But Sounds Distorted

**Possible Causes & Solutions**:

1. **Volume too high**:
   ```cpp
   #define VOLUME 15  // Reduce volume
   ```

2. **Poor quality MP3**:
   - Re-encode at 128kbps
   - Use standard sample rate (44.1kHz)

3. **Power supply insufficient**:
   - Use 1A power supply minimum
   - Add 100µF capacitor near ESP32

4. **Speaker impedance mismatch**:
   - Use 4-8Ω speaker
   - Check speaker wattage (3W recommended)

### Audio Cuts Off Early

**Cause**: Playback delay too short

**Solution**:
```cpp
#define PLAYBACK_WAIT_TIME 5000  // Increase to match file length
#define PLAYBACK_DELAY 8000       // Increase if still cutting off
```

### Wrong Track Plays

**Cause**: Mismatch between code and files

**Solution**:
1. Verify file naming: Track 1 = `0001.mp3`
2. Check `TRACK_NUMBER` in code matches desired file
3. Ensure files are in `mp3` folder

---

## Deep Sleep Issues

### ESP32 Won't Enter Deep Sleep

**Possible Causes & Solutions**:

1. **Serial monitor open**:
   - Close Serial Monitor before deep sleep

2. **Code issue**:
   - Check `ENABLE_DEEP_SLEEP` is true
   - Verify `enterDeepSleep()` is called

3. **Debug mode**:
   - Set `DEBUG_ENABLED` to false for production

### ESP32 Won't Wake from Deep Sleep

**Possible Causes & Solutions**:

1. **Wrong GPIO pin**:
   - Button must be on RTC GPIO (e.g., GPIO 33)
   - Valid RTC GPIOs: 0, 2, 4, 12-15, 25-27, 32-36, 39

2. **Button not connected to GND**:
   - Verify one button pin → GPIO 33
   - Other button pin → GND
   - Internal pullup is enabled in code

3. **Wakeup level wrong**:
   ```cpp
   esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, 0);
   // 0 = wake on LOW (button to GND) ✓
   // 1 = wake on HIGH (button to VCC)
   ```

4. **Button debounce issues**:
   - Add 0.1µF capacitor across button
   - Hold button for 1-2 seconds

### ESP32 Wakes Up Randomly

**Cause**: Electrical noise on wake pin

**Solution**:
1. Add 0.1µF capacitor between GPIO 33 and GND
2. Keep wires short
3. Use shielded cable for button if long distance
4. Add 10kΩ pull-up resistor (code already enables internal)

---

## Power Problems

### ESP32 Resets Randomly

**Possible Causes & Solutions**:

1. **Insufficient power supply**:
   - Use at least 1A power supply
   - USB ports may not provide enough current

2. **Power supply noise**:
   - Add 100µF electrolytic capacitor near ESP32
   - Add 0.1µF ceramic capacitor near ESP32

3. **DFPlayer causing voltage drops**:
   - DFPlayer can draw 200mA at high volume
   - Use separate 5V power supply for DFPlayer
   - Or add bulk capacitor (470µF-1000µF)

### High Power Consumption in Deep Sleep

**Expected**: ~10µA in deep sleep

**If higher**:

1. **Peripherals not disabled**:
   - Code already calls `SD.end()`
   - DFPlayer stays powered (expected)

2. **LED indicators**:
   - Disconnect or desolder power LEDs on modules

3. **Voltage regulator inefficiency**:
   - Use ESP32 module with efficient LDO
   - Or use switching regulator

---

## Hardware Connection Issues

### Intermittent Operation

**Possible Causes & Solutions**:

1. **Loose connections**:
   - Check all jumper wire connections
   - Solder connections for permanent setup
   - Use breadboard with good contact

2. **Bad jumper wires**:
   - Test continuity with multimeter
   - Replace suspect wires

3. **Cold solder joints** (if soldered):
   - Reheat and apply fresh solder
   - Check for cracks

### No Serial Output

**Possible Causes & Solutions**:

1. **Wrong baud rate**:
   - Set Serial Monitor to 115200 baud

2. **Wrong port selected**:
   - Tools → Port → Select correct port

3. **USB cable is power-only**:
   - Use data-capable USB cable

4. **Driver issue**:
   - Reinstall USB driver (CP210x or CH340)

---

## Advanced Debugging

### Enable Verbose Logging

Add to code:
```cpp
#define VERBOSE_LOGGING true

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);  // Enable ESP32 debug output
  esp_log_level_set("*", ESP_LOG_VERBOSE);
}
```

### Test Individual Components

**Test DFPlayer Only**:
```cpp
void setup() {
  Serial.begin(115200);
  dfPlayerSerial.begin(9600, SERIAL_8N1, 16, 17);
  delay(1000);
  
  if (dfPlayer.begin(dfPlayerSerial)) {
    Serial.println("DFPlayer OK");
    dfPlayer.volume(25);
    dfPlayer.play(1);
  }
}

void loop() {
  delay(10000);
}
```

**Test Button Only**:
```cpp
void setup() {
  Serial.begin(115200);
  pinMode(33, INPUT_PULLUP);
}

void loop() {
  Serial.println(digitalRead(33));
  delay(100);
}
```

**Test Deep Sleep Only**:
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Sleeping in 5 seconds...");
  delay(5000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  esp_deep_sleep_start();
}
```

### Multimeter Testing

1. **Power rails**:
   - ESP32 5V pin: Should read ~5V
   - ESP32 3.3V pin: Should read ~3.3V
   - DFPlayer VCC: Should read ~5V

2. **Continuity**:
   - Button to GND: 0Ω when pressed, infinite when released
   - All GND connections: 0Ω to each other

3. **GPIO states**:
   - Button pin (GPIO 33): ~3.3V when not pressed, ~0V when pressed

---

## Getting Help

If you're still stuck:

1. **Check Serial Monitor output** - Most issues show error messages
2. **Test components individually** - Isolate the problem
3. **Try example code** - Verify hardware works
4. **Open GitHub Issue** - Include:
   - Serial Monitor output
   - Wiring photo
   - Code modifications made
   - Steps already tried

## Common Error Messages Decoded

| Error Message | Meaning | Solution |
|--------------|---------|----------|
| "SD card mount failed" | SD card not detected | Check wiring, reformat card |
| "Unable to begin DFPlayer" | Can't communicate with DFPlayer | Check TX/RX connections |
| "No files found on DFPlayer SD" | SD card empty or wrong structure | Add files to `mp3` folder |
| "Failed to enter deep sleep" | Deep sleep command failed | Check GPIO is RTC-capable |

---

**Still having issues? Check our GitHub Issues page or open a new issue with detailed information about your problem.**
