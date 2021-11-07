void Status_On() {
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels0.setPixelColor(index, mPixels0.Color(255, 90, 0));
  }
  mPixels0.show();
}

void Status_Off() {
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels0.setPixelColor(index, mPixels0.Color(0, 0, 0));
  }
  mPixels0.show();
}

void Status_Disconnect() {
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels0.setPixelColor(index, mPixels0.Color(255, 0, 0));
  }
  mPixels0.show();
}
