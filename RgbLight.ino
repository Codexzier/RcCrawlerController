void AnimateInitialRgbLight() {
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

void UpdateRgbLights() {
  for(int index = 0; index < mCountRgbLeds1; index++) {
    int red = mMoveLightArray_1_Red[index];
    int green = mMoveLightArray_1_Green[index];
    int blue = mMoveLightArray_1_Blue[index];
    mPixels1.setPixelColor(index, mPixels1.Color(red, green, blue));
  }
  
  mPixels1.show();
  
}

void SetAllBrightWhite(){
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mMoveLightArray_1_Red[index] = 255;
    mMoveLightArray_1_Green[index] = 255;
    mMoveLightArray_1_Blue[index] = 255;
  }
}

void SetAllOff(){
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mMoveLightArray_1_Red[index] = 255;
    mMoveLightArray_1_Green[index] = 255;
    mMoveLightArray_1_Blue[index] = 255;
  }
}

void SetMovingDirection(bool &forward, int &index, int countRgbLeds, bool setNextColor){
  if(forward) {
    if(index < countRgbLeds - 1) { index++; }
    else { forward = false; }
  }
  else { 
    if(index > 0) { index--; }
    else { 
      forward = true; 
      if(setNextColor){ SetNextColor(); }
    }
  }
}

void SetNextColor() {

  bool result = mMoveLightArray_1_ColorSwitch < 10;
  Serial.print("Result: ");
  Serial.print(result, BIN);
  Serial.print(" = ");
  Serial.print(mMoveLightArray_1_ColorSwitch, DEC);
  Serial.println(" < 10");
  
  
  if(mMoveLightArray_1_ColorSwitch < 10) {
    mMoveLightArray_1_ColorSwitch++;
    Serial.println("plus");
  }
  else {
    Serial.println("set to 0");
    mMoveLightArray_1_ColorSwitch = 0;
  }

  Serial.print("Color nr: ");
  Serial.println(mMoveLightArray_1_ColorSwitch, DEC);
}

void SetMoving() {
  
  // RED
  SetMovingDirection(mMoveLightArray_1_ForwardRed, mMoveLightArray_1_IndexRed, mCountRgbLeds1, true);
  SetMovingDirection(mMoveLightArray_1_ForwardGreen, mMoveLightArray_1_IndexGreen, mCountRgbLeds1, false);
  SetMovingDirection(mMoveLightArray_1_ForwardBlue, mMoveLightArray_1_IndexBlue, mCountRgbLeds1, false);

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

  SetRgbColor(mMoveLightArray_1_ColorSwitch,
    mMoveLightArray_1_Red[mMoveLightArray_1_IndexRed],
    mMoveLightArray_1_Green[mMoveLightArray_1_IndexGreen],
    mMoveLightArray_1_Blue[mMoveLightArray_1_IndexBlue]);
}

//
// 0 = Orange
// 1 = gelb
// 2 = türkis

// 8 = lila
// 9 = rosa
void SetRgbColor(int colorIndex, int &red, int &green, int &blue) {
 
  switch(colorIndex) {
    case(0): {
      // Orange
      red = 255;
      green = 90;
      blue = 0;
      break;
    }
    case(1): {
      // gelb
      red = 255;
      green = 234;
      blue = 0;
      break;
    }
    case(2): {
      // türkis
      red = 0;
      green = 255;
      blue = 18;
      break;
    }
    case(3): {
      // hellblau
      red = 0;
      green = 204;
      blue = 255;
      break;
    }
    case(4): {
      // defuse neutral
      red = 154;
      green = 128;
      blue = 97;
      break;
    }
    case(5): {
      // dark tyrkis
      red = 28;
      green = 72;
      blue = 82;
      break;
    }
    case(6): {
      // dark wine red
      red = 107;
      green = 68;
      blue = 70;
      break;
    }
    case(7): {
      // 
      red = 90;
      green = 79;
      blue = 50;
      break;
    }
    case(8): {
      // lila
      red = 156;
      green = 0;
      blue = 255;
      break;
    }
    case(9): {
      // rosa
      red = 240;
      green = 0;
      blue = 255;
      break;
    }
     case(10): {
      // hell orange
      red = 255;
      green = 222;
      blue = 122;
      break;
    }
    default: {

      // random
      //red = 9; //random(0, 150);
      //green = 0; //random(0, 150);
      //blue = 0; //random(0, 150);
      break;
    }
  }
}
