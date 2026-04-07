//
// Created by Tobias W. on 15.02.22.
//

#include "NeoPixelManager.h"


NeoPixelManager::NeoPixelManager(int dataPinIn, int pixelcountIn, int loopRateHzIn)
        : LEDStrip(pixelcountIn, loopRateHzIn) {
    dataPin = dataPinIn;
    pixels = new Adafruit_NeoPixel(pixelcount, dataPin, NEO_GRB + NEO_KHZ800);
}

NeoPixelManager::~NeoPixelManager() {
    delete pixels;
}

void NeoPixelManager::init() {
    pixels->begin();
    pixels->clear();
    pixels->show();
}

void NeoPixelManager::updateStripFromBuffer() {
    for (int i = 0; i < pixelcount; i++) {
        Color pixelColor = ledCurrentStateBuffer[i].toInt8();
        pixels->setPixelColor(i, pixelColor.r, pixelColor.g, pixelColor.b);
    }
    pixels->show();
}
