# Performance Parameters

All parameters listed below are verified directly from the source code or derived from known hardware specifications.

## Waveform Generator

| Parameter | Value | Verification |
|-----------|-------|-------------|
| DAC Resolution | 5-bit (32 levels) | Source: `PORTB` mask `0x1F`, LUT values 0-31 |
| LUT Size | 32 samples/cycle | Source: `NUM_SAMPLES = 32` |
| Number of Waveforms | 4 (sine, triangle, sawtooth, square) | Source: 4 LUT arrays |
| DAC Output Pins | 5 (D8-D12 via PORTB) | Source: pin configuration |
| Step Delay | 50 μs (configurable) | Source: `DEFAULT_STEP_DELAY_US = 50` |
| Theoretical Frequency | ~625 Hz at 50 μs step delay | Calculated: 1/(32 × 50 μs) |
| Actual Frequency | Lower than theoretical | Loop includes `analogRead()` + `Serial.println()` overhead |
| DAC Voltage Range | 0 to V_ref (nominally 5V) | Determined by R-2R ladder and Arduino logic level |
| Voltage Resolution | ~161 mV per step | V_ref / 31 ≈ 5V / 31 |
| Button Debounce | 50 ms, non-blocking (`millis()`) | Source: `DEBOUNCE_DELAY_MS = 50` |

## Display and Acquisition

| Parameter | Value | Verification |
|-----------|-------|-------------|
| ADC Resolution | 10-bit (1024 levels) | Arduino `analogRead()` specification |
| ADC Input Pin | A5 | Source: `ADC_PIN = A5` |
| Display Resolution | 320 × 240 pixels | Source: `DISPLAY_WIDTH = 320`, `DISPLAY_HEIGHT = 240` |
| Samples per Sweep | 320 (1 per pixel column) | Source: loop `i < DISPLAY_WIDTH` |
| Display Y Mapping | ADC 0→Y:230, ADC 1023→Y:30 | Source: `map(val, 0, 1023, 230, 30)` |
| Waveform Color | Green (0x07E0) | Source: `COLOR_WAVEFORM = 0x07E0` |
| Time Delay Range | 0 to 5000 μs | Source: `MIN_TIME_DELAY_US`, `MAX_TIME_DELAY_US` |
| Time Delay Step | 50 μs | Source: `TIME_STEP_US = 50` |
| Display Interface | 8-bit parallel (via MCUFRIEND shield) | Source: MCUFRIEND_kbv library |
| Rendering Method | Incremental line redraw | Source: `drawWaveform()` erases old, draws new |

## STM32 Controller

| Parameter | Value | Verification |
|-----------|-------|-------------|
| MCU | STM32F103C8T6 | Source: file header, board selection |
| UART Baud Rate | 9600 | Source: `COMMAND_BAUD_RATE = 9600` |
| UART Commands | 4: 'H', 'U', 'D', 'S' | Source: command definitions |
| Button Count | 4 (PA1-PA4) | Source: pin definitions |
| Debounce Method | 200 ms blocking delay | Source: `DEBOUNCE_DELAY_MS = 200` |

## Parameters NOT Verified

The following parameters cannot be confirmed from the available source code and are **not claimed**:

| Parameter | Reason |
|-----------|--------|
| Actual sampling rate (Sa/s) | Depends on `analogRead()` execution time, not measured |
| Display refresh rate (FPS) | Depends on TFT SPI throughput and waveform complexity |
| Maximum input frequency | Not tested or specified in code |
| ADC accuracy / linearity | Not calibrated |
| R-2R DAC accuracy | Depends on resistor tolerance, not measured |
| Waveform THD | Not measured |
