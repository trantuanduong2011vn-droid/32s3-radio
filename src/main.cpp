#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <Audio.h>  // từ thư viện ESP32-audioI2S

// Màn hình TFT
TFT_eSPI tft = TFT_eSPI();

// Cấu hình WiFi
const char* ssid = "Bin§Bon";
const char* password = "khongcanpass";

// Module âm thanh MAX98357A
Audio audio;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("ESP32-S3 ST7789 Player", 20, 100);

  WiFi.begin(ssid, password);
  tft.drawString("Connecting WiFi...", 20, 130);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi Connected!", 20, 120);

  // Khởi tạo audio
  audio.setPinout(25, 26, 27); // LRCK, BCLK, DIN — chỉnh theo wiring của bạn
  audio.setVolume(15);
  audio.connecttohost("https://icecast.radiofrance.fr/fip-midfi.mp3");
}

void loop() {
  audio.loop();
}
