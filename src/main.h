//#define BLYNK_PRINT Serial
//#define OTA_PRINT   Serial

/* --------------------------------------------------------------------------------------------------------------- */

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoOTA.h>
#include "mydebug.h"
#include "esp8266gpio.h"
#include "general.h"
#include "stairs.h"
/* --------------------------------------------------------------------------------------------------------------- */

//#define WIFI_CONNECT_HOME             //WiFi connection ( without Blynk )
#define OTA_PGM                         //Over the Air programming
#define BUILTIN_LED_ON                  //Turn ON led indication in loading 
#define TELNET_DEBUG                    //Telnet debug

#define OTA_HOSTNAME                    "stair"
#define HOST_NAME                       "stair"
#define BLYNK_SERVER_IP                 "192.168.0.50"
#define BLYNK_AUTH_TOKEN                "T4ArKw_cnTk0Gb35OUjI8UDYgjyPAZ1e"      //*.*.*.60

#define DEBUG_MSG_INTERVAL              (5000)                                 //ms 
#define BPIN_UPTIME                     V9                                      //*.*.*.60

/* --------------------------------------------------------------------------------------------------------------- */

#ifdef BUILTIN_LED_ON
  #define BLINK_BUILTIN(__delay__)    ledBuiltinBlink(__delay__)
#else
  #define BLINK_BUILTIN(any)     ;
  #pragma "Builtin led is off. To led on define 'BUILTIN_LED_ON'"  
#endif

/* --------------------------------------------------------------------------------------------------------------- */

void ledBuiltinBlink(uint16 d);
void localUptime(void);

/* --------------------------------------------------------------------------------------------------------------- */