#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Audio.h"

// ================== Cấu hình WiFi ==================
const char* ssid     = "Bin§Bon";
const char* password = "khongcanpass";

// ================== Cấu hình màn hình ST7789 ==================
TFT_eSPI tft = TFT_eSPI();  // dùng config sẵn trong User_Setup_Select.h

// ================== Cấu hình Audio MAX98357A ==================
Audio audio;
#define I2S_DOUT  25 
#define I2S_BCLK  26
#define I2S_LRC   27

// ================== Link phát thử ==================
const char* radioStream = "https://stream.live.vc.bbcmedia.co.uk/bbc_world_service";

// ================== Hàm setup ==================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Khởi động màn hình
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.println("ESP32-S3 Radio");
  tft.setTextColor(TFT_WHITE);
  tft.println("Connecting WiFi...");

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
    if (++counter > 30) {
      tft.println(" WiFi failed!");
      return;
    }
  }
  tft.println("\nWiFi connected!");
  tft.println(WiFi.localIP());

  // Cấu hình âm thanh
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15); // âm lượng 0-21

  // Bắt đầu phát radio
  audio.connectToHost(radioStream);

  tft.setTextColor(TFT_GREEN);
  tft.println("Playing online radio...");
}

// ================== Vòng lặp ==================
void processLoop() {
  audio.processLoop();  // giữ luồng phát liên tục
}
