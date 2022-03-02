unsigned long mBumperLastCurrentTime = 0;

int8_t mBumperOfflineState = 0;

boolean Bumper_CurrentTimeup(){
  if(mCurrentMillis - mBumperLastCurrentTime < 40) {
    return true;
  }
  mBumperLastCurrentTime = mCurrentMillis;
  return false;
}

// ========================================================================================
// 
void Bumper_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  if(Bumper_CurrentTimeup()) {
    return;
  }
  
  // bumper lights off (signal is near 1000 or 2000)
  if(inputValue < 1000) {//minValue + mThresholdValue) {

    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper Aus");
    }
    Bumper_Off();
    return;
  }

  // bumper lights on (signal is near 1500)
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper An");
    }
    Bumper_On();
    return;
  }

  // bumper animation for walking light (signal is near to 2000 or 1000)
  if(inputValue > maxValue - mThresholdValue) {
    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper Animation");
    }
    
    Bumper_WalkingLight();
  }
}

// ========================================================================================
// 
void Bumper_Update(){

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    uint8_t red = mMoveLightArray_Bumper[index].Red;
    uint8_t green = mMoveLightArray_Bumper[index].Green;
    uint8_t blue = mMoveLightArray_Bumper[index].Blue;

//    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
//    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
//    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels_Bumper.setPixelColor(index, mPixels_Bumper.Color(red, green, blue));
  }

  mPixels_Bumper.show();
}

// ========================================================================================
// used if the remote connection lost
void Bumper_GoOnline(){

  if(Bumper_CurrentTimeup()) {
    return;
  }

  if(mBumper_GoOnline_Index >= mCountRgbLeds2) {
    mBumper_GoOnline_Finish = true;
    return;
  }
  
  mMoveLightArray_Bumper[mCountRgbLeds2 - mBumper_GoOnline_Index - 1].Red = 100;
  mMoveLightArray_Bumper[mBumper_GoOnline_Index].Green = 0;
  mMoveLightArray_Bumper[mBumper_GoOnline_Index].Blue = 100;

  if(mBumper_GoOnline_Index < mCountRgbLeds2) {
    mBumper_GoOnline_Index++;
  }
}

void Bumper_GoOnline_Fadeout() {

  if(Bumper_CurrentTimeup()) {
    return;
  }

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Red, 3);
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Green, 5);
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Blue, 5);
  }

  if(mMoveLightArray_Bumper[0].Red == 0 &&
     mMoveLightArray_Bumper[0].Red == 0 &&
     mMoveLightArray_Bumper[0].Red == 0) {
      delay(300);
      mRoof_GoOnline_Finish = true;
  }
}

// ========================================================================================
// Set first RGB LED on Front Bumper.
void Bumper_On() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    mMoveLightArray_Bumper[index].Red = 100;
    mMoveLightArray_Bumper[index].Green = 234;
    mMoveLightArray_Bumper[index].Blue = 100;
  }
  
//  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
//    mPixels_Bumper.setPixelColor(index, mPixels_Bumper.Color(100, 234, 100));
//  }
//  mPixels_Bumper.show();
}

// ========================================================================================
// set all off
void Bumper_Off() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    mMoveLightArray_Bumper[index].Red = 50;
    mMoveLightArray_Bumper[index].Green = 50;
    mMoveLightArray_Bumper[index].Blue = 50;
  }
  
//  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
//    mPixels_Bumper.setPixelColor(index, mPixels_Bumper.Color(0, 0, 0));
//  }
//  mPixels_Bumper.show();
}

int8_t mBumperAnimationIndex = 0;
bool mBumperAnimationLeftToRight = true;

void Bumper_WalkingLight() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Red, 30);
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Green, 30);
    WS2812_Helper_Reduce(mMoveLightArray_Bumper[index].Blue, 30);
  }

  if(mBumperAnimationLeftToRight && mBumperAnimationIndex < mCountRgbLeds2) {
    mBumperAnimationIndex++;
    if(mBumperAnimationIndex >= mCountRgbLeds2) {
      mBumperAnimationLeftToRight = false;
    }
  }

  if(!mBumperAnimationLeftToRight && mBumperAnimationIndex > 0) {
    mBumperAnimationIndex--;
    if(mBumperAnimationIndex <= 0) {
      mBumperAnimationLeftToRight = true;
    }
  }

  mMoveLightArray_Bumper[mBumperAnimationIndex].Red = 156;
  mMoveLightArray_Bumper[mBumperAnimationIndex].Green = 0;
  mMoveLightArray_Bumper[mBumperAnimationIndex].Blue = 255;
}
