#include "main.h"

Stair stair(STEP_MAX, LED_MAX);

void setup() {
  /* --- Start setup --- */
  ledBuiltinBlink(1000);
  delay(250);
  ledBuiltinBlink(1000);
  delay(250);

  
  /* --- Serial--- */
  Serial.begin(115200);
  ledBuiltinBlink(1000);
  delay(250);
  Serial.println("Booting");
  
  
  /* --- Wifi support --- */
  #ifdef WIFI_CONNECT_HOME
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        ledBuiltinBlink(250);
        WiFi.begin(ssid, password);
        Serial.println("Retrying connection...");
    }
    IPAddress localIp = WiFi.localIP();
    Serial.print("Start network with IP: ");
    Serial.println(localIp);
  #endif


  
  /* --- OTA programming (working with WiFi support) --- */
  #ifdef OTA_PGM
    Serial.print("Starting OTA with HOSTNAME: ");
    Serial.print(OTA_HOSTNAME);
    Serial.print("...");
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.onStart([]() {
          Serial.println("Start updating ");
          ledBuiltinBlink(0);
        }
    );
    ArduinoOTA.onEnd([]() {
          Serial.println("\nEnd");          
        }
    );
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        ledBuiltinBlink(0);
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
    Serial.println("done.");
    ledBuiltinBlink(1000);
  #endif
  
  
  /* --- Stairs setup--- */
  stair.setup();
  uint8_t lps[] = {1,1,1,1,1,1,1,1,1,1};  
  stair.setLedPerStep(lps);
  
  
  /* --- Finish setup--- */
  ledBuiltinBlink(1000);
  delay(250);
  ledBuiltinBlink(1000);
  delay(250);
  ledBuiltinBlink(1000);
  Serial.println("Ready");
};

void loop() {
    ArduinoOTA.handle();
    stair.handle();
}


void ledBuiltinBlink(uint16 d) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(d);
  digitalWrite(LED_BUILTIN, HIGH);
}