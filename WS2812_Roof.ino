unsigned long mRoofLastCurrentTime = 0;

// TODO: kann auch zentralsiiert werden
unsigned long mRoofSpeed = 50;

bool mRoof_On_Prepare = false;
bool mRoof_Off_Prepare = false;

boolean Roof_CurrentTimeup(){
  if(mCurrentMillis - mRoofLastCurrentTime < mRoofSpeed) {
    return true;
  }
  mRoofLastCurrentTime = mCurrentMillis;
  return false;
}

// ========================================================================================
// 
void Roof_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  mRoofSpeed = 20;
  if(Roof_CurrentTimeup()) {
    return;
  }
  
  // roof lights off (signal is near 1000 or 2000)
  if(inputValue < minValue + mThresholdValue) {

    mRoof_On_Prepare = false;
    Roof_Off();
    return;
  }

  // roof lights on (signal is near 1500)
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {
    
    mRoof_Off_Prepare = false;
    Roof_On();
    return;
  }

  // roof lights animation for walking light (signal is near to 2000 or 1000)
  if(inputValue > maxValue - mThresholdValue) {

    mRoof_Off_Prepare = false;
    mRoof_On_Prepare = false;
    Roof_WalkingLight();
  }
}

void Roof_Update(){
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {

    uint8_t red = mRgbSetup_Roof[index].Red;
    uint8_t green = mRgbSetup_Roof[index].Green;
    uint8_t blue = mRgbSetup_Roof[index].Blue;

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

  mRoofSpeed = 50;
  if(Roof_CurrentTimeup()) {
    return;
  }

  if(mRoof_GoOnline_Index >= mCountRgbLeds1) {
    mRoof_GoOnline_Finish = true;
    return;
  }

  int8_t moveLeft = GetMoveLeft(mRoof_GoOnline_Index, mCountRgbLeds1);
  int8_t moveRight = GetMoveRight(mRoof_GoOnline_Index, mCountRgbLeds1);

  mRgbSetup_Roof[moveLeft].Red = 100;
  mRgbSetup_Roof[moveLeft].Green = 0;
  mRgbSetup_Roof[moveLeft].Blue = 0;

  mRgbSetup_Roof[moveRight].Red = 100;
  mRgbSetup_Roof[moveRight].Green = 0;
  mRgbSetup_Roof[moveRight].Blue = 0;

  if(mRoof_GoOnline_Index < mCountRgbLeds1) {
    mRoof_GoOnline_Index++;
  }
}

void Roof_GoOnline_Fadeout() {

  if(Roof_CurrentTimeup()) {
    return;
  }

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Red, 6);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Green, 10);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Blue, 12);
  }

  if(mSerialMonitor) {
    Serial.print("- "); Serial.println(mRgbSetup_Roof[10].Blue);
  }

  if(mRgbSetup_Roof[10].Red == 0 &&
     mRgbSetup_Roof[10].Green == 0 &&
     mRgbSetup_Roof[10].Blue == 0) {
      delay(300);
      mRoof_GoOnline_Finish = true;

      if(mSerialMonitor) {
        Serial.println("Fadeout finish");
      }
  }
}

void Roof_FadeToTarget(uint8_t index){
  uint8_t red = mRgbSetup_Roof[index].Red;
  uint8_t green = mRgbSetup_Roof[index].Green;
  uint8_t blue = mRgbSetup_Roof[index].Blue;

  if(red > mRgbSetup_Roof[index].TargetRed){
    red--;
  }
  if(red < mRgbSetup_Roof[index].TargetRed){
    red++;
  }

  if(green > mRgbSetup_Roof[index].TargetGreen){
    green--;
  }
  if(green < mRgbSetup_Roof[index].TargetGreen){
    green++;
  }

  if(blue > mRgbSetup_Roof[index].TargetBlue){
    blue--;
  }
  if(blue < mRgbSetup_Roof[index].TargetBlue){
    blue++;
  }

  mRgbSetup_Roof[index].Red = red;
  mRgbSetup_Roof[index].Green = green;
  mRgbSetup_Roof[index].Blue = blue;
}

// ========================================================================================
// Set first RGB LED on Front Roof.
void Roof_On() {

  Roof_On_Prepare();
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    Roof_FadeToTarget(index);
//    mRgbSetup_Roof[index].Red = 100;
//    mRgbSetup_Roof[index].Green = 100;
//    mRgbSetup_Roof[index].Blue = 100;
  }
}

void Roof_On_Prepare(){
  
  if(mRoof_On_Prepare) {
    return;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mRgbSetup_Roof[index].TargetRed = 100;
    mRgbSetup_Roof[index].TargetGreen = 100;
    mRgbSetup_Roof[index].TargetBlue = 100;
  }
  
  mRoof_On_Prepare = true;
}

void Roof_Off() {
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mRgbSetup_Roof[index].Red = 0;
    mRgbSetup_Roof[index].Green = 0;
    mRgbSetup_Roof[index].Blue = 0;
  }
}

void Roof_Off_Prepare() {
  
  if(mRoof_Off_Prepare) {
    return;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mRgbSetup_Roof[index].TargetRed = 0;
    mRgbSetup_Roof[index].TargetGreen = 0;
    mRgbSetup_Roof[index].TargetBlue = 0;
  }
  
  mRoof_Off_Prepare = true;
}


int8_t mRoofAnimationIndex = 0;
int8_t mRoofAnimationIndex2 = 0;
bool mRoofAnimationLeftToRight = true;
bool mRoofAnimationChangedLeftRightSide = true;

void Roof_WalkingLight() {
    
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Red, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Green, 4);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Blue, 5);
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

  if(mRoofAnimationIndex2 < mCountRgbLeds1) {
    mRoofAnimationIndex2++;
  }
  else {
    mRoofAnimationIndex2 = 0;
    mRoofAnimationChangedLeftRightSide = !mRoofAnimationChangedLeftRightSide;
  }
  if(mSerialMonitor) {
    Serial.print("- mRoofAnimationIndex2 "); Serial.println(mRoofAnimationIndex2, DEC);
  }

  int8_t moveLeft = GetMoveLeft(mRoofAnimationIndex2, mCountRgbLeds1);
  int8_t moveRight = GetMoveRight(mRoofAnimationIndex2, mCountRgbLeds1);

  if(mSerialMonitor) {
    Serial.print("- moveLeft "); Serial.println(moveLeft, DEC);
    Serial.print("- moveRight "); Serial.println(moveRight, DEC);
  }

  uint8_t red = 10;
  uint8_t green = 0;
  uint8_t blue = 0;
  Helper_SetRgbColor(8, red, green, blue);
    
  if(mRoofAnimationChangedLeftRightSide){
    if(moveLeft > -1) {
      mRgbSetup_Roof[moveLeft].Red = red;
      mRgbSetup_Roof[moveLeft].Green = green;
      mRgbSetup_Roof[moveLeft].Blue = blue;
    }
    
    if(moveRight > -1) {
      mRgbSetup_Roof[moveRight].Red = red;
      mRgbSetup_Roof[moveRight].Green = green;
      mRgbSetup_Roof[moveRight].Blue = blue;
    }
  }
  else {
    if(moveLeft > -1) {
      mRgbSetup_Roof[moveRight].Red = red;
      mRgbSetup_Roof[moveRight].Green = green;
      mRgbSetup_Roof[moveRight].Blue = blue;
    }
    
    if(moveRight > -1) {
      mRgbSetup_Roof[moveLeft].Red = red;
      mRgbSetup_Roof[moveLeft].Green = green;
      mRgbSetup_Roof[moveLeft].Blue = blue;
    }
  }
  
}
