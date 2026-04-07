#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_LEDSTRIP_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_LEDSTRIP_H

#include "../helperStructures.h"

class LEDStrip {
public:
    LEDStrip(int pixelCount, int looprateHz);
    virtual ~LEDStrip();

    virtual void init() = 0;
    void loopHandler();
    void setPixelArea(int start, int end, Color color);
    void setMode(int mode);
    void setSmoothStepwidth(float stepwidth);
    void setColorWait(float waitTimeSeconds);
    Color getColorOfUint32(uint32_t colorRAW);
    Color *getCurrentColorBuffer();
    Color *getTargetColorBuffer();
    int getPixelCount() const;

protected:
    Color *ledTargetStateBuffer;
    Color *ledCurrentStateBuffer;
    int mode;
    int pixelcount;
    float smoothStepwidth;
    int colorWaitTimeRemain;
    int loopRateHz;

    void updateLEDStateFromBuffer();
    void updateBufferWithStepwidth(float stepwidth);
    virtual void updateStripFromBuffer() = 0;
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_LEDSTRIP_H
