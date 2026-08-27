/**
 * TFT Display and Signal Acquisition Module
 *
 * Hardware: Arduino Uno/Mega with MCUFRIEND TFT Shield (ILI9341)
 * Platform: Arduino IDE
 * Libraries: Adafruit_GFX, MCUFRIEND_kbv
 *
 * Acquires analog signals via analogRead() on pin A5 and renders
 * real-time waveform traces on a 320x240 TFT display with an
 * oscilloscope-style grid.
 *
 * Controls:
 *   HOLD (D10) — Toggle waveform freeze
 *   SET  (D11) — Reset time delay to 0
 *   UP   (D12) — Increase inter-sample delay by TIME_STEP_US
 *   DOWN (D13) — Decrease inter-sample delay by TIME_STEP_US
 *
 * Integration Note:
 *   This module uses local buttons for control. It does not currently
 *   receive UART commands from the STM32 master controller.
 *   See docs/limitations.md for details.
 */

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

// ============================================================
// Display Color Definitions
// ============================================================
#define COLOR_BACKGROUND  0x0000  // Black
#define COLOR_BORDER      0xFFFF  // White
#define COLOR_WAVEFORM    0x07E0  // Green
#define COLOR_UI_TEXT     0xFFE0  // Yellow
#define COLOR_HOLD_LABEL  0xF800  // Red
#define COLOR_GRID        0x39E7  // Dark grey

// ============================================================
// Display Geometry
// ============================================================
#define DISPLAY_WIDTH     320
#define DISPLAY_HEIGHT    240
#define UI_BAR_HEIGHT     20     // Top status bar
#define GRID_TOP          25     // Waveform area top edge
#define GRID_BOTTOM       240    // Waveform area bottom edge
#define GRID_H_SPACING    40     // Horizontal grid spacing (pixels)
#define GRID_V_SPACING    40     // Vertical grid spacing (pixels)
#define GRID_TICK_SIZE    2      // Grid tick mark length

// ============================================================
// ADC Configuration
// ============================================================
#define ADC_PIN           A5
#define ADC_MIN           0
#define ADC_MAX           1023
#define DISPLAY_Y_MIN     30     // Top of waveform area (high voltage)
#define DISPLAY_Y_MAX     230    // Bottom of waveform area (low voltage)
#define CENTER_GRIDLINE_Y 130    // Y position of center gridline

// ============================================================
// Pin Definitions — Control Buttons
// ============================================================
#define HOLD_PIN    10
#define SET_PIN     11
#define UP_PIN      12
#define DOWN_PIN    13

// ============================================================
// Timing Constants
// ============================================================
#define TIME_STEP_US        50    // Delay increment per UP/DOWN press
#define MAX_TIME_DELAY_US   5000  // Maximum inter-sample delay
#define MIN_TIME_DELAY_US   0     // Minimum inter-sample delay
#define HOLD_DEBOUNCE_MS    300   // Debounce for HOLD and SET buttons
#define ARROW_DEBOUNCE_MS   150   // Debounce for UP and DOWN buttons

// ============================================================
// Data Buffers
// ============================================================
uint8_t data[DISPLAY_WIDTH];     // Current waveform samples (Y coordinates)
uint8_t oldData[DISPLAY_WIDTH];  // Previous frame (for incremental redraw)

// ============================================================
// State Variables
// ============================================================
bool hold      = false;   // When true, acquisition is paused
int  timeDelay = 0;       // Inter-sample delay in microseconds

// ============================================================
// Setup
// ============================================================
void setup() {
  Serial.begin(9600);

  pinMode(ADC_PIN,  INPUT);
  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(SET_PIN,  INPUT_PULLUP);
  pinMode(UP_PIN,   INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  // Initialize TFT display
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3 || ID == 0xFFFF) {
    ID = 0x9341;  // Fallback to ILI9341
  }
  tft.begin(ID);
  tft.setRotation(1);  // Landscape orientation
  tft.fillScreen(COLOR_BACKGROUND);

  drawGrid();
  drawUI();
}

// ============================================================
// Main Loop
// ============================================================
void loop() {
  handleButtons();

  if (!hold) {
    readSignal();
    drawWaveform();
  }
}

// ============================================================
// Button Handling
// ============================================================

/**
 * Read all control buttons and update state.
 * Uses blocking delay() for debounce (prototype-level implementation).
 */
void handleButtons() {
  // HOLD toggle
  if (digitalRead(HOLD_PIN) == LOW) {
    hold = !hold;
    drawUI();
    delay(HOLD_DEBOUNCE_MS);
  }

  // UP — increase inter-sample delay (slower sweep)
  if (digitalRead(UP_PIN) == LOW) {
    timeDelay += TIME_STEP_US;
    if (timeDelay > MAX_TIME_DELAY_US) {
      timeDelay = MAX_TIME_DELAY_US;
    }
    drawUI();
    delay(ARROW_DEBOUNCE_MS);
  }

  // DOWN — decrease inter-sample delay (faster sweep)
  if (digitalRead(DOWN_PIN) == LOW) {
    timeDelay -= TIME_STEP_US;
    if (timeDelay < MIN_TIME_DELAY_US) {
      timeDelay = MIN_TIME_DELAY_US;
    }
    drawUI();
    delay(ARROW_DEBOUNCE_MS);
  }

  // SET — reset delay to minimum
  if (digitalRead(SET_PIN) == LOW) {
    timeDelay = MIN_TIME_DELAY_US;
    drawUI();
    delay(HOLD_DEBOUNCE_MS);
  }
}

// ============================================================
// Signal Acquisition
// ============================================================

/**
 * Acquire one full sweep of DISPLAY_WIDTH samples.
 * Each sample is read via analogRead() and mapped to display Y coordinates.
 * The Y axis is inverted: higher voltage maps to smaller Y (higher on screen).
 */
void readSignal() {
  for (int i = 0; i < DISPLAY_WIDTH; i++) {
    int val = analogRead(ADC_PIN);
    data[i] = map(val, ADC_MIN, ADC_MAX, DISPLAY_Y_MAX, DISPLAY_Y_MIN);

    if (timeDelay > 0) {
      delayMicroseconds(timeDelay);
    }
  }
}

// ============================================================
// Display Rendering
// ============================================================

/**
 * Draw waveform using incremental update (erase-then-draw).
 * Only redraws pixels that have changed, avoiding full-screen refresh.
 */
void drawWaveform() {
  for (int i = 0; i < DISPLAY_WIDTH - 1; i++) {
    // Erase old segment if data changed
    if (oldData[i] != data[i] || oldData[i + 1] != data[i + 1]) {
      tft.drawLine(i, oldData[i], i + 1, oldData[i + 1], COLOR_BACKGROUND);

      // Restore center gridline if it was erased
      if (oldData[i] == CENTER_GRIDLINE_Y) {
        tft.drawPixel(i, CENTER_GRIDLINE_Y, COLOR_GRID);
      }
    }

    // Draw new segment
    tft.drawLine(i, data[i], i + 1, data[i + 1], COLOR_WAVEFORM);
    oldData[i] = data[i];
  }
  oldData[DISPLAY_WIDTH - 1] = data[DISPLAY_WIDTH - 1];
}

/**
 * Draw the status bar at the top of the display.
 * Shows current time delay and HOLD indicator.
 */
void drawUI() {
  tft.fillRect(0, 0, DISPLAY_WIDTH, UI_BAR_HEIGHT, COLOR_BACKGROUND);
  tft.setTextSize(2);

  // Time delay display
  tft.setCursor(10, 5);
  tft.setTextColor(COLOR_UI_TEXT);
  tft.print("Delay:");
  tft.print(timeDelay);
  tft.print("us");

  // HOLD indicator
  tft.setCursor(250, 5);
  tft.setTextColor(hold ? COLOR_HOLD_LABEL : COLOR_BACKGROUND);
  tft.print("HOLD");
}

/**
 * Draw the oscilloscope-style measurement grid.
 * Dotted horizontal and vertical lines at GRID_H/V_SPACING intervals.
 */
void drawGrid() {
  // Outer border
  tft.drawRect(0, GRID_TOP, DISPLAY_WIDTH, GRID_BOTTOM - GRID_TOP, COLOR_BORDER);

  // Horizontal gridlines (dotted)
  for (int y = GRID_TOP + GRID_V_SPACING; y < GRID_BOTTOM; y += GRID_V_SPACING) {
    for (int x = 0; x < DISPLAY_WIDTH; x += 10) {
      tft.drawFastHLine(x, y, GRID_TICK_SIZE, COLOR_GRID);
    }
  }

  // Vertical gridlines (dotted)
  for (int x = GRID_H_SPACING; x < DISPLAY_WIDTH; x += GRID_H_SPACING) {
    for (int y = GRID_TOP; y < GRID_BOTTOM; y += 10) {
      tft.drawFastVLine(x, y, GRID_TICK_SIZE, COLOR_GRID);
    }
  }
}
