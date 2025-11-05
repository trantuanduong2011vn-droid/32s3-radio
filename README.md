SmartDisplay_ESP32S3_ST7789_Radio_GHA

This project is pre-configured to build firmware.bin for ESP32-S3 with PlatformIO.
It includes a GitHub Actions workflow so if you upload this project to a GitHub repo
and push to the main branch, GitHub Actions will build the firmware and produce
an artifact "firmware.bin" you can download.

Steps to use:
1. Create a new GitHub repository.
2. Upload all files from this ZIP to the repo (or push via git).
3. Ensure branch 'main' exists, then push.
4. Go to Actions tab in your repo -> run the "Build ESP32-S3 Firmware" workflow (or wait on push).
5. After the workflow finishes, download the artifact 'firmware.bin' from the workflow run.
6. Flash firmware.bin to your ESP32-S3 at address 0x0 via ESP Flash Download Tool or esptool.

Notes:
- WiFi credentials are embedded (SSID=Bin§Bon).
- TFT_eSPI User_Setup.h must match your ST7789 wiring; the build expects you to configure the library before compiling if needed.
- If you need me to change any code before upload, tell me now and I will update the project files in this ZIP.
