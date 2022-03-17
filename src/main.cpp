using namespace std;

#include <Arduino.h>
#include <WebsiteController.h>
#include <WifiManager.h>
#include "../lib/AZDeliveryESP32_pinMapping.h"

#include "../lib/NeoPixelManager/NeoPixelManager.h"
#include "../lib/TouchHandler/TouchHandler.h"
#include "../lib/helperStructures.h"
#include "../lib/NeopixelAnimator/NeopixelAnimator.h"
#include "../lib/PersistentStorage/PersistentStorage.h"


#define NOSE_BOOP_PIN G12
#define STRIPDATAPIN G14
#define PIXELCOUNT 300

NeoPixelManager *ledManager;
TouchHandler *touchHandler;
NeopixelAnimator *animator;
WebsiteController *website;
WifiManager *wifi;
PersistentStorage *storage;

Persistence persistentData;

void createAllObjects(){
    touchHandler = new TouchHandler(NOSE_BOOP_PIN, &persistentData);
    wifi = new WifiManager();
    ledManager = new NeoPixelManager(STRIPDATAPIN, PIXELCOUNT);
    website = new WebsiteController(&persistentData);
    animator = new NeopixelAnimator(ledManager,&persistentData);
    storage = new PersistentStorage(&persistentData);
}

void initAll(){
    storage->init();
    touchHandler->init();
    ledManager->init();
    wifi->init();
    website->init();
    animator->init();
}

void printCurrentConfig() {
    Serial.println(persistentData.idleColor.toUint8String());
    Serial.println(persistentData.boopColor.toUint8String());
    Serial.println(persistentData.touchThreshold);
    Serial.println(persistentData.fadespeed);
}

void loopHandlers() {
    ledManager->loopHandler();
    touchHandler->loopHandler();
    animator->loopHandler();
    wifi->loopHandler();
    website->loopHandler();
    if(persistentData.savePersistentToEMMC)
        printCurrentConfig();
    storage->loopHandler();
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
    while(true){
        loopHandlers();
        delay(1);
    }
}