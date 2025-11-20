SmartDisplay ESP32-S3 (ST7789 240x240) - ready for GitHub Actions build

Files:
 - main.ino         -> Arduino sketch (WiFi embedded)
 - platformio.ini   -> PlatformIO config
 - .github/workflows/build.yml -> GitHub Actions to build firmware.bin

IMPORTANT:
 - Before building, ensure TFT_eSPI User_Setup.h in library matches:
    #define ST7789_DRIVER
    #define TFT_WIDTH 240
    #define TFT_HEIGHT 240
    #define TFT_MOSI 41
    #define TFT_SCLK 42
    #define TFT_DC   19
    #define TFT_RST  18
    #define TFT_CS    5
 - After pushing to GitHub, run the workflow in Actions and download artifact 'firmware'.

Security:
 - WiFi credentials are embedded into the binary. Keep the firmware secure.
