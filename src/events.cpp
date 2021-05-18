#include "events.h"
StairEvent::StairEvent() {
    active = false;
    time = 0;
}
void StairEvent::timeSave(void) {
    time = millis();
}
bool StairEvent::timeMoreThen(uint32_t ms) {
    return (this->time > ms);
}
void StairEvent::activateEvent(void) {
    this->active = true;
}
void StairEvent::deactivateEvent(void) {
    this->active = false;
}
bool StairEvent::isActivatedEvent(void) {
    return this->active;
}