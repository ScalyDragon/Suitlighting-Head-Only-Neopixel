#include "FanController.h"

FanController::FanController(Persistence *persistenceIn, int pwmPinIn, int tachPinIn, bool invertOutputIn) {
    persistentData = persistenceIn;
    pwmPin = pwmPinIn;
    tachPin = tachPinIn;
    invertOutput = invertOutputIn;
}

void FanController::init() {
    pinMode(pwmPin, OUTPUT);
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY_HZ, PWM_RESOLUTION_BITS);
    ledcAttachPin(pwmPin, PWM_CHANNEL);

    if (hasTacho()) {
        pinMode(tachPin, INPUT_PULLUP);
        attachInterruptArg(tachPin, tachoInterruptHandler, this, FALLING);
    }

    updateFromPersistence();
    writePWM();
    lastRPMRefresh = millis();
    persistentData->lock();
    persistentData->fanRPM = 0;
    persistentData->unlock();
}

void FanController::loopHandler() {
    updateFromPersistence();
    refreshRPM();
}

void FanController::setSpeedPercent(uint8_t speedPercentIn) {
    speedPercent = speedPercentIn > 100 ? 100 : speedPercentIn;
    writePWM();

    persistentData->lock();
    persistentData->fanspeed = speedPercent;
    persistentData->savePersistentToEMMC = true;
    persistentData->unlock();
}

uint8_t FanController::getSpeedPercent() {
    return speedPercent;
}

uint32_t FanController::getRPM() {
    return rpm;
}

uint32_t FanController::getTachoPulseCount() {
    return getPulseSnapshot();
}

int FanController::getTachoPin() {
    return tachPin;
}

bool FanController::hasTacho() {
    return tachPin >= 0;
}

void IRAM_ATTR FanController::tachoInterruptHandler(void *arg) {
    auto *fanController = static_cast<FanController *>(arg);
    fanController->tachoPulseCount++;
}

void FanController::writePWM() {
    uint32_t dutyCycle = (PWM_MAX_VALUE * speedPercent) / 100;
    if (invertOutput) {
        dutyCycle = PWM_MAX_VALUE - dutyCycle;
    }
    ledcWrite(PWM_CHANNEL, dutyCycle);
}

void FanController::updateFromPersistence() {
    persistentData->lock();
    uint8_t requestedSpeed = persistentData->fanspeed > 100 ? 100 : persistentData->fanspeed;
    persistentData->unlock();

    if (requestedSpeed != speedPercent) {
        speedPercent = requestedSpeed;
        writePWM();
    }
}

void FanController::refreshRPM() {
    if (!hasTacho()) {
        rpm = 0;
        persistentData->lock();
        persistentData->fanRPM = rpm;
        persistentData->unlock();
        return;
    }

    uint32_t now = millis();
    uint32_t elapsed = now - lastRPMRefresh;
    if (elapsed < RPM_SAMPLE_WINDOW_MS) {
        return;
    }

    uint32_t pulseSnapshot = getPulseSnapshot();
    uint32_t pulseDelta = pulseSnapshot - lastTachoSnapshot;

    lastTachoSnapshot = pulseSnapshot;
    lastRPMRefresh = now;

    rpm = (pulseDelta * 60000UL) / (elapsed * PULSES_PER_REVOLUTION);
    persistentData->lock();
    persistentData->fanRPM = rpm;
    persistentData->unlock();
}

uint32_t FanController::getPulseSnapshot() {
    noInterrupts();
    uint32_t pulseSnapshot = tachoPulseCount;
    interrupts();
    return pulseSnapshot;
}
