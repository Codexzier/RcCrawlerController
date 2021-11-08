unsigned long mBumperLastCurrentTime = 0;

int8_t mBumperOfflineState = 0;


// ========================================================================================
// used if the remote connection lost
void Bumper_Offline(){
  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
    mPixels2.setPixelColor(index, mPixels2.Color(100, 0, 0));
    mPixels2.show();
    delay(20);
    mPixels2.setPixelColor(index, mPixels2.Color(0, 0, 0));
  }

  mPixels2.show();
}

// ========================================================================================
// Set first RGB LED on Front Bumper.
void Bumper_On() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    mMoveLightArray_2_Red[index] = 100;
    mMoveLightArray_2_Green[index] = 234;
    mMoveLightArray_2_Blue[index] = 100;
  }
  
//  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
//    mPixels2.setPixelColor(index, mPixels2.Color(100, 234, 100));
//  }
//  mPixels2.show();
}

// ========================================================================================
// set all off
void Bumper_Off() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    mMoveLightArray_2_Red[index] = 50;
    mMoveLightArray_2_Green[index] = 50;
    mMoveLightArray_2_Blue[index] = 50;
  }
  
//  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
//    mPixels2.setPixelColor(index, mPixels2.Color(0, 0, 0));
//  }
//  mPixels2.show();
}

// ========================================================================================
// 
void Bumper_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  if(mCurrentMillis - mBumperLastCurrentTime < 70) {
    return;
  }
  mBumperLastCurrentTime = mCurrentMillis;
  
  // set off lights
  if(inputValue < 1000) {//minValue + mThresholdValue) {

    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper Aus");
    }
    Bumper_Off();
    return;
  }

  // headlight on
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper An");
    }
    Bumper_On();
    return;
  }

  // animation
  if(inputValue > maxValue - mThresholdValue) {
    if(mSerialMonitor) {
      Serial.println("WS2812 RGB - Bumper Animation");
    }
    
    // TODO: Animation
    Bumper_WalkingLight();
    //delay(100);
  }
}

// ========================================================================================
// 
void Bumper_Update(){

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    uint8_t red = mMoveLightArray_2_Red[index];
    uint8_t green = mMoveLightArray_2_Green[index];
    uint8_t blue = mMoveLightArray_2_Blue[index];

//    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
//    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
//    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels2.setPixelColor(index, mPixels2.Color(red, green, blue));
  }

  //Serial.println();
  mPixels2.show();
}

int8_t mBumperAnimationIndex = 0;
bool mBumperAnimationLeftToRight = true;

void Bumper_WalkingLight() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    WS2812_Helper_Reduce(mMoveLightArray_2_Red[index], 15);
    WS2812_Helper_Reduce(mMoveLightArray_2_Green[index], 15);
    WS2812_Helper_Reduce(mMoveLightArray_2_Blue[index], 15);
  }

//  if(mBumperAnimationIndex < mCountRgbLeds2) {
//    mBumperAnimationIndex++;
//  }
//  else {
//    mBumperAnimationIndex = 0;
//  }

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

  mMoveLightArray_2_Red[mBumperAnimationIndex] = 156;
  mMoveLightArray_2_Green[mBumperAnimationIndex] = 0;
  mMoveLightArray_2_Blue[mBumperAnimationIndex] = 255;
}
