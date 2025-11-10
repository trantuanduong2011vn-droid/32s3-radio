#include <WiFi.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Audio.h"

// =======================================================================
// PHẠM VI TOÀN CỤC (GLOBAL SCOPE) - FIX LỖI PHẠM VI
// =======================================================================

// ⚠️ CẤU HÌNH WIFI & STREAM (THAY THẾ BẰNG THÔNG TIN CỦA BẠN)
const char* ssid = "Big§Bon"; 
const char* password = "khongcanpass";
const char *radioStream = "http://radio.stream.url/here"; 

// ⚠️ CẤU HÌNH CHÂN TFT SPI AN TOÀN (FIX LỖI CRASH COLD BOOT)
// Sử dụng các chân an toàn: GPIO 9, 37, 38, 41, 42
#define ST7789_DRIVER   1
#define TFT_WIDTH       240
#define TFT_HEIGHT      240
#define TFT_MOSI        41 // SCL/SDA
#define TFT_SCLK        42 // SCL/SDA
#define TFT_DC          19
#define TFT_RST         18 
#define TFT_CS          5 

// KHAI BÁO ĐỐI TƯỢNG (OBJECTS)
TFT_eSPI tft = TFT_eSPI();
Audio myAudio; // Tên đối tượng là 'myAudio' để tránh nhầm lẫn với tên Class 'Audio'
// =======================================================================


void setup() {
    Serial.begin(115200);

    // ================== KHỞI TẠO TFT ==================
    // Cấu hình chân an toàn đã giúp chip khởi động
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Connecting...", 20, 20, 4);
    
    // ================== KHỞI TẠO WIFI ==================
    Serial.print("Connecting to WiFi ");
    Serial.print(ssid);
    
    WiFi.begin(ssid, password);
    
    // Vòng lặp chờ kết nối WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    tft.drawString("WiFi OK!", 20, 40, 2);

    // ================== KHỞI TẠO AUDIO (CÚ PHÁP V2.0.4) ==================
    // 🛑 KHÔNG dùng setpinout() để sử dụng I2S nối cứng trên mainboard

    // Sử dụng cú pháp chữ thường đã xác nhận hoạt động
    myAudio.setVolume(15); 

    // Sử dụng cú pháp chữ thường đã xác nhận hoạt động
    if (myAudio.connecttohost(radioStream)) { 
        Serial.println("Connected to stream.");
    } else {
        Serial.println("Connection failed.");
    }
}

void loop() {
    // Sử dụng cú pháp chữ thường đã xác nhận hoạt động
    myAudio.loop(); 
    delay(1);
}
