#include "DmxManager.h"

DmxManager *DmxManager::instance = nullptr;

DmxManager::DmxManager(Persistence *persistentDataIn) {
    persistentData = persistentDataIn;
    instance = this;
}

void DmxManager::init() {
    artnet.begin();
    artnet.setArtDmxCallback(onDmxFrameProxy);
}

void DmxManager::loopHandler() {
    artnet.read();
}

void DmxManager::onDmxFrameProxy(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP) {
    (void) remoteIP;
    if (instance != nullptr) {
        instance->onDmxFrame(universe, length, sequence, data);
    }
}

void DmxManager::onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data) {
    (void) sequence;

    if (universe != DMX_UNIVERSE || data == nullptr) {
        return;
    }

    const uint8_t startIndex = DMX_START_CHANNEL - 1;
    if (length <= startIndex + 2) {
        return;
    }

    persistentData->lock();
    persistentData->idleColor = Color(data[startIndex], data[startIndex + 1], data[startIndex + 2]);
    persistentData->touch = false;
    persistentData->unlock();
}
