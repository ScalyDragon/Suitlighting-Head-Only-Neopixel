//
// Created by Tobias W. on 07.03.22.
//

#include "NeopixelAnimator.h"

#define STRIPLENGTH 10

void NeopixelAnimator::init() {}

void NeopixelAnimator::loopHandler() {
    if(persistentState->touch) {
        ledmanager->setPixelArea(0, STRIPLENGTH, boopColor);
        ledmanager->setSmooth(false);
    }
    else {
        ledmanager->setPixelArea(0, STRIPLENGTH, idleColor);
        ledmanager->setSmooth(true);
    }
}

Color NeopixelAnimator::getCurrentTargetColor() {
    return currentTargetColor;
}

void NeopixelAnimator::setBoopColor(Color boopcolorIn) {
    boopColor.setColor(boopcolorIn);
}

void NeopixelAnimator::setIdleColor(Color idlecolorIn) {
    idleColor.setColor(idlecolorIn);
}

NeopixelAnimator::NeopixelAnimator(NeoPixelManager *ledmanager, Persistence *persistentState) : ledmanager(ledmanager), persistentState(persistentState) {}
