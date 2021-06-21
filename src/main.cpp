#include "main.h"
/* --------------------------------------------------------------------------------------------------------------- */

const char* ssid = "IrMa";  
const char* password = "4045041990";

Stair stair(STEP_MAX, LED_MAX);
RemoteDebug Debug;
BlynkTimer timer_Uptime;

/* --------------------------------------------------------------------------------------------------------------- */

void setup() {
  /* --- Start setup --- */
  BLINK_BUILTIN(1000);
  delay(250);
  BLINK_BUILTIN(1000);
  delay(250);

  
  /* --- Serial--- */
  Serial.begin(115200);
  BLINK_BUILTIN(1000);
  delay(250);
  Serial.println("Booting");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, BLYNK_SERVER_IP, 8080);
  timer_Uptime.setInterval(1000UL, localUptime);
  /* Check connection. Restart ESP if need */
  while(!Blynk.connected()) {
    for(int i = 0; i < 10; i ++) {
      BLINK_BUILTIN(250);
    }
    delay(1000);
    ESP.restart();
  }
  
  
  
  /* --- Wifi support --- */
  #ifdef WIFI_CONNECT_HOME
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        BLINK_BUILTIN(250);
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
          BLINK_BUILTIN(0);
        }
    );
    ArduinoOTA.onEnd([]() {
          Serial.println("\nEnd");          
        }
    );
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        BLINK_BUILTIN(0);
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
    BLINK_BUILTIN(1000);
  #endif
  
  	/* --- RemoteDebug --- */
  #ifdef TELNET_DEBUG
    Debug.begin(HOST_NAME); // Initialize the WiFi server
    Debug.setResetCmdEnabled(true); // Enable the reset command
    Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
    Debug.showColors(true); // Colors

    String hostNameWifi = HOST_NAME;
  hostNameWifi.concat(".local");

  #ifdef ESP8266 // Only for it
    WiFi.hostname(hostNameWifi);
  #endif

  #ifdef USE_MDNS  // Use the MDNS ?
    /*if (MDNS.begin(HOST_NAME)) {
        Debug.print("* MDNS responder started. Hostname -> ");
        Debug.println(HOST_NAME);
    }*/
    MDNS.begin(HOST_NAME);
    MDNS.addService("telnet", "tcp", 23);

  #endif
  #endif
  

  /* --- Stairs setup--- */
  stair.setup();
  uint8_t lps[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};  
  stair.setLedPerStep(lps);
  
  
  /* --- Finish setup--- */
  BLINK_BUILTIN(1000);
  delay(250);
  BLINK_BUILTIN(1000);
  delay(250);
  BLINK_BUILTIN(1000);
  Serial.println("Ready");
};

/* --------------------------------------------------------------------------------------------------------------- */
#define VIRT_MDOE     V1
#define VIRT_COLOR    V0

BLYNK_WRITE(VIRT_COLOR) {
  CRGB rgb(param[0].asInt(), param[1].asInt(), param[2].asInt());
  CHSV color = rgb2hsv_approximate(rgb);
  stair.mHue = color.hue;
  stair.mSaturation = color.saturation;
  stair.mValue = color.value;
}

BLYNK_WRITE(VIRT_MDOE) {
  switch (param.asInt())
  {
    case 1:   stair.setMode(FLASHTOGO);   break;
    case 2:   stair.setMode(MODE1);       break;
    case 3:                               break;
    case 0:   default:                    break;
  }
}

uint32_t mLastTime;

void loop() {
  #ifdef OTA_PGM
    ArduinoOTA.handle();
  #endif

  #ifdef TELNET_DEBUG
    if(millis() - mLastTime >= DEBUG_MSG_INTERVAL) {
      Debug.print("Local uptime: ");
      Debug.println(millis() / 1000);

      Debug.printf("HSV: [%d : %d : %d]\n", stair.mHue, stair.mSaturation, stair.mValue);

      mLastTime = millis();
    }
    Debug.handle();
  #endif

  Blynk.run();
  ArduinoOTA.handle();
  stair.handle();
  timer_Uptime.run();
}

void localUptime(void) {
  Blynk.virtualWrite(BPIN_UPTIME, millis() / 1000);
}

void ledBuiltinBlink(uint16 d) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(d);
  digitalWrite(LED_BUILTIN, HIGH);
}