# System Architecture

## High-Level Overview

The Digital Oscilloscope system consists of three independent microcontroller modules connected through analog signal paths and UART communication.

```mermaid
graph TD
    subgraph STM32_Controller["STM32 Master Controller"]
        BTN_H[HOLD Button PA1]
        BTN_U[UP Button PA2]
        BTN_D[DOWN Button PA3]
        BTN_S[SET Button PA4]
        STM32[STM32F103C8T6]
        BTN_H --> STM32
        BTN_U --> STM32
        BTN_D --> STM32
        BTN_S --> STM32
    end

    subgraph Waveform_Gen["Waveform Generator"]
        BTN_W[Mode Button D2]
        LUT[Lookup Tables<br/>32 samples x 5-bit]
        AVR1[Arduino AVR]
        DAC[5-bit R-2R DAC<br/>Pins D8-D12]
        BTN_W --> AVR1
        AVR1 --> LUT
        LUT --> DAC
    end

    subgraph Display_Module["Display & Acquisition"]
        ADC_IN[ADC Input A5]
        AVR2[Arduino AVR]
        TFT[ILI9341 TFT<br/>320x240]
        BTN_LOCAL[Local Buttons<br/>D10-D13]
        ADC_IN --> AVR2
        BTN_LOCAL --> AVR2
        AVR2 --> TFT
    end

    STM32 -->|"UART 9600 baud<br/>Commands: H U D S"| AVR1
    STM32 -.->|"Planned: UART to<br/>Display Module"| AVR2
    DAC -->|"Analog Signal"| ADC_IN

    style STM32_Controller fill:#1a1a2e,stroke:#e94560,color:#fff
    style Waveform_Gen fill:#1a1a2e,stroke:#0f3460,color:#fff
    style Display_Module fill:#1a1a2e,stroke:#16213e,color:#fff
```

## Module Responsibilities

### STM32 Master Controller (`firmware/stm32_controller/`)

| Responsibility | Implementation |
|---------------|---------------|
| Button input | 4 buttons with INPUT_PULLUP, edge detection |
| UART output | Single-char commands via Serial1 (PA9, 9600 baud) |
| Debug output | USB Serial at 9600 baud |

**UART Command Protocol:**

| Command | Trigger | Intended Action |
|---------|---------|----------------|
| `'H'` | HOLD button press | Toggle waveform hold |
| `'U'` | UP button press | Increase parameter |
| `'D'` | DOWN button press | Decrease parameter |
| `'S'` | SET button press | Reset/set parameter |

### Waveform Generator (`waveform_generator/`)

| Responsibility | Implementation |
|---------------|---------------|
| Waveform synthesis | 32-sample LUTs for 4 waveform types |
| DAC output | Direct PORTB register write to R-2R ladder |
| Mode selection | Button on D2 with non-blocking debounce |
| Feedback | analogRead(A0) to Serial plotter |

**Waveform Types:**

| Mode | Type | LUT Characteristics |
|------|------|-------------------|
| 0 | Sine | Quantized, values 0-31, symmetric |
| 1 | Triangle | Linear ramp, values 0-31, up then down |
| 2 | Sawtooth | Linear 0-31 in 32 steps |
| 3 | Square | 31 for 16 steps, 0 for 16 steps |

### Display and Acquisition (`firmware/display_acquisition/`)

| Responsibility | Implementation |
|---------------|---------------|
| Signal acquisition | `analogRead(A5)`, 10-bit, 320 samples per sweep |
| Display rendering | Line-drawing on ILI9341 via MCUFRIEND_kbv |
| UI | Delay value display, HOLD indicator |
| Controls | Local buttons for HOLD, UP, DOWN, SET |

## Data Flow

```mermaid
sequenceDiagram
    participant User
    participant WaveBtn as Waveform Button
    participant WaveGen as Waveform Generator
    participant DAC as R-2R DAC
    participant ADC as Display ADC (A5)
    participant Display as TFT Display

    User->>WaveBtn: Press to cycle mode
    WaveBtn->>WaveGen: Select next LUT
    loop Every 50μs per sample
        WaveGen->>DAC: Write 5-bit value to PORTB
        DAC->>ADC: Analog voltage
        ADC->>Display: Map 10-bit value to Y coordinate
    end
    Display->>Display: Draw waveform trace
    Display->>Display: Update UI (delay, HOLD)
```

## Integration Status

| Link | Status | Notes |
|------|--------|-------|
| Waveform Generator → DAC → Display ADC | Implemented | Analog signal path works |
| STM32 → Waveform Generator (UART) | Partial | STM32 transmits, but waveform generator does not read UART |
| STM32 → Display Module (UART) | Not implemented | Display uses local buttons |
| Waveform Generator → Serial Plotter | Implemented | Debug feedback via analogRead(A0) |
