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
            InterprocessorLock.lock();
            sensitivity = strtol(receivedMessage.c_str(),NULL,10);
            InterprocessorLock.unlock();
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/update",HTTP_GET,[this](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        Serial.print('.');
        DynamicJsonDocument json(1024);
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