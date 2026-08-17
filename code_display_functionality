#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

#define BLACK    0x0000
#define WHITE    0xFFFF
#define RED      0xF800
#define GREEN    0x07E0
#define YELLOW   0xFFE0
#define DARKGREY 0x39E7

#define ADC_PIN    A5  
#define HOLD_PIN   10
#define SET_PIN    11
#define UP_PIN     12
#define DOWN_PIN   13

#define WIDTH      320
#define HEIGHT     240

uint8_t data[WIDTH];
uint8_t oldData[WIDTH];
bool hold = false;
int timeDelay = 0; 

void setup() {
  Serial.begin(9600);

  // Using internal resistors to prevent "floating" antenna noise
  pinMode(ADC_PIN, INPUT);
  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(SET_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  uint16_t ID = tft.readID();
  if (ID == 0xD3D3 || ID == 0xFFFF) ID = 0x9341; 
  tft.begin(ID);
  
  tft.setRotation(1); 
  tft.fillScreen(BLACK);
  
  drawGrid();
  drawUI();
}

void loop() {
  // Looking for LOW because pressing the button connects it to GND
  if (digitalRead(HOLD_PIN) == LOW) {
    hold = !hold;
    drawUI();
    delay(300); 
  }

  if (digitalRead(UP_PIN) == LOW) {
    timeDelay += 50; 
    if (timeDelay > 5000) timeDelay = 5000; 
    drawUI();
    delay(150); 
  }

  if (digitalRead(DOWN_PIN) == LOW) {
    timeDelay -= 50;
    if (timeDelay < 0) timeDelay = 0; 
    drawUI();
    delay(150); 
  }

  if (digitalRead(SET_PIN) == LOW) {
    timeDelay = 0; 
    drawUI();
    delay(300); 
  }

  if (!hold) {
    readSignal();
    drawWave();
  }
}

void readSignal() {
  for (int i = 0; i < WIDTH; i++) {
    int val = analogRead(ADC_PIN);
    data[i] = map(val, 0, 1023, 230, 30); 
    
    if (timeDelay > 0) {
      delayMicroseconds(timeDelay); 
    }
  }
}

void drawWave() {
  for (int i = 0; i < WIDTH - 1; i++) {
    if (oldData[i] != data[i] || oldData[i+1] != data[i+1]) {
      tft.drawLine(i, oldData[i], i + 1, oldData[i + 1], BLACK);
      if (oldData[i] == 130) tft.drawPixel(i, 130, DARKGREY); 
    }
    tft.drawLine(i, data[i], i + 1, data[i + 1], GREEN);
    oldData[i] = data[i];
  }
  oldData[WIDTH-1] = data[WIDTH-1];
}

void drawUI() {
  tft.fillRect(0, 0, WIDTH, 20, BLACK);
  tft.setTextSize(2);
  
  tft.setCursor(10, 5);
  tft.setTextColor(YELLOW);
  tft.print("Delay:");
  tft.print(timeDelay);
  tft.print("us");

  tft.setCursor(250, 5); 
  tft.setTextColor(hold ? RED : BLACK);
  tft.print("HOLD");
}

void drawGrid() {
  tft.drawRect(0, 25, 320, 215, WHITE); 
  for (int y = 65; y < 240; y += 40) {
    for (int x = 0; x < 320; x += 10) {
      tft.drawFastHLine(x, y, 2, DARKGREY);
    }
  }
  for (int x = 40; x < 320; x += 40) {
    for (int y = 25; y < 240; y += 10) {
      tft.drawFastVLine(x, y, 2, DARKGREY);
    }
  }
}
