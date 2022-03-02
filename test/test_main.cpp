//
// Created by Tobias W. on 15.02.22.
//
using namespace std;
#include <Arduino.h>
#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/TouchHandler/TouchHandler.h"
#include <unity.h>
#include "../lib/AZDeliveryESP32_pinMapping.h"

#define TOUCHPIN G12
#define NEOPIXELPIN G14
#define PIXELCOUNT

NeoPixelManager *ledManager;
TouchHandler *touchHandler;
RgbColor blue(0,0,255);
RgbColor red(255,0,0);
persistence persistentData;

void test_NeoPixelManager_objectCreation(void){
    ledManager = new NeoPixelManager(NEOPIXELPIN);
}

void test_NeoPixelManager_init(void){
    ledManager->init();
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
    touchHandler = new TouchHandler(TOUCHPIN);
}

void test_TouchHandler_init(void){
    touchHandler->init(&persistentData);
}

void test_TouchHandler_nonZeroValue(void){
    TEST_ASSERT_GREATER_THAN_INT(0, touchHandler->getRAW());
}

void test_TouchHandler_thresholdPersistenceUpdated(void){
    touchHandler->setThreshold(10);
    TEST_ASSERT_EQUAL(persistentData.touchThreshold, 10);
    touchHandler->setThreshold(100);
    TEST_ASSERT_EQUAL(persistentData.touchThreshold, 100);
}

void test_TouchHandler_noTouchDetected(void){
    touchHandler->setThreshold(10);
    delay(100);
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(persistentData.touch, false);
}

void test_TouchHandler_touchDetected(void) {
    touchHandler->setThreshold(1000);
    delay(100);
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(persistentData.touch, true);
}

void setup(){
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_NeoPixelManager_objectCreation);
    RUN_TEST(test_NeoPixelManager_init);
    delay(500);
    RUN_TEST(test_NeoPixelManager_setPixelCount);
    RUN_TEST(test_TouchHandler_objectCreation);
    RUN_TEST(test_TouchHandler_init);
}

void loop() {
    RUN_TEST(test_NeoPixelManager_setPixel1to10);
    RUN_TEST(test_TouchHandler_nonZeroValue);
    RUN_TEST(test_TouchHandler_thresholdPersistenceUpdated);
    RUN_TEST(test_TouchHandler_noTouchDetected);
    RUN_TEST(test_TouchHandler_touchDetected);
    UNITY_END();
}