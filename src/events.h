#include "general.h"
#include <HCSR04.h>

#define EVENT_HANDLE_TIMEMS     (500)      //time min T-hcsr04 (0.011 s)
#define EVENT_HANDLE_DST_NOISE  (50)        //in cm
#define EVENT_HANDLE_TIMEOUT    (15000)     //in ms

#define SENSOR_TOP_NAME         "HCSR top"
#define SENSOR_BTM_NAME         "HCSR btm"

/* --- HCSR04 connection pin settings -- */
#define SENSOR_BTM_TRIG     D0      
#define SENSOR_BTM_ECHO     D1
#define SENSOR_TOP_TRIG     D2
#define SENSOR_TOP_ECHO     D6

class StairEvent {
    private:
        HCSR04 * sensor;
        String name;
        bool active;
        uint32_t time;
        float distance;
        void activateEvent(void);
        void deactivateEvent(void);

    public:
        StairEvent();
        void sensorInit_HCSR04(const char * name, int trig, int echo);
        void timeSave(void);
        uint32_t timeGetSaved(void);
        bool timeMoreThen(uint32_t ms);
        int8_t timeComp(uint32_t t);
        void timeReset();
        bool isActivatedEvent(void);

        void handle();
};