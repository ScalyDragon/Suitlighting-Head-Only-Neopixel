//
// Created by Tobias W. on 23.02.22.
//
#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
using namespace std;

#include "stdint.h"
#include "math.h"

static uint16_t sadd16(uint16_t a, uint16_t b) {
    return (a > 0xFFFF - b) ? 0xFFFF : a + b;
}

static uint32_t sadd32(uint32_t a, uint32_t b) {
    return (a > 0xFFFFFFFF - b) ? 0xFFFFFFFF : a + b;
}

static uint16_t ssub16(uint16_t a, uint16_t b) {
    return (b > a) ? 0 : a - b;
}

static uint32_t ssub32(uint32_t a, uint32_t b) {
    return (b > a) ? 0 : a - b;
}

struct Persistence {
    bool touch;
    uint16_t touchThreshold, touchMinVal, touchMaxVal;
};

struct Color {
    uint16_t r, g, b;

    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    Color normalized() {
        float length = sqrtf(r * r + g * g + b * b);
        return Color(lroundf((r / length)), lroundf(g / length), lroundf(b / length));
    }

    Color add(Color toAdd) {
        return Color(sadd16(r,toAdd.r),sadd16(g,toAdd.g),sadd16(b,toAdd.b));
    }

    Color sub(Color toSub) {
        return Color(ssub16(r,toSub.r), ssub16(g,toSub.g), ssub16(b, toSub.b));
    }
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
