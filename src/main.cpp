#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Audio.h>
#include <TimeLib.h>

#define WIFI_SSID "Bin§Bon"
#define WIFI_PASS "khongcanpass"

TFT_eSPI tft = TFT_eSPI();  // Màn hình ST7789
Audio audio;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  tft.println("Đang kết nối WiFi...");
  int c = 0;
  while (WiFi.status() != WL_CONNECTED && c < 30) {
    delay(500);
    tft.print(".");
    c++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    tft.println("\n✅ Kết nối WiFi thành công!");
  } else {
    tft.println("\n❌ Không thể kết nối WiFi.");
  }
}

void setup() {
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.println("ESP32-S3 Smart Display");
  
  connectWiFi();

  // Cấu hình âm thanh I2S
  audio.setPinout(40, 41, 42);  // BCLK, LRC, DOUT
  audio.setVolume(10);          // Âm lượng 0-21
  if (WiFi.status() == WL_CONNECTED) {
    // URL stream radio online (bạn có thể đổi)
    audio.connecttohost("http://stream.live.vc.bbcmedia.co.uk/bbc_radio_one");
    tft.println("🎵 Đang phát nhạc online...");
  }
}

void loop() {
  audio.loop();

  // Hiển thị giờ thực
  tft.setCursor(0, 200);
  tft.setTextSize(2);
  tft.printf("Time: %02d:%02d:%02d", hour(), minute(), second());
  delay(1000);
}
