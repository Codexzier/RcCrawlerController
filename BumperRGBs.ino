int mBumperLastMillis = 0;

// ========================================================================================
// Set first RGB LED on Front Bumper.
void BumperStateOn() {
  
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels2.setPixelColor(index, mPixels2.Color(0, 0, 0));
  }

  // color yellow
  mPixels2.setPixelColor(0, mPixels2.Color(255, 234, 0));
  mPixels2.show();
}

void BumperUpdateRgbLights() {

  if (mCurrentMillis - mBumperLastMillis < 10) {
    return;
  }
  mBumperLastMillis = mCurrentMillis;
  
  for(int index = 0; index < mCountRgbLeds1; index++) {
    int red = mMoveLightArray_2_Red[index];
    int green = mMoveLightArray_2_Green[index];
    int blue = mMoveLightArray_2_Blue[index];
    mPixels2.setPixelColor(index, mPixels2.Color(red, green, blue));
  }
  
  mPixels2.show();
}

void BumperAnimationFadeOn() {
  for(int index = 0; index < mCountRgbLeds2; index++) {

    if(mMoveLightArray_2_Red[index] < 160) {
      mMoveLightArray_2_Red[index] += 10;
    }

    if(mMoveLightArray_2_Green[index] < 160) {
      mMoveLightArray_2_Green[index] += 10;
    }

    if(mMoveLightArray_2_Blue[index] < 160) {
      mMoveLightArray_2_Blue[index] += 10;
    }    
  }
}

void BumperAnimationFadeOut() {
  for(int index = 0; index < mCountRgbLeds2; index++) {

    if(mMoveLightArray_2_Red[index] > 20) {
      mMoveLightArray_2_Red[index] -= 10;
    }

    if(mMoveLightArray_2_Green[index] > 20) {
      mMoveLightArray_2_Green[index] -= 10;
    }

    if(mMoveLightArray_2_Blue[index] > 20) {
      mMoveLightArray_2_Blue[index] -= 10;
    }    
  }
}
