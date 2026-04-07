#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_RGBSTRIPMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_RGBSTRIPMANAGER_H

#include "../LEDStrip/LEDStrip.h"

class RGBStripManager : public LEDStrip {
public:
    RGBStripManager(int redPin, int greenPin, int bluePin, int looprateHz, bool invertOutput = false);

    void init() override;

protected:
    void updateStripFromBuffer() override;

private:
    int redPin;
    int greenPin;
    int bluePin;
    bool invertOutput;

    void writeColor(Color color);
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_RGBSTRIPMANAGER_H
