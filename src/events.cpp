#include "events.h"

/* === PRIVATE === */
// ---- 
void StairEvent::activateEvent(void) {
    #ifdef SERIAL_DEBUG
        Serial.print("Event: ");
        Serial.print(name);
        Serial.println(": Activate.");
    #endif
    active = true;
}

// ---- 
void StairEvent::deactivateEvent(void) {
    #ifdef SERIAL_DEBUG
        Serial.print("Event: ");
        Serial.print(name);
        Serial.println(": Deactivate.");
    #endif
    active = false;
}
/* === END PRIVATE === */


// ---- 
StairEvent::StairEvent() {
    active = false;
    time = 0;
    distance = 0;
}

void StairEvent::sensorInit_HCSR04(const char * name, int trig, int echo) {
    this->name = String(name);
    sensor = new HCSR04(trig, echo);
}
// ---- 
void StairEvent::timeSave(void) {
    time = millis();
    activateEvent();

    #ifdef SERIAL_DEBUG
        Serial.print("Event: ");
        Serial.print(name);
        Serial.print(": Saved time ");
        Serial.println(timeGetSaved());        
    #endif
}

uint32_t StairEvent::timeGetSaved(void) {
    return time;
}
// ---- 
bool StairEvent::timeMoreThen(uint32_t ms) {
    return (time > ms);
}

// ---- 
#define MORE (1)
#define LESS (-1)
#define EQUAL (0)
int8_t StairEvent::timeComp(uint32_t t) {
    return (time > t) ? MORE: (time < t) ? LESS: EQUAL; 
}

// ---- 
void StairEvent::timeReset() {
    time = 0;
    deactivateEvent();
    
    #ifdef SERIAL_DEBUG
        Serial.print("Event: ");
        Serial.print(name);
        Serial.println(": Time reset.");
    #endif
}

// ---- 
bool StairEvent::isActivatedEvent() {
    return active;
}

// ---- 
void StairEvent::handle() {
    float dstc = sensor->dist();
    #ifdef SERIAL_DEBUG
        Serial.print("Event: ");
        Serial.print(name);
        Serial.print(": distance ");
        Serial.println(dstc);
    #endif
    //if sensor find object
    if(!isActivatedEvent() && dstc - distance > EVENT_HANDLE_DST_NOISE) {
        timeSave();
        distance = dstc;
    }
    //if time is out of object
    else if(isActivatedEvent() && millis() - timeGetSaved() >= EVENT_HANDLE_TIMEOUT) {
        timeReset();
        distance = dstc;
        #ifdef SERIAL_DEBUG 
            Serial.print("Event: ");
            Serial.print(this->name);
            Serial.println(": Timeout active status.");
        #endif
    }    
}