//
// Created by Tobias W. on 23.02.22.
//
#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
using namespace std;

#include <mutex>
#include "stdint.h"
#include "math.h"
#include "WString.h"
#include "Print.h"
#include "stdio.h"

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

static uint16_t satf16(float f) {
    return f > 0xFFFF ? 0xFFFF : f < 0 ? 0 : roundf(f);
}

static uint8_t satf8(float f) {
    return f > 0xFF ? 0xFF : f < 0 ? 0 : roundf(f);
}

struct Color {
    float r, g, b;

    Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

    Color normalized() {
        float length = lengthf();
        return Color(r / length, g / length, b / length);
    }

    Color add(Color toAdd) {
        return Color(r + toAdd.r, g + toAdd.g, b + toAdd.b);
    }

    Color sub(Color toSub) {
        return Color(r - toSub.r, g - toSub.g, b - toSub.b);
    }

    Color smul(float mul) {
        return Color(r * mul, g * mul, b * mul);
    }

    float lengthf() {
        return sqrtf(r * r + g * g + b * b);
    }

    int length() {
        return roundl(lengthf());
    }

    bool equals(Color c1) {
        return r == c1.r && g == c1.g && b == c1.b;
    }

    void setColor(Color c1) {
        r = c1.r;
        g = c1.g;
        b = c1.b;
    }

    Color toUint16() {
        return Color(satf16(r), satf16(g), satf16(b));
    }

    Color toInt8() {
        return Color(satf8(r), satf8(g), satf8(b));
    }

    Color getColorOfUint32(uint32_t colorRAW) {
        int r = colorRAW >> 16;
        int g = colorRAW >> 8 & 0xff;
        int b = colorRAW & 0xff;
        return {r, g, b};
    }

    uint32_t getUint32OfColor() {
        return (fRoundI(r) << 16) + (fRoundI(g) << 8) + fRoundI(b);
    }

    int fRoundI(float f) {
        return (int) floorf(f + 0.5f);
    }

    String toUint8String() {
        String retStr = "";
        char buf[6];
        sprintf(buf, "%02X%02X%02X", fRoundI(r), fRoundI(g), fRoundI(b));
        retStr += buf;
        return retStr;
    }
};

struct Persistence : mutex {
    bool touch, savePersistentToEMMC = false;
    uint16_t touchThreshold, touchMinVal, touchMaxVal, fadespeed;
    Color idleColor = Color(0, 0, 255), boopColor = Color(255, 0, 0);
    char *appw = "#Dr4gonG0esR@wr";
    char *ssid = "ScalyTech";
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_HELPERSTRUCTURES_H
