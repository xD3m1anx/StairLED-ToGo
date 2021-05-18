#include "general.h"

class StairEvent {
    private: 
        bool active;
        uint32_t time;

    public:
        StairEvent();
        void timeSave(void);
        bool timeMoreThen(uint32_t ms);

        void activateEvent(void);
        void deactivateEvent(void);
        bool isActivatedEvent(void);
};