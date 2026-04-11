#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_DMXMANAGER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_DMXMANAGER_H

#include "ArtnetWifi.h"
#include "../helperStructures.h"

class DmxManager {
public:
    explicit DmxManager(Persistence *persistentDataIn);
    void init();
    void loopHandler();

private:
    static constexpr uint16_t DMX_UNIVERSE = 0;
    static constexpr uint8_t DMX_START_CHANNEL = 1;

    Persistence *persistentData;
    ArtnetWifi artnet;

    void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);
    static void onDmxFrameProxy(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP);
    static DmxManager *instance;
};

#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_DMXMANAGER_H
