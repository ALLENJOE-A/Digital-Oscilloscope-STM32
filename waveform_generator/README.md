# Waveform Generator

## Purpose

This module runs on an Arduino and generates analog waveforms using a 5-bit R-2R resistor ladder DAC. It outputs four selectable waveform types using 32-sample lookup tables.

## Waveform Types

| Mode | Waveform  | LUT Values (32 steps, 0-31) |
|------|-----------|----------------------------|
| 0    | Sine      | Quantized sine wave        |
| 1    | Triangle  | Linear ramp up/down        |
| 2    | Sawtooth  | Linear ramp 0-31           |
| 3    | Square    | 31 for 16 steps, 0 for 16  |

## Hardware Interface

| Pin | Function |
|-----|----------|
| D8  | DAC Bit 0 (LSB) |
| D9  | DAC Bit 1 |
| D10 | DAC Bit 2 |
| D11 | DAC Bit 3 |
| D12 | DAC Bit 4 (MSB) |
| D2  | Waveform select button (cycles through modes) |
| A0  | ADC feedback (serial plotter output) |

## Timing

- Default step delay: 50 us per sample
- Samples per cycle: 32
- Approximate frequency: 1 / (32 x 50 us) = ~625 Hz (without loop overhead)

## Build

- **Platform**: Arduino (AVR)
- **Board**: Arduino Uno or compatible
- **IDE**: Arduino IDE
