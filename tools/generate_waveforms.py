#!/usr/bin/env python3
"""
Waveform Visualization Generator

Generates reproducible waveform plots directly from the firmware lookup tables.
All LUT values are extracted verbatim from waveform_generator/waveform_generator.ino.

Outputs:
  results/simulated/sine_wave_lut.png
  results/simulated/triangle_wave_lut.png
  results/simulated/sawtooth_wave_lut.png
  results/simulated/square_wave_lut.png
  results/simulated/tft_sine.png
  results/simulated/tft_triangle.png
  results/simulated/tft_sawtooth.png
  results/simulated/tft_square.png

Usage:
  pip install matplotlib numpy
  python tools/generate_waveforms.py
"""

import os
import numpy as np

try:
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
except ImportError:
    print("ERROR: matplotlib is required. Install with: pip install matplotlib numpy")
    raise SystemExit(1)

# ============================================================
# Firmware LUT Data (verbatim from waveform_generator.ino)
# 32 samples each, 5-bit resolution (0-31)
# ============================================================
LUTS = {
    "sine": [
        15, 18, 21, 24, 26, 28, 30, 31,
        31, 31, 30, 28, 26, 24, 21, 18,
        15, 12,  9,  7,  5,  3,  1,  0,
         0,  0,  1,  3,  5,  7,  9, 12
    ],
    "triangle": [
         0,  2,  4,  6,  8, 10, 12, 14,
        16, 18, 20, 22, 24, 26, 28, 30,
        31, 29, 27, 25, 23, 21, 19, 17,
        15, 13, 11,  9,  7,  5,  3,  1
    ],
    "sawtooth": [
         0,  1,  2,  3,  4,  5,  6,  7,
         8,  9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31
    ],
    "square": [
        31, 31, 31, 31, 31, 31, 31, 31,
        31, 31, 31, 31, 31, 31, 31, 31,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0
    ],
}

NUM_SAMPLES = 32
DAC_MAX = 31
V_REF = 5.0  # Arduino 5V logic level

# Display constants from display_acquisition.ino
TFT_WIDTH = 320
TFT_HEIGHT = 240
GRID_TOP = 25
DISPLAY_Y_MIN = 30   # High voltage (top of waveform area)
DISPLAY_Y_MAX = 230   # Low voltage (bottom of waveform area)

OUTPUT_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                          "results", "simulated")


def generate_lut_plot(name, lut_values):
    """Generate a stepped LUT waveform plot showing 2 cycles."""
    samples = np.array(lut_values * 2)  # 2 cycles
    voltage = samples * (V_REF / DAC_MAX)
    x = np.arange(len(samples))

    fig, ax = plt.subplots(figsize=(10, 4))
    ax.step(x, voltage, where='post', linewidth=1.5, color='#00cc66')
    ax.scatter(x, voltage, s=12, color='#00cc66', zorder=3)

    ax.set_xlabel('Sample Index')
    ax.set_ylabel('DAC Output Voltage (V)')
    ax.set_title(f'{name.capitalize()} Wave — Firmware LUT ({NUM_SAMPLES} samples/cycle, 5-bit)')
    ax.set_xlim(-0.5, len(samples) - 0.5)
    ax.set_ylim(-0.3, V_REF + 0.3)
    ax.axhline(y=V_REF / 2, color='gray', linestyle='--', alpha=0.4)
    ax.grid(True, alpha=0.3)
    fig.set_facecolor('#1a1a1a')
    ax.set_facecolor('#1a1a1a')
    ax.tick_params(colors='#cccccc')
    ax.xaxis.label.set_color('#cccccc')
    ax.yaxis.label.set_color('#cccccc')
    ax.title.set_color('#ffffff')
    for spine in ax.spines.values():
        spine.set_color('#444444')

    fig.tight_layout()
    path = os.path.join(OUTPUT_DIR, f"{name}_wave_lut.png")
    fig.savefig(path, dpi=150, facecolor=fig.get_facecolor())
    plt.close(fig)
    print(f"  Generated: {path}")


def generate_tft_plot(name, lut_values):
    """Generate an oscilloscope-style TFT display simulation."""
    # Repeat LUT to fill 320 pixels
    repeats = TFT_WIDTH // NUM_SAMPLES  # 10 full cycles
    samples = (lut_values * repeats)[:TFT_WIDTH]

    # Map DAC values to display Y coordinates (same as firmware)
    # map(val_10bit, 0, 1023, 230, 30) — we simulate 10-bit ADC reading
    adc_values = [int(s * (1023.0 / DAC_MAX)) for s in samples]
    y_coords = [int(DISPLAY_Y_MAX - (v / 1023.0) * (DISPLAY_Y_MAX - DISPLAY_Y_MIN))
                for v in adc_values]

    fig, ax = plt.subplots(figsize=(8, 6))
    fig.set_facecolor('black')
    ax.set_facecolor('black')

    # Draw grid (matching firmware drawGrid)
    # Outer border
    ax.plot([0, TFT_WIDTH-1, TFT_WIDTH-1, 0, 0],
            [GRID_TOP, GRID_TOP, TFT_HEIGHT-1, TFT_HEIGHT-1, GRID_TOP],
            color='white', linewidth=0.8)

    # Horizontal gridlines (dotted)
    for y in range(GRID_TOP + 40, TFT_HEIGHT, 40):
        for x in range(0, TFT_WIDTH, 10):
            ax.plot([x, x+1], [y, y], color='#555555', linewidth=0.5)

    # Vertical gridlines (dotted)
    for x in range(40, TFT_WIDTH, 40):
        for y in range(GRID_TOP, TFT_HEIGHT, 10):
            ax.plot([x, x], [y, y+1], color='#555555', linewidth=0.5)

    # Draw waveform
    x_vals = list(range(TFT_WIDTH))
    ax.plot(x_vals, y_coords, color='#00ff00', linewidth=1.2)

    # Draw UI bar
    ax.text(10, 12, 'Delay:0us', color='yellow', fontsize=10,
            fontfamily='monospace', fontweight='bold')

    # Invert Y axis (TFT coordinate system: 0 at top)
    ax.set_ylim(TFT_HEIGHT, 0)
    ax.set_xlim(0, TFT_WIDTH)
    ax.set_aspect('equal')
    ax.axis('off')
    ax.set_title(f'Firmware-Reconstructed TFT Output — {name.capitalize()} Wave',
                 color='white', fontsize=11, pad=10)

    fig.tight_layout()
    path = os.path.join(OUTPUT_DIR, f"tft_{name}.png")
    fig.savefig(path, dpi=150, facecolor='black', bbox_inches='tight')
    plt.close(fig)
    print(f"  Generated: {path}")


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)

    print("Generating LUT waveform plots...")
    for name, lut in LUTS.items():
        generate_lut_plot(name, lut)

    print("\nGenerating TFT display simulations...")
    for name, lut in LUTS.items():
        generate_tft_plot(name, lut)

    print(f"\nAll outputs saved to: {OUTPUT_DIR}")
    print("\nNote: These are software reconstructions from firmware LUT data.")
    print("They are NOT photographs or live captures from hardware.")


if __name__ == "__main__":
    main()
