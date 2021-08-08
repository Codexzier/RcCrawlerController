void RcInputsReadInputs() {
  mReadValueA = pulseIn(PIN_INPUT_A, HIGH);
  mReadValueB = pulseIn(PIN_INPUT_B, HIGH);
  mReadValueC = pulseIn(PIN_INPUT_C, HIGH);
  mReadValueD = pulseIn(PIN_INPUT_D, HIGH);


  if(mSerialMonitor) {
//    Serial.print("INPUT A: "); Serial.print(mReadValueA);
//    Serial.print("\tINPUT B: "); Serial.print(mReadValueB);
//    Serial.print("\tINPUT C: "); Serial.print(mReadValueC);
//    Serial.print("\tINPUT D: "); Serial.print(mReadValueD);
//    Serial.print("\t");
    delay(100);
  }
}

int mBlinkerLastCurrentTime;
bool mChangeBlinkerOnOff = false;

void RxInputBlinker(int inputValue, int minValue, int maxValue, int middleValue) {

  // straigt ahead
  if(inputValue > middleValue - mDeathbandPlusMinus && 
     inputValue < middleValue + mDeathbandPlusMinus) {
  
    CarLEDsSetOffBlinkers();
    return;
  }

  if(mCurrentMillis - mBlinkerLastCurrentTime > 200) {
    mChangeBlinkerOnOff = !mChangeBlinkerOnOff;
    mBlinkerLastCurrentTime = mCurrentMillis;
  }
  
  if(mChangeBlinkerOnOff) {
    CarLEDsSetOffBlinkers();
    return;
  }
  
  // set left
  if(inputValue >= middleValue + mDeathbandPlusMinus) {
    CarLEDsSetOnBlinkersLeft();
    return;
  }

  // set right
  if(inputValue <= middleValue - mDeathbandPlusMinus) {
    CarLEDsSetOnBlinkersRight();
    return;
  }
}



void RxInputSetOnStandLightOrDriveLight(int inputValue, int minValue, int maxValue, int middleValue) {

  // set off lights
  if(inputValue < minValue + mThresholdValue) {
    CarLEDsSetOffStandAndNormalLights();
    return;
  }

  // stand lights
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {
    CarLEDsSetOnStandLights();
    CarLEDsSetOffNormalLights();
    return;
  }

  // drive lights
  if(inputValue > maxValue - mThresholdValue) {
    CarLEDsSetOnNormalLights();
  }
}

bool mRxResetMovingLight = false;

void RxInputSetOnStripHeadlightOrStripAnimation(int inputValue, int minValue, int maxValue, int middleValue) {

  // set off lights
  if(inputValue < minValue + mThresholdValue) {

    RoofAnimationFadeOut();
    RoofUpdateRgbLights();

    BumperAnimationFadeOut();
    BumperUpdateRgbLights();

    mRxResetMovingLight = true;
    return;
  }

  // headlight on
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    RoofAnimationFadeOn();
    RoofUpdateRgbLights();

    BumperAnimationFadeOn();
    BumperUpdateRgbLights();

    mRxResetMovingLight = true;
    return;
  }

  // animation
  if(inputValue > maxValue - mThresholdValue) {

    if(mRxResetMovingLight) {
      Serial.print("Reset moving light\t");
      RoofMovingReset();
      mRxResetMovingLight = false;
    }

    RoofSetMoving();
    
    // update prepared rgb values setup values to show.
    RoofUpdateRgbLights();
    BumperUpdateRgbLights();
  }
}

int mStripAnimLastCurrentTime;

void RxInputSetLightBrightness(int inputValue, int minValue, int maxValue, int middleValue) {

  int mappedValueForRgb = map(inputValue, minValue, maxValue, 0, 255);
  int mappedValueForLEDs = map(inputValue, minValue, maxValue, 0, 100);

  if(mSerialMonitor) {
//    Serial.print("Mapped Value for RGBs: "); Serial.print(mappedValueForRgb, DEC); Serial.print("\t");
//    Serial.print("Mapped Value for LEDs: "); Serial.print(mappedValueForLEDs, DEC); Serial.print("\t");
  }
  
  mRgbBrightnessMaxValue1 = mappedValueForRgb;
  mRgbBrightnessMaxValue2 = mappedValueForRgb;
  mRgbBrightnessMaxValue3 = mappedValueForRgb;
  mRgbBrightnessMaxValue4 = mappedValueForRgb;

  mCarLedBrightness = mappedValueForLEDs;
}

void RxInputSetValueToTargetLights(){

  // read all input receiver signals
  RcInputsReadInputs();
  
  RxInputSetValueTo(mInputValueA_To, mReadValueA, mInputMinA, mInputMaxA, mInputMiddleA);
  RxInputSetValueTo(mInputValueB_To, mReadValueB, mInputMinB, mInputMaxB, mInputMiddleB);
  RxInputSetValueTo(mInputValueC_To, mReadValueC, mInputMinC, mInputMaxC, mInputMiddleC);
  RxInputSetValueTo(mInputValueD_To, mReadValueD, mInputMinD, mInputMaxD, mInputMiddleD);
}

void RxInputSetValueTo(int target, int value, int minValue, int maxValue, int middleValue){
  switch(target) {
    case(1): {
      RxInputBlinker(value, minValue, maxValue, middleValue);
      break;
    }
    case(2): {
      RxInputSetOnStandLightOrDriveLight(value, minValue, maxValue, middleValue);
      CarLEDsUpdate();
      break;
    }
    case(3): {
      RxInputSetOnStripHeadlightOrStripAnimation(value, minValue, maxValue, middleValue);
      break;
    }
    case(4): {
      RxInputSetLightBrightness(value, minValue, maxValue, middleValue);
      break;
    }
    default: {
      break;
    }
  }
}
