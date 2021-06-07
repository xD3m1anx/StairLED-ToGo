#include "stairs.h"

Stair::Stair(uint8_t amountStep, uint8_t amountLed) {
    mAmountStep = amountStep;
    mAmountLed = amountLed;
    mTopEventHandler = StairEvent();
    mTopEventHandler.sensorInit_HCSR04(SENSOR_TOP_NAME, SENSOR_TOP_TRIG, SENSOR_TOP_ECHO);
    mBtmEventHandler = StairEvent();
    mBtmEventHandler.sensorInit_HCSR04(SENSOR_BTM_NAME, SENSOR_BTM_TRIG, SENSOR_BTM_ECHO);
    mStatus = {0};
    mStatus.idle = true;
    mMode = FLASHTOGO;
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
int16_t Stair::render() {
    for(int onStep = 0; onStep < getLedsPerStep(onStep); onStep++) {
        mLeds[mLedIndex].setHSV(mHue, mSaturation, mValue);
        mLedIndex++;
    }
    FastLED.show();
    FastLED.delay(STAIR_DELAY);
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
    static uint32_t oldMS = 0;
    if(millis() - oldMS >= EVENT_HANDLE_TIMEMS) {
        oldMS = millis();
        mBtmEventHandler.handle();
        mTopEventHandler.handle();
    }

    switch (mMode)
    {
        case FLASHTOGO:
            stairMode_FlashToGo2();
            break;

        case MODE1:           
            break;

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
            mStepIndex++ ;
        }
        //Already brightup leds on current step

        //Stiars. All leds are working
        if(mStepIndex == STEP_MAX) {
            mStepIndex = 0;
            mLedIndex = 0;
            mValue = BRGHT_WORKING;
        }

        //show
        render();
        mLedIndex -= this->getLedsPerStep(mStepIndex);
        mValue += BRGHT_PREDIV;
    }
    
    if(!direction) {

        render();
    }
}

void Stair::stairMode_FlashToGo() {
    /*static uint16_t ledIndex = 0,
                    stepIndex = 0;
    static uint8_t  value = BRGHT_IDLE,
                    satur = 255,
                    hue  =   0;*/
    static bool direction = true;

    render();

    if(direction) {
        //to next step on stairs
        mValue += BRGHT_PREDIV;
        if((mValue >= BRGHT_WORKING)) {
            if(mStepIndex == 0 || mStepIndex == STEP_MAX - 2) {
                mValue = BRGHT_IDLE;
                Serial.printf("Step = %d, value = %d\n", mStepIndex, mValue);
            }
            else {
                mValue = 0;
                Serial.printf("Step = %d, value = %d\n", mStepIndex, mValue);
            }
            mStepIndex++;
        }
        else //while bright < BRGHT_WORKING flash only one step
            mLedIndex -= this->getLedsPerStep(mStepIndex);

        //The end direction 'rising'
        if(mStepIndex >= STEP_MAX) {
            mValue = BRGHT_WORKING;  //for dir = falling
            direction = false;
        }
    }
    else {
        //to next step on stairs
        if((mValue == 0 && (mStepIndex > 0 && mStepIndex < STEP_MAX)) ||  (mValue == BRGHT_IDLE && (mStepIndex == 0 || mStepIndex == (STEP_MAX - 1)))) {  //if index equal 0 or MAX. In min or max
            mValue = BRGHT_WORKING;
            Serial.printf("Step = %d, value = %d\n", mStepIndex, mValue);
            mStepIndex++;

        }
        else {  //while bright < BRGHT_WORKING flash only one step
            mValue -= BRGHT_PREDIV;
            mLedIndex -= this->getLedsPerStep(mStepIndex);
        }
        
        //The end direction 'falling'
        if(mStepIndex == STEP_MAX) {
            mValue = BRGHT_IDLE;     //for dir = rising
            direction = true; 
        }
    }

    if(mStepIndex == STEP_MAX) {
        mStepIndex = 0;
        mLedIndex = 0;
    }
}
