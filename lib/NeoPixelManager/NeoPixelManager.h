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
    NeoPixelManager(int dataPin, int pixelCount, int looprateHz);
    void init();
    void loopHandler();
    void setPixelArea(int start, int end, Color color);
    void setMode(int mode);
    void setSmoothStepwidth(float stepwidth);
    void setColorWait(float waitTimeSeconds); //NOTEST
    Color getColorOfUint32(uint32_t colorRAW);
    Color* getCurrentColorBuffer();
    Color* getTargetColorBuffer();
private:
    Adafruit_NeoPixel* pixels;
    int mode; //0 = INSTANT, 1 = SMOOTH, 2 = WAIT
    int dataPin;
    int pixelcount;
    float smoothStepwidth;
    Color* ledTargetStateBuffer;
    Color* ledCurrentStateBuffer;
    void updateLEDStateFromBuffer();
    void updateBufferWithStepwidth(float stepwidth);
    void updateNeopixelsFromBuffer();
    int colorWaitTimeRemain;
    int loopRateHz;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
