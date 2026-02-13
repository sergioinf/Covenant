# Audio Files Setup Guide

This guide explains how to prepare and organize audio files for the ESP32 Doorbell system.

## Quick Start

1. **Format SD Card**: FAT32, max 32GB
2. **Create folder**: `/mp3/` on SD card root
3. **Add audio files**: Named as `0001.mp3`, `0002.mp3`, etc.
4. **Insert SD card**: Into DFPlayer Mini module
5. **Configure code**: Set `TRACK_NUMBER` in sketch

## Detailed Instructions

### Step 1: Format the SD Card

**Requirements:**
- SD/microSD card: 4GB-32GB
- File system: FAT32 (mandatory)
- Cluster size: Default (4KB or 8KB)

**Windows:**
1. Right-click SD card in File Explorer
2. Select "Format..."
3. Choose "FAT32" as file system
4. Click "Start"

**macOS:**
1. Open Disk Utility
2. Select the SD card
3. Click "Erase"
4. Format: MS-DOS (FAT32)
5. Scheme: Master Boot Record
6. Click "Erase"

**Linux:**
```bash
sudo mkfs.vfat -F 32 /dev/sdX1
```
(Replace sdX1 with your SD card device)

### Step 2: Create Folder Structure

The DFPlayer Mini requires a specific folder structure:

```
SD Card Root
└── mp3/
    ├── 0001.mp3
    ├── 0002.mp3
    ├── 0003.mp3
    └── ...
```

**Important:**
- Folder name must be lowercase: `mp3`
- Files must be in this folder, not in root
- File names must be exactly 4 digits + extension

### Step 3: Prepare Audio Files

**Supported Formats:**
- MP3: 8kbps to 320kbps
- WAV: 16-bit PCM (limited support)

**Recommended Settings:**
- Format: MP3
- Bitrate: 128kbps (good balance of quality and size)
- Sample rate: 44.1kHz
- Channels: Mono (stereo works but speaker is mono)
- Duration: 2-5 seconds for doorbell sounds

**File Naming Convention:**
- `0001.mp3` = Track 1
- `0002.mp3` = Track 2
- `0003.mp3` = Track 3
- ...
- `0255.mp3` = Track 255 (max)

❌ **Invalid names:**
- `doorbell.mp3` (not numeric)
- `1.mp3` (not 4 digits)
- `001.mp3` (not 4 digits)
- `01.mp3` (not 4 digits)

✅ **Valid names:**
- `0001.mp3`
- `0042.mp3`
- `0100.mp3`

### Step 4: Convert Audio Files

If you have audio in other formats, convert to MP3:

**Using Audacity (Free):**
1. Download from: https://www.audacityteam.org/
2. Open your audio file
3. File → Export → Export as MP3
4. Set quality: 128kbps
5. Save with correct naming (e.g., `0001.mp3`)

**Using ffmpeg (Command line):**
```bash
# Convert WAV to MP3
ffmpeg -i doorbell.wav -codec:a libmp3lame -b:a 128k 0001.mp3

# Convert any format to optimized MP3
ffmpeg -i input.* -codec:a libmp3lame -b:a 128k -ar 44100 -ac 1 0001.mp3
```

**Using Online Converters:**
- CloudConvert: https://cloudconvert.com/
- Online Audio Converter: https://online-audio-converter.com/

### Step 5: Organize Multiple Sounds

You can have multiple doorbell sounds:

```
mp3/
├── 0001.mp3  (Default doorbell)
├── 0002.mp3  (Alternative doorbell)
├── 0003.mp3  (Notification sound)
├── 0004.mp3  (Alert sound)
└── 0005.mp3  (Custom sound)
```

To change which sound plays, modify the sketch:
```cpp
#define TRACK_NUMBER    1  // Change to 2, 3, 4, 5, etc.
```

## Finding Doorbell Sounds

### Free Sound Resources

1. **Freesound.org**
   - URL: https://freesound.org/
   - Search: "doorbell"
   - License: Creative Commons
   - Quality: High

2. **Zapsplat.com**
   - URL: https://www.zapsplat.com/
   - Category: Doorbells
   - License: Free with attribution
   - Variety: Excellent

3. **SoundBible.com**
   - URL: http://soundbible.com/
   - Search: "doorbell" or "bell"
   - License: Public Domain
   - Format: WAV/MP3

### Creating Custom Sounds

**Text-to-Speech:**
Create custom announcements using TTS:
1. Visit: https://ttsmp3.com/
2. Type your message: "Visitor at the door"
3. Download as MP3
4. Rename to `0001.mp3`

**Recording Your Own:**
1. Use smartphone voice recorder
2. Record your custom sound
3. Transfer to computer
4. Convert to MP3 (128kbps, 44.1kHz)
5. Rename appropriately

## Testing Audio Files

Before inserting SD card into DFPlayer:

1. **Play on Computer**: Verify audio plays correctly
2. **Check File Size**: Should be reasonable (50KB-500KB for doorbell)
3. **Verify Naming**: Exactly 4 digits + .mp3
4. **Check Folder**: Files in `/mp3/` folder

## Troubleshooting Audio Issues

| Problem | Solution |
|---------|----------|
| No sound plays | - Check file is in `/mp3/` folder<br>- Verify file name is correct (0001.mp3)<br>- Try different MP3 file |
| Sound is distorted | - Reduce bitrate to 128kbps<br>- Lower volume in sketch<br>- Check speaker connections |
| Sound cuts off | - Check file plays completely on computer<br>- Increase `PLAYBACK_WAIT_TIME` in code |
| Wrong track plays | - Verify `TRACK_NUMBER` matches file<br>- Check file naming (0001 = track 1) |
| DFPlayer won't read SD | - Reformat as FAT32<br>- Use smaller SD card (< 32GB)<br>- Try different SD card |

## Audio Quality Tips

**For Best Results:**
- Keep files under 5 seconds for doorbells
- Use 128kbps-192kbps MP3 encoding
- Normalize volume to -3dB to prevent clipping
- Remove silence at start/end of file
- Test volume level before final installation

**Optimizing File Size:**
- Doorbell: 128kbps is sufficient
- Voice: 96kbps mono is acceptable
- Music: 192kbps recommended

## Advanced: Multiple Folders

DFPlayer Mini supports multiple folders:

```
SD Card Root
├── 01/
│   ├── 001.mp3
│   └── 002.mp3
├── 02/
│   ├── 001.mp3
│   └── 002.mp3
└── mp3/
    └── 0001.mp3
```

To use folders, modify the sketch to use:
```cpp
dfPlayer.playFolder(1, 1);  // Folder 01, Track 001
```

## Example Audio Durations

Typical doorbell sound durations:
- Classic ding-dong: 2-3 seconds
- Chime: 3-5 seconds
- Melody: 5-10 seconds
- Custom announcement: 3-8 seconds

Adjust `PLAYBACK_WAIT_TIME` in sketch accordingly:
```cpp
#define PLAYBACK_WAIT_TIME  3000  // 3 seconds for short sounds
#define PLAYBACK_WAIT_TIME  8000  // 8 seconds for longer sounds
```

## Quick Reference

✅ **Good MP3 Settings:**
- Bitrate: 128kbps
- Sample Rate: 44.1kHz
- Channels: Mono or Stereo
- Duration: 2-5 seconds

✅ **Proper File Structure:**
```
/mp3/0001.mp3
/mp3/0002.mp3
```

❌ **Common Mistakes:**
- Files in root instead of /mp3/ folder
- Wrong file names (not 4 digits)
- NTFS or exFAT instead of FAT32
- Corrupted MP3 files
- SD card > 32GB
