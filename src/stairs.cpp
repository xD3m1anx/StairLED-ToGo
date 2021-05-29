#include "stairs.h"

Stair::Stair(uint8_t amountStep, uint8_t amountLed) {
    mAmountStep = amountStep;
    mAmountLed = amountLed;
    mTopEventHandler = StairEvent();
    mTopEventHandler.sensorInit_HCSR04(SENSOR_TOP_TRIG, SENSOR_TOP_ECHO);
    mBtmEventHandler = StairEvent();
    mBtmEventHandler.sensorInit_HCSR04(SENSOR_BTM_TRIG, SENSOR_BTM_ECHO);
    mStatus = {0};
    mStatus.idle = true;
    mMode = MODE1;
    mLeds = new CRGB[amountLed];
}

void Stair::setup() {
    FastLED.addLeds<CHIPSET, LED_PIN, LED_SCK, BGR>(mLeds, LED_MAX);
    for(int i = 0; i < LED_MAX; i++)
        mLeds[i] = CRGB::Black;
    FastLED.show();
}
void Stair::render() {
    FastLED.show();
}
void Stair::render(CRGB * stairLeds, CRGB fillPixel) {
    for(int i = 0; i < LED_MAX; i++) {
        stairLeds[i] = fillPixel;
    }
    FastLED.show();
}

void Stair::draw() {

}
void Stair::draw(CRGB * stairLeds, CRGB fill) {
    for(int i = 0; i < LED_MAX; i++) {
        stairLeds[i] = fill;
    }
}

void Stair::standBy() {
    if(Stair::isIdle()) {
       ;
    }
}
void Stair::idle() {
    
}

bool Stair::isIdle() {
    return mStatus.idle;
}  
bool Stair::isDraw() {
    return mStatus.draw;
}
bool Stair::isRender() {
    return mStatus.render;
}
bool Stair::isStandBy() {
    return mStatus.standBy;
}

uint8_t Stair::getUnits() {
    return mStatus.units;
}

void Stair::setIdle(bool flag) {
    this->mStatus.idle = flag;
}  
void Stair::setDraw(bool flag) {
    this->mStatus.draw = flag;
}
void Stair::setRender(bool flag) {
    this->mStatus.render = flag;
}
void Stair::setStandBy(bool flag) {
    this->mStatus.standBy = flag;
}
void Stair::setUnits(uint8_t units) {
    this->mStatus.units = units;
}

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
    static uint32_t oldMs = 0;
    if(millis() - oldMs >= 1000) {
        oldMs = millis();
        mBtmEventHandler.handle();
        mTopEventHandler.handle();
    }
    switch (mMode)
    {
    case FLASHTOGO:
        for(int i = 0; i < getLedsPerStep(0); i++) {
            mLeds[i].setHSV(20, 0xff, BRGHT_IDLE);
            FastLED.show();
        }
        

        for (int i = LED_MAX - getLedsPerStep(STEP_MAX - 1); i < LED_MAX; i++) {
            mLeds[i].setHSV(20, 0xff, BRGHT_IDLE);
            FastLED.show();
        }
        break;

    case MODE1:
        mode1();
        break;
    
    default:
        break;
    }
}

void Stair::mode1() {
    static uint16_t ledIndex = 0,
                    stepIndex = 0;
    static uint8_t bright = BRGHT_IDLE,
                   hue  =   0;
    static bool direction = true;

    if(direction) {
        for(int onStep = 0; onStep < this->getLedsPerStep(onStep); onStep++) {
            mLeds[ledIndex].setHSV(hue, 255, bright);
            if(ledIndex < LED_MAX)
                ledIndex++;
            else
                break;
        }
        FastLED.show();
        FastLED.delay(STAIR_DELAY);

        //to next step on stairs
        if(++bright == BRGHT_WORKING) {
            bright = 0;
            stepIndex++;
            hue+= HUE_PER_STEP;
        }
        else { //while bright < BRGHT_WORKING flash only one step
            ledIndex -= this->getLedsPerStep(stepIndex);
        }

        //The end direction 'up'
        if(stepIndex == STEP_MAX) {
            stepIndex = 0;
            ledIndex = 0;
            bright = BRGHT_WORKING;
            direction = false;
        }
    }
    else {
        for(int onStep = 0; onStep < this->getLedsPerStep(onStep); onStep++) {
            mLeds[ledIndex].setHSV(hue, 255, bright);
            if(ledIndex < LED_MAX)
                ledIndex++;
            else
                break;
        }
        FastLED.show();
        FastLED.delay(STAIR_DELAY);

        //to next step on stairs
        if(bright-- == 0) {
            bright = BRGHT_WORKING;
            stepIndex++;
            hue-= HUE_PER_STEP;
        }
        else { //while bright < BRGHT_WORKING flash only one step
            ledIndex -= this->getLedsPerStep(stepIndex);
        }

        //The end direction 'up'
        if(stepIndex == STEP_MAX) {
            stepIndex = 0;
            ledIndex = 0;
            bright = 0;
            direction = true;
        }
    }
}