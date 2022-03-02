//
// Created by Tobias W. on 15.02.22.
//

#include "NeoPixelManager.h"


NeoPixelManager::NeoPixelManager(int dataPinIn, int pixelcountIn) {
    dataPin = dataPinIn;
    pixelcount = pixelcountIn;
    pixels = new Adafruit_NeoPixel(pixelcount, dataPin, NEO_GRB + NEO_KHZ800);
    ledstateBuffer = new Color[pixelcount];
}

void NeoPixelManager::init() {
    pixels->begin();
    pixels->clear();
    pixels->show();
}

void NeoPixelManager::loopHandler() {
    pixels->show();
}

void NeoPixelManager::setPixelArea(int start, int end, Color color) {
    for (int i = start; i < end; i++) {
        ledstateBuffer[i] = color;
    }
}

void NeoPixelManager::setSmooth(bool smoothIn) {
    smoothIn = smooth;
}

Color NeoPixelManager::getColorOfUint32(uint32_t colorRAW) {
    int r = colorRAW >> 16;
    int g = colorRAW >> 8 & 0xff;
    int b = colorRAW & 0xff;
    Color returnValue(r, g, b);
    return returnValue;
}