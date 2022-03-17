//
// Created by Tobias W. on 15.02.22.
//

#include "TouchHandler.h"
#include <Arduino.h>
#include "../helperStructures.h"

TouchHandler::TouchHandler(int dataPin_In, Persistence *persistenceDataIn) {
    persistenceData = persistenceDataIn;
    dataPin = dataPin_In;
}

static bool touchDetectedRAW;
static void interruptHandler(){
    touchDetectedRAW = true;
}

void TouchHandler::init() {
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
}

void TouchHandler::updateFromPersistence() {
    persistenceData->lock();
    if(persistenceData->touchThreshold != touchThreshold) {
        setThreshold(persistenceData->touchThreshold);
    }
    persistenceData->unlock();
}

void TouchHandler::pushToPersistence() {
    persistenceData->lock();
    persistenceData->touch              = touchDetected;
    persistenceData->unlock();
}

void TouchHandler::syncInterrupt() {
    touchDetected = touchDetectedRAW;
    pushToPersistence();
}

void TouchHandler::resetInterrupt() {
    touchDetectedRAW = false;
}

int TouchHandler::getTouchThreshold() {
    return touchThreshold;
}