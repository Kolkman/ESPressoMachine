#ifndef ESPRressoMach_WEB_API_H
#define ESPRressoMach_WEB_API_H
#include <Arduino.h>
#include "EspressoWebServer.h"
#include "ESPressoMachine.h"

class webInterfaceAPI
{
public:
    void begin(EspressoWebServer *server, ESPressoMachine * );
    webInterfaceAPI();
private:
    EspressoWebServer *_server;
    ESPressoMachine * _machine;
    size_t content_len;
    void handleStatus(AsyncWebServerRequest *);
    void handleFirmware(AsyncWebServerRequest *);
    void handleSet(AsyncWebServerRequest *);

};

extern webInterfaceAPI webAPI;

#endif