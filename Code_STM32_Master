// --- STM32 Master Debug Code ---
#define HOLD_PIN  PA1
#define UP_PIN    PA2
#define DOWN_PIN  PA3
#define SET_PIN   PA4

bool lastHoldState = HIGH;
bool lastUpState = HIGH;
bool lastDownState = HIGH;
bool lastSetState = HIGH;

void setup() {
  Serial.begin(9600);   // USB channel to your PC Monitor
  Serial1.begin(9600);  // Hardware channel out of PA9 to Arduino

  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(SET_PIN, INPUT_PULLUP);
  
  delay(2000); // Give USB time to connect
  Serial.println("STM32 Master Ready. Waiting for buttons...");
}

void loop() {
  bool currentHold = digitalRead(HOLD_PIN);
  bool currentUp   = digitalRead(UP_PIN);
  bool currentDown = digitalRead(DOWN_PIN);
  bool currentSet  = digitalRead(SET_PIN);

  if (currentHold == LOW && lastHoldState == HIGH) {
    Serial1.print('H'); // Send to Arduino
    Serial.println("HOLD Button Pressed -> Sent 'H'"); // Show on PC
    delay(200);
  }
  
  if (currentUp == LOW && lastUpState == HIGH) {
    Serial1.print('U'); 
    Serial.println("UP Button Pressed -> Sent 'U'"); 
    delay(200);
  }

  if (currentDown == LOW && lastDownState == HIGH) {
    Serial1.print('D'); 
    Serial.println("DOWN Button Pressed -> Sent 'D'"); 
    delay(200);
  }

  if (currentSet == LOW && lastSetState == HIGH) {
    Serial1.print('S'); 
    Serial.println("SET Button Pressed -> Sent 'S'"); 
    delay(200);
  }

  lastHoldState = currentHold;
  lastUpState   = currentUp;
  lastDownState = currentDown;
  lastSetState  = currentSet;
}
