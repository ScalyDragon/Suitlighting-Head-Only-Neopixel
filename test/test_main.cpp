//
// Created by Tobias W. on 15.02.22.
//
using namespace std;

#include <Arduino.h>
#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/TouchHandler/TouchHandler.h"
#include "../lib/NeopixelAnimator/NeopixelAnimator.h"
#include "../lib/WebsiteController/WebsiteController.h"
#include "../lib/WifiManager/WifiManager.h"
#include "../lib/PersistentStorage/PersistentStorage.h"
#include <unity.h>
#include "../lib/AZDeliveryESP32_pinMapping.h"

#define TOUCHPIN G12
#define NEOPIXELPIN G14
#define PIXELCOUNT 300

NeoPixelManager *ledManager;
TouchHandler *touchHandler;
NeopixelAnimator *animator;
WebsiteController *website;
WifiManager *wifi;
PersistentStorage *storage;

Color blue(0, 0, 255);
Color red(255, 0, 0);
Persistence persistentData;

void test_TouchHandler_objectCreation(void) {
    touchHandler = new TouchHandler(TOUCHPIN, &persistentData);
}

void test_TouchHandler_init(void) {
    touchHandler->init();
}

void test_TouchHandler_nonZeroValue(void) {
    TEST_ASSERT_GREATER_THAN_INT(0, touchHandler->getRAW());
}

void test_TouchHandler_thresholdPersistenceUpdated(void) {
    persistentData.touchThreshold = 10;
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(10,touchHandler->getTouchThreshold());
    persistentData.touchThreshold = 100;
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(100,touchHandler->getTouchThreshold());
}

void test_TouchHandler_noTouchDetected(void) {
    persistentData.touchThreshold = 10;
    touchHandler->loopHandler();
    delay(100);
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(persistentData.touch, false);
}

void test_TouchHandler_touchDetected(void) {
    persistentData.touchThreshold = 1000;
    touchHandler->loopHandler();
    delay(100);
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(persistentData.touch, true);
}

void test_TouchHandler_lockRelease(void) {
    touchHandler->loopHandler();
    TEST_ASSERT_EQUAL(true,persistentData.try_lock());
    persistentData.unlock();
}

void test_Persistance_Color_normalize(void) {
    Color color(255, 0, 0);
    TEST_ASSERT_EQUAL(1, color.normalized().r);
    Color color2(0, 255, 0);
    TEST_ASSERT_EQUAL(1, color2.normalized().g);
    Color color3(0, 0, 255);
    TEST_ASSERT_EQUAL(1, color3.normalized().b);
    Color color4(0.5f, 0, 0);
    TEST_ASSERT_EQUAL(1.0f, color4.normalized().r);
}

void isColorEqual(Color c1, Color c2) {
    TEST_ASSERT_EQUAL(c1.r, c2.r);
    TEST_ASSERT_EQUAL(c1.g, c2.g);
    TEST_ASSERT_EQUAL(c1.b, c2.b);
}

void test_Persistance_Color_toUint16(void) {
    Color c1(0xFFFF, 100, 0);
    Color c2(0xFFFF * 2, 100, 0);
    isColorEqual(Color(0xFFFF, 100, 0), c1.toUint16());
    isColorEqual(Color(0xFFFF, 100, 0), c2.toUint16());
}

void test_Persistence_Color_add(void) {
    Color c1(0xFFFF, 0, 0xFFFF);
    Color c2(0xFFFF, 0xFFFF, 0xFFFF);
    Color c3(0xFFFF, 100, 0xFFFF);
    isColorEqual(Color(0xFFFF * 2, 0xFFFF, 0xFFFF * 2), c1.add(c2));
    isColorEqual(Color(0xFFFF * 2, 100, 0xFFFF * 2), c1.add(c3));
}

void test_Persistance_Color_sub(void) {
    Color c1(255, 150, 255);
    Color c2(255, 200, 255);
    Color c3(100, 100, 100);
    isColorEqual(Color(0, -50.0f, 0), c1.sub(c2));
    isColorEqual(Color(155, 50, 155), c1.sub(c3));
}

void test_Persistance_Color_smul(void) {
    Color c1(255, 150, 200);
    isColorEqual(Color(510, 300, 400), c1.smul(2.0f));
    Color c2(100, 100, 100);
    isColorEqual(Color(50, 50, 50), c2.smul(0.5f));
}

void test_Persistence_Color_halfDivision(void) {
    Color c1 = Color(0.5f, 0, 0);
    TEST_ASSERT_EQUAL(1, 0.5f / c1.lengthf());
}

void test_Persistance_Color_length(void) {
    Color c1(100, 0, 0);
    TEST_ASSERT_EQUAL(100, c1.length());
}

void test_Persistance_Color_lengthf(void) {
    Color c1(100, 0, 0);
    TEST_ASSERT_EQUAL(100.0f, c1.lengthf());
    Color c2(0.5f, 0, 0);
    TEST_ASSERT_EQUAL(0.5f, c2.lengthf());
}

void test_Persistance_Color_equals(void) {
    Color c1(100, 0, 0);
    Color c2(0, 100, 0);
    Color c3(0, 100, 0);
    TEST_ASSERT_EQUAL(false, c1.equals(c2));
    TEST_ASSERT_EQUAL(true, c2.equals(c3));
}

void test_Persistance_Color_toUint8String(void) {
    Color c1(100, 50, 10);
    TEST_ASSERT_EQUAL_STRING("64320A", c1.toUint8String().c_str());
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
    ledManager = new NeoPixelManager(NEOPIXELPIN, PIXELCOUNT,1000);
}

void test_NeoPixelManager_init(void) {
    ledManager->init();
}

void test_NeoPixelManager_setPixel1to10(void) {
    ledManager->setMode(0);
    ledManager->setPixelArea(0, 5, blue);
    ledManager->loopHandler();
    isColorEqual(blue, ledManager->getCurrentColorBuffer()[0]);
    delay(1000);
    ledManager->setPixelArea(3, 10, red);
    ledManager->loopHandler();
    isColorEqual(red, ledManager->getCurrentColorBuffer()[3]);
    delay(1000);
}

void test_NeoPixelManager_getColorOfUint32(void) {
    Color color = ledManager->getColorOfUint32(0xddeeff);
    TEST_ASSERT_EQUAL(0xdd, color.r);
    TEST_ASSERT_EQUAL(0xee, color.g);
    TEST_ASSERT_EQUAL(0xff, color.b);
}

void isColorAreaEqual(Color *buffer, Color target, int start, int end) {
    for (int i = start; i < end; i++) {
        isColorEqual(target, buffer[i]);
    }
}

void test_NeoPixelManager_getTargetBufferValid(void) {
    ledManager->setPixelArea(0, 10, Color(0, 0, 0));
    isColorEqual(Color(0, 0, 0), ledManager->getTargetColorBuffer()[0]);
    ledManager->setPixelArea(0, 10, Color(0, 0, 100));
    isColorEqual(Color(0, 0, 100), ledManager->getTargetColorBuffer()[0]);
}

void test_NeoPixelManager_setArea(void) {
    ledManager->setMode(0);
    ledManager->setSmoothStepwidth(1.0f);
    ledManager->setPixelArea(0, 10, Color(0, 0, 0));
    ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 0), 0, 10);
    ledManager->setPixelArea(0, 10, Color(0, 0, 100));
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 0), 0, 10);
    ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 100), 0, 10);
}

void test_NeoPixelManager_fadeIn(void) {
    ledManager->setMode(0);
    ledManager->setPixelArea(0, 10, Color(0, 0, 0));
    ledManager->loopHandler();
    ledManager->setMode(1);
    ledManager->setSmoothStepwidth(1);
    ledManager->setPixelArea(0, 10, Color(0, 0, 100));
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 0), 0, 10);
    ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 1), 0, 10);
    for (int i = 1; i < 100; i++)
        ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 100), 0, 10);
}

void test_NeoPixelManager_slowFadeIn(void) {
    ledManager->setMode(0);
    ledManager->setPixelArea(0, 10, Color(0, 0, 0));
    ledManager->loopHandler();
    ledManager->setMode(1);
    ledManager->setSmoothStepwidth(0.5f);
    ledManager->setPixelArea(0, 10, Color(0, 0, 100));
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 0), 0, 10);
    ledManager->loopHandler();
    ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 1), 0, 10);
    for (int i = 1; i < 100; i++)
        ledManager->loopHandler();
    isColorAreaEqual(ledManager->getCurrentColorBuffer(), Color(0, 0, 50), 0, 10);
}

void test_NeoPixelManager_lockRelease(void){
    ledManager->loopHandler();
    TEST_ASSERT_EQUAL(true,persistentData.try_lock());
    persistentData.unlock();
}

void test_NeopixelAnimator_objectCreation(void) {
    animator = new NeopixelAnimator(ledManager, &persistentData);
}

void test_NeopixelAnimator_init(void) {
    animator->init();
}

void test_NeopixelAnimator_loop(void) {
    animator->loopHandler();
}

void test_NeopixelAnimator_setBoopColor(void) {
    animator->setBoopColor(Color(200, 0, 0));
    isColorEqual(Color(200, 0, 0), persistentData.boopColor);
}

void test_NeopixelAnimator_setIdleColor(void) {
    animator->setIdleColor(Color(0, 0, 200));
    isColorEqual(Color(0, 0, 200), persistentData.idleColor);
}

void test_NeoPixelAnimator_getCurrentTargetColor(void) {
    isColorEqual(Color(100, 0, 0), animator->getCurrentTargetColor());
}

void test_NeoPixelAnimator_lockRelease(void) {
    animator->loopHandler();
    TEST_ASSERT_EQUAL(true, persistentData.try_lock());
    persistentData.unlock();
}

void test_WebsiteController_objectCreation(void) {
    website = new WebsiteController(&persistentData);
}

void test_WebsiteController_init(void) {
    website->init();
}

void test_WebsiteController_loop(void) {
    website->loopHandler();
}

void test_WebsiteController_SPIFFScheckIfFileExists(void) {
    TEST_ASSERT_EQUAL(1, website->checkSPIFFS());
}

void test_WebsiteController_lockRelease(void) {
    website->loopHandler();
    TEST_ASSERT_EQUAL(true,persistentData.try_lock());
    persistentData.unlock();
}

void test_WifiManager_objectCreation(void) {
    wifi = new WifiManager();
}

void test_WifiManager_init(void) {
    wifi->init();
}

void test_WifiManager_loop(void) {
    wifi->loopHandler();
}

void test_WifiManager_lockRelease(void) {
    wifi->loopHandler();
    TEST_ASSERT_EQUAL(true,persistentData.try_lock());
    persistentData.unlock();
}

void test_PersistentStorage_objectCreation(void) {
    storage = new PersistentStorage(&persistentData);
}

void test_PersistentStorage_init(void) {
    storage->init();
}

void test_PersistentStorage_loop(void) {
    storage->loopHandler();
}

void test_PersistentStorage_savePersistenceToMemory(void) {
    persistentData.idleColor.setColor(Color(0, 255, 0));
    persistentData.boopColor.setColor(Color(0, 0, 255));
    persistentData.touchThreshold = 123;
    persistentData.fadespeed = 124;
    storage->savePersistenceToMemory();
}

void test_PersistentStorage_updatePersistenceFromMemory(void) {
    persistentData.idleColor.setColor(Color(255, 0, 0));
    persistentData.boopColor.setColor(Color(255,0,0));
    persistentData.touchThreshold = 1;
    persistentData.fadespeed = 2;
    storage->updatePersistenceFromMemory();
    isColorEqual(Color(0, 255, 0), persistentData.idleColor);
    isColorEqual(Color(0, 0, 255), persistentData.boopColor);
    TEST_ASSERT_EQUAL(123, persistentData.touchThreshold);
    TEST_ASSERT_EQUAL(124, persistentData.fadespeed);
}

void test_PersistentStorage_lockRelease(void){
    storage->loopHandler();
    TEST_ASSERT_EQUAL(true, persistentData.try_lock());
    persistentData.unlock();
}

void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_TouchHandler_objectCreation);
    RUN_TEST(test_WifiManager_objectCreation);
    RUN_TEST(test_NeoPixelManager_objectCreation);
    RUN_TEST(test_WebsiteController_objectCreation);
    RUN_TEST(test_NeopixelAnimator_objectCreation);
    RUN_TEST(test_PersistentStorage_objectCreation);
    RUN_TEST(test_PersistentStorage_init);
    RUN_TEST(test_TouchHandler_init);
    RUN_TEST(test_NeoPixelManager_init);
    RUN_TEST(test_WifiManager_init);
    RUN_TEST(test_WebsiteController_init);
    RUN_TEST(test_NeopixelAnimator_init);
    RUN_TEST(test_NeoPixelManager_lockRelease);
    RUN_TEST(test_TouchHandler_lockRelease);
    RUN_TEST(test_NeoPixelAnimator_lockRelease);
    RUN_TEST(test_WifiManager_lockRelease);
    RUN_TEST(test_WebsiteController_lockRelease);
    RUN_TEST(test_PersistentStorage_lockRelease);
    RUN_TEST(test_WifiManager_loop);
    RUN_TEST(test_WebsiteController_loop);
    RUN_TEST(test_WebsiteController_SPIFFScheckIfFileExists);
    RUN_TEST(test_Persistance_Color_normalize);
    RUN_TEST(test_Persistance_sadd16);
    RUN_TEST(test_Persistance_sadd32);
    RUN_TEST(test_Persistance_ssub16);
    RUN_TEST(test_Persistance_ssub32);
    RUN_TEST(test_Persistence_Color_add);
    RUN_TEST(test_Persistance_Color_sub);
    RUN_TEST(test_Persistance_Color_smul);
    RUN_TEST(test_Persistance_Color_normalize);
    RUN_TEST(test_Persistance_Color_length);
    RUN_TEST(test_Persistance_Color_lengthf);
    RUN_TEST(test_Persistance_Color_equals);
    RUN_TEST(test_Persistance_Color_toUint16);
    RUN_TEST(test_Persistence_Color_halfDivision);
    RUN_TEST(test_Persistance_Color_toUint8String);
    RUN_TEST(test_NeoPixelManager_getTargetBufferValid);
    RUN_TEST(test_NeopixelAnimator_setBoopColor);
    RUN_TEST(test_NeopixelAnimator_setIdleColor);
    RUN_TEST(test_NeopixelAnimator_loop);
    delay(500);
    RUN_TEST(test_TouchHandler_nonZeroValue);
    RUN_TEST(test_TouchHandler_thresholdPersistenceUpdated);
    RUN_TEST(test_TouchHandler_noTouchDetected);
    RUN_TEST(test_TouchHandler_touchDetected);
    RUN_TEST(test_NeoPixelManager_setPixel1to10);
    RUN_TEST(test_NeoPixelManager_getColorOfUint32);
    RUN_TEST(test_NeoPixelManager_setArea);
    RUN_TEST(test_NeoPixelManager_fadeIn);
    RUN_TEST(test_NeoPixelManager_slowFadeIn);
    RUN_TEST(test_PersistentStorage_loop);
    RUN_TEST(test_PersistentStorage_savePersistenceToMemory);
    RUN_TEST(test_PersistentStorage_updatePersistenceFromMemory);
    UNITY_END();
    ledManager->setSmoothStepwidth(1);
    persistentData.touchThreshold = 140;
    animator->setBoopColor(red);
    animator->setIdleColor(blue);

    Serial.begin(115200);
    //wifi->init();
    //website->init();
}

void loop() {
    ledManager->loopHandler();
    touchHandler->loopHandler();
    animator->loopHandler();
    wifi->loopHandler();
    website->loopHandler();
    delay(1);
}