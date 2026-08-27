# Current Limitations

This document honestly describes the limitations of the current prototype implementation.

## DAC Resolution

- **5-bit resolution** (32 discrete voltage levels)
- Waveform output is visibly stepped, especially for sine and triangle waves
- Voltage resolution is approximately 161 mV per step (at 5V reference)
- **Improvement**: Use an 8-bit or 10-bit R-2R ladder, or an external DAC IC (e.g., MCP4921)

## Lookup Table Resolution

- **32 samples per waveform cycle**
- Limited waveform fidelity, particularly for sine waves
- **Improvement**: Increase to 64, 128, or 256 samples per cycle

## Waveform Timing

- Output timing relies on `delayMicroseconds()` — not timer-interrupt driven
- Loop overhead from `analogRead()` and `Serial.println()` reduces actual output frequency below theoretical values
- Frequency accuracy is not guaranteed
- **Improvement**: Use hardware Timer/Counter with interrupt-driven output, or Timer + DMA for zero-jitter waveform generation

## Signal Acquisition

- Uses `analogRead()` (blocking, polling-based) — not DMA or timer-triggered
- Acquisition rate is limited by Arduino's ADC conversion time (~100 μs per sample)
- No continuous background sampling — CPU is blocked during each `analogRead()` call
- **Improvement**: Use STM32 ADC with timer trigger and DMA circular buffer for continuous sampling at configurable rates

## No Trigger Implementation

- The display module does not implement any trigger detection
- Waveform display is free-running — traces may drift or jitter
- **Improvement**: Implement software rising/falling edge trigger with configurable threshold and hysteresis; add pre-trigger buffer for stable waveform alignment

## No DMA Acquisition

- Despite the original README claiming DMA-based sampling, the current source code does not use DMA
- All ADC reads are software-polled
- **Improvement**: Configure STM32 DMA channel with circular buffer for hardware-managed continuous acquisition

## Integration Gap — STM32 UART Commands

- The STM32 controller transmits UART commands ('H', 'U', 'D', 'S')
- The waveform generator does not implement `Serial.read()` to receive these commands
- The display module uses local buttons and does not receive UART input
- The three modules operate partially independently
- **Improvement**: Add UART receive handlers in the waveform generator and/or display module; implement a command protocol parser

## Blocking Button Debounce (Display Module)

- Button handling in the display module uses `delay()` (blocking)
- During the delay, no acquisition or display updates occur
- **Improvement**: Implement `millis()`-based non-blocking debounce (already done in waveform generator)

## No Voltage or Time Calibration

- The display Y-axis shows raw mapped values, not calibrated voltage
- No voltage/division or time/division indicators
- **Improvement**: Add calibrated voltage scale based on known ADC reference and input attenuation; calculate and display time/division from sample delay

## Hardware Output Unavailable

- The original prototype hardware has been dismantled
- No photographs of live hardware operation are available
- All waveform outputs in `results/simulated/` are software reconstructions from firmware LUT data
- **Improvement**: Rebuild prototype and capture actual oscilloscope screenshots and photographs

## Summary Table

| Limitation | Severity | V2 Priority |
|-----------|----------|-------------|
| No trigger detection | High | P0 |
| Polling-based ADC | High | P0 |
| STM32-display integration gap | High | P0 |
| 5-bit DAC resolution | Medium | P1 |
| 32-sample LUT | Medium | P1 |
| Delay-based waveform timing | Medium | P1 |
| Blocking button debounce (display) | Low | P2 |
| No voltage/time calibration | Low | P2 |
| No hardware photographs | Low | P3 |
