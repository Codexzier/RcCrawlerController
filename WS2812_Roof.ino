unsigned long mRoofLastCurrentTime = 0;
unsigned long mRoofLastCurrentTime2 = 0;

// TODO: kann auch zentralsiiert werden
unsigned long mRoofSpeed = 50;
unsigned long mRoofSpeed2 = 15;

bool mRoof_On_Prepare = false;
bool mRoof_Off_Prepare = false;

boolean Roof_CurrentTimeup(){
  if(mCurrentMillis - mRoofLastCurrentTime < mRoofSpeed) {
    return true;
  }
  mRoofLastCurrentTime = mCurrentMillis;
  return false;
}

boolean Roof_CurrentTimeup2(){
  if(mCurrentMillis - mRoofLastCurrentTime2 < mRoofSpeed2) {
    return true;
  }
  mRoofLastCurrentTime2 = mCurrentMillis;
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

    Roof_PulseLight();
//    switch(mInputD_AnimationOption) {
//      case(1): { Roof_PulseLight(); break; }
//      case(2): { Roof_GlitterEffect(); break; }
//      default: {
//        Roof_GlitterEffect();
//        break;
//      }
//    }
  }
}

void Roof_Update(){
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mPixels_Roof.setPixelColor(index, mRgbSetup_Roof[index].GetColor());
  }
  
  mPixels_Roof.show();
}

// ========================================================================================
// used if the remote connection lost
void Roof_GoOnline() {

  mRoofSpeed = 30;
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
  uint8_t red = mRgbSetup_Roof[index].AltRed;
  uint8_t green = mRgbSetup_Roof[index].AltGreen;
  uint8_t blue = mRgbSetup_Roof[index].AltBlue;

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
  mRgbSetup_Roof[index].AltRed = red;
  mRgbSetup_Roof[index].AltGreen = green;
  mRgbSetup_Roof[index].AltBlue = blue;
}

// ========================================================================================
// Set first RGB LED on Front Roof.
void Roof_On() {

  Roof_On_Prepare();
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    Roof_FadeToTarget(index);
  }
}

void Roof_On_Prepare(){
  
  if(mRoof_On_Prepare) {
    return;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mRgbSetup_Roof[index].TargetRed = 250;
    mRgbSetup_Roof[index].TargetGreen = 250;
    mRgbSetup_Roof[index].TargetBlue = 250;
  }
  
  mRoof_On_Prepare = true;
}

void Roof_Off() {

  Roof_Off_Prepare();
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    Roof_FadeToTarget(index);
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

void Roof_PulseLight() {
    
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Red, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Green, 4);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Blue, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltRed, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltGreen, 4);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltBlue, 5);
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
      mRgbSetup_Roof[moveLeft].AltRed = red;
      mRgbSetup_Roof[moveLeft].AltGreen = green;
      mRgbSetup_Roof[moveLeft].AltBlue = blue;
    }
    
    if(moveRight > -1) {
      mRgbSetup_Roof[moveRight].Red = red;
      mRgbSetup_Roof[moveRight].Green = green;
      mRgbSetup_Roof[moveRight].Blue = blue;
      mRgbSetup_Roof[moveRight].AltRed = red;
      mRgbSetup_Roof[moveRight].AltGreen = green;
      mRgbSetup_Roof[moveRight].AltBlue = blue;
    }
  }
  else {
    if(moveLeft > -1) {
      mRgbSetup_Roof[moveRight].Red = red;
      mRgbSetup_Roof[moveRight].Green = green;
      mRgbSetup_Roof[moveRight].Blue = blue;
      mRgbSetup_Roof[moveRight].AltRed = red;
      mRgbSetup_Roof[moveRight].AltGreen = green;
      mRgbSetup_Roof[moveRight].AltBlue = blue;
    }
    
    if(moveRight > -1) {
      mRgbSetup_Roof[moveLeft].Red = red;
      mRgbSetup_Roof[moveLeft].Green = green;
      mRgbSetup_Roof[moveLeft].Blue = blue;
      mRgbSetup_Roof[moveLeft].AltRed = red;
      mRgbSetup_Roof[moveLeft].AltGreen = green;
      mRgbSetup_Roof[moveLeft].AltBlue = blue;
    }
  }
}

uint8_t mRed = 100;
uint8_t mRedSec = 0;
  
void Roof_SignalLost(){
  
  mRoofSpeed = 5;
  if(Roof_CurrentTimeup()) {
    return;
  }

  uint16_t middle = mCountRgbLeds1 / 2;

  if(mRgbSetup_Roof[0].Red == 0 &&
    mRgbSetup_Roof[0].Green == 0 &&
    mRgbSetup_Roof[0].Blue == 0) {
      mRed = 100;
      mRedSec = 0;
  }
  
  if(mRgbSetup_Roof[0].Red >= 100 &&
    mRgbSetup_Roof[0].Green == 0 &&
    mRgbSetup_Roof[0].Blue == 0) {
      mRed = 0;
      mRedSec = 100;
  }
  
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    
    if(index < middle) {
      mRgbSetup_Roof[index].TargetRed = mRed;
      mRgbSetup_Roof[index].TargetGreen = 0;
      mRgbSetup_Roof[index].TargetBlue = 0;
    }
    else {
      mRgbSetup_Roof[index].TargetRed = mRedSec;
      mRgbSetup_Roof[index].TargetGreen = 0;
      mRgbSetup_Roof[index].TargetBlue = 0;
    }
  }

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    Roof_FadeToTarget(index);
  }  

  if(mSerialMonitor) {
      Serial.print("Roof Signal Lost: ");
      Serial.println(mRgbSetup_Roof[0].Red, DEC);
      Serial.print("Roof Signal Lost Target: ");
      Serial.println(mRgbSetup_Roof[0].TargetRed, DEC);
      Serial.print("Roof Signal Lost: ");
      Serial.println(mRgbSetup_Roof[10].Red, DEC);
      Serial.print("Roof Signal Lost Target: ");
      Serial.println(mRgbSetup_Roof[10].TargetRed, DEC);
  }
}

uint8_t mRoof_State = 0;
uint8_t mRoof_Blinker_Red = 100;
uint8_t mRoof_Blinker_Green = 100;
uint8_t mRoof_Blinker_Blue = 0;

void Roof_Blinker(int inputValue, int minValue, int maxValue, int middleValue){

  mRoofSpeed2 = 20;
  if(Roof_CurrentTimeup2()) {
    return;
  }

  if(mRoof_State == 0) {
    mRoof_Blinker_Red = 100;
    mRoof_Blinker_Green = 100;
    mRoof_Blinker_Blue = 0;
  }
  
  if(mRoof_State >= 5) {
    mRoof_Blinker_Red = 0;
    mRoof_Blinker_Green = 0;
    mRoof_Blinker_Blue = 0;
  }

  if(mRoof_State >= 10) {
    mRoof_State = 0;
  }
  else {
    mRoof_State++;
  }

  // set right
  if(inputValue <= middleValue - mDeathbandPlusMinus) {
  
    if(mSerialMonitor) {
      //Serial.println("Blinker LEFT");
    }

    for(uint8_t index = 0; index < 3; index++) {
      if(mRoof_Blinker_Red == 0 &&
        mRoof_Blinker_Green == 0 &&
        mRoof_Blinker_Blue == 0) {
        mRgbSetup_Roof[index].Red = mRgbSetup_Roof[index].AltRed;
        mRgbSetup_Roof[index].Green = mRgbSetup_Roof[index].AltGreen;
        mRgbSetup_Roof[index].Blue = mRgbSetup_Roof[index].AltBlue;
      }
      else {
        mRgbSetup_Roof[index].Red = mRoof_Blinker_Red;
        mRgbSetup_Roof[index].Green = mRoof_Blinker_Green;
        mRgbSetup_Roof[index].Blue = mRoof_Blinker_Blue;
      }
    }

    return;
  }

  // set left
  if(inputValue >= middleValue + mDeathbandPlusMinus) {

    if(mSerialMonitor) {
      //Serial.println("Blinker RIGHT");
    }

    for(uint8_t index = mCountRgbLeds1 - 3; index < mCountRgbLeds1; index++) {
      if(mRoof_Blinker_Red == 0 &&
        mRoof_Blinker_Green == 0 &&
        mRoof_Blinker_Blue == 0) {
        mRgbSetup_Roof[index].Red = mRgbSetup_Roof[index].AltRed;
        mRgbSetup_Roof[index].Green = mRgbSetup_Roof[index].AltGreen;
        mRgbSetup_Roof[index].Blue = mRgbSetup_Roof[index].AltBlue;
      }
      else {
        mRgbSetup_Roof[index].Red = mRoof_Blinker_Red;
        mRgbSetup_Roof[index].Green = mRoof_Blinker_Green;
        mRgbSetup_Roof[index].Blue = mRoof_Blinker_Blue;
      }
    }

    return;
  }
}

void Roof_GlitterEffect(){

  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    
    if(mRgbSetup_Roof[index].Up) {

      if(mRgbSetup_Roof[index].Red >= mRgbSetup_Roof[index].TargetRed &&
         mRgbSetup_Roof[index].Green >= mRgbSetup_Roof[index].TargetGreen &&
         mRgbSetup_Roof[index].Blue >= mRgbSetup_Roof[index].TargetBlue) {
        mRgbSetup_Roof[index].Up = false;
        mRgbSetup_Roof[index].TargetRed = 0;
        mRgbSetup_Roof[index].TargetGreen = 0;
        mRgbSetup_Roof[index].TargetBlue = 0;
      }
      else {

        Roof_FadeToTarget(index);
        continue;
      }
    }
    
    if(mRgbSetup_Roof[index].On && 
      mRgbSetup_Roof[index].Red <= 10 &&
      mRgbSetup_Roof[index].Green <= 10 &&
      mRgbSetup_Roof[index].Blue <= 10) {
        
      mRgbSetup_Roof[index].On = false;
      mRgbSetup_Roof[index].TargetRed = 0;
      mRgbSetup_Roof[index].TargetGreen = 0;
      mRgbSetup_Roof[index].TargetBlue = 0;
    }
    
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Red, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Green, 4);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].Blue, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltRed, 5);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltGreen, 4);
    WS2812_Helper_Reduce(mRgbSetup_Roof[index].AltBlue, 5);
    

    if(mRgbSetup_Roof[index].On) {
      if(mSerialMonitor) {
        Serial.println("mRgbSetup_Roof[index].On");
      }
      continue;
    }

    int r = random(0, 10);
    if(mSerialMonitor) {
      Serial.print("Random value (0,10): ");
      Serial.println(r, DEC);
    }
    
    if(r > 8) {
      
      mRgbSetup_Roof[index].Up = true;
      mRgbSetup_Roof[index].On = true;

      int rColor = random(0, 5);
      switch(rColor) {
        case(0): {
          mRgbSetup_Roof[index].TargetRed = 20;
          mRgbSetup_Roof[index].TargetGreen = 120;
          mRgbSetup_Roof[index].TargetBlue = 10;
          break;
        }
        case(1): {
          mRgbSetup_Roof[index].TargetRed = 0;
          mRgbSetup_Roof[index].TargetGreen = 160;
          mRgbSetup_Roof[index].TargetBlue = 10;
          break;
        }
        case(2): {
          mRgbSetup_Roof[index].TargetRed = 50;
          mRgbSetup_Roof[index].TargetGreen = 160;
          mRgbSetup_Roof[index].TargetBlue = 30;
          break;
        }
        case(3): {
          mRgbSetup_Roof[index].TargetRed = 10;
          mRgbSetup_Roof[index].TargetGreen = 70;
          mRgbSetup_Roof[index].TargetBlue = 10;
          break;
        }
        default:{
          mRgbSetup_Roof[index].TargetRed = 30;
          mRgbSetup_Roof[index].TargetGreen = 60;
          mRgbSetup_Roof[index].TargetBlue = 30;
          break;
        }
      }
    }

    Roof_FadeToTarget(index);
  }
}
