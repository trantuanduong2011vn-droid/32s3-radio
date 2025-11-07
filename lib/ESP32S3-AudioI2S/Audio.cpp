#include "Audio.h"
void Audio::begin() { Serial.println("Audio started"); }
void Audio::play(const char* url) { Serial.printf("Playing: %s\n", url); }
void Audio::stop() { Serial.println("Audio stopped"); }
