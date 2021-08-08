int mRoofLastMillis = 0;

// ========================================================================================
// Set first RGB LED on Front Roof.
void RoofStateOn() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(0, 0, 0));
  }

  // green
  mPixels1.setPixelColor(0, mPixels1.Color(0, 100, 0));
  mPixels1.show();
}

void RoofRxOffline() {

  // green
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(0, 100, 0));
  }
  mPixels1.show();
}

void RoofAnimateInitialRgbLight() {
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(255, 0, 0));
    mPixels1.setPixelColor(mCountRgbLeds1 - index - 1, mPixels1.Color(255, 0, 0));
    mPixels1.show();
    delay(20);
    mPixels1.setPixelColor(index, mPixels1.Color(0, 0, 0));
    mPixels1.setPixelColor(mCountRgbLeds1 - index - 1, mPixels1.Color(0, 0, 0));
    mPixels1.show();
  }
}

void RoofAnimationFadeOn() {
  for(int index = 0; index < mCountRgbLeds1; index++) {

    if(mMoveLightArray_1_Red[index] < 160) {
      mMoveLightArray_1_Red[index] += 10;
    }

    if(mMoveLightArray_1_Green[index] < 160) {
      mMoveLightArray_1_Green[index] += 10;
    }

    if(mMoveLightArray_1_Blue[index] < 160) {
      mMoveLightArray_1_Blue[index] += 10;
    }    
  }
}

void RoofAnimationFadeOut() {
  for(int index = 0; index < mCountRgbLeds1; index++) {

    if(mMoveLightArray_1_Red[index] > 20) {
      mMoveLightArray_1_Red[index] -= 10;
    }

    if(mMoveLightArray_1_Green[index] > 20) {
      mMoveLightArray_1_Green[index] -= 10;
    }

    if(mMoveLightArray_1_Blue[index] > 20) {
      mMoveLightArray_1_Blue[index] -= 10;
    }    
  }
}

void RoofUpdateRgbLights() {

  //Serial.println("\t");
  for(int index = 0; index < mCountRgbLeds1; index++) {
    int red = mMoveLightArray_1_Red[index];
    int green = mMoveLightArray_1_Green[index];
    int blue = mMoveLightArray_1_Blue[index];

//    if(mSerialMonitor) {
//      Serial.print("B: "); Serial.print(mRgbBrightnessMaxValue1, DEC); Serial.print("\t");
//      Serial.print("R: "); Serial.print(red, DEC); Serial.print("\t");
//      Serial.print("G: "); Serial.print(green, DEC); Serial.print("\t");
//      Serial.print("B: "); Serial.print(blue, DEC); Serial.println("\t");
//    }
    
    mPixels1.setPixelColor(index, mPixels1.Color(red, green, blue));
  }
  
  mPixels1.show();
}

void RoofSetAllBrightWhite(){
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mMoveLightArray_1_Red[index] = 255;
    mMoveLightArray_1_Green[index] = 255;
    mMoveLightArray_1_Blue[index] = 255;
  }
}

void RoofSetAllOff(){
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mMoveLightArray_1_Red[index] = 255;
    mMoveLightArray_1_Green[index] = 255;
    mMoveLightArray_1_Blue[index] = 255;
  }
}

// ========================================================================================
// Count the index up or down to animate the chaser per pass.
// ----------------------------------------------------------------------------------------
// forward      = true count up, false count down. 
//                When the limit is reached, the state is inverted.
// index        = Value for the target RGB LED.
// countRgbLeds = Number of maximum RGB LEDs.
// setNextColor = Change color
void RoofSetMovingDirection(bool &forward, int &index, int countRgbLeds, bool setNextColor){
  if(forward) {
    if(index < countRgbLeds - 1) { index++; }
    else { forward = false; }
  }
  else { 
    if(index > 0) { index--; }
    else { 
      forward = true; 
      if(setNextColor){ RoofSetNextColor(); }
    }
  }
}

void RoofSetNextColor() {

  bool result = mMoveLightArray_1_ColorSwitch < 10;
  if(mSerialMonitor) {
//    Serial.print("Result: ");
//    Serial.print(result, BIN);
//    Serial.print(" = ");
//    Serial.print(mMoveLightArray_1_ColorSwitch, DEC);
//    Serial.print(" < 10");
//    Serial.print("\t");
  }
  
  if(mMoveLightArray_1_ColorSwitch < 10) {
    mMoveLightArray_1_ColorSwitch++;
    if(mSerialMonitor) {
      //Serial.print("plus");  Serial.print("\t");
    }
  }
  else {
    if(mSerialMonitor) {
      //Serial.print("set to 0"); Serial.print("\t");
    }
    mMoveLightArray_1_ColorSwitch = 0;
  }

  if(mSerialMonitor) {
//    Serial.print("Color nr: ");
//    Serial.print(mMoveLightArray_1_ColorSwitch, DEC);
//    Serial.print("\t");
  }
}

void RoofMovingReset(){
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mMoveLightArray_1_Red[index] = 0;
    mMoveLightArray_1_Green[index] = 0;
    mMoveLightArray_1_Blue[index] = 0;
  }

  mMoveLightArray_1_IndexRed = 0;
  mMoveLightArray_1_IndexGreen = 0;
  mMoveLightArray_1_IndexBlue = 0;
}

void RoofSetMoving() {
  if (mCurrentMillis - mRoofLastMillis < 5) {
    return;
  }
  mRoofLastMillis = mCurrentMillis;
  
  RoofSetMovingDirection(mMoveLightArray_1_ForwardRed, mMoveLightArray_1_IndexRed, mCountRgbLeds1, false);
  RoofSetMovingDirection(mMoveLightArray_1_ForwardGreen, mMoveLightArray_1_IndexGreen, mCountRgbLeds1, false);
  RoofSetMovingDirection(mMoveLightArray_1_ForwardBlue, mMoveLightArray_1_IndexBlue, mCountRgbLeds1, false);

  if(mSerialMonitor) {
    //Serial.print("Index: "); Serial.print(mMoveLightArray_1_IndexRed, DEC); Serial.print("\t");
  }

  for(int index = 0; index < mCountRgbLeds1; index++) {
    if(mMoveLightArray_1_Red[index] > 0){
      mMoveLightArray_1_Red[index] -= mTailRgbLeds1;
    }
    if(mMoveLightArray_1_Green[index] > 0){
      mMoveLightArray_1_Green[index] -= mTailRgbLeds1;
    }
    if(mMoveLightArray_1_Blue[index] > 0){
      mMoveLightArray_1_Blue[index] -= mTailRgbLeds1;
    }

    if(mMoveLightArray_1_Red[index] < 0){
      mMoveLightArray_1_Red[index] = 0;
    }
    if(mMoveLightArray_1_Green[index] < 0){
      mMoveLightArray_1_Green[index] = 0;
    }
    if(mMoveLightArray_1_Blue[index] < 0){
      mMoveLightArray_1_Blue[index] = 0;
    }
  }

  HelperSetRgbColor(mMoveLightArray_1_ColorSwitch,
    mMoveLightArray_1_Red[mMoveLightArray_1_IndexRed],
    mMoveLightArray_1_Green[mMoveLightArray_1_IndexGreen],
    mMoveLightArray_1_Blue[mMoveLightArray_1_IndexBlue]);

  int red = mMoveLightArray_1_Red[mMoveLightArray_1_IndexRed];
  int green = mMoveLightArray_1_Green[mMoveLightArray_1_IndexGreen];
  int blue = mMoveLightArray_1_Blue[mMoveLightArray_1_IndexBlue];

  if(mSerialMonitor) {
//    Serial.print("B: "); Serial.print(mRgbBrightnessMaxValue1, DEC); Serial.print("\t");
//    Serial.print("R: "); Serial.print(red, DEC); Serial.print("\t");
//    Serial.print("G: "); Serial.print(green, DEC); Serial.print("\t");
//    Serial.print("B: "); Serial.print(blue, DEC); Serial.print("\t");
  }

//  red = map(mRgbBrightnessMaxValue1, 0, 255, 0, red);
//  green = map(mRgbBrightnessMaxValue1, 0, 255, 0, green);
//  blue = map(mRgbBrightnessMaxValue1, 0, 255, 0, blue);
//
//  if(mSerialMonitor) {
//    Serial.print("B: "); Serial.print(mRgbBrightnessMaxValue1, DEC); Serial.print("\t");
//    Serial.print("R: "); Serial.print(red, DEC); Serial.print("\t");
//    Serial.print("G: "); Serial.print(green, DEC); Serial.print("\t");
//    Serial.print("B: "); Serial.print(blue, DEC); Serial.print("\t");
//  }
  
  mMoveLightArray_1_Red[mMoveLightArray_1_IndexRed] = red;
  mMoveLightArray_1_Green[mMoveLightArray_1_IndexGreen] = green;
  mMoveLightArray_1_Blue[mMoveLightArray_1_IndexBlue] = blue;
}
