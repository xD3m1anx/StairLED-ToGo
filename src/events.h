#include "general.h"
#include <HCSR04.h>

#define SENSOR_TOP_TRIG     16
#define SENSOR_TOP_ECHO     5
#define SENSOR_BTM_TRIG     0
#define SENSOR_BTM_ECHO     2

class StairEvent {
    private:
        HCSR04 * sensor;
        bool active;
        uint32_t time;
        void activateEvent(void);
        void deactivateEvent(void);

    public:
        StairEvent();
        void sensorInit_HCSR04(int trig, int echo);
        void timeSave(void);
        bool timeMoreThen(uint32_t ms);
        int8_t timeComp(uint32_t t);
        void timeReset();

        bool isActivatedEvent(void);

        void handle();
};