unsigned long mBumperLastCurrentTime = 0;

// TODO: kann auch zentralsiiert werden
unsigned long mBumperSpeed = 50;

int8_t mBumperOfflineState = 0;
bool mBumper_On_Prepare = false;
bool mBumper_Off_Prepare = false;

boolean Bumper_CurrentTimeup(){
  if(mCurrentMillis - mBumperLastCurrentTime < mBumperSpeed) {
    return true;
  }
  mBumperLastCurrentTime = mCurrentMillis;
  return false;
}

// ========================================================================================
// 
void Bumper_SetAnimationMod(int inputValue, int minValue, int maxValue, int middleValue) {

  mBumperSpeed = 40;
  if(Bumper_CurrentTimeup()) {
    return;
  }
  
  // bumper lights off (signal is near 1000 or 2000)
  if(inputValue < 1000) {//minValue + mThresholdValue) {

    if(mSerialMonitor) {
      //Serial.println("WS2812 RGB - Bumper Aus");
    }
    mBumper_On_Prepare = false;
    Bumper_Off();
    return;
  }

  // bumper lights on (signal is near 1500)
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {

    if(mSerialMonitor) {
      //Serial.println("WS2812 RGB - Bumper An");
    }
    mBumper_Off_Prepare = false;
    Bumper_On();
    return;
  }

  // bumper animation for walking light (signal is near to 2000 or 1000)
  if(inputValue > maxValue - mThresholdValue) {
    if(mSerialMonitor) {
      //Serial.println("WS2812 RGB - Bumper Animation");
    }

    mBumper_Off_Prepare = false;
    mBumper_On_Prepare = false;
    Bumper_WalkingLight();
  }
}

// ========================================================================================
// 
void Bumper_Update(){

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    mPixels_Bumper.setPixelColor(index, mRgbSetup_Bumper[index].GetColor());
  }

  mPixels_Bumper.show();
}

// ========================================================================================
// used if the remote connection lost
void Bumper_GoOnline(){

  mBumperSpeed = 100;
  if(Bumper_CurrentTimeup()) {
    return;
  }

  if(mBumper_GoOnline_Index >= mCountRgbLeds2) {
    mBumper_GoOnline_Finish = true;
    return;
  }
  
  int8_t moveLeft = GetMoveLeft(mBumper_GoOnline_Index, mCountRgbLeds2);
  int8_t moveRight = GetMoveRight(mBumper_GoOnline_Index, mCountRgbLeds2);

  mRgbSetup_Bumper[moveLeft].Red = 20;
  mRgbSetup_Bumper[moveLeft].Green = 100;
  mRgbSetup_Bumper[moveLeft].Blue = 20;

  mRgbSetup_Bumper[moveRight].Red = 20;
  mRgbSetup_Bumper[moveRight].Green = 100;
  mRgbSetup_Bumper[moveRight].Blue = 20;

  if(mBumper_GoOnline_Index < mCountRgbLeds2) {
    mBumper_GoOnline_Index++;
  }
}

void Bumper_GoOnline_Fadeout() {

  mBumperSpeed = 40;
  if(Bumper_CurrentTimeup()) {
    return;
  }

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Red, 3);
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Green, 5);
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Blue, 2);
  }

  if(mRgbSetup_Bumper[0].Red == 0 &&
     mRgbSetup_Bumper[0].Green == 0 &&
     mRgbSetup_Bumper[0].Blue == 0) {
      delay(300);
      mBumper_GoOnline_Finish = true;
  }
}

void Bumper_FadeToTarget(uint8_t index){
  uint8_t red = mRgbSetup_Bumper[index].Red;
  uint8_t green = mRgbSetup_Bumper[index].Green;
  uint8_t blue = mRgbSetup_Bumper[index].Blue;

  if(red > mRgbSetup_Bumper[index].TargetRed){
    red--;
  }
  if(red < mRgbSetup_Bumper[index].TargetRed){
    red++;
  }

  if(green > mRgbSetup_Bumper[index].TargetGreen){
    green--;
  }
  if(green < mRgbSetup_Bumper[index].TargetGreen){
    green++;
  }

  if(blue > mRgbSetup_Bumper[index].TargetBlue){
    blue--;
  }
  if(blue < mRgbSetup_Bumper[index].TargetBlue){
    blue++;
  }

  mRgbSetup_Bumper[index].Red = red;
  mRgbSetup_Bumper[index].Green = green;
  mRgbSetup_Bumper[index].Blue = blue;
}

// ========================================================================================
// Set first RGB LED on Front Bumper.
void Bumper_On() {

  Bumper_On_Prepare();
  
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
     Bumper_FadeToTarget(index);
  }
}

void Bumper_On_Prepare(){
  
  if(mBumper_On_Prepare) {
    return;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    mRgbSetup_Bumper[index].TargetRed = 100;
    mRgbSetup_Bumper[index].TargetGreen = 100;
    mRgbSetup_Bumper[index].TargetBlue = 100;
  }
  
  mBumper_On_Prepare = true;
}

// ========================================================================================
// set all off
void Bumper_Off() {

  Bumper_Off_Prepare();
  
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    Bumper_FadeToTarget(index);
  }
}

void Bumper_Off_Prepare() {
  
  if(mBumper_Off_Prepare) {
    return;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    mRgbSetup_Bumper[index].TargetRed = 0;
    mRgbSetup_Bumper[index].TargetGreen = 0;
    mRgbSetup_Bumper[index].TargetBlue = 0;
  }
  
  mBumper_Off_Prepare = true;
}

int8_t mBumperAnimationIndex = 0;
bool mBumperAnimationLeftToRight = true;

void Bumper_WalkingLight() {

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Red, 30);
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Green, 30);
    WS2812_Helper_Reduce(mRgbSetup_Bumper[index].Blue, 30);
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

  Helper_SetRgbColor(
    mBumper_ColorIndex,
    mRgbSetup_Bumper[mBumperAnimationIndex].Red,
    mRgbSetup_Bumper[mBumperAnimationIndex].Green,
    mRgbSetup_Bumper[mBumperAnimationIndex].Blue);
}

void Bumper_SignalLost(){

  mBumperSpeed = 10;
  if(Bumper_CurrentTimeup()) {
    return;
  }

  if(mRgbSetup_Bumper[mBumperAnimationIndex].Red == 0 &&
    mRgbSetup_Bumper[mBumperAnimationIndex].Green == 0 &&
    mRgbSetup_Bumper[mBumperAnimationIndex].Blue == 0) {
    
    for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
      mRgbSetup_Bumper[index].TargetRed = 100;
      mRgbSetup_Bumper[index].TargetGreen = 0;
      mRgbSetup_Bumper[index].TargetBlue = 0;
    }
  }
  
  if(mRgbSetup_Bumper[mBumperAnimationIndex].Red >= 100 &&
    mRgbSetup_Bumper[mBumperAnimationIndex].Green == 0 &&
    mRgbSetup_Bumper[mBumperAnimationIndex].Blue == 0) {

     for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
      mRgbSetup_Bumper[index].TargetRed = 0;
      mRgbSetup_Bumper[index].TargetGreen = 0;
      mRgbSetup_Bumper[index].TargetBlue = 0;
    }
  }

  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
     Bumper_FadeToTarget(index);
  }
}
