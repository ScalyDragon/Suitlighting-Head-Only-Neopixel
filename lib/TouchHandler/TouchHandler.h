//
// Created by Tobias W. on 15.02.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H


#include <stdint.h>
#include <atomic>
#include "../helperStructures.h"

class TouchHandler {
public:
    TouchHandler(int dataPin_In, Persistence *persistenceDataIn);
    void init();
    void loopHandler();
    int getRAW();
    int getTouchThreshold();
private:
    int dataPin;
    uint16_t touchThreshold = 0;
    uint16_t interruptThreshold = UINT16_MAX;
    bool touchDetected = false;
    bool adaptiveTouch = true;
    bool initialized = false;
    bool baselineReady = false;
    bool continuousTouchTimerRunning = false;
    uint32_t continuousTouchStartedAt = 0;
    float touchBaseline = 0.0f;
    float configuredTouchDistance = 0.0f;
    Persistence* persistenceData;
    void updateFromPersistence();
    void pushToPersistence(int rawValue);
    void calibrateBaseline();
    void configureAdaptiveDistance();
    bool detectAdaptiveTouch(int rawValue);
    bool handleContinuousTouchTimeout();
    void updateAdaptiveBaseline(int rawValue, bool touchActive);
    void refreshInterruptThreshold();
    void resetInterrupt();
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
