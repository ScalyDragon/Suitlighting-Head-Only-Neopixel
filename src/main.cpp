#include <Arduino.h>
#include <NeoPixelBus.h>
#include <FastLED.h>
#include "AZDeliveryESP32_pinMapping.h"


#define NOSE_BOOP_PIN NC
#define STRIPDATAPIN G14
#define PIXELCOUNTPERSTRIP 300

RgbColor red(255,0,0);
RgbColor green(0,255,0);
RgbColor blue(0,0,255);
RgbColor white(255);
RgbColor pink(240,10,200);
RgbColor gelb(255,220,10);
RgbColor schwarz(0);


NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PIXELCOUNTPERSTRIP, STRIPDATAPIN);

void setup() {
  strip.Begin();
  strip.Show();
}

void loop() {

}