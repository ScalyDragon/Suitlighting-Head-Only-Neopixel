#include "RGBStripManager.h"

#include <Arduino.h>

namespace {
    const uint8_t PWM_RESOLUTION_BITS = 8;
    const uint32_t PWM_FREQUENCY_HZ = 5000;
    const uint32_t PWM_MAX_VALUE = (1 << PWM_RESOLUTION_BITS) - 1;
}

RGBStripManager::RGBStripManager(int redPinIn, int greenPinIn, int bluePinIn, int loopRateHzIn, bool invertOutputIn)
        : LEDStrip(1, loopRateHzIn) {
    redPin = redPinIn;
    greenPin = greenPinIn;
    bluePin = bluePinIn;
    invertOutput = invertOutputIn;
}

void RGBStripManager::init() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    analogWriteResolution(PWM_RESOLUTION_BITS);
    analogWriteFrequency(PWM_FREQUENCY_HZ);
    writeColor(Color(0, 0, 0));
}

void RGBStripManager::updateStripFromBuffer() {
    writeColor(ledCurrentStateBuffer[0].toInt8());
}

void RGBStripManager::writeColor(Color color) {
    uint32_t redValue = color.fRoundI(color.r);
    uint32_t greenValue = color.fRoundI(color.g);
    uint32_t blueValue = color.fRoundI(color.b);

    if (invertOutput) {
        redValue = PWM_MAX_VALUE - redValue;
        greenValue = PWM_MAX_VALUE - greenValue;
        blueValue = PWM_MAX_VALUE - blueValue;
    }

    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}
