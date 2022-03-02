//
// Created by Tobias W. on 15.02.22.
//
using namespace std;
#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H

#include "../AZDeliveryESP32_pinMapping.h"
#include "../helperStructures.h"
#include "Adafruit_NeoPixel.h"


class NeoPixelManager {
public:
    NeoPixelManager(int dataPin, int pixelCount);
    void init();
    void loopHandler();
    void setPixelArea(int start, int end, Color color);
    void setSmooth(bool smooth);
    Color getColorOfUint32(uint32_t colorRAW);
private:
    Adafruit_NeoPixel* pixels;
    bool smooth;
    int dataPin;
    int pixelcount;
    Color* ledstateBuffer;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
