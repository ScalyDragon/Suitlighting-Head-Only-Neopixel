//
// Created by Tobias W. on 07.03.22.
//

#include "NeopixelAnimator.h"

#define STRIPLENGTH 32

void NeopixelAnimator::init() {
    ledmanager->setSmoothStepwidth(1.0f);
}

void NeopixelAnimator::loopHandler() {
    updatePersistence();
    handleButtonPress();
    if (activated)
        debouncedTouchHandler();
    else
        blackout();
    
}

void NeopixelAnimator::handleButtonPress() {
    if (!oldButtonState && buttonState)
        activated = !activated;
    oldButtonState = buttonState;
}

void NeopixelAnimator::blackout() {
    ledmanager->setSmoothStepwidth(1.0f);
    ledmanager->setMode(1);
    ledmanager->setPixelArea(0, STRIPLENGTH, Color(0, 0, 0));
}

void NeopixelAnimator::debouncedTouchHandler() {
    if (touch) {
        ledmanager->setPixelArea(0, STRIPLENGTH, boopColor);
        ledmanager->setMode(0);
        ledmanager->setColorWait(fadespeed);
        ledmanager->setSmoothStepwidth(fadespeed / 10.0f);
        touchDebounce = 10;
    } else if (touchDebounce == 0) {
        ledmanager->setPixelArea(0, STRIPLENGTH, idleColor);
        ledmanager->setMode(2);
    } else touchDebounce--;
}

void NeopixelAnimator::updatePersistence() {
    persistentState->lock();
    boopColor.setColor(persistentState->boopColor);
    idleColor.setColor(persistentState->idleColor);
    fadespeed = persistentState->fadespeed;
    touch = persistentState->touch;
    buttonState = persistentState->buttonPressed;
    persistentState->unlock();
}

Color NeopixelAnimator::getCurrentTargetColor() {
    return currentTargetColor;
}

void NeopixelAnimator::setBoopColor(Color boopcolorIn) {
    boopColor.setColor(boopcolorIn);
    persistentState->lock();
    persistentState->boopColor.setColor(boopcolorIn);
    persistentState->unlock();
}

void NeopixelAnimator::setIdleColor(Color idlecolorIn) {
    idleColor.setColor(idlecolorIn);
    persistentState->lock();
    persistentState->idleColor.setColor(idlecolorIn);
    persistentState->unlock();
}

NeopixelAnimator::NeopixelAnimator(NeoPixelManager *ledmanager, Persistence *persistentState) : ledmanager(ledmanager), persistentState(persistentState) {}
