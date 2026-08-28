#ifndef TEST_STUB_ARDUINO_H
#define TEST_STUB_ARDUINO_H

#include <cstdint>
#include <cstdlib>

#define INPUT_PULLUP 0x2
#define OUTPUT 0x3
#define FALLING 0x4
#define IRAM_ATTR

namespace ArduinoStub {
extern int digitalReadValue;
extern uint32_t nowMs;
extern uint32_t ledcDuty;
extern int ledcPin;
extern int analogValues[64];
extern void (*interruptHandler)(void *);
extern void *interruptArgument;
void reset();
}

inline void pinMode(int, int) {}
inline int digitalRead(int) { return ArduinoStub::digitalReadValue; }
inline uint32_t millis() { return ArduinoStub::nowMs; }
inline void delay(uint32_t duration) { ArduinoStub::nowMs += duration; }
inline void ledcSetup(uint8_t, uint32_t, uint8_t) {}
inline void ledcAttachPin(int pin, uint8_t) { ArduinoStub::ledcPin = pin; }
inline void ledcWrite(uint8_t, uint32_t duty) { ArduinoStub::ledcDuty = duty; }
inline void analogWriteResolution(uint8_t) {}
inline void analogWriteFrequency(uint32_t) {}
inline void analogWrite(int pin, uint32_t value) {
    if (pin >= 0 && pin < 64) ArduinoStub::analogValues[pin] = static_cast<int>(value);
}
inline void attachInterruptArg(int, void (*handler)(void *), void *argument, int) {
    ArduinoStub::interruptHandler = handler;
    ArduinoStub::interruptArgument = argument;
}
inline void noInterrupts() {}
inline void interrupts() {}

struct SerialStub {
    template<typename T> void print(const T &) {}
    template<typename T> void println(const T &) {}
};

extern SerialStub Serial;

#endif
