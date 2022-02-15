using namespace std;

#include <Arduino.h>
#include "../lib/AZDeliveryESP32_pinMapping.h"

#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/TouchHandler/TouchHandler.h"


#define NOSE_BOOP_PIN G25
#define STRIPDATAPIN G14
#define PIXELCOUNT 300

NeoPixelManager ledManager(STRIPDATAPIN);
TouchHandler touchHandler(NOSE_BOOP_PIN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(STRIPDATAPIN, PIXELCOUNT);

RgbColor blue(0, 0, 255);

void setup() {
    ledManager.init(&strip);
    touchHandler.init();
    ledManager.setPixelArea(0, 10, blue);
}

void loop() {
    while(true){
        ledManager.loopHandler();
        touchHandler.loopHandler();
    }
}