#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "general.h"
#include "stairs.h"

#define OTA_HOSTNAME    "ToksovoStairs"

void ledBuiltinBlink(uint16 d);
#ifdef BUILTIN_LED_ON
  #define BLINK_BUILTIN(__delay__)    ledBuiltinBlink(__delay__)
#else
  #define BLINK_BUILTIN(any)     ;
  #pragma "Builtin led is off. To led on define 'BUILTIN_LED_ON'"  
#endif

const char* ssid = "IrMa";  
const char* password = "4045041990";
