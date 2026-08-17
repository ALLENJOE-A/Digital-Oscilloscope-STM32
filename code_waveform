/*
 * 5-Bit R-2R Function Generator Master Code
 * Outputs on Pins 8, 9, 10, 11, and 12
 * Button on Pin 2 to toggle waveforms
 */

// --- WAVEFORM LOOK-UP TABLES (32 steps each, max value 31) ---

const byte sineWave[32] = {
  15, 18, 21, 24, 26, 28, 30, 31, 
  31, 31, 30, 28, 26, 24, 21, 18, 
  15, 12, 9,  7,  5,  3,  1,  0,  
  0,  0,  1,  3,  5,  7,  9,  12
};

const byte triangleWave[32] = {
  0, 2, 4, 6, 8, 10, 12, 14, 
  16, 18, 20, 22, 24, 26, 28, 30, 
  31, 29, 27, 25, 23, 21, 19, 17, 
  15, 13, 11, 9, 7, 5, 3, 1
};

const byte sawtoothWave[32] = {
  0, 1, 2, 3, 4, 5, 6, 7, 
  8, 9, 10, 11, 12, 13, 14, 15, 
  16, 17, 18, 19, 20, 21, 22, 23, 
  24, 25, 26, 27, 28, 29, 30, 31
};

const byte squareWave[32] = {
  31, 31, 31, 31, 31, 31, 31, 31, 
  31, 31, 31, 31, 31, 31, 31, 31, 
  0,  0,  0,  0,  0,  0,  0,  0,  
  0,  0,  0,  0,  0,  0,  0,  0
};

// =======================================================
// WAVEFORM STATE VARIABLES
// =======================================================
const byte* currentWave = sineWave; // Start with Sine Wave by default
int waveMode = 0;                   // 0=Sine, 1=Triangle, 2=Sawtooth, 3=Square

// =======================================================
// BUTTON DEBOUNCE VARIABLES
// =======================================================
const int BUTTON_PIN = 2;
int buttonState;            
int lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

int stepIndex = 0;
int delayTime = 50; 

void setup() {
  // Set Pins 8 through 12 as OUTPUTs
  pinMode(8, OUTPUT);  // LSB (Pin 0 of ladder)
  pinMode(9, OUTPUT);  // Pin 1 of ladder
  pinMode(10, OUTPUT); // Pin 2 of ladder
  pinMode(11, OUTPUT); // Pin 3 of ladder
  pinMode(12, OUTPUT); // MSB (Pin 4 of ladder)
  
  // Set Button Pin as INPUT with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop() {
  // --- 1. NON-BLOCKING BUTTON LOGIC ---
  int reading = digitalRead(BUTTON_PIN);

  // If the switch changed, due to noise or pressing
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset the debouncing timer
  }

  // If the state has been stable longer than the debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has officially changed
    if (reading != buttonState) {
      buttonState = reading;

      // Only toggle the wave if the new button state is LOW (pressed)
      if (buttonState == LOW) {
        waveMode++;
        if (waveMode > 3) {
          waveMode = 0; // Wrap around back to Sine Wave
        }

        // Update the pointer to the new waveform
        switch (waveMode) {
          case 0: currentWave = sineWave; break;
          case 1: currentWave = triangleWave; break;
          case 2: currentWave = sawtoothWave; break;
          case 3: currentWave = squareWave; break;
        }
      }
    }
  }
  lastButtonState = reading; // Save the reading for the next loop

  // --- 2. WAVEFORM GENERATION LOGIC ---
  // Mask out the bottom 5 bits and inject our current wave value directly.
  PORTB = (PORTB & 0xE0) | currentWave[stepIndex];

  // Move to the next slice of the wave
  stepIndex++;
  
  // Reset back to the start when we finish the 32 steps
  if (stepIndex >= 32) {
    stepIndex = 0;
  }

  // Pause briefly before the next step to set the frequency
  delayMicroseconds(delayTime); 
  
  // --- 3. SERIAL PLOTTER LOGIC ---
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
}
