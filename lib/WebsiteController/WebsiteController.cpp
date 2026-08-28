//
// Created by Tobias W. on 08.03.22.
//

#include "WebsiteController.h"

WebsiteController::WebsiteController(Persistence *persistence) {
    persistentData = persistence;
    server = new AsyncWebServer(80);
    SPIFFS.begin();
}

void WebsiteController::init() {
    persistentData->lock();
    idleColor = persistentData->idleColor.getUint32OfColor();
    boopColor = persistentData->boopColor.getUint32OfColor();
    sensitivity = persistentData->touchThreshold;
    fadespeed = persistentData->fadespeed;
    fanspeed = persistentData->fanspeed;
    adaptiveTouch = persistentData->adaptiveTouch;
    persistentData->unlock();
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/configsite.html");
    });
    server->on("/idlecolor", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            receivedMessage = "0x"+receivedMessage;
            InterprocessorLock.lock();
            idleColor = strtol(receivedMessage.c_str(), NULL, 16);
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/boopcolor", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            receivedMessage = "0x"+receivedMessage;
            InterprocessorLock.lock();
            boopColor = strtol(receivedMessage.c_str(), NULL, 16);
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/dimspeed",HTTP_GET, [this](AsyncWebServerRequest *request){
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            InterprocessorLock.lock();
            fadespeed = strtol(receivedMessage.c_str(), NULL, 10);
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/sensitivity",HTTP_GET,[this](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            int requestedSensitivity = strtol(receivedMessage.c_str(),NULL,10);
            if (requestedSensitivity < 0) {
                requestedSensitivity = 0;
            } else if (requestedSensitivity > 255) {
                requestedSensitivity = 255;
            }
            InterprocessorLock.lock();
            sensitivity = requestedSensitivity;
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/adaptivetouch",HTTP_GET,[this](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            InterprocessorLock.lock();
            adaptiveTouch = strtol(receivedMessage.c_str(), NULL, 10) != 0;
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/fanspeed",HTTP_GET,[this](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            int requestedFanspeed = strtol(receivedMessage.c_str(), NULL, 10);
            if (requestedFanspeed < 0) {
                requestedFanspeed = 0;
            } else if (requestedFanspeed > 100) {
                requestedFanspeed = 100;
            }
            InterprocessorLock.lock();
            fanspeed = requestedFanspeed;
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/update",HTTP_GET,[this](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        Serial.print('.');
        JsonDocument json;
        persistentData->lock();
        Serial.print('.');
        String idleColor = persistentData->idleColor.toUint8String();
        Serial.print('.');
        String boopColor = persistentData->boopColor.toUint8String();
        Serial.print('.');
        json["idlecolor"] = idleColor;
        Serial.print(this->idleColor);
        json["boopcolor"] = boopColor;
        Serial.print(this->boopColor);
        json["fadespeed"] = persistentData->fadespeed;
        Serial.print('.');
        json["sensitivity"] = persistentData->touchThreshold;
        json["adaptivetouch"] = persistentData->adaptiveTouch;
        json["touchvalue"] = persistentData->touchRawValue;
        json["effectivetouchthreshold"] = persistentData->touchEffectiveThreshold;
        json["fanspeed"] = persistentData->fanspeed;
        json["fanrpm"] = persistentData->fanRPM;
        Serial.print('.');
        persistentData->unlock();
        serializeJson(json, *response);
        request->send(response);
    });
    server->begin();
}

void WebsiteController::updatePersistence() {
    persistentData->lock();
    InterprocessorLock.lock();
    persistentData->idleColor.setColor(persistentData->idleColor.getColorOfUint32(idleColor));
    persistentData->boopColor.setColor(persistentData->boopColor.getColorOfUint32(boopColor));
    persistentData->fadespeed = fadespeed;
    persistentData->touchThreshold = sensitivity;
    persistentData->adaptiveTouch = adaptiveTouch;
    persistentData->fanspeed = fanspeed;
    if(saveReq) {
        persistentData->savePersistentToEMMC = true;
        saveReq = false;
    }
    InterprocessorLock.unlock();
    persistentData->unlock();
}

void WebsiteController::loopHandler() {
    updatePersistence();
}

int WebsiteController::checkSPIFFS() {
    return SPIFFS.exists("/configsite.html");
}
