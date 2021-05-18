#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "general.h"
#include "stairs.h"

#define OTA_PGM
#define WIFI_CONNECT_HOME

#define OTA_HOSTNAME    "ToksovoStairs"


void ledBuiltinBlink(uint16 d);

const char* ssid = "IrMa";  
const char* password = "4045041990";
