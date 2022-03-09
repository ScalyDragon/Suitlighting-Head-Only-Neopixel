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

class WebsiteController {
public:
    WebsiteController(Persistence *persistence);
    void init();
    void loopHandler();
private:
    AsyncWebServer* server;
    Persistence *persistentData;
    long idleColor = 0x0000FF;
    long boopColor = 0xFF0000;
    void updatePersistence();
};


#endif //FURSUITAUGEN_NEOPIXEL_BOOPABLE_NOSE_WEBSITECONTROLLER_H
