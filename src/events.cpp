#include "events.h"

#ifdef TELNET_DEBUG
    extern RemoteDebug Debug;
#endif

/* === PRIVATE === */
// ---- 
void StairEvent::activateEvent(void) {
    #ifdef EVENTS_DEBUG
        EVENTS_PRINT.print("Event: ");
        EVENTS_PRINT.print(name);
        EVENTS_PRINT.println(": Activate.");
    #endif
    active = true;
}

// ---- 
void StairEvent::deactivateEvent(void) {
    #ifdef EVENTS_DEBUG
        EVENTS_PRINT.print("Event: ");
        EVENTS_PRINT.print(name);
        EVENTS_PRINT.println(": Deactivate.");
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

    #ifdef EVENTS_DEBUG
        EVENTS_PRINT.print("Event: ");
        EVENTS_PRINT.print(name);
        EVENTS_PRINT.print(": Saved time ");
        EVENTS_PRINT.println(timeGetSaved());        
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
    
    #ifdef EVENTS_DEBUG
        EVENTS_PRINT.print("Event: ");
        EVENTS_PRINT.print(name);
        EVENTS_PRINT.println(": Time reset.");
    #endif
}


// ---- 
bool StairEvent::isActivatedEvent() {
    return active;
}
// ---- 
void StairEvent::eventComplete() {
    timeReset();
    distance = sensor->dist();
}
// ---- 
void StairEvent::handle() {
    float dstc = sensor->dist();
    #ifdef EVENTS_DEBUG
        EVENTS_PRINT.print("Event: ");
        EVENTS_PRINT.print(name);
        EVENTS_PRINT.print(": distance ");
        EVENTS_PRINT.println(dstc);
    #endif
    //if sensor find object
    if(!isActivatedEvent() && abs(dstc - distance) > EVENT_HANDLE_DST_NOISE) {
        timeSave();
        distance = dstc;
    }
    //if time is out of object
    else if(isActivatedEvent() && millis() - timeGetSaved() >= EVENT_HANDLE_TIMEOUT) {
        timeReset();
        distance = dstc;
        #ifdef EVENTS_DEBUG 
            EVENTS_PRINT.print("Event: ");
            EVENTS_PRINT.print(this->name);
            EVENTS_PRINT.println(": Timeout active status.");
        #endif
    }    
}