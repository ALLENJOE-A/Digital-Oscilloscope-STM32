# STM32 Digital Oscilloscope

> Portable Digital Storage Oscilloscope (DSO) built using the STM32F103C8T6 microcontroller featuring high-speed ADC acquisition, DMA-based sampling, software edge triggering, and real-time waveform visualization on a 2.4-inch ILI9341 TFT display.

---

## Project Status

**Status:** Completed

**Type:** Academic Embedded Systems Project

**Institution:** Vellore Institute of Technology, Chennai

**Course:** Embedded C Programming

---

# Table of Contents

- Overview
- Demonstration
- Key Features
- System Specifications
- Hardware Architecture
- Software Architecture
- Working Principle
- Hardware Components
- Software Stack
- Repository Structure
- Performance Evaluation
- Test Results
- Bill of Materials
- Pin Configuration
- Images
- Future Improvements
- References
- Author

---

# Overview

This repository presents the implementation of a portable Digital Storage Oscilloscope (DSO) using the STM32F103C8T6 ARM Cortex-M3 microcontroller.

The project demonstrates efficient real-time signal acquisition using the internal 12-bit ADC operating with DMA-based memory transfers. Captured samples are processed using software edge triggering before being rendered on a 320×240 ILI9341 TFT display through the SPI interface.

Unlike traditional oscilloscopes requiring dedicated acquisition hardware or FPGA implementations, this design achieves high performance using only an ARM Cortex-M microcontroller and optimized embedded firmware.

---

# Demonstration

> Add demonstration images or videos here.

```
images/demo.gif
```

or

```
images/demo.mp4
```

---

# Key Features

- Real-time waveform acquisition
- 12-bit ADC sampling
- DMA-based data transfer
- Circular buffer implementation
- Software edge triggering
- Optimized SPI display rendering
- Analog Front-End (AFE)
- Over-voltage protection
- AC/DC signal conditioning
- Portable USB-powered system

---

# System Specifications

| Parameter | Value |
|------------|-------|
| MCU | STM32F103C8T6 |
| CPU | ARM Cortex-M3 |
| Clock Frequency | 72 MHz |
| ADC Resolution | 12-bit |
| Sampling Rate | ~857 kSa/s |
| Display | ILI9341 TFT |
| Resolution | 320 × 240 |
| Communication | SPI |
| Power | USB 5V |

---

# Hardware Architecture

```
                +-----------------------+
                |   Input Probe         |
                +----------+------------+
                           |
                           |
                 Analog Front-End
                           |
          AC/DC Coupling & Protection
                           |
                           ▼
                STM32F103C8T6 ADC
                           |
                           ▼
                     DMA Controller
                           |
                           ▼
                   Circular Buffer
                           |
                           ▼
                 Trigger Detection
                           |
                           ▼
                  Waveform Processing
                           |
                           ▼
                      SPI Interface
                           |
                           ▼
                  ILI9341 TFT Display
```

---

# Software Architecture

```
System Initialization

↓

Peripheral Configuration

↓

ADC Initialization

↓

DMA Configuration

↓

Timer Configuration

↓

Continuous Sampling

↓

DMA Interrupt

↓

Trigger Detection

↓

Signal Scaling

↓

Display Rendering

↓

Repeat
```

---

# Working Principle

1. Analog signal enters the Analog Front-End.

2. Signal conditioning adjusts voltage level and provides protection.

3. STM32 ADC samples the signal continuously.

4. DMA stores samples directly into SRAM.

5. Software trigger identifies stable waveform positions.

6. Waveform coordinates are generated.

7. TFT display is refreshed through SPI.

---

# Hardware Components

| Component | Description |
|------------|-------------|
| STM32F103C8T6 | ARM Cortex-M3 MCU |
| ILI9341 TFT | 2.4" LCD Display |
| Analog Front-End | Signal Conditioning |
| AMS1117 | 3.3V Voltage Regulator |
| USB Power | Power Supply |

---

# Software Stack

| Layer | Technology |
|--------|------------|
| Language | Embedded C |
| IDE | Arduino IDE / STM32CubeIDE |
| Graphics | ILI9341 Library |
| Display Driver | SPI |
| ADC Driver | STM32 HAL |
| DMA | STM32 DMA |
| Trigger Algorithm | Software Edge Trigger |

---

# Repository Structure

```
STM32-Digital-Oscilloscope
│
├── docs/
│   ├── Project_Report.pdf
│   ├── Circuit_Diagram.pdf
│   └── Presentation.pdf
│
├── source/
│   ├── main.c
│   ├── adc.c
│   ├── adc.h
│   ├── display.c
│   ├── display.h
│   └── utils.c
│
├── images/
│   ├── Hardware_Setup.png
│   ├── Block_Diagram.png
│   ├── Circuit_Diagram.png
│   ├── TFT_Display.png
│   └── Waveform.png
│
├── results/
│   ├── SineWave.png
│   ├── SquareWave.png
│   ├── TriangleWave.png
│   └── Performance.png
│
├── LICENSE
│
└── README.md
```

---

# Performance Evaluation

| Metric | Value |
|----------|--------|
| ADC Resolution | 12-bit |
| Sampling Rate | ~857 kSa/s |
| Display FPS | ≥15 FPS |
| Maximum Signal Tested | 100 kHz |
| Trigger Type | Software Edge Trigger |

---

# Test Results

| Input Signal | Status |
|--------------|--------|
| Sine Wave | Passed |
| Square Wave | Passed |
| Triangle Wave | Passed |
| PWM Signal | Passed |

---

# Bill of Materials (BOM)

| Component | Quantity |
|------------|----------|
| STM32F103C8T6 | 1 |
| ILI9341 TFT | 1 |
| AMS1117-3.3 | 1 |
| Capacitors | Multiple |
| Resistors | Multiple |
| USB Connector | 1 |

---

# Pin Configuration

| STM32 Pin | Function |
|------------|----------|
| PA0 | ADC Input |
| PA5 | SPI Clock |
| PA7 | SPI MOSI |
| PB0 | TFT DC |
| PB1 | TFT RESET |

---

# Images

## Hardware Setup

```
images/Hardware_Setup.png
```

---

## Circuit Diagram

```
images/Circuit_Diagram.png
```

---

## Block Diagram

```
images/Block_Diagram.png
```

---

## Waveform Output

```
images/Waveform.png
```

---

# Future Improvements

- FFT-based spectrum analyzer
- Dual-channel acquisition
- USB PC interface
- SD card logging
- Touchscreen GUI
- Auto-ranging
- Cursor measurements
- Higher sampling rates using STM32F4/F7
- Digital filtering
- Trigger modes (Auto/Normal/Single)

---

# References

1. STM32F103 Datasheet
2. ARM Cortex-M3 Technical Reference Manual
3. ILI9341 Datasheet
4. STMicroelectronics Application Notes

---

# Author

**Allen Joe A**

B.Tech Electronics and VLSI Engineering

Vellore Institute of Technology, Chennai

---

# License

This project is intended for academic and educational purposes.
