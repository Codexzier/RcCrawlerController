void StatusStateOn() {
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels0.setPixelColor(index, mPixels0.Color(0, 0, 0));
  }

  mPixels0.setPixelColor(0, mPixels0.Color(255, 90, 0));
  mPixels0.show();
}

void StatusStateOn2() {
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels0.setPixelColor(index, mPixels0.Color(0, 0, 0));
  }

  mPixels0.setPixelColor(0, mPixels0.Color(0, 90, 255));
  mPixels0.show();
}
