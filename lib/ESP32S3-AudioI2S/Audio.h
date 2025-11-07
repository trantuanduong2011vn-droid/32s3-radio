// ESP32-AudioI2S header stub
#ifndef AUDIO_H
#define AUDIO_H
#include <Arduino.h>
class Audio {
public:
  void begin();
  void play(const char* url);
  void stop();
};
#endif
