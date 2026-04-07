//
// Created by Tobias W. on 15.02.22.
//
#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H

#include "../LEDStrip/LEDStrip.h"
#include "Adafruit_NeoPixel.h"


class NeoPixelManager : public LEDStrip {
public:
    NeoPixelManager(int dataPin, int pixelCount, int looprateHz);
    ~NeoPixelManager() override;

    void init() override;

protected:
    void updateStripFromBuffer() override;

private:
    Adafruit_NeoPixel* pixels;
    int dataPin;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
