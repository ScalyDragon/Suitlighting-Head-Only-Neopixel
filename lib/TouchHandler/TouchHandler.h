//
// Created by Tobias W. on 15.02.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H


#include <stdint.h>
#include <atomic>
#include "../helperStructures.h"

class TouchHandler {
public:
    TouchHandler(int dataPin_In, Persistence *persistenceDataIn);
    void init();
    void loopHandler();
    int getRAW();
    int getTouchThreshold();
private:
    int dataPin;
    uint16_t touchThreshold;
    bool touchDetected;
    Persistence* persistenceData;
    void updateFromPersistence();
    void pushToPersistence();
    void syncInterrupt();
    void resetInterrupt();
    void setThreshold(int threshold);
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
