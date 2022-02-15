//
// Created by Tobias W. on 15.02.22.
//
using namespace std;
#include <Arduino.h>
#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/TouchHandler/TouchHandler.h"
#include <unity.h>
#include "../lib/AZDeliveryESP32_pinMapping.h"

NeoPixelManager *ledManager;
TouchHandler *touchHandler;
RgbColor blue(0,0,255);
RgbColor red(255,0,0);

void test_NeoPixelManager_objectCreation(void){
    ledManager = new NeoPixelManager(G14);
}

void test_NeoPixelManager_setPixelCount(void){
    ledManager->setPixelcount(300);
}

void test_NeoPixelManager_setPixel1to10(void){
    ledManager->setPixelArea(0,5,blue);
    ledManager->loopHandler();
    delay(1000);
    ledManager->setPixelArea(3,10,red);
    ledManager->loopHandler();
}

void test_TouchHandler_objectCreation(void){
    touchHandler = new TouchHandler(G25);
}

void setup(){
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_NeoPixelManager_objectCreation);
    delay(500);
    RUN_TEST(test_NeoPixelManager_setPixelCount);
}

void loop() {
    RUN_TEST(test_NeoPixelManager_setPixel1to10);
    UNITY_END();
}