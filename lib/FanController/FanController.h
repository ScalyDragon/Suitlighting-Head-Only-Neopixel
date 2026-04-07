//
// Created by Codex on 29.03.26.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_FANCONTROLLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_FANCONTROLLER_H

#include "../helperStructures.h"
#include "Arduino.h"

class FanController {
public:
    FanController(Persistence *persistenceIn, int pwmPinIn, int tachPinIn = -1, bool invertOutputIn = false);

    void init();
    void loopHandler();

    void setSpeedPercent(uint8_t speedPercentIn);
    uint8_t getSpeedPercent();

    uint32_t getRPM();
    uint32_t getTachoPulseCount();
    int getTachoPin();
    bool hasTacho();

private:
    static constexpr uint8_t PWM_CHANNEL = 0;
    static constexpr uint8_t PWM_RESOLUTION_BITS = 8;
    static constexpr uint32_t PWM_FREQUENCY_HZ = 25000;
    static constexpr uint32_t RPM_SAMPLE_WINDOW_MS = 1000;
    static constexpr uint8_t PULSES_PER_REVOLUTION = 2;
    static constexpr uint32_t PWM_MAX_VALUE = (1 << PWM_RESOLUTION_BITS) - 1;

    Persistence *persistentData;
    int pwmPin;
    int tachPin;
    bool invertOutput;
    uint8_t speedPercent = 0;
    volatile uint32_t tachoPulseCount = 0;
    uint32_t lastTachoSnapshot = 0;
    uint32_t rpm = 0;
    uint32_t lastRPMRefresh = 0;

    static void IRAM_ATTR tachoInterruptHandler(void *arg);
    void writePWM();
    void updateFromPersistence();
    void refreshRPM();
    uint32_t getPulseSnapshot();
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_FANCONTROLLER_H
