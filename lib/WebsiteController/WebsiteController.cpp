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
    idleColor = persistentData->idleColor.getUint32OfColor();
    boopColor = persistentData->boopColor.getUint32OfColor();
    sensitivity = persistentData->touchThreshold;
    fadespeed = persistentData->fadespeed;
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/configsite.html");
    });
    server->on("/idlecolor", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            receivedMessage = "0x"+receivedMessage;
            idleColor = strtol(receivedMessage.c_str(), NULL, 16);
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/boopcolor", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            receivedMessage = "0x"+receivedMessage;
            boopColor = strtol(receivedMessage.c_str(), NULL, 16);
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/dimspeed",HTTP_GET, [this](AsyncWebServerRequest *request){
        if (request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            fadespeed = strtol(receivedMessage.c_str(), NULL, 10);
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/sensitivity",HTTP_GET,[this](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String receivedMessage = request->getParam("value")->value();
            sensitivity = strtol(receivedMessage.c_str(),NULL,10);
            saveReq = true;
            request->send(200);
        }
    });
    server->on("/update",HTTP_GET,[this](AsyncWebServerRequest *request){
        persistentData->lock();
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(1024);
        json["idlecolor"] = persistentData->idleColor.toUint8String();
        json["boopcolor"] = persistentData->boopColor.toUint8String();
        json["fadespeed"] = persistentData->fadespeed;
        json["sensitivity"] = persistentData->touchThreshold;
        serializeJson(json, *response);
        request->send(response);
        persistentData->unlock();
    });
    server->begin();
}

void WebsiteController::updatePersistence() {
    persistentData->lock();
    persistentData->idleColor.setColor(persistentData->idleColor.getColorOfUint32(idleColor));
    persistentData->boopColor.setColor(persistentData->boopColor.getColorOfUint32(boopColor));
    persistentData->fadespeed = fadespeed;
    persistentData->touchThreshold = sensitivity;
    if(saveReq) {
        persistentData->savePersistentToEMMC = true;
        saveReq = false;
    }
    persistentData->unlock();
}

void WebsiteController::loopHandler() {
    updatePersistence();
}

int WebsiteController::checkSPIFFS() {
    return SPIFFS.exists("/configsite.html");
}