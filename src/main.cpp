#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Audio.h"

// ================== WiFi Config ==================
const char* ssid     = "BinBon";       // ⚠️ Đổi tên SSID bỏ ký tự đặc biệt
const char* password = "khongcanpass";

// ================== TFT Display ==================
TFT_eSPI tft = TFT_eSPI();  // TFT_eSPI config trong platformio.ini

// ================== Audio MAX98357A ==================
Audio audio;
#define I2S_DOUT  25
#define I2S_BCLK  26
#define I2S_LRC   27

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
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15); // Âm lượng (0-21)
  
  if (audio.connecttohost(radioStream)) {
    tft.drawString("Đang phát Radio...", 30, 130, 2);
  } else {
    tft.drawString("Không kết nối được stream!", 10, 130, 2);
  }
}

// ================== Loop ==================
void loop() {
  audio.loop();
}

// ================== Debug Callback (tuỳ chọn) ==================
void audio_info(const char *info) {
  Serial.print("Audio Info: ");
  Serial.println(info);
}
