/**
 * STM32 Master Controller — Button Input & UART Command Transmitter
 *
 * Hardware: STM32F103C8T6 (Blue Pill)
 * Platform: STM32duino (Arduino Core for STM32)
 *
 * Reads four push buttons and transmits single-character UART commands
 * to downstream modules (waveform generator / display).
 *
 * UART Command Protocol:
 *   'H' — HOLD (toggle waveform freeze)
 *   'U' — UP   (increase parameter)
 *   'D' — DOWN (decrease parameter)
 *   'S' — SET  (reset parameter)
 *
 * Integration Note:
 *   The waveform generator and display modules do not currently implement
 *   UART receive handlers. These commands are transmitted but not processed
 *   by downstream firmware. See docs/limitations.md.
 */

// ============================================================
// Pin Definitions
// ============================================================
#define HOLD_PIN  PA1
#define UP_PIN    PA2
#define DOWN_PIN  PA3
#define SET_PIN   PA4

// ============================================================
// UART Configuration
// ============================================================
#define DEBUG_BAUD_RATE    9600   // USB Serial (debug output to PC)
#define COMMAND_BAUD_RATE  9600   // Hardware UART (Serial1, TX on PA9)

// ============================================================
// Timing Constants
// ============================================================
#define USB_INIT_DELAY_MS  2000   // Wait for USB enumeration
#define DEBOUNCE_DELAY_MS  200    // Simple blocking debounce

// ============================================================
// UART Command Characters
// ============================================================
#define CMD_HOLD  'H'
#define CMD_UP    'U'
#define CMD_DOWN  'D'
#define CMD_SET   'S'

// ============================================================
// State Variables
// ============================================================
bool lastHoldState = HIGH;
bool lastUpState   = HIGH;
bool lastDownState = HIGH;
bool lastSetState  = HIGH;

// ============================================================
// Setup
// ============================================================
void setup() {
  Serial.begin(DEBUG_BAUD_RATE);
  Serial1.begin(COMMAND_BAUD_RATE);

  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(UP_PIN,   INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(SET_PIN,  INPUT_PULLUP);

  delay(USB_INIT_DELAY_MS);
  Serial.println("STM32 Master Ready. Waiting for buttons...");
}

// ============================================================
// Main Loop
// ============================================================
void loop() {
  handleButtons();
}

/**
 * Read all buttons and transmit UART commands on falling edge.
 * Uses simple edge detection (HIGH -> LOW transition).
 *
 * Note: Uses blocking delay() for debounce. A millis()-based
 * non-blocking approach would be preferable for production use.
 */
void handleButtons() {
  bool currentHold = digitalRead(HOLD_PIN);
  bool currentUp   = digitalRead(UP_PIN);
  bool currentDown = digitalRead(DOWN_PIN);
  bool currentSet  = digitalRead(SET_PIN);

  if (currentHold == LOW && lastHoldState == HIGH) {
    sendCommand(CMD_HOLD, "HOLD");
  }

  if (currentUp == LOW && lastUpState == HIGH) {
    sendCommand(CMD_UP, "UP");
  }

  if (currentDown == LOW && lastDownState == HIGH) {
    sendCommand(CMD_DOWN, "DOWN");
  }

  if (currentSet == LOW && lastSetState == HIGH) {
    sendCommand(CMD_SET, "SET");
  }

  lastHoldState = currentHold;
  lastUpState   = currentUp;
  lastDownState = currentDown;
  lastSetState  = currentSet;
}

/**
 * Transmit a single command character via hardware UART
 * and log the action to USB Serial for debugging.
 */
void sendCommand(char cmd, const char* label) {
  Serial1.print(cmd);
  Serial.print(label);
  Serial.print(" Button Pressed -> Sent '");
  Serial.print(cmd);
  Serial.println("'");
  delay(DEBOUNCE_DELAY_MS);
}
