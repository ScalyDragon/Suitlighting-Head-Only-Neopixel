#include "LEDStrip.h"


LEDStrip::LEDStrip(int pixelCountIn, int loopRateHzIn) {
    pixelcount = pixelCountIn;
    loopRateHz = loopRateHzIn;
    ledTargetStateBuffer = new Color[pixelcount]();
    ledCurrentStateBuffer = new Color[pixelcount]();
    mode = 0;
    smoothStepwidth = 1.0f;
    colorWaitTimeRemain = 0;
}

LEDStrip::~LEDStrip() {
    delete[] ledTargetStateBuffer;
    delete[] ledCurrentStateBuffer;
}

void LEDStrip::loopHandler() {
    updateLEDStateFromBuffer();
    updateStripFromBuffer();
}

void LEDStrip::updateLEDStateFromBuffer() {
    switch (mode) {
        case 0:
            updateBufferWithStepwidth(0xFFFF);
            break;
        case 1:
            updateBufferWithStepwidth(smoothStepwidth);
            break;
        case 2:
            if (--colorWaitTimeRemain <= 0) {
                updateBufferWithStepwidth(0xFFFF);
            }
            break;
        default:
            break;
    }
}

void LEDStrip::updateBufferWithStepwidth(float stepwidth) {
    for (int i = 0; i < pixelcount; i++) {
        if (!ledCurrentStateBuffer[i].equals(ledTargetStateBuffer[i])) {
            Color diffvec = ledTargetStateBuffer[i].sub(ledCurrentStateBuffer[i]);
            if (diffvec.lengthf() > stepwidth) {
                ledCurrentStateBuffer[i].setColor(
                        ledCurrentStateBuffer[i].add(diffvec.normalized().smul(stepwidth)));
            } else {
                ledCurrentStateBuffer[i].setColor(ledTargetStateBuffer[i]);
            }
        }
    }
}

Color *LEDStrip::getCurrentColorBuffer() {
    return ledCurrentStateBuffer;
}

Color *LEDStrip::getTargetColorBuffer() {
    return ledTargetStateBuffer;
}

void LEDStrip::setPixelArea(int start, int end, Color color) {
    if (start < 0) {
        start = 0;
    }
    if (end > pixelcount) {
        end = pixelcount;
    }
    if (start >= end) {
        return;
    }

    for (int i = start; i < end; i++) {
        ledTargetStateBuffer[i].setColor(color);
    }
}

void LEDStrip::setColorWait(float waitTimeSeconds) {
    colorWaitTimeRemain = roundl(waitTimeSeconds * loopRateHz);
}

void LEDStrip::setMode(int modeIn) {
    mode = modeIn;
}

void LEDStrip::setSmoothStepwidth(float stepwidthIn) {
    smoothStepwidth = stepwidthIn;
}

Color LEDStrip::getColorOfUint32(uint32_t colorRAW) {
    float r = (colorRAW >> 16) & 0xff;
    float g = (colorRAW >> 8) & 0xff;
    float b = colorRAW & 0xff;
    return {r, g, b};
}

int LEDStrip::getPixelCount() const {
    return pixelcount;
}
