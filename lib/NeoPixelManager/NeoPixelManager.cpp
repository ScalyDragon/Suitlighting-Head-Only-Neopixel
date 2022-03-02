//
// Created by Tobias W. on 15.02.22.
//

#include "NeoPixelManager.h"
#include "../helperStructures.h"

NeoPixelManager::NeoPixelManager(int dataPin) {
    strip = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(dataPin, pixelcount);
}

void NeoPixelManager::init() {
    strip->Begin();
    strip->Show();
}

void NeoPixelManager::loopHandler() {
    strip->Show();
}

void NeoPixelManager::setPixelcount(int pixelcount_in) {
    pixelcount = pixelcount_in;
}

void NeoPixelManager::setPixelArea(int start, int end, RgbColor color) {
    for(int i = start; i < end; i++)
        strip->SetPixelColor(i,color);
}

