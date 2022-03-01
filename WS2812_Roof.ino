unsigned long mRoofLastCurrentTime = 0;
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

  mPixels1.show();
}


// ========================================================================================
// Set first RGB LED on Front Roof.
void Roof_On() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(100, 150, 100));
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

  if(mCurrentMillis - mRoofLastCurrentTime < 22) {
    return;
  }
  mRoofLastCurrentTime = mCurrentMillis;

  // roof lights off (signal is near 1000 or 2000)
  if(inputValue < minValue + mThresholdValue) {

    Roof_Off();
    return;
  }

  // roof lights on (signal is near 1500)
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    Roof_On();
    return;
  }

  // roof lights animation for walking light (signal is near to 2000 or 1000)
  if(inputValue > maxValue - mThresholdValue) {
    Roof_WalkingLight();
  }
}

void Roof_Update(){
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {

    uint8_t red = mMoveLightArray_1_Red[index];
    uint8_t green = mMoveLightArray_1_Green[index];
    uint8_t blue = mMoveLightArray_1_Blue[index];

//    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
//    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
//    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels1.setPixelColor(index, mPixels1.Color(red, green, blue));
  }
  
  mPixels1.show();
}

int8_t mRoofAnimationIndex = 0;
bool mRoofAnimationLeftToRight = true;

void Roof_WalkingLight() {

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    
    WS2812_Helper_Reduce(mMoveLightArray_1_Red[index], 10);
    WS2812_Helper_Reduce(mMoveLightArray_1_Green[index], 10);
    WS2812_Helper_Reduce(mMoveLightArray_1_Blue[index], 10);
  }

  if(mRoofAnimationLeftToRight && mRoofAnimationIndex < mCountRgbLeds1) {
    mRoofAnimationIndex++;
    if(mRoofAnimationIndex >= mCountRgbLeds1) {
      mRoofAnimationLeftToRight = false;
    }
  }

  if(!mRoofAnimationLeftToRight && mRoofAnimationIndex > 0) {
    mRoofAnimationIndex--;
    if(mRoofAnimationIndex <= 0) {
      mRoofAnimationLeftToRight = true;
    }
  }

  mMoveLightArray_1_Red[mRoofAnimationIndex] = 100;
  mMoveLightArray_1_Green[mRoofAnimationIndex] = 0; //20;
  mMoveLightArray_1_Blue[mRoofAnimationIndex] = 0;//240;
}
