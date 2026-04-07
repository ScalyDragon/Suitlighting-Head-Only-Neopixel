#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BOARDPINMAPPING_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BOARDPINMAPPING_H

#if defined(ARDUINO_D1_MINI32)
#include "WemosD1MiniESP32_pinMapping.h"
#elif defined(ARDUINO_NodeMCU_32S)
#include "AZDeliveryESP32_pinMapping.h"
#else
#error Unsupported board for BoardPinMapping.h
#endif

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_BOARDPINMAPPING_H
