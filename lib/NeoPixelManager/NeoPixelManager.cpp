//
// Created by Tobias W. on 15.02.22.
//

#include "NeoPixelManager.h"


NeoPixelManager::NeoPixelManager(int dataPinIn, int pixelcountIn) {
    dataPin = dataPinIn;
    pixelcount = pixelcountIn;
    pixels = new Adafruit_NeoPixel(pixelcount, dataPin, NEO_GRB + NEO_KHZ800);
    ledTargetStateBuffer = new Color[pixelcount]();
    ledCurrentStateBuffer = new Color[pixelcount]();
    smooth = false;
    smoothStepwidth = 1;
}

void NeoPixelManager::init() {
    pixels->begin();
    pixels->clear();
    pixels->show();
}

void NeoPixelManager::loopHandler() {
    updateLEDStateFromBuffer();
    updateNeopixelsFromBuffer();
}

void NeoPixelManager::updateNeopixelsFromBuffer() {
    for(int i = 0; i < pixelcount; i++){
        pixels->setPixelColor(i,
                              ledCurrentStateBuffer[i].toInt8().r,
                              ledCurrentStateBuffer[i].toInt8().g,
                              ledCurrentStateBuffer[i].toInt8().b);
    }
    pixels->show();
}

void NeoPixelManager::updateLEDStateFromBuffer() {
    if (smooth)
        updateBufferWithStepwidth(smoothStepwidth);
    else
        updateBufferWithStepwidth(0xFFFF);
}

void NeoPixelManager::updateBufferWithStepwidth(float stepwidth) {
    for (int i = 0; i < pixelcount; i++) {
        if (!ledCurrentStateBuffer[i].equals(ledTargetStateBuffer[i])) {
            Color diffvec = ledTargetStateBuffer[i].sub(ledCurrentStateBuffer[i]);
            if (diffvec.lengthf() > stepwidth)
                ledCurrentStateBuffer[i].setColor(ledCurrentStateBuffer[i].add(diffvec.normalized().smul(stepwidth)));
            else
                ledCurrentStateBuffer[i].setColor(ledTargetStateBuffer[i]);
        }
    }
}

Color *NeoPixelManager::getCurrentColorBuffer() {
    return ledCurrentStateBuffer;
}

Color *NeoPixelManager::getTargetColorBuffer() {
    return ledTargetStateBuffer;
}

void NeoPixelManager::setPixelArea(int start, int end, Color color) {
    for (int i = start; i < end; i++) {
        ledTargetStateBuffer[i].setColor(color);
    }
}

void NeoPixelManager::setSmooth(bool smoothIn) {
    smooth = smoothIn;
}

void NeoPixelManager::setSmoothStepwidth(float stepwidthIn) {
    smoothStepwidth = stepwidthIn;
}

Color NeoPixelManager::getColorOfUint32(uint32_t colorRAW) {
    int r = colorRAW >> 16;
    int g = colorRAW >> 8 & 0xff;
    int b = colorRAW & 0xff;
    Color returnValue(r, g, b);
    return returnValue;
}