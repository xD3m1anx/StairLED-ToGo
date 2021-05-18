#include "stairs.h"

Stair::Stair(uint8_t amountStep, uint8_t amountLed) {
    mAmountStep = amountStep;
    mAmountLed = amountLed;  
    mStatus = {0};
    mStatus.idle = true;
    mMode = FLASHTOGO;
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
    switch (mMode)
    {
    case FLASHTOGO:
        for(int i = 0; i < getLedsPerStep(0); i++) {
            mLeds[i].setHSV(20, 0xff, BRGHT_IDLE);
        }
        for (int i = LED_MAX - getLedsPerStep(STEP_MAX - 1); i < LED_MAX; i++) {
            mLeds[i].setHSV(20, 0xff, BRGHT_IDLE);
        }
        FastLED.show();
        break;

    case MODE1:
        //StairEffects::mode1();
        break;
    
    default:
        break;
    }
}