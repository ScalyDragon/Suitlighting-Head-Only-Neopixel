//
// Created by Tobias W. on 08.03.22.
//

#include "WebsiteController.h"

WebsiteController::WebsiteController(Persistence* persistence) {
    persistentData = persistence;
    server = new AsyncWebServer(80);
}

void WebsiteController::init() {
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/configsite.html");
    });
    server->on("/idlecolor", HTTP_GET, [this](AsyncWebServerRequest *request){
        if(request->hasParam("value")){
            String receivedMessage = request->getParam("value")->value();
            idleColor = receivedMessage.toInt();
        }
    });
    server->on("/boopcolor", HTTP_GET, [this](AsyncWebServerRequest *request){
        if(request->hasParam("value")){
            String receivedMessage = request->getParam("value")->value();
            boopColor = receivedMessage.toInt();
        }
    });
    server->begin();
}

void WebsiteController::updatePersistence() {
    persistentData->idleColor.setColor(persistentData->idleColor.getColorOfUint32(idleColor));
    persistentData->boopColor.setColor(persistentData->boopColor.getColorOfUint32(boopColor));
}

void WebsiteController::loopHandler() {
    updatePersistence();
}