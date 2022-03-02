//
// Created by Tobias W. on 15.02.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H


#include <stdint.h>
#include "../helperStructures.h"

class TouchHandler {
public:
    TouchHandler(int dataPin_In);
    void init(Persistence* persistenceDataIn);
    void loopHandler();
    int getRAW();
    void setThreshold(int threshold);
private:
    int dataPin;
    uint16_t touchThreshold;
    bool touchDetected;
    Persistence* persistenceData;
    void updateFromPersistence();
    void pushToPersistence();
    void syncInterrupt();
    void resetInterrupt();
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_TOUCHHANDLER_H
