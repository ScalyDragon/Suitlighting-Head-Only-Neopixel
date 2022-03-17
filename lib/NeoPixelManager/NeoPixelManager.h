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
    void setSmoothStepwidth(float stepwidth);
    Color getColorOfUint32(uint32_t colorRAW);
    Color* getCurrentColorBuffer();
    Color* getTargetColorBuffer();
private:
    Adafruit_NeoPixel* pixels;
    bool smooth;
    int dataPin;
    int pixelcount;
    float smoothStepwidth;
    Color* ledTargetStateBuffer;
    Color* ledCurrentStateBuffer;
    void updateLEDStateFromBuffer();
    void updateBufferWithStepwidth(float stepwidth);
    void updateNeopixelsFromBuffer();
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
