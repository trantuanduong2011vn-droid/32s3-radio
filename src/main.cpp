#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <Audio.h>  // Thư viện âm thanh

// Cấu hình màn hình
TFT_eSPI tft = TFT_eSPI();

// Cấu hình WiFi
const char* ssid = "Bin§Bon";
const char* password = "khongcanpass";

// Đối tượng Audio (MAX98357A)
Audio audio;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("ESP32-S3 Internet Radio", 10, 50);

  WiFi.begin(ssid, password);
  tft.drawString("Connecting WiFi...", 10, 100);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi Connected!", 10, 100);

  // Thiết lập chân I2S của module âm thanh
  audio.setPinout(25, 26, 27); // LRCK, BCLK, DIN — chỉnh lại nếu cần
  audio.setVolume(15);

  // Phát nhạc trực tuyến
  audio.connecttohost("https://icecast.radiofrance.fr/fip-midfi.mp3");
}

void loop() {
  audio.loop();
}
