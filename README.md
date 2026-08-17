# HT1621 LCD Reverse Engineering

> Reverse engineering an undocumented segmented LCD from a Thomson RCD-203U radio — and making it work with an ESP32.

##  Project Overview

This project demonstrates the complete reverse engineering and reuse of the LCD module from a **Thomson RCD-203U radio**. The original LCD was driven by two ICs (ET1621 and SC3610). The goal was to:

- Understand the undocumented hardware architecture
- Map the arbitrary, non-contiguous memory layout of the HT1621
- Adapt an existing HT1621 driver library to work with this specific display
- Successfully control the original LCD using an **ESP32**

##  What Was Built

| Component | Description |
|-----------|-------------|
| **HT1621 Driver** | Custom-modified C++ library for the ET1621/HT1621 |
| **Memory Mapper** | Mapped the LCD's undocumented 32×4-bit RAM to physical segments |
| **ESP32 Integration** | Full control of 7-segment digits, icons, and custom characters |
| **Hardware Reverse Engineering** | Identified COM/SEG lines, protocol timing, and pinout |

## Key Technical Challenges & Solutions

### 1. Undocumented Memory Layout
The HT1621's 32×4-bit RAM did **not** follow a logical digit-by-digit organization. Each RAM byte controlled segments belonging to multiple digits — e.g., one byte controlled Segment A of Digit 1, Segment F of Digit 2, and an icon segment of Digit 3.

**Solution:** Systematically mapped every address-value combination using the `wrOne()` function, creating a custom lookup table.

### 2. Library Incompatibility
The existing HT1621 library assumed one RAM location = one digit. This assumption was invalid.

**Solution:** Rewrote the library architecture:

```
Original:  Character → Segment Pattern → Buffer → RAM → LCD
New:       Character → Lookup Table → Custom RAM Mapper → HT1621 RAM → LCD
```

**Removed:** `print()`, `printCelsius()`, `setDecimalSeparator()`, `charToSegBits()`, etc.

**Added:** `setSegmentBinary()`, `setSegmentInDigit()`, `setDigit()`, `printnum()`, `printicon()`, and more.

### 3. Hardware Unknowns
- No LCD glass datasheet
- Unknown pin behavior and protocol timing
- Missing reflective diffuser (contrast issue)

**Solution:** Used datasheets, logic analysis, and systematic testing to reverse-engineer everything.

## 📁 Repository Structure

```
├── src/
│   └── HT1621.cpp / HT1621.h      # Modified library
├── examples/
│   └── esp32_demo.ino             # ESP32 demo sketch
├── docs/
│   └── LCD_reverse_engineering.pdf  # Full documentation (Canva PDF)
├── README.md
└── LICENSE
```

##  Getting Started

### Hardware
- ESP32 (any dev board)
- Thomson RCD-203U LCD module (or any HT1621-based LCD)
- Jumper wires

### Wiring
| ESP32 Pin | LCD Module |
|-----------|------------|
| GPIO 5    | CS         |
| GPIO 18   | WR         |
| GPIO 23   | DATA       |
| 3.3V      | VDD        |
| GND       | GND        |

### Software
1. Install the Arduino IDE or PlatformIO
2. Clone this repository
3. Open `examples/esp32_demo.ino`
4. Upload to your ESP32

### Example Usage

```cpp
#include <HT1621.h>

#define LCD_CS_PIN 5
#define LCD_WR_PIN 18
#define LCD_DATA_PIN 23

HT1621 lcd;

void setup() {
    lcd.begin(LCD_CS_PIN, LCD_WR_PIN, LCD_DATA_PIN);
    lcd.clear();
}

void loop() {
    lcd.printnum(1234);        // Display a number
    delay(2000);
    lcd.print("HELLO");        // Display text
    delay(2000);
    lcd.printicon("MP3");      // Display an icon
    delay(2000);
}
```

## 📚 Full Documentation

**📄 [Project Documentation (PDF)](LCD%20reverse%20engineering.pdf)**

> The PDF contains detailed reverse engineering notes, memory maps, circuit diagrams, photos, and step-by-step methodology.

*(The PDF was created in Canva and includes embedded photos and diagrams.)*

## 🎥 Project Demo

*(https://canva.link/hcvckzcn1l8z89q)*

##  Technologies Used

- C++ / Embedded C++
- ESP32
- HT1621 / ET1621
- Bit-banging / SPI
- LCD Multiplexing (COM/SEG)
- Hardware Reverse Engineering
- Memory Mapping

## 📖 What I Learned

- LCD multiplexing and COM/SEG operation
- HT1621 communication protocol
- Bit-level programming and memory mapping
- Library architecture and modification
- Hardware debugging without documentation

## 📬 Contact

**Ahmed Bouhlel** — movingbreadd@gmail.com

---

*Built as part of an engineering portfolio project.*
