//
// Created by Tobias W. on 08.03.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WIFIMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WIFIMANAGER_H

#include "WiFi.h"
#include "../helperStructures.h"

class WifiManager {
public:
    WifiManager();
    void init();
    void loopHandler();
private:
    Persistence persistentData;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WIFIMANAGER_H
