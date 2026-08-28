//
// Created by Tobias W. on 15.02.22.
//

#include "TouchHandler.h"
#include <Arduino.h>
#include "../helperStructures.h"

static constexpr uint8_t BASELINE_SAMPLE_COUNT = 16;
static constexpr float MAX_SENSITIVITY = 255.0f;
static constexpr float MIN_TOUCH_DISTANCE = 1.0f;
static constexpr float MAX_TOUCH_DISTANCE_RATIO = 0.5f;
static constexpr float RELEASE_DISTANCE_RATIO = 0.65f;
static constexpr float BASELINE_RISE_ALPHA = 0.02f;
static constexpr float BASELINE_FALL_ALPHA = 0.001f;
static constexpr uint32_t CONTINUOUS_TOUCH_TIMEOUT_MS = 60000UL;

TouchHandler::TouchHandler(int dataPin_In, Persistence *persistenceDataIn) {
    persistenceData = persistenceDataIn;
    dataPin = dataPin_In;
}

static volatile bool touchDetectedRAW;
static void interruptHandler(){
    touchDetectedRAW = true;
}

void TouchHandler::init() {
    touchSetCycles(16384,0x1000);
    updateFromPersistence();
    calibrateBaseline();
    configureAdaptiveDistance();
    initialized = true;
    refreshInterruptThreshold();
}

void TouchHandler::loopHandler() {
    updateFromPersistence();
    int rawValue = getRAW();
    bool interruptDetected = touchDetectedRAW;
    resetInterrupt();

    if (adaptiveTouch) {
        touchDetected = interruptDetected || detectAdaptiveTouch(rawValue);
        bool baselineRecalibrated = handleContinuousTouchTimeout();
        if (!baselineRecalibrated) {
            updateAdaptiveBaseline(rawValue, touchDetected);
        }
        refreshInterruptThreshold();
    } else {
        continuousTouchTimerRunning = false;
        touchDetected = interruptDetected || rawValue < touchThreshold;
    }

    pushToPersistence(rawValue);
}

int TouchHandler::getRAW() {
    return touchRead(dataPin);
}

void TouchHandler::updateFromPersistence() {
    uint16_t nextThreshold;
    bool nextAdaptiveTouch;

    persistenceData->lock();
    nextThreshold = persistenceData->touchThreshold;
    nextAdaptiveTouch = persistenceData->adaptiveTouch;
    persistenceData->unlock();

    bool thresholdChanged = nextThreshold != touchThreshold;
    bool adaptiveModeChanged = nextAdaptiveTouch != adaptiveTouch;
    touchThreshold = nextThreshold;
    adaptiveTouch = nextAdaptiveTouch;

    if (!initialized) {
        return;
    }

    if (adaptiveModeChanged && adaptiveTouch) {
        calibrateBaseline();
    }
    if (adaptiveTouch && (thresholdChanged || adaptiveModeChanged)) {
        configureAdaptiveDistance();
    }
    if (thresholdChanged || adaptiveModeChanged) {
        touchDetected = false;
        continuousTouchTimerRunning = false;
        resetInterrupt();
        refreshInterruptThreshold();
    }
}

void TouchHandler::pushToPersistence(int rawValue) {
    persistenceData->lock();
    persistenceData->touch = touchDetected;
    persistenceData->touchRawValue = satf16(rawValue);
    persistenceData->touchEffectiveThreshold = interruptThreshold;
    persistenceData->unlock();
}

void TouchHandler::calibrateBaseline() {
    uint32_t rawSum = 0;
    for (uint8_t i = 0; i < BASELINE_SAMPLE_COUNT; i++) {
        rawSum += touchRead(dataPin);
        delay(1);
    }
    touchBaseline = static_cast<float>(rawSum) / BASELINE_SAMPLE_COUNT;
    baselineReady = true;
    continuousTouchTimerRunning = false;
    resetInterrupt();
}

void TouchHandler::configureAdaptiveDistance() {
    if (!baselineReady) {
        return;
    }

    float maxTouchDistance = touchBaseline * MAX_TOUCH_DISTANCE_RATIO;
    if (maxTouchDistance < MIN_TOUCH_DISTANCE) {
        maxTouchDistance = MIN_TOUCH_DISTANCE;
    }

    float sensitivity = touchThreshold > MAX_SENSITIVITY ? MAX_SENSITIVITY : touchThreshold;
    float inverseSensitivity = 1.0f - (sensitivity / MAX_SENSITIVITY);
    float sensitivityCurve = inverseSensitivity * inverseSensitivity * inverseSensitivity;
    configuredTouchDistance = MIN_TOUCH_DISTANCE
            + ((maxTouchDistance - MIN_TOUCH_DISTANCE) * sensitivityCurve);
}

bool TouchHandler::detectAdaptiveTouch(int rawValue) {
    if (!baselineReady) {
        return false;
    }

    float touchDistance = touchBaseline - rawValue;
    float requiredDistance = touchDetected
            ? configuredTouchDistance * RELEASE_DISTANCE_RATIO
            : configuredTouchDistance;
    return touchDistance >= requiredDistance;
}

bool TouchHandler::handleContinuousTouchTimeout() {
    if (!touchDetected) {
        continuousTouchTimerRunning = false;
        return false;
    }

    uint32_t now = millis();
    if (!continuousTouchTimerRunning) {
        continuousTouchStartedAt = now;
        continuousTouchTimerRunning = true;
        return false;
    }
    if (static_cast<uint32_t>(now - continuousTouchStartedAt) < CONTINUOUS_TOUCH_TIMEOUT_MS) {
        return false;
    }

    calibrateBaseline();
    configureAdaptiveDistance();
    touchDetected = false;
    return true;
}

void TouchHandler::updateAdaptiveBaseline(int rawValue, bool touchActive) {
    if (!baselineReady || touchActive) {
        return;
    }

    float distanceBelowBaseline = touchBaseline - rawValue;
    float releaseDistance = configuredTouchDistance * RELEASE_DISTANCE_RATIO;
    if (distanceBelowBaseline > releaseDistance) {
        return;
    }

    float alpha = rawValue > touchBaseline ? BASELINE_RISE_ALPHA : BASELINE_FALL_ALPHA;
    touchBaseline += (rawValue - touchBaseline) * alpha;
}

void TouchHandler::refreshInterruptThreshold() {
    uint16_t nextInterruptThreshold = touchThreshold;
    if (adaptiveTouch && baselineReady) {
        nextInterruptThreshold = satf16(touchBaseline - configuredTouchDistance);
    }

    if (nextInterruptThreshold != interruptThreshold) {
        interruptThreshold = nextInterruptThreshold;
        touchAttachInterrupt(dataPin, interruptHandler, interruptThreshold);
    }
}

void TouchHandler::resetInterrupt() {
    touchDetectedRAW = false;
}

int TouchHandler::getTouchThreshold() {
    return touchThreshold;
}
