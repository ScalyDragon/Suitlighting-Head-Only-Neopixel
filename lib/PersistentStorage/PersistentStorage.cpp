//
// Created by Tobias W. on 17.03.22.
//

#include "PersistentStorage.h"

PersistentStorage::PersistentStorage(Persistence *persistenceIn) {
    persistentData = persistenceIn;
}

void PersistentStorage::init() {
    EEPROM.begin(1024);
    updatePersistenceFromMemory();
}

void PersistentStorage::loopHandler() {
    persistentData->lock();
    if (persistentData->savePersistentToEMMC) {
        persistentData->unlock();
        savePersistenceToMemory();
        persistentData->savePersistentToEMMC = false;
    } else persistentData->unlock();
}

void PersistentStorage::saveColor(int addr, Color c) {
    EEPROM.write(addr, c.fRoundI(c.toInt8().r));
    EEPROM.write(addr + 1, c.fRoundI(c.toInt8().g));
    EEPROM.write(addr + 2, c.fRoundI(c.toInt8().b));
    EEPROM.commit();
}

Color PersistentStorage::readColor(int addr) {
    int r = EEPROM.read(addr);
    int g = EEPROM.read(addr + 1);
    int b = EEPROM.read(addr + 2);
    return Color(r, g, b);
}

void PersistentStorage::updatePersistenceFromMemory() {
    persistentData->lock();
    persistentData->idleColor.setColor(readColor(COLOR1_ADDR));
    persistentData->boopColor.setColor(readColor(COLOR2_ADDR));
    persistentData->touchThreshold = EEPROM.read(TOUCHTHRESHOLD_ADDR);
    persistentData->fadespeed = EEPROM.read(FADESPEED_ADDR);
    persistentData->savePersistentToEMMC = false;
    persistentData->unlock();
}

void PersistentStorage::savePersistenceToMemory() {
    persistentData->lock();
    saveColor(COLOR1_ADDR, persistentData->idleColor);
    saveColor(COLOR2_ADDR, persistentData->boopColor);
    EEPROM.write(TOUCHTHRESHOLD_ADDR, persistentData->touchThreshold);
    EEPROM.write(FADESPEED_ADDR, persistentData->fadespeed);
    EEPROM.commit();
    persistentData->unlock();
}