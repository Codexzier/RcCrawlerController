int8_t mRoofOfflineState = 0;

// ========================================================================================
// used if the remote connection lost
void Roof_Offline() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(100, 0, 0));
    mPixels1.show();
    delay(20);
    mPixels1.setPixelColor(index, mPixels1.Color(0, 0, 0));
  }
//  if(mRoofOfflineState < mCountRgbLeds1 - 1) {
//    mRoofOfflineState++;
//  }
//  else {
//    mRoofOfflineState = 0;
//  }
//  
//  mPixels1.setPixelColor(mRoofOfflineState, mPixels1.Color(70, 0, 0));
  mPixels1.show();
}


// ========================================================================================
// Set first RGB LED on Front Roof.
void Roof_On() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(255, 150, 255));
  }
  mPixels1.show();
}

void Roof_Off() {
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(0, 0, 0));
  }
  mPixels1.show();
}



// ========================================================================================
// 
void Roof_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  // set off lights
  if(inputValue < minValue + mThresholdValue) {

    Roof_Off();
    return;
  }

  // headlight on
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    Roof_On();
    return;
  }

  // animation
  if(inputValue > maxValue - mThresholdValue) {

    // TODO: Animation
  }
}

void Roof_Update(){
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {

    uint8_t red = mMoveLightArray_1_Red[index];
    uint8_t green = mMoveLightArray_1_Green[index];
    uint8_t blue = mMoveLightArray_1_Blue[index];

    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels1.setPixelColor(index, mPixels1.Color(red, green, blue));
  }
  
  mPixels1.show();
}
