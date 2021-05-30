#include "events.h"

/* === PRIVATE === */
// ---- 
void StairEvent::activateEvent(void) {
    this->active = true;
}

// ---- 
void StairEvent::deactivateEvent(void) {
    this->active = false;
}
/* === END PRIVATE === */


// ---- 
StairEvent::StairEvent() {
    this->active = false;
    this->time = 0;
    this->distance = 0;
}

void StairEvent::sensorInit_HCSR04(int trig, int echo) {
    sensor = new HCSR04(trig, echo);
}
// ---- 
void StairEvent::timeSave(void) {
    this->time = millis();
    activateEvent();

    #ifdef SERIAL_DEBUG
        Serial.print("Event: Activate. Saved time ");
        Serial.println(timeGetSaved());        
    #endif
}

uint32_t StairEvent::timeGetSaved(void) {
    return this->time;
}
// ---- 
bool StairEvent::timeMoreThen(uint32_t ms) {
    return (this->time > ms);
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
        Serial.println("Event: Deactivate. Time reset");
    #endif
}

// ---- 
bool StairEvent::isActivatedEvent() {
    return this->active;
}

// ---- 
void StairEvent::handle() {
    float dstc = sensor->dist();
    #ifdef SERIAL_DEBUG
        Serial.print("Event: distance  ");
        Serial.println(dstc);
    #endif

    if(!isActivatedEvent() && dstc - this->distance >= EVENT_HANDLE_DST_NOISE) {
        timeSave();
        this->distance = dstc;
    }
    else if(isActivatedEvent() && millis() - timeGetSaved() >= EVENT_HANDLE_TIMEOUT) {
        deactivateEvent();
        timeReset();
        this->distance = dstc;
        #ifdef SERIAL_DEBUG 
            Serial.println("Event: Timeout active status. Event deactivate.");
        #endif
    }    
}