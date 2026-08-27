# Display and Signal Acquisition Module

## Purpose

This module runs on an Arduino with a MCUFRIEND TFT shield (ILI9341-compatible, 320x240). It performs:

1. **Analog signal acquisition** via `analogRead()` on pin A5 (10-bit ADC)
2. **Real-time waveform display** on the TFT using line-drawing
3. **User interface** with HOLD indicator and delay (time base) control

## Controls

| Pin | Function | Behavior |
|-----|----------|----------|
| A5  | ADC Input | Analog signal acquisition |
| D10 | HOLD     | Toggle waveform freeze |
| D11 | SET      | Reset delay to 0 |
| D12 | UP       | Increase sample delay by 50 us |
| D13 | DOWN     | Decrease sample delay by 50 us |

## Display Layout

- **Top bar** (0-20 px): Shows current delay value and HOLD status
- **Grid area** (25-240 px): Oscilloscope grid with dotted lines at 40 px intervals
- **Waveform**: Green trace on black background, 320 samples per sweep

## Build

- **Platform**: Arduino (AVR or compatible)
- **Board**: Arduino Uno/Mega with MCUFRIEND TFT shield
- **Libraries**: `Adafruit_GFX`, `MCUFRIEND_kbv`
- **IDE**: Arduino IDE
