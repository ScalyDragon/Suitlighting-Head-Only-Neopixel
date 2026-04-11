using namespace std;

#include <Arduino.h>
#include <WebsiteController.h>
#include <WifiManager.h>
#include "../lib/DmxManager/DmxManager.h"
#include "../lib/BoardPinMapping.h"

#include "../lib/FanController/FanController.h"
#include "../lib/LEDStrip/LEDStrip.h"
#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/RGBStripManager/RGBStripManager.h"
#include "../lib/TouchHandler/TouchHandler.h"
#include "../lib/helperStructures.h"
#include "../lib/NeopixelAnimator/NeopixelAnimator.h"
#include "../lib/PersistentStorage/PersistentStorage.h"
#include "../lib/ButtonHandler/ButtonHandler.h"


#define NOSE_BOOP_PIN IO4
#define STRIPDATAPIN G14
#define LED_GREEN_PIN IO17
#define LED_RED_PIN IO16
#define LED_BLUE_PIN IO21
#define BUTTONPIN IO27
#define FAN_PWM_PIN IO19
#define FAN_TACH_PIN IO18
#define PIXELCOUNT 300

LEDStrip *ledManager;
TouchHandler *touchHandler;
NeopixelAnimator *animator;
WebsiteController *website;
WifiManager *wifi;
DmxManager *dmx;
PersistentStorage *storage;
ButtonHandler *button;
FanController *fan;

Persistence persistentData;

void createAllObjects() {
    touchHandler = new TouchHandler(NOSE_BOOP_PIN, &persistentData);
    wifi = new WifiManager(&persistentData);
    dmx = new DmxManager(&persistentData);
    //ledManager = new NeoPixelManager(STRIPDATAPIN, PIXELCOUNT,1000);
    ledManager = new RGBStripManager(LED_RED_PIN,LED_GREEN_PIN,LED_BLUE_PIN,5000,true);
    website = new WebsiteController(&persistentData);
    animator = new NeopixelAnimator(ledManager, &persistentData);
    storage = new PersistentStorage(&persistentData);
    button = new ButtonHandler(&persistentData, BUTTONPIN);
    fan = FAN_PWM_PIN >= 0 ? new FanController(&persistentData, FAN_PWM_PIN, FAN_TACH_PIN) : nullptr;
}

void initAll() {
    storage->init();
    touchHandler->init();
    ledManager->init();
    button->init();
    if (fan != nullptr) {
        fan->init();
    }
    Serial.println(persistentData.configMode);
    wifi->init();
    dmx->init();
    if (persistentData.configMode) {
        website->init();
    }
    animator->init();
}

void printCurrentConfig() {
    Serial.println(persistentData.idleColor.toUint8String());
    Serial.println(persistentData.boopColor.toUint8String());
    Serial.println(persistentData.touchThreshold);
    Serial.println(persistentData.fadespeed);
    Serial.println(persistentData.fanspeed);
}

void loopHandlers() {
    ledManager->loopHandler();
    touchHandler->loopHandler();
    animator->loopHandler();
    if (fan != nullptr) {
        fan->loopHandler();
    }
    wifi->loopHandler();
    dmx->loopHandler();
    if (persistentData.configMode) {
        website->loopHandler();
    }
    if (persistentData.savePersistentToEMMC) {
        printCurrentConfig();
    }
    storage->loopHandler();
    button->loopHandler();
}

void setup() {
    Serial.begin(115200);
    Serial.print("Starting");
    createAllObjects();
    Serial.print('.');
    initAll();
    Serial.print('.');
    Serial.println("Done!");
    printCurrentConfig();
}

void loop() {
    while (true) {
        loopHandlers();
        delay(1);
    }
}
