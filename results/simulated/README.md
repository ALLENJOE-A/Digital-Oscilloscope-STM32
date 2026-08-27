# Simulated Results

These outputs are **software reconstructions** generated directly from the waveform lookup tables implemented in the firmware. They are reproducible code-derived visualizations and are **not** photographs or live captures from the original hardware.

The original prototype has been dismantled and real output images are unavailable.

All outputs are reproducible using: `python tools/generate_waveforms.py`

## LUT Waveform Plots

Stepped waveform plots showing the exact 32-sample lookup table values converted to voltage:

| Waveform | File |
|----------|------|
| Sine | `sine_wave_lut.png` |
| Triangle | `triangle_wave_lut.png` |
| Sawtooth | `sawtooth_wave_lut.png` |
| Square | `square_wave_lut.png` |

## Firmware-Reconstructed TFT Display Output

Oscilloscope-style visualizations matching the firmware's display geometry (320x240, grid layout, Y-axis mapping):

| Waveform | File |
|----------|------|
| Sine | `tft_sine.png` |
| Triangle | `tft_triangle.png` |
| Sawtooth | `tft_sawtooth.png` |
| Square | `tft_square.png` |

These TFT simulations use the same display coordinate mapping as the firmware:
- `map(adc_value, 0, 1023, 230, 30)` for Y-axis placement
- 320-pixel sweep width
- Grid spacing matching `drawGrid()` implementation
