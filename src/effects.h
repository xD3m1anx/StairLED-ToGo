#include "general.h"

class StairEffects {
    private: 
        uint16_t mDelayFTG;
    ;

    public:
    void setDelayFTG(uint16_t delay_ms);
    static void flashToGo(CRGB * leds, uint16_t cnt);
    static void mode1();
};