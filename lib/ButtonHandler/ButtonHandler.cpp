//
// Created by Tobias W. on 18.03.22.
//

#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(Persistence *persistenceIn, int pinNumber) {
    persistentData = persistenceIn;
    switchPin = pinNumber;
}

void ButtonHandler::init() {
    pinMode(switchPin, INPUT_PULLUP);
    configModeActive = !digitalRead(switchPin);
    refreshPersistence();
}

void ButtonHandler::loopHandler() {
    debouncedButtonRefresh();
    refreshPersistence();
}

void ButtonHandler::debouncedButtonRefresh() {
    refreshDebounceCounter();
    if (buttonPressed) {
        if (debounceCounter < -1 * DEBOUNCE_TRIGGER)
            buttonPressed = false;
    }
    else {
        if (debounceCounter > DEBOUNCE_TRIGGER)
            buttonPressed = true;
    }
}

void ButtonHandler::refreshDebounceCounter() {
    if(!digitalRead(switchPin))
        limitedSum(1);
    else
        limitedSum(-1);
}

void ButtonHandler::limitedSum(int toAdd) {
    debounceCounter = abs(debounceCounter + toAdd) < DEBOUNCE_STEPS ?
                      debounceCounter + toAdd : debounceCounter;
}

void ButtonHandler::refreshPersistence() {
    persistentData->lock();
    persistentData->buttonPressed = buttonPressed;
    persistentData->configMode = configModeActive;
    persistentData->unlock();
}