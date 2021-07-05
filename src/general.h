#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>
#include <inttypes.h>
#include "esp8266gpio.h"
#include "mydebug.h"

//#define BLYNK_PRINT Serial
//#define OTA_PRINT   Serial
#define EVENTS_PRINT    Debug

//#define SERIAL_DEBUG
//#define WIFI_CONNECT_HOME             //WiFi connection ( without Blynk )
#define OTA_PGM                         //Over the Air programming
#define BUILTIN_LED_ON                  //Turn ON led indication in loading 
#define TELNET_DEBUG                    //Telnet debug
#define EVENTS_DEBUG                    //Debug events handler
