unsigned long mRoofLastCurrentTime = 0;

boolean Roof_CurrentTimeup(){
  if(mCurrentMillis - mRoofLastCurrentTime < 22) {
    return true;
  }
  mRoofLastCurrentTime = mCurrentMillis;
  return false;
}

// ========================================================================================
// 
void Roof_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  if(Roof_CurrentTimeup()) {
    return;
  }
  
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

    uint8_t red = mMoveLightArray_1[index].Red;
    uint8_t green = mMoveLightArray_1[index].Green;
    uint8_t blue = mMoveLightArray_1[index].Blue;

//    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
//    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
//    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
    
    mPixels_Roof.setPixelColor(index, mPixels_Roof.Color(red, green, blue));
  }
  
  mPixels_Roof.show();
}

// ========================================================================================
// used if the remote connection lost
void Roof_GoOnline() {

  if(Roof_CurrentTimeup()) {
    return;
  }

  if(mRoof_GoOnline_Index >= mCountRgbLeds1) {
    mRoof_GoOnline_Finish = true;
    return;
  }
  
  mMoveLightArray_1[mRoof_GoOnline_Index].Red = 0;
  mMoveLightArray_1[mRoof_GoOnline_Index].Green = 100;
  mMoveLightArray_1[mCountRgbLeds1 - mRoof_GoOnline_Index - 1].Blue = 100;

  if(mRoof_GoOnline_Index < mCountRgbLeds1) {
    mRoof_GoOnline_Index++;
  }
}

void Roof_GoOnline_Fadeout() {

  if(Roof_CurrentTimeup()) {
    return;
  }

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Red, 3);
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Green, 5);
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Blue, 2);
  }

  if(mMoveLightArray_1[0].Red == 0 &&
     mMoveLightArray_1[0].Green == 0 &&
     mMoveLightArray_1[0].Blue == 0) {
      delay(300);
      mRoof_GoOnline_Finish = true;
  }
}

// ========================================================================================
// Set first RGB LED on Front Roof.
void Roof_On() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels_Roof.setPixelColor(index, mPixels_Roof.Color(100, 150, 100));
  }
  mPixels_Roof.show();
}

void Roof_Off() {
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels_Roof.setPixelColor(index, mPixels_Roof.Color(0, 0, 0));
  }
  mPixels_Roof.show();
}

int8_t mRoofAnimationIndex = 0;
bool mRoofAnimationLeftToRight = true;

void Roof_WalkingLight() {

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Red, 10);
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Green, 10);
    WS2812_Helper_Reduce(mMoveLightArray_1[index].Blue, 10);
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

  mMoveLightArray_1[mRoofAnimationIndex].Red = 100;
  mMoveLightArray_1[mRoofAnimationIndex].Green = 0; //20;
  mMoveLightArray_1[mRoofAnimationIndex].Blue = 0;//240;
}
