//
// Created by Tobias W. on 08.03.22.
//

#include "WifiManager.h"

WifiManager::WifiManager(Persistence *persistentDataIn) {
    persistentData = persistentDataIn;
}

void WifiManager::init() {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(persistentData->ssid, persistentData->appw);
}

void WifiManager::loopHandler() {

}
