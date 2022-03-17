//
// Created by Tobias W. on 17.03.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_PERSISTENTSTORAGE_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_PERSISTENTSTORAGE_H

#include "../helperStructures.h"
#include "EEPROM.h"

#define COLOR1_ADDR 0
#define COLOR2_ADDR COLOR1_ADDR+3
#define TOUCHTHRESHOLD_ADDR COLOR2_ADDR +3
#define FADESPEED_ADDR TOUCHTHRESHOLD_ADDR +1

class PersistentStorage {
public:
    PersistentStorage(Persistence *persistenceIn);

    void init();

    void loopHandler();

    void updatePersistenceFromMemory();

    void savePersistenceToMemory();

private:
    Persistence *persistentData;

    void saveColor(int addr, Color c);

    Color readColor(int addr);
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_PERSISTENTSTORAGE_H
