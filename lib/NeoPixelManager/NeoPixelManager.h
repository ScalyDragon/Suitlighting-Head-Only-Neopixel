//
// Created by Tobias W. on 15.02.22.
//
using namespace std;
#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H

#include <NeoPixelBus.h>
#include "../AZDeliveryESP32_pinMapping.h"

#define STRIPDATAPIN G14
#define PIXELCOUNT 300

//Helper includes for IDE
#include "internal/NeoAvrMethod.h"
#include "internal/RgbColor.h"
#include "internal/NeoColorFeatures.h"
#include "internal/NeoEsp32I2sMethod.h"
#include "NeoPixelBus.h"

class NeoPixelManager {
public:
    NeoPixelManager(int dataPin);
    void setPixelcount(int pixelcount);
    void init(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip_In);
    void loopHandler();
    void setPixelArea(int start, int end, RgbColor color);
private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip;
    int pixelcount;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_NEOPIXELMANAGER_H
