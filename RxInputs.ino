void RcInputsReadInputs() {
  mReadValueA = pulseIn(PIN_INPUT_A, HIGH);
  mReadValueB = pulseIn(PIN_INPUT_B, HIGH);
  mReadValueC = pulseIn(PIN_INPUT_C, HIGH);
  mReadValueD = pulseIn(PIN_INPUT_D, HIGH);

  // TODO: Ausgabe der Werte wieder geben.
  if(mSerialMonitor) {
//    Serial.println("Read channel signals:");
    Serial.print("INPUT A: "); Serial.print(mReadValueA);
    Serial.print("\tINPUT B: "); Serial.print(mReadValueB);
    Serial.print("\tINPUT C: "); Serial.print(mReadValueC);
    Serial.print("\tINPUT D: "); Serial.println(mReadValueD);
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

  if(mCurrentMillis - mBlinkerLastCurrentTime > 300) {
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
      RoofMovingReset();
      mRxResetMovingLight = false;
    }

    RoofSetMoving();
    
//    // set animationmode
//    switch(mPixel_1_AnimationMode) {
//      case(0): {
//        // Set next rgb LED
//        RoofSetMoving();
//        break;
//      }
//      case(1): {
//        RoofAnimationFadeOut();
//        break;
//      }
//      case(2): {
//        RoofSetMoving();
//        break;
//      }
//      default: {
//        RoofAnimationFadeOut();
//        break;
//      }
//    }

//    switch(mPixel_2_AnimationMode) {
//      case(0): {
//        BumperAnimationFadeOut();
//        break;
//      }
//      case(1): {
//        // TODO offen
//        break;
//      }
//      case(2): {
//        // TODO offen
//        break;
//      }
//      default: {
//        RoofAnimationFadeOut();
//        break;
//      }
//    }
    
    // update prepared rgb values setup values to show.
    RoofUpdateRgbLights();
    BumperUpdateRgbLights();
  }
}

int mStripAnimLastCurrentTime;

void RxInputSetStripAnimMode() {
  if(mCurrentMillis - mStripAnimLastCurrentTime > 1000) {

    if(mPixel_1_AnimationMode < 4) {
      mPixel_1_AnimationMode++;
      mPixel_2_AnimationMode++;
      mPixel_3_AnimationMode++;
      mPixel_4_AnimationMode++;
    }
    else {
      mPixel_1_AnimationMode = 0;
      mPixel_2_AnimationMode = 0;
      mPixel_3_AnimationMode = 0;
      mPixel_4_AnimationMode = 0;
    }
    
    mStripAnimLastCurrentTime = mCurrentMillis;
  }
}

void RxInputSetValueToTargetLights(){
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
      break;
    }
    case(3): {
      RxInputSetOnStripHeadlightOrStripAnimation(value, minValue, maxValue, middleValue);
      break;
    }
    case(4): {
      //RxInputSetStripAnimMode();
      break;
    }
    default: {
      break;
    }
  }
}
