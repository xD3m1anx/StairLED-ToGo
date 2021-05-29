#include "general.h"


class StairEvent {
    private: 
        bool active;
        uint32_t time;
        void activateEvent(void);
        void deactivateEvent(void);

    public:
        StairEvent();
        void timeSave(void);
        bool timeMoreThen(uint32_t ms);
        int8_t timeComp(uint32_t t);
        void timeReset();

        bool isActivatedEvent(void);
};