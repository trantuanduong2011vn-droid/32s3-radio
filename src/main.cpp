#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <Audio.h>

TFT_eSPI tft = TFT_eSPI();
Audio audio;

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("ESP32-S3 Radio", 10, 50);

  WiFi.begin(ssid, password);
  tft.drawString("Connecting WiFi...", 10, 100);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi Connected!", 10, 100);

  audio.setPinout(25, 26, 27);  // LRCK, BCLK, DIN (chỉnh nếu cần)
  audio.setVolume(15);
  audio.connecttohost("https://icecast.radiofrance.fr/fip-midfi.mp3");
}

void loop() {
  audio.loop();
}
