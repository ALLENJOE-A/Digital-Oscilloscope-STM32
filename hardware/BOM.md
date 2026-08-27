# Bill of Materials (BOM)

Components identified from the source code and project documentation.

| # | Component | Quantity | Role | Verified From |
|---|-----------|----------|------|---------------|
| 1 | STM32F103C8T6 (Blue Pill) | 1 | Master controller, button input, UART TX | Source code |
| 2 | Arduino Uno (or compatible AVR) | 2 | Waveform generator + Display/acquisition | Source code (PORTB usage, MCUFRIEND shield) |
| 3 | MCUFRIEND 2.4" TFT Shield (ILI9341) | 1 | 320x240 waveform display | Source code (MCUFRIEND_kbv library) |
| 4 | Resistors (R value, e.g., 10 kΩ) | 5 | R-2R ladder "R" resistors | Implied by R-2R DAC design |
| 5 | Resistors (2R value, e.g., 20 kΩ) | 5 | R-2R ladder "2R" resistors | Implied by R-2R DAC design |
| 6 | Push buttons | 5 | 4x STM32 control + 1x waveform select | Source code |
| 7 | Jumper wires | Multiple | Inter-module connections | Implied |
| 8 | USB cables | 2-3 | Power + programming | Implied |
| 9 | Breadboard | 1-2 | Prototyping platform | Implied |

> **Note**: Exact resistor values and passive component specifications cannot be verified from the source code alone. The values listed above (10 kΩ / 20 kΩ) are standard for R-2R ladder DACs but should be confirmed against the original hardware build.

## Optional Components

| Component | Purpose | Status |
|-----------|---------|--------|
| AMS1117-3.3V regulator | Voltage regulation for STM32 | Listed in original docs, not verified in code |
| Decoupling capacitors | Power supply filtering | Standard practice, not specified in code |
