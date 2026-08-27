# STM32 Master Controller

## Purpose

The STM32F103C8T6 acts as the master controller for the oscilloscope system. It reads four physical buttons and transmits single-character UART commands to the downstream waveform generator and display modules.

## Button Mapping

| Pin | Function | UART Command |
|-----|----------|-------------|
| PA1 | HOLD     | `'H'`       |
| PA2 | UP       | `'U'`       |
| PA3 | DOWN     | `'D'`       |
| PA4 | SET      | `'S'`       |

## Communication

- **USB Serial** (`Serial`, 9600 baud): Debug output to PC
- **Hardware UART** (`Serial1`, 9600 baud, PA9 TX): Command output to Arduino

## Build

- **Platform**: STM32 Arduino Core (STM32duino)
- **Board**: Generic STM32F103C8T6 (Blue Pill)
- **IDE**: Arduino IDE with STM32 board package installed

## Integration Note

The current prototype sends UART commands, but the display acquisition module reads buttons locally rather than receiving UART commands. This is a known integration limitation documented in `docs/limitations.md`.
