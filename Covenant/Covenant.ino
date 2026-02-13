/**
 * ESP32 Doorbell with SD Card and Audio Playback
 * 
 * This sketch implements a doorbell system using ESP32 that:
 * - Reads audio files from an SD card
 * - Plays sound through a DFPlayer Mini module
 * - Enters deep sleep mode to conserve power
 * - Wakes up when the doorbell button is pressed
 * 
 * Hardware Required:
 * - ESP32 Development Board
 * - DFPlayer Mini MP3 Player Module
 * - SD Card (FAT32 formatted) with MP3 files
 * - Push button for doorbell
 * - Speaker (3W recommended)
 * 
 * Connections:
 * DFPlayer Mini:
 *   - TX -> GPIO 16 (RX2)
 *   - RX -> GPIO 17 (TX2)
 *   - VCC -> 5V
 *   - GND -> GND
 *   - SPK1/SPK2 -> Speaker
 * 
 * Button:
 *   - One side -> GPIO 33 (BUTTON_PIN)
 *   - Other side -> GND
 *   - Internal pullup resistor enabled
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

// Pin Definitions
#define BUTTON_PIN          33    // GPIO pin for doorbell button (RTC GPIO)
#define SD_CS_PIN           5     // SD card chip select pin
#define DFPLAYER_RX         16    // Connect to DFPlayer TX
#define DFPLAYER_TX         17    // Connect to DFPlayer RX

// Configuration
#define VOLUME              25    // Volume level (0-30)
#define TRACK_NUMBER        1     // Track number to play from SD card
#define PLAYBACK_DELAY      5000  // Delay after playback before sleep (ms)
#define DEBOUNCE_DELAY      50    // Button debounce delay (ms)

// Global Objects
HardwareSerial dfPlayerSerial(2);  // Use UART2
DFRobotDFPlayerMini dfPlayer;

// Boot count for debugging
RTC_DATA_ATTR int bootCount = 0;

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(1000);
  
  // Increment boot count
  bootCount++;
  Serial.println("\n========================================");
  Serial.println("ESP32 Doorbell System");
  Serial.println("========================================");
  Serial.printf("Boot count: %d\n", bootCount);
  
  // Print wakeup reason
  printWakeupReason();
  
  // Initialize button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Small delay to ensure hardware is ready
  delay(100);
  
  // Initialize SD card
  Serial.println("\nInitializing SD card...");
  if (!initSDCard()) {
    Serial.println("ERROR: SD card initialization failed!");
    Serial.println("System will enter sleep mode.");
    enterDeepSleep();
  }
  Serial.println("SD card initialized successfully");
  
  // Initialize DFPlayer Mini
  Serial.println("\nInitializing DFPlayer Mini...");
  if (!initDFPlayer()) {
    Serial.println("ERROR: DFPlayer initialization failed!");
    Serial.println("System will enter sleep mode.");
    SD.end();
    enterDeepSleep();
  }
  Serial.println("DFPlayer initialized successfully");
  
  // Configure DFPlayer settings
  dfPlayer.volume(VOLUME);
  Serial.printf("Volume set to: %d\n", VOLUME);
  
  // Play the doorbell sound
  playDoorbellSound();
  
  // Wait for playback to complete
  waitForPlaybackComplete();
  
  // Additional delay before sleep
  delay(PLAYBACK_DELAY);
  
  // Cleanup and enter deep sleep
  Serial.println("\nPlayback complete. Entering deep sleep mode...");
  SD.end();
  enterDeepSleep();
}

void loop() {
  // This should never be reached as we enter deep sleep in setup()
  // But included for safety
  delay(1000);
}

/**
 * Initialize SD card
 */
bool initSDCard() {
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card mount failed!");
    return false;
  }
  
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached!");
    return false;
  }
  
  // Print SD card information
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  
  return true;
}

/**
 * Initialize DFPlayer Mini
 */
bool initDFPlayer() {
  // Initialize serial communication with DFPlayer
  dfPlayerSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  
  delay(500); // Give DFPlayer time to initialize
  
  if (!dfPlayer.begin(dfPlayerSerial)) {
    Serial.println("Unable to begin DFPlayer:");
    Serial.println("1. Please check the connection!");
    Serial.println("2. Please insert the SD card into DFPlayer!");
    return false;
  }
  
  Serial.println("DFPlayer Mini online");
  
  // Check number of files on SD card
  int fileCount = dfPlayer.readFileCounts();
  Serial.printf("Total files on DFPlayer SD: %d\n", fileCount);
  
  if (fileCount == 0) {
    Serial.println("WARNING: No files found on DFPlayer SD card!");
  }
  
  return true;
}

/**
 * Play the doorbell sound
 */
void playDoorbellSound() {
  Serial.printf("\nPlaying track %d...\n", TRACK_NUMBER);
  dfPlayer.play(TRACK_NUMBER);
  delay(100); // Small delay to ensure command is processed
}

/**
 * Wait for playback to complete
 * Note: DFPlayer Mini doesn't have a reliable "playback finished" detection
 * This function waits for a reasonable time based on typical doorbell sounds
 */
void waitForPlaybackComplete() {
  Serial.println("Waiting for playback to complete...");
  
  // Query the current state (this helps ensure playback started)
  delay(500);
  int state = dfPlayer.readState();
  Serial.printf("DFPlayer state: %d\n", state);
  
  // Wait for typical doorbell sound duration (adjust as needed)
  // Most doorbell sounds are 2-5 seconds
  delay(3000);
  
  Serial.println("Playback wait complete");
}

/**
 * Print the wakeup reason
 */
void printWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  Serial.print("Wakeup reason: ");
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("External signal (RTC_IO)");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("External signal (RTC_CNTL)");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Timer");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
      Serial.println("Touchpad");
      break;
    case ESP_SLEEP_WAKEUP_ULP:
      Serial.println("ULP program");
      break;
    default:
      Serial.printf("Not deep sleep wakeup (%d)\n", wakeup_reason);
      break;
  }
}

/**
 * Configure and enter deep sleep mode
 */
void enterDeepSleep() {
  Serial.println("\n----------------------------------------");
  Serial.println("Configuring deep sleep mode...");
  
  // Configure wakeup on button press (EXT0)
  // BUTTON_PIN connected to GND when pressed, so wake on LOW
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, 0);
  
  Serial.printf("Configured GPIO %d as wakeup source (active LOW)\n", BUTTON_PIN);
  Serial.println("Press the doorbell button to wake up");
  Serial.println("Entering deep sleep now...");
  Serial.flush();
  
  delay(100);
  
  // Enter deep sleep
  esp_deep_sleep_start();
  
  // This line should never be reached
  Serial.println("ERROR: Failed to enter deep sleep!");
}
