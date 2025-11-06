/*
  SmartDisplay ESP32-S3 Radio
  - For ST7789 240x240
  - MAX98357A I2S DAC
  - WiFi credentials embedded (SSID="Bin§Bon")
*/

#include <WiFi.h>
#include <Audio.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// === CONFIG - Embedded WiFi credentials ===
const char* ssid = "Bin§Bon";
const char* password = "khongcanpass";

// I2S pins for MAX98357A
#define I2S_BCLK 26
#define I2S_LRC 25
#define I2S_DOUT 22

// Button pin to change station (active LOW)
#define BTN_NEXT 0

// === ST7789 pins (defaults) ===
// MOSI = 23, SCLK = 18, DC = 2, RST = 4, CS = 5
// Make sure TFT_eSPI User_Setup.h matches these pins and TFT size 240x240

// === RADIO STATIONS ===
const char* stations[] = {
  "https://stream.radioparadise.com/mp3-192",
  "http://us1.internet-radio.com:8105/",
  "http://icecast.radiofrance.fr/fip-midfi.mp3"
};
int currentStation = 0;
int totalStations = sizeof(stations) / sizeof(stations[0]);

// Objects
Audio audio;
TFT_eSPI tft = TFT_eSPI();

// UI state
int barHeight[20];
unsigned long lastDraw = 0;
unsigned long lastDebounce = 0;
bool btnLastState = HIGH;

void setup() {
  Serial.begin(115200);
  delay(100);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // Welcome screen
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("ESP32-S3 Radio", 120, 40);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Initializing...", 120, 80);

  // WiFi connect (if credentials provided)
  if (strlen(ssid) > 0) {
    tft.drawString("Connecting WiFi...", 120, 120);
    WiFi.begin(ssid, password);
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
      delay(200);
      tft.drawString(".", 200, 120); // simple progress
    }
    if (WiFi.status() == WL_CONNECTED) {
      tft.fillScreen(TFT_BLACK);
      tft.drawString("WiFi connected", 120, 120);
      delay(800);
    } else {
      tft.fillScreen(TFT_BLACK);
      tft.drawString("WiFi failed", 120, 120);
      delay(800);
    }
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.drawString("No WiFi in config", 120, 120);
    delay(800);
  }

  // configure audio I2S pins for MAX98357A
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15); // 0..21

  pinMode(BTN_NEXT, INPUT_PULLUP);

  // prepare bars
  for (int i = 0; i < 20; i++) barHeight[i] = random(10, 80);

  // start first station (only if WiFi connected)
  if (WiFi.status() == WL_CONNECTED && strlen(stations[0]) > 0) {
    audio.connecttohost(stations[currentStation]);
  }

  drawInterface();
}

void loop() {
  audio.loop();

  // draw visualizer every 100ms
  if (millis() - lastDraw > 100) {
    drawBars();
    lastDraw = millis();
  }

  // Button to change station (debounced)
  bool btn = digitalRead(BTN_NEXT);
  if (btn != btnLastState) {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > 50) {
    if (btn == LOW && btnLastState == HIGH) {
      // pressed
      nextStation();
    }
  }
  btnLastState = btn;

  // Serial commands
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'n') nextStation();
    else if (c == 'p') prevStation();
    else if (c == 's') printStatus();
    else if (c == 'c') {
      if (WiFi.status() != WL_CONNECTED && strlen(ssid) > 0) {
        WiFi.begin(ssid, password);
      }
    }
  }
}

void drawInterface() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_SKYBLUE);
  tft.drawString("ESP32-S3 RADIO", 120, 18);

  tft.setTextColor(TFT_YELLOW);
  tft.drawString(String("Station ") + String(currentStation + 1), 120, 48);

  tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Press button or 'n' on Serial to change station", 120, 210);
}

void drawBars() {
  // clear area
  tft.fillRect(8, 70, 224, 140, TFT_BLACK);

  for (int i = 0; i < 20; i++) {
    int x = 10 + i * 11;
    int h = barHeight[i];
    int y = 200 - h;
    // draw bar
    tft.fillRect(x, y, 8, h, TFT_CYAN);
    // randomize next
    barHeight[i] = constrain((int)(h + random(-20, 30)), 8, 140);
  }

  // display station name/function
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  String name = "Station " + String(currentStation + 1);
  tft.drawString(name, 120, 40);
}

void nextStation() {
  currentStation = (currentStation + 1) % totalStations;
  audio.connecttohost(stations[currentStation]);
  drawInterface();
  Serial.println("Switched to station " + String(currentStation + 1));
}

void prevStation() {
  currentStation = (currentStation - 1 + totalStations) % totalStations;
  audio.connecttohost(stations[currentStation]);
  drawInterface();
  Serial.println("Switched to station " + String(currentStation + 1));
}

void printStatus() {
  Serial.println("WiFi status: " + String(WiFi.status()));
  Serial.println("Current station: " + String(currentStation + 1));
}
