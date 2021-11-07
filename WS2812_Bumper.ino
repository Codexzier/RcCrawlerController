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

//  if(mBumperOfflineState < mCountRgbLeds2 - 1) {
//    mBumperOfflineState++;
//  }
//  else {
//    mBumperOfflineState = 0;
//  }
//  
//  mPixels2.setPixelColor(mBumperOfflineState, mPixels2.Color(100, 0, 0));
  mPixels2.show();
}

// ========================================================================================
// Set first RGB LED on Front Bumper.
void Bumper_On() {
  
  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
    mPixels2.setPixelColor(index, mPixels2.Color(100, 234, 100));
  }
  mPixels2.show();
}

// ========================================================================================
// set all off
void Bumper_Off() {
  for(int8_t index = 0; index < mCountRgbLeds2; index++) {
    mPixels2.setPixelColor(index, mPixels2.Color(0, 0, 0));
  }
  mPixels2.show();
}

// ========================================================================================
// 
void Bumper_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  // set off lights
  if(inputValue < minValue + mThresholdValue) {

    Bumper_Off();
    return;
  }

  // headlight on
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    Bumper_On();
    return;
  }

  // animation
  if(inputValue > maxValue - mThresholdValue) {

    // TODO: Animation
  }
}

// ========================================================================================
// 
void Bumper_Update(){
  
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    
    uint8_t red = mMoveLightArray_2_Red[index];
    uint8_t green = mMoveLightArray_2_Green[index];
    uint8_t blue = mMoveLightArray_2_Blue[index];

    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels2.setPixelColor(index, mPixels2.Color(red, green, blue));
  }
  
  mPixels2.show();
}
