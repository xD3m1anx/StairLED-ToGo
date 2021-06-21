
#include "events.h"
#include "general.h"

/* --- Stairs settings --- */
#define STEP_MAX            12      //Stairs length in steps
#define STAIR_SPEED         2       //Rising/Falling speed in steps per second
#define BRGHT_PREDIV        5
#define BRGHT_IDLE          40      //must be div. BRGHT_PREDIV 
#define BRGHT_WORKING       225     //must be div. BRGHT_PREDIV
#define STAIR_DELAY         (1000 / ((BRGHT_WORKING - BRGHT_IDLE) / BRGHT_PREDIV) / STAIR_SPEED)
/* --- end --- */

/* --- Leds settings --- */
#define CHIPSET  SK9822
#define LED_MAX  36
#define LED_PIN  13
#define LED_SCK  14
/* --- end --- */

#define HUE_PER_STEP        (255 / STEP_MAX)

typedef unsigned char uint8_t;

/* --- Режим работы --- */
enum mode_e {FLASHTOGO, MODE1};


/* --- ступень --- */
typedef struct step_s {
        uint8_t id;
        uint8_t ledsOnStep;
      } step_t;


/* --- лестница --- */
typedef struct steps_s {
    step_t step[STEP_MAX];
    } steps_t;


/* --- Главный класс --- */
class Stair {
    private:
        uint8_t mAmountStep;    
        uint16_t mAmountLed;
        steps_t mLps;
        mode_e mMode;
        CRGB * mLeds;
        uint8_t mStepIndex;
        uint8_t mLedIndex;
        StairEvent mTopEventHandler;
        StairEvent mBtmEventHandler;
    
    public:
        uint8_t mHue;
        uint8_t mSaturation;
        uint8_t mValue;
        
        Stair(uint8_t amountStep, uint8_t amountLed);

        int16_t render();
        void render(CRGB * stairLeds, CRGB fillPixel);

        void drawStep(uint8_t nStep);
        void draw();
        void draw(CRGB * stairLeds, CRGB fill);
        

        uint8_t getAmountStep();
        void setLedPerStep(uint8_t newLPS[]);
        uint8_t getLedsPerStep(uint8_t OnStep);

        void setup();
        void setMode(mode_e mode);
        void stairMode_FlashToGo();
        void stairMode_FlashToGo2();
        void mode1();
        void handle();  
};

