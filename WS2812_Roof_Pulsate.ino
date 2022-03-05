//// ========================================================================================
//
//class Base_Animator {
//  public:
//   
//    // virtual functions
//    virtual void Animate();
//    virtual RgbSetup GetRgbSetup(int index);
//};
//class Roof_Animator_Pulsate : public Base_Animator {
//  public:
//
//    RgbSetup RgbSetup_Roof[20];
//    
//    // virtual functions
//    void Animate() {
//      if(mSerialMonitor) {
//        Serial.println("Roof Animator Pulsate");
//      }
//
//      Roof_WalkingLight();
//    }
//    RgbSetup GetRgbSetup(int index){
//      return RgbSetup_Roof[index];
//    }
//    
//    int8_t mRoofAnimationIndex = 0;
//    int8_t mRoofAnimationIndex2 = 0;
//    bool mRoofAnimationLeftToRight = true;
//    bool mRoofAnimationChangedLeftRightSide = true;
//    
//    void Roof_WalkingLight() {
//    
//      for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
//        WS2812_Helper_Reduce(RgbSetup_Roof[index].Red, 5);
//        WS2812_Helper_Reduce(RgbSetup_Roof[index].Green, 4);
//        WS2812_Helper_Reduce(RgbSetup_Roof[index].Blue, 5);
//      }
//    
//      if(mRoofAnimationLeftToRight && mRoofAnimationIndex < mCountRgbLeds1) {
//        mRoofAnimationIndex++;
//        if(mRoofAnimationIndex >= mCountRgbLeds1) {
//          mRoofAnimationLeftToRight = false;
//        }
//      }
//    
//      if(!mRoofAnimationLeftToRight && mRoofAnimationIndex > 0) {
//        mRoofAnimationIndex--;
//        if(mRoofAnimationIndex <= 0) {
//          mRoofAnimationLeftToRight = true;
//        }
//      }
//    
//      if(mRoofAnimationIndex2 < mCountRgbLeds1) {
//        mRoofAnimationIndex2++;
//      }
//      else {
//        mRoofAnimationIndex2 = 0;
//        mRoofAnimationChangedLeftRightSide = !mRoofAnimationChangedLeftRightSide;
//      }
//      if(mSerialMonitor) {
//        Serial.print("- mRoofAnimationIndex2 "); Serial.println(mRoofAnimationIndex2, DEC);
//      }
//    
//      int8_t moveLeft = GetMoveLeft(mRoofAnimationIndex2, mCountRgbLeds1);
//      int8_t moveRight = GetMoveRight(mRoofAnimationIndex2, mCountRgbLeds1);
//    
//      if(mSerialMonitor) {
//        Serial.print("- moveLeft "); Serial.println(moveLeft, DEC);
//        Serial.print("- moveRight "); Serial.println(moveRight, DEC);
//      }
//    
//      uint8_t red = 10;
//      uint8_t green = 0;
//      uint8_t blue = 0;
//      Helper_SetRgbColor(8, red, green, blue);
//        
//      if(mRoofAnimationChangedLeftRightSide){
//        if(moveLeft > -1) {
//          RgbSetup_Roof[moveLeft].Red = red;
//          RgbSetup_Roof[moveLeft].Green = green;
//          RgbSetup_Roof[moveLeft].Blue = blue;
//        }
//        
//        if(moveRight > -1) {
//          RgbSetup_Roof[moveRight].Red = red;
//          RgbSetup_Roof[moveRight].Green = green;
//          RgbSetup_Roof[moveRight].Blue = blue;
//        }
//      }
//      else {
//        if(moveLeft > -1) {
//          RgbSetup_Roof[moveRight].Red = red;
//          RgbSetup_Roof[moveRight].Green = green;
//          RgbSetup_Roof[moveRight].Blue = blue;
//        }
//        
//        if(moveRight > -1) {
//          RgbSetup_Roof[moveLeft].Red = red;
//          RgbSetup_Roof[moveLeft].Green = green;
//          RgbSetup_Roof[moveLeft].Blue = blue;
//        }
//      }
//      
//    }
//};
//
//class Roof_Animator_Scanner : public Base_Animator {
//  public:
//    RgbSetup RgbSetup_Roof[20];
//    // virtual functions
//    void Animate() {
//      //Serial.println("Roof Animator Scanner erbt von Base Animator");
//    }
//    RgbSetup GetRgbSetup(int index){
//      return RgbSetup_Roof[index];
//    }
//};
//
//Base_Animator* mRoof_Animatoren[] = {
//    new Roof_Animator_Pulsate(),
//    new Roof_Animator_Scanner()
//  };
//
//int mRoof_LengthOfArray = sizeof(mRoof_Animatoren) / sizeof(mRoof_Animatoren[0]);
//
//void Roof_Animators() {
//
//  for(int i = 0; i < mRoof_LengthOfArray; i++) {
//    mRoof_Animatoren[i]->Animate();
//  }
//}
//
//void Roof_Animations_Update(){
//  
//  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
//
//    RgbSetup rgbSetup = mRoof_Animatoren[0]->GetRgbSetup(index);
//    uint8_t red = rgbSetup.Red;
//    uint8_t green = rgbSetup.Green;
//    uint8_t blue = rgbSetup.Blue;
//
////    red = map(mRgbBrightnessMaxValue2, 0, 255, 0, red);
////    green = map(mRgbBrightnessMaxValue2, 0, 255, 0, green);
////    blue = map(mRgbBrightnessMaxValue2, 0, 255, 0, blue);
//    
//    mPixels_Roof.setPixelColor(index, mPixels_Roof.Color(red, green, blue));
//  }
//  
//  mPixels_Roof.show();
//}
