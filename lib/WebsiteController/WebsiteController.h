//
// Created by Tobias W. on 08.03.22.
//

#ifndef FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WEBSITECONTROLLER_H
#define FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WEBSITECONTROLLER_H

using namespace std;

#include "WebServer.h"
#include "ESPAsyncWebServer.h"
#include "../helperStructures.h"
#include "SPIFFS.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

class WebsiteController {
public:
    WebsiteController(Persistence *persistence);

    void init();

    void loopHandler();

    int checkSPIFFS();

private:
    AsyncWebServer *server;
    Persistence *persistentData;
    long idleColor = 0x0000FF;
    long boopColor = 0xFF0000;
    uint16_t fadespeed = 10;
    uint16_t sensitivity;
    void updatePersistence();
    bool saveReq = false;
    mutex InterprocessorLock;
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WEBSITECONTROLLER_H
