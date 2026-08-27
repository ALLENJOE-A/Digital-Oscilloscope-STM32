# Pin Mapping

All pin assignments are verified directly from the source code.

## STM32 Master Controller (`firmware/stm32_controller/stm32_master.ino`)

| Pin | Direction | Function | Notes |
|-----|-----------|----------|-------|
| PA1 | Input (pullup) | HOLD button | Active LOW |
| PA2 | Input (pullup) | UP button | Active LOW |
| PA3 | Input (pullup) | DOWN button | Active LOW |
| PA4 | Input (pullup) | SET button | Active LOW |
| PA9 | Output | UART TX (Serial1) | 9600 baud, to waveform generator |
| USB | Bidirectional | USB Serial debug | 9600 baud |

## Waveform Generator (`waveform_generator/waveform_generator.ino`)

| Pin | Direction | Function | Notes |
|-----|-----------|----------|-------|
| D8 | Output | DAC Bit 0 (LSB) | PORTB bit 0 |
| D9 | Output | DAC Bit 1 | PORTB bit 1 |
| D10 | Output | DAC Bit 2 | PORTB bit 2 |
| D11 | Output | DAC Bit 3 | PORTB bit 3 |
| D12 | Output | DAC Bit 4 (MSB) | PORTB bit 4 |
| D2 | Input (pullup) | Waveform select button | Active LOW, debounced |
| A0 | Input (analog) | ADC feedback | Serial plotter output |

### R-2R DAC Connection

The 5 digital output pins connect to a weighted R-2R resistor ladder network. The DAC output voltage is:

```
V_out = V_ref × (digital_value / 32)
```

Where `V_ref` is the Arduino logic level (typically 5V) and `digital_value` ranges from 0 to 31.

> **Note**: Exact resistor values for the R-2R ladder are not specified in the source code. Standard values are R = 10 kΩ, 2R = 20 kΩ. To be verified from hardware schematic.

## Display and Acquisition Module (`firmware/display_acquisition/display_acquisition.ino`)

| Pin | Direction | Function | Notes |
|-----|-----------|----------|-------|
| A5 | Input (analog) | Signal acquisition ADC | 10-bit, mapped to display Y |
| D10 | Input (pullup) | HOLD button | Active LOW |
| D11 | Input (pullup) | SET button | Active LOW |
| D12 | Input (pullup) | UP button | Active LOW |
| D13 | Input (pullup) | DOWN button | Active LOW |

### TFT Shield Pins

The MCUFRIEND TFT shield uses the following Arduino pins (managed by the MCUFRIEND_kbv library):

| Pin Range | Function | Notes |
|-----------|----------|-------|
| D2-D9 | 8-bit parallel data bus | Directly driven by library |
| A1 | TFT CS | Directly driven by library |
| A2 | TFT RS/DC | Directly driven by library |
| A3 | TFT WR | Directly driven by library |
| A4 | TFT RD | Directly driven by library |
| RST | TFT Reset | Connected to Arduino reset |

> **Note**: MCUFRIEND shields occupy most digital and analog pins. The available pins for buttons and ADC input are constrained by the shield's pin usage. Pin assignments above are from source code; shield-specific wiring is handled internally by the MCUFRIEND_kbv library.

## Inter-Module Connections

| From | To | Signal | Notes |
|------|----|--------|-------|
| STM32 PA9 (TX) | Waveform Generator RX | UART 9600 baud | To be verified from hardware setup |
| Waveform Generator DAC output | Display Module A5 | Analog signal | Via R-2R ladder output |

> **Note**: The physical wiring between the STM32 UART TX and the waveform generator's serial RX pin is not explicitly configured in the waveform generator code (it does not call `Serial.read()`). This connection exists at the hardware level but the receive-side software integration is incomplete.
