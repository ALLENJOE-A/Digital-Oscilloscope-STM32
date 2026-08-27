/**
 * 5-Bit R-2R DAC Waveform Generator
 *
 * Hardware: Arduino Uno (AVR ATmega328P)
 * Platform: Arduino IDE
 *
 * Generates analog waveforms using a 5-bit R-2R resistor ladder DAC
 * connected to digital pins 8-12 (PORTB bits 0-4).
 *
 * Waveform Types (32-sample lookup tables, values 0-31):
 *   Mode 0: Sine wave
 *   Mode 1: Triangle wave
 *   Mode 2: Sawtooth wave
 *   Mode 3: Square wave
 *
 * Output Frequency (approximate):
 *   f = 1 / (NUM_SAMPLES * step_period)
 *   With DEFAULT_STEP_DELAY_US = 50 and no overhead: ~625 Hz
 *   Actual frequency is lower due to analogRead() and Serial overhead.
 *
 * Button on pin D2 cycles through waveform modes with debounce.
 * ADC on A0 provides feedback to Serial Plotter for verification.
 */

// ============================================================
// Waveform Lookup Tables — 32 samples, 5-bit resolution (0-31)
// ============================================================
#define NUM_SAMPLES  32
#define DAC_MAX      31

const byte sineWave[NUM_SAMPLES] = {
  15, 18, 21, 24, 26, 28, 30, 31,
  31, 31, 30, 28, 26, 24, 21, 18,
  15, 12,  9,  7,  5,  3,  1,  0,
   0,  0,  1,  3,  5,  7,  9, 12
};

const byte triangleWave[NUM_SAMPLES] = {
   0,  2,  4,  6,  8, 10, 12, 14,
  16, 18, 20, 22, 24, 26, 28, 30,
  31, 29, 27, 25, 23, 21, 19, 17,
  15, 13, 11,  9,  7,  5,  3,  1
};

const byte sawtoothWave[NUM_SAMPLES] = {
   0,  1,  2,  3,  4,  5,  6,  7,
   8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26, 27, 28, 29, 30, 31
};

const byte squareWave[NUM_SAMPLES] = {
  31, 31, 31, 31, 31, 31, 31, 31,
  31, 31, 31, 31, 31, 31, 31, 31,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0
};

// ============================================================
// Pin Definitions
// ============================================================
#define BUTTON_PIN            2    // Waveform select button (INPUT_PULLUP)
#define ADC_FEEDBACK_PIN      A0   // Analog feedback for Serial Plotter

// DAC output uses PORTB bits 0-4 (Arduino pins 8-12)
#define DAC_PORT_MASK         0x1F // Lower 5 bits of PORTB
#define DAC_PORT_PRESERVE     0xE0 // Upper 3 bits preserved

// ============================================================
// Timing Constants
// ============================================================
#define DEFAULT_STEP_DELAY_US  50  // Microseconds between DAC steps
#define DEBOUNCE_DELAY_MS      50  // Button debounce threshold

// ============================================================
// Waveform Modes
// ============================================================
#define NUM_WAVEFORMS  4
#define MODE_SINE      0
#define MODE_TRIANGLE  1
#define MODE_SAWTOOTH  2
#define MODE_SQUARE    3

// ============================================================
// State Variables
// ============================================================
const byte* currentWave = sineWave;
int waveMode  = MODE_SINE;
int stepIndex = 0;
int delayTime = DEFAULT_STEP_DELAY_US;

// Button debounce state
int buttonState     = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

// ============================================================
// Setup
// ============================================================
void setup() {
  // Configure DAC output pins (D8-D12)
  for (int pin = 8; pin <= 12; pin++) {
    pinMode(pin, OUTPUT);
  }

  // Configure button with internal pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configure ADC feedback input
  pinMode(ADC_FEEDBACK_PIN, INPUT);

  // Serial for plotter output
  Serial.begin(115200);
}

// ============================================================
// Main Loop
// ============================================================
void loop() {
  handleWaveformButton();
  outputDAC();
  serialPlotterOutput();
}

/**
 * Non-blocking button handler with millis()-based debounce.
 * Each press cycles to the next waveform mode.
 */
void handleWaveformButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        waveMode = (waveMode + 1) % NUM_WAVEFORMS;
        updateWaveform();
      }
    }
  }

  lastButtonState = reading;
}

/**
 * Update the LUT pointer based on current waveform mode.
 */
void updateWaveform() {
  switch (waveMode) {
    case MODE_SINE:     currentWave = sineWave;     break;
    case MODE_TRIANGLE: currentWave = triangleWave; break;
    case MODE_SAWTOOTH: currentWave = sawtoothWave; break;
    case MODE_SQUARE:   currentWave = squareWave;   break;
  }
}

/**
 * Write the current LUT sample to the R-2R DAC via PORTB.
 * Direct port manipulation for fast, simultaneous bit output.
 * Advances stepIndex and wraps at NUM_SAMPLES.
 */
void outputDAC() {
  PORTB = (PORTB & DAC_PORT_PRESERVE) | (currentWave[stepIndex] & DAC_PORT_MASK);

  stepIndex++;
  if (stepIndex >= NUM_SAMPLES) {
    stepIndex = 0;
  }

  delayMicroseconds(delayTime);
}

/**
 * Read the DAC output via ADC and send to Serial Plotter.
 * This adds overhead to the waveform loop, reducing actual output frequency.
 */
void serialPlotterOutput() {
  int sensorValue = analogRead(ADC_FEEDBACK_PIN);
  Serial.println(sensorValue);
}
