#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Audio.h"

// ================== WiFi Config ==================
WiFi.begin(ssid,paswork);

// ================== TFT Display ==================
TFT_eSPI tft = TFT_eSPI();  // TFT_eSPI config trong platformio.ini

// ================== Link Radio ==================
const char* radioStream = "http://stream.radioparadise.com/aac-320";

// ================== Setup ==================
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("🔌 Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");
  Serial.println(WiFi.localIP());

  // Màn hình
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("ESP32-S3 Radio", 40, 100, 2);

  // ================== Audio Config ==================
  // ⚠️ Cấu hình pinout đúng cú pháp của AudioI2S v3.0.x
  Audio.setpinout(...)
  Audio.setVolume(15); // Âm lượng (0-21)
  
  if (Audio.connecttohost(radioStream)) {
    tft.drawString("Đang phát Radio...", 30, 130, 2);
  } else {
    tft.drawString("Không kết nối được stream!", 10, 130, 2);
  }
}

// ================== Loop ==================
void loop() {
  Audio.loop();
}

// ================== Debug Callback (tuỳ chọn) ==================
void audio_info(const char *info) {
  Serial.print("Audio Info: ");
  Serial.println(info);
}
