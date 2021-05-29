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
    active = false;
    time = 0;
}

void StairEvent::sensorInit_HCSR04(int trig, int echo) {
    sensor = new HCSR04(trig, echo);
}
// ---- 
void StairEvent::timeSave(void) {
    time = millis();
    activateEvent();
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
}

// ---- 
bool StairEvent::isActivatedEvent() {
    return this->active;
}

// ---- 
void StairEvent::handle() {
    Serial.println(this->sensor->dist());
}