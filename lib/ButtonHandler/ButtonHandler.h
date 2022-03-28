//
// Created by Tobias W. on 18.03.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BUTTONHANDLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BUTTONHANDLER_H

#include "../helperStructures.h"
#include "Arduino.h"

#define DEBOUNCE_STEPS 10
#define DEBOUNCE_TRIGGER 8

class ButtonHandler {
public:
    ButtonHandler(Persistence * persistenceIn, int pinNumber);
    void init();
    void loopHandler();
private:
    Persistence * persistentData;
    int switchPin;
    bool configModeActive;
    short debounceCounter = 0;
    bool buttonPressed;
    void debouncedButtonRefresh();
    void refreshDebounceCounter();
    void limitedSum(int toAdd);
    void refreshPersistence();
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BUTTONHANDLER_H
