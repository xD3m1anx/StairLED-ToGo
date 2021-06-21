#if defined ESP8266 || defined ESP32
#define USE_MDNS true
#else
// RemoteDebug library is now only to Espressif boards,
// as ESP32 and ESP82266,
// If need for another WiFi boards,
// please add an issue about this
// and we will see if it is possible made the port for your board.
// access: https://github.com/JoaoLopesF/RemoteDebug/issues

#error "The board must be ESP8266 or ESP32"
#endif // ESP

//////// Libraries
#if defined ESP8266
    // Includes of ESP8266
    #include <ESP8266WiFi.h>

    #ifdef USE_MDNS
        #include <DNSServer.h>
        #include <ESP8266mDNS.h>
    #endif

#elif defined ESP32
    // Includes of ESP32
    #include <WiFi.h>
    #ifdef USE_MDNS
        #include <DNSServer.h>
        #include "ESPmDNS.h"
    #endif
#endif // ESP

// Remote debug over WiFi - not recommended for production, only for development
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
