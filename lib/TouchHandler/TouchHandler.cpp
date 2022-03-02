//
// Created by Tobias W. on 15.02.22.
//

#include "TouchHandler.h"
#include <Arduino.h>
#include "../helperStructures.h"

TouchHandler::TouchHandler(int dataPin_In) {
    dataPin = dataPin_In;
}

static bool touchDetectedRAW;
static void interruptHandler(){
    touchDetectedRAW = true;
}

void TouchHandler::init(Persistence *persistenceDataIn) {
    persistenceData = persistenceDataIn;
    updateFromPersistence();
    touchSetCycles(16384,0x1000);
    touchAttachInterrupt(dataPin,interruptHandler,touchThreshold);
}

void TouchHandler::loopHandler() {
    updateFromPersistence();
    syncInterrupt();
    resetInterrupt();
    pushToPersistence();
}

int TouchHandler::getRAW() {
    return touchRead(dataPin);
}

void TouchHandler::setThreshold(int thresholdIn) {
    touchThreshold = thresholdIn;
    touchAttachInterrupt(dataPin, interruptHandler, touchThreshold);
    pushToPersistence();
}

void TouchHandler::updateFromPersistence() {
    touchThreshold = persistenceData->touchThreshold;
}

void TouchHandler::pushToPersistence() {
    persistenceData->touchThreshold     = touchThreshold;
    persistenceData->touch              = touchDetected;
}

void TouchHandler::syncInterrupt() {
    touchDetected = touchDetectedRAW;
    pushToPersistence();
}

void TouchHandler::resetInterrupt() {
    touchDetectedRAW = false;
}