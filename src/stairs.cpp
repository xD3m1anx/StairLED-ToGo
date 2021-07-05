#include "stairs.h"

#ifdef TELNET_DEBUG
    extern RemoteDebug Debug;
#endif


Stair::Stair(uint8_t amountStep, uint8_t amountLed) {
    mAmountStep = amountStep;
    mAmountLed = amountLed;
    mTopEventHandler = StairEvent();
    mTopEventHandler.sensorInit_HCSR04(SENSOR_TOP_NAME, SENSOR_TOP_TRIG, SENSOR_TOP_ECHO);
    mBtmEventHandler = StairEvent();
    mBtmEventHandler.sensorInit_HCSR04(SENSOR_BTM_NAME, SENSOR_BTM_TRIG, SENSOR_BTM_ECHO);
    mMode = DEMO;
    mLeds = new CRGB[amountLed];
    mStepIndex = 0;
    mLedIndex = 0;
}

void Stair::setup() {
    FastLED.addLeds<CHIPSET, LED_PIN, LED_SCK, BGR>(mLeds, LED_MAX);
    for(int i = 0; i < LED_MAX; i++)
        mLeds[i] = CRGB::Black;
    FastLED.show();
}

void Stair::setMode(mode_e mode) {
    mMode = mode;
}

void Stair::render() {
    drawStep(mStepIndex);
    FastLED.show();
    FastLED.delay(STAIR_DELAY);
}

void Stair::drawStep(uint8_t nStep) {
    //loop for n-leds on step № nStep.
    for(int onStep = 0; onStep < getLedsPerStep(mStepIndex); onStep++) {
        mLeds[mLedIndex].setHSV(mHue, mSaturation, mValue);
        mLedIndex++;
    }
}
/*
void Stair::draw(CRGB * stairLeds, CRGB fill) {
    for(int i = 0; i < LED_MAX; i++) {
        stairLeds[i] = fill;
    }
}
*/
uint8_t Stair::getAmountStep() {
    return mAmountStep;
}

void Stair::setLedPerStep(uint8_t newLPS[]) {
    for(int i = 0; i < mAmountStep; i++)
        mLps.step[i].ledsOnStep = newLPS[i];
}
    
uint8_t Stair::getLedsPerStep(uint8_t OnStep) {
    return mLps.step[OnStep].ledsOnStep;
}

void Stair::handle() {
    static uint32_t oldMS = 0;
    if(millis() - oldMS >= EVENT_HANDLE_TIMEMS) {
        oldMS = millis();
        //mBtmEventHandler.handle();
        //mTopEventHandler.handle();
    }

    switch (mMode)
    {
        case FLASHTOGO:
            stairMode_FlashToGo2();
            break;

        case MODE1: 
            mode1();          
            break;

        case MODE2:
            mode2();
            break;

        case MODE3:
            mode3();
            break;
 
        case DEMO:
            static uint8_t stage = 0;
            demoMode(stage);
            if(++stage == 12)
                stage = 0;

        default:
            break;
    }
}

void Stair::stairMode_FlashToGo2() {
    static bool direction = true;

    if(direction) {

        //All leds are working on current step. Stepup!
        if(mValue >= BRGHT_WORKING) {
            //Control minimum bright for first and last steps
            mValue = ( mStepIndex == 0 || mStepIndex == STEP_MAX - 1 ) ? BRGHT_IDLE : 0;
            mLedIndex += getLedsPerStep(mStepIndex);
            mStepIndex++ ;
        }

        //All leds are working
        if(mStepIndex == STEP_MAX) {
            mStepIndex = 0;
            mLedIndex = 0;
            mValue = BRGHT_WORKING;
        }

        render();                                     //show
        
        mLedIndex -= getLedsPerStep(mStepIndex);      //stepback for finish brightup
        
        mValue += BRGHT_PREDIV;                       //brightup
    }
    
    if(!direction) {

        render();
    }
}


void Stair::mode1() {
    for(int i = 0; i < LED_MAX; i++) {
        mLeds[i].setHSV(mHue, mSaturation, mValue);
    }
    FastLED.show();
}

void Stair::mode2() {
    bool toUpDetector = mBtmEventHandler.isActivatedEvent();
    bool toDownDetector = mTopEventHandler.isActivatedEvent();
    
    if(toUpDetector) {
        Debug.print("in");
        mBtmEventHandler.eventComplete();
        for(int i = 0; i < LED_MAX; i++)
            mLeds[i].r = ~mLeds[i].r;
    }
    if(toDownDetector) {
        mTopEventHandler.eventComplete();
        for(int i = 0; i < LED_MAX; i++)
            mLeds[i].b = ~mLeds[i].b;
    }
    FastLED.show();
}

void Stair::mode3() {
    for(int hue = 0; hue < 256; hue++) {
        for(int i = 0; i < LED_MAX; i++) {   
            mLeds[i].setHSV((hue + 1) / LED_MAX * i, 255, 255);
        }
        FastLED.delay(5);
        FastLED.show();
    }  
}



/* --------------------------------------------------------------------------------------------------------------- */
/* ---                                DEMO     MODE                                                            --- */
/* --------------------------------------------------------------------------------------------------------------- */
#define ledLoop             for(int i = 0; i < LED_MAX; i++)
#define lednLoop(cnt)        for( int j = 0; j < cnt; j++)
#define lednLoopD(cnt)        for( int j = cnt; j >= 0; j--)

void Stair::setStepColor(uint8_t step, CRGB clr) {
    uint8_t ledOnS = getLedsPerStep(step) * (step); 
    for( int l = ledOnS; l < ledOnS + getLedsPerStep(step); l++) {
        mLeds[l] = clr;
    }
}

void Stair::setStepColor(uint8_t step, CHSV clr) {
    uint8_t ledOnS = getLedsPerStep(step) * (step); 
    for( int l = ledOnS; l < ledOnS + getLedsPerStep(step); l++) {
        mLeds[l].setHSV(clr.h, clr.s, clr.v);
    }
}

void Stair::demoMode(uint8_t stage) {
    uint8_t rainbowHue[STEP_MAX];
    lednLoop(STEP_MAX) {
        rainbowHue[j] = 255 / (float)STEP_MAX * j;
    }
    CHSV tHsv;

    switch (stage) {
        case 0:
            FastLED.clear();
            FastLED.show();
            break;
        
        case 1:
            //risen
            lednLoop(51) {
                ledLoop {
                    mLeds[i].setHSV(0, 0, 5 * j);
                }
                FastLED.show();
                delay(15);
            }
            delay(400);
            break;
        case 2:
            //step down
            for(int onStep = STEP_MAX - 1; onStep >= 0; onStep--) {
                setStepColor(onStep, CRGB::Black);
                FastLED.show();
                delay(125);
            }
            break;

        case 3:
            //step up and paint rainbow with saturation = 0
            for(int onStep = 0; onStep < STEP_MAX; onStep++) {
                setStepColor(onStep, CHSV(rainbowHue[onStep], 0, 255));
                FastLED.show();
                delay(125);
            }
            break;

        case 4:
            //step up saturation
            for(uint8_t val = 0; val < 255; val++) {
                for(uint8_t onStep = 0; onStep < STEP_MAX; onStep++) {
                    setStepColor(onStep, CHSV(rainbowHue[onStep], val, 255));
                    FastLED.show();
                }
                delay(3);
            }
            break;

        case 5:
            //step down rainbow
            for(int onStep = STEP_MAX - 1; onStep >= 0; onStep--) {
                setStepColor(onStep, CRGB::Black);
                FastLED.show();
                delay(125);
            }
            break;
        
        case 6:
            //Step up to center - RED
            for(int i = 0; i < STEP_MAX / 2; i++) {
                setStepColor(i, CHSV(0, 255, 255));
                setStepColor(STEP_MAX - (i + 1), CHSV(0, 255, 255));
                FastLED.show();
                delay(125);
            }
            break;
        case 7:
            //Step down from center - GREEN
            for(int i = STEP_MAX / 2; i >= 0 ; i--) {
                setStepColor(i, CHSV(96, 255, 255));
                setStepColor(STEP_MAX - (i + 1), CHSV(96, 255, 255));
                FastLED.show();
                delay(125);
            }
            break;
        
        case 8:
            //Step up to center - BLUE
            for(int i = 0; i < STEP_MAX / 2; i++) {
                setStepColor(i, CHSV(160, 255, 255));
                setStepColor(STEP_MAX - (i + 1), CHSV(160, 255, 255));
                FastLED.show();
                delay(125);
            }
            break;

        case 9:
            break;

        case 10:
            //run rainbow
            for(int clr = 0; clr < 255; clr++) {
                for(int onStep = 0; onStep < STEP_MAX; onStep++) {
                    rainbowHue[onStep]++;
                    setStepColor(onStep, CHSV(rainbowHue[onStep], 255, 255));
                }
                FastLED.show();
                delay(15);
            }
            break;

        case 11:
            //fallen rainbow
            for(int val = 255; val >= 0; val--) {
                for(int onStep = 0; onStep < STEP_MAX; onStep++) {
                    setStepColor(onStep, CHSV(rainbowHue[onStep], 255, val));
                }
                FastLED.show();
                delay(1);
            }
            break;

        default:
            break;
    }
}