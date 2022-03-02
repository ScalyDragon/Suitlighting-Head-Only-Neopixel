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
#define PIXELCOUNT 300

NeoPixelManager *ledManager;
TouchHandler *touchHandler;
Color blue(0, 0, 255);
Color red(255, 0, 0);
Persistence persistentData;

void test_TouchHandler_objectCreation(void) {
    touchHandler = new TouchHandler(TOUCHPIN);
}

void test_TouchHandler_init(void) {
    touchHandler->init(&persistentData);
}

void test_TouchHandler_nonZeroValue(void) {
    TEST_ASSERT_GREATER_THAN_INT(0, touchHandler->getRAW());
}

void test_TouchHandler_thresholdPersistenceUpdated(void) {
    touchHandler->setThreshold(10);
    TEST_ASSERT_EQUAL(persistentData.touchThreshold, 10);
    touchHandler->setThreshold(100);
    TEST_ASSERT_EQUAL(persistentData.touchThreshold, 100);
}

void test_TouchHandler_noTouchDetected(void) {
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

void test_Persistance_Color_normalize(void) {
    Color color(255, 0, 0);
    TEST_ASSERT_EQUAL(1, color.normalized().r);
    Color color2(0, 255, 0);
    TEST_ASSERT_EQUAL(1, color2.normalized().g);
    Color color3(0, 0, 255);
    TEST_ASSERT_EQUAL(1, color3.normalized().b);
}

void isColorEqual(Color c1, Color c2) {
    TEST_ASSERT_EQUAL(c1.r, c2.r);
    TEST_ASSERT_EQUAL(c1.g, c2.g);
    TEST_ASSERT_EQUAL(c1.b, c2.b);
}

void test_Persistence_Color_add(void) {
    Color c1(0xFFFF, 0, 0xFFFF);
    Color c2(0xFFFF, 0xFFFF, 0xFFFF);
    Color c3(0xFFFF, 100, 0xFFFF);
    isColorEqual(Color(0xFFFF, 0xFFFF, 0xFFFF), c1.add(c2));
    isColorEqual(Color(0xFFFF, 100, 0xFFFF), c1.add(c3));
}

void test_Persistance_Color_sub(void) {
    Color c1(255, 150, 255);
    Color c2(255, 200, 255);
    Color c3(100, 100, 100);
    isColorEqual(Color(0, 0, 0), c1.sub(c2));
    isColorEqual(Color(155, 50, 155), c1.sub(c3));
}

void test_Persistance_sadd16(void) {
    TEST_ASSERT_EQUAL(0xFFFF, sadd16(0xFFFF, 1));
    TEST_ASSERT_EQUAL(100, sadd16(75, 25));
}

void test_Persistance_sadd32(void) {
    TEST_ASSERT_EQUAL(0xFFFFFFFF, sadd32(0xFFFFFFFF, 1));
    TEST_ASSERT_EQUAL(100, sadd32(75, 25));
}

void test_Persistance_ssub16(void) {
    TEST_ASSERT_EQUAL(0, ssub16(100, 1000));
    TEST_ASSERT_EQUAL(10, ssub16(50, 40));
}

void test_Persistance_ssub32(void) {
    TEST_ASSERT_EQUAL(0, ssub32(100, 1000));
    TEST_ASSERT_EQUAL(10, ssub32(50, 40));
}

void test_NeoPixelManager_objectCreation(void) {
    ledManager = new NeoPixelManager(NEOPIXELPIN, PIXELCOUNT);
}

void test_NeoPixelManager_init(void) {
    ledManager->init();
}

void test_NeoPixelManager_setPixel1to10(void) {
    ledManager->setPixelArea(0, 5, blue);
    ledManager->loopHandler();
    delay(1000);
    ledManager->setPixelArea(3, 10, red);
    ledManager->loopHandler();
}

void test_NeoPixelManager_getColorOfUint32(void) {
    Color color = ledManager->getColorOfUint32(0xddeeff);
    TEST_ASSERT_EQUAL(0xdd, color.r);
    TEST_ASSERT_EQUAL(0xee, color.g);
    TEST_ASSERT_EQUAL(0xff, color.b);
}

void test_NeoPixelManager_fadeIn(void) {
    ledManager->setSmooth(true);
}

void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_TouchHandler_objectCreation);
    RUN_TEST(test_TouchHandler_init);
    RUN_TEST(test_NeoPixelManager_objectCreation);
    RUN_TEST(test_NeoPixelManager_init);
    RUN_TEST(test_Persistance_Color_normalize);
    RUN_TEST(test_Persistance_sadd16);
    RUN_TEST(test_Persistance_sadd32);
    RUN_TEST(test_Persistance_ssub16);
    RUN_TEST(test_Persistance_ssub32);
    RUN_TEST(test_Persistence_Color_add);
    RUN_TEST(test_Persistance_Color_sub);
    delay(500);
}

void loop() {
    RUN_TEST(test_TouchHandler_nonZeroValue);
    RUN_TEST(test_TouchHandler_thresholdPersistenceUpdated);
    RUN_TEST(test_TouchHandler_noTouchDetected);
    RUN_TEST(test_TouchHandler_touchDetected);
    RUN_TEST(test_NeoPixelManager_setPixel1to10);
    RUN_TEST(test_NeoPixelManager_getColorOfUint32);
    UNITY_END();
    exit(0);
}