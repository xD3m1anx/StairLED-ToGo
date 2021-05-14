#include <ESP8266WiFi.h>
#include "stairs.h"
#include <ArduinoOTA.h>
#include "general.h"

#define OTA_PGM
#define WIFI_CONNECT_HOME

#define OTA_HOSTNAME    "ToksovoStairs"

//led per step
CRGB leds[LED_MAX];
Stair stair(STEP_MAX, LED_MAX);

#define checkIdleStep(step)  (step < lps[0] || step >= LED_MAX - lps[STEP_MAX - 1] ? BRGHT_IDLE: 0)

const char* ssid = "IrMa"; 
const char* password = "4045041990";

void setup() {
    uint8_t lps[] = {1,1,1,1,1,1,1,1,1,1};
    stair.setLedPerStep(lps);
    
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Booting");
    
    #ifdef WIFI_CONNECT_HOME
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.println("Retrying connection...");
  }
    #endif

    #ifdef OTA_PGM
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.onStart([]() {
          Serial.println("Start updating ");
        }
    );
    ArduinoOTA.onEnd([]() {
          Serial.println("\nEnd");
        }
    );
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        }
    );
    ArduinoOTA.onError([](ota_error_t error) {
          Serial.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
          } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
          } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
          } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
          } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
          }
        }
    );
    ArduinoOTA.begin();
    #endif
    
    Serial.println("Ready");
    
    FastLED.addLeds<CHIPSET, LED_PIN, LED_SCK, BGR>(leds, LED_MAX);
    for(CRGB & px: leds)
      px = CRGB::Black;
    FastLED.show();
};

uint16_t ledIndx = 0,
         step = 0;
uint8_t brght = BRGHT_IDLE;

void loop() {
  
    ArduinoOTA.handle();
    stair.handle();
}