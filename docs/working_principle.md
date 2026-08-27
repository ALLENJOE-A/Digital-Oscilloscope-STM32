# Working Principle

## System Operation — Step by Step

### 1. Power-Up Initialization

When the system is powered:

- **STM32 Controller** initializes GPIO pins PA1-PA4 as INPUT_PULLUP, configures USB Serial (debug) and Serial1 (UART TX at 9600 baud), then waits 2 seconds for USB enumeration.
- **Waveform Generator** configures digital pins 8-12 as OUTPUT (R-2R DAC), pin 2 as INPUT_PULLUP (waveform select button), and pin A0 as analog input. Serial begins at 115200 baud for plotter output. Default waveform is sine.
- **Display Module** initializes the MCUFRIEND TFT shield (auto-detecting ID, defaulting to 0x9341), sets landscape rotation, draws the oscilloscope grid, and renders the initial UI.

### 2. Waveform Generation

The waveform generator continuously outputs analog signals through the R-2R DAC:

1. The current LUT value at `stepIndex` is written directly to PORTB bits 0-4 using a bitmask operation: `PORTB = (PORTB & 0xE0) | currentWave[stepIndex]`
2. `stepIndex` increments from 0 to 31 and wraps back to 0
3. A `delayMicroseconds(50)` call between steps sets the output rate
4. The resulting 5-bit digital pattern is converted to an analog voltage by the R-2R resistor ladder

**Approximate output frequency:**

```
f = 1 / (32 × T_step)
```

Where `T_step` includes the 50 μs delay plus loop overhead (analogRead + Serial.println). The actual frequency is lower than the theoretical 625 Hz due to this overhead.

### 3. Waveform Selection

Pressing the button on pin D2 cycles through waveform modes:

- **Debounce**: Non-blocking debounce using `millis()` with 50 ms threshold
- **Cycle order**: Sine → Triangle → Sawtooth → Square → Sine
- **Mechanism**: Pointer `currentWave` is updated to point to the selected LUT array

### 4. Signal Acquisition

The display module acquires the analog signal:

1. `analogRead(A5)` reads the R-2R DAC output (10-bit, 0-1023)
2. The value is mapped to display Y coordinates: `map(val, 0, 1023, 230, 30)` — inverting the axis so higher voltage appears higher on screen
3. 320 samples are collected per sweep (one per display pixel column)
4. If `timeDelay > 0`, an additional `delayMicroseconds(timeDelay)` is inserted between samples to slow the sweep

### 5. Display Rendering

The TFT display uses an incremental drawing approach:

1. **Erase old trace**: For each column where data changed, draw the old line segment in BLACK
2. **Restore grid**: If the erased area crossed the center gridline (y=130), redraw that pixel
3. **Draw new trace**: Draw the new waveform as connected GREEN line segments
4. **Update buffer**: Copy current data to `oldData[]` for the next erase cycle

This avoids full-screen redraws, improving refresh performance.

### 6. User Controls (Display Module)

| Button | Action |
|--------|--------|
| HOLD (D10) | Toggle freeze: stops acquisition when active |
| UP (D12) | Increase inter-sample delay by 50 μs (slower sweep) |
| DOWN (D13) | Decrease inter-sample delay by 50 μs (faster sweep) |
| SET (D11) | Reset delay to 0 μs (fastest sweep) |

The UI bar at the top shows the current delay value and HOLD status.

### 7. STM32 Button Control (Partial Integration)

The STM32 controller provides a separate button interface:

1. Reads buttons on PA1-PA4 with edge detection (HIGH→LOW transition)
2. Transmits single ASCII characters ('H', 'U', 'D', 'S') via UART Serial1
3. Echoes button actions to USB Serial for debug monitoring

**Current limitation**: The display module and waveform generator do not include UART receive handlers. The STM32 commands are transmitted but not processed by other modules. See [limitations.md](limitations.md) for details.
