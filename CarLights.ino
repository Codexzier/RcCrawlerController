
int8_t mShowSingleLedAndIndex_Index = 0;        // That used for tests the LEDs positions.
int16_t mCarLights_FadeSteps = 20;               // steps to fade-in and fade-out.


// ========================================================================================
// Test. used it for configuratiion the mapping array.
void CarLight_ShowSingleLedAndIndex(){

  if(mShowSingleLedAndIndex_Index < 12) {
    mShowSingleLedAndIndex_Index++;
  }
  else {
    mShowSingleLedAndIndex_Index = 6;
  }

  //Serial.print("LED INDEX: "); Serial.println(mShowSingleLedAndIndex_Index, DEC);

  mCarLights.setPWM(mShowSingleLedAndIndex_Index, 0, 0);
  delay(4000);
  mCarLights.setPWM(mShowSingleLedAndIndex_Index, 4095, 0);
}

// ========================================================================================
// used if the remote connection lost
void CarLight_Offline() {
  
  if(mSerialMonitor) {
      Serial.println("CarLight offline!");
  }
}

// ========================================================================================
// set all LEDs on
void CarLight_On() {
  if(mSerialMonitor) {
      Serial.println("CarLight on!");
  }

  for (uint8_t pwmnum=0; pwmnum < mLEDs_Size; pwmnum++) {  
    CarLedTypeSetValue(mLEDs[pwmnum].portNumber, true);
  }
}

// ========================================================================================
// set all LEDs off
void CarLight_Off(){
  if(mSerialMonitor) {
      Serial.println("CarLight off!");
  }
  
  for (uint8_t pwmnum=0; pwmnum < mLEDs_Size; pwmnum++) {
    CarLedTypeSetValue(mLEDs[pwmnum].portNumber, false);
  }
}

// ========================================================================================
// update aktual all pwm output
void CarLight_Update() {

  for(int index = 0; index < mLEDs_Size; index++) {
    if(mSerialMonitor && index == 1) {
//      Serial.print("CarLight INDEX: "); 
//      Serial.print(index, DEC);
//      Serial.print(", Mapped Port: ");
//      Serial.print(mLEDs[index].portNumber, DEC);
//      Serial.print(", LED Value (off): ");
//      Serial.print(mLEDs[index].off, DEC);
//      Serial.print(", Step value: ");
//      Serial.println(mLEDs[index].noChanged, DEC);
    }

    if(mLEDs[index].lastOnValue != mLEDs[index].off) {
      mCarLights.setPWM(
        mLEDs[index].portNumber, 
        mLEDs[index].on, 
        mLEDs[index].off);
    }
    
  }
}

// ========================================================================================
// Stand lights and drive lights
// ========================================================================================

void CarLight_SetOnStandLightOrDriveLight(int inputValue, int minValue, int maxValue, int middleValue) {

  // set off lights
  if(inputValue < minValue + mThresholdValue) {
    CarLight_SetOffStandAndNormalLights();
    return;
  }

  // stand lights
  if(inputValue > middleValue - mThresholdValue &&
     inputValue < middleValue + mThresholdValue) {
    CarLight_SetOnStandLights();
    return;
  }

  // drive lights
  if(inputValue > maxValue - mThresholdValue) {
    CarLight_SetOnNormalLights();
  }
}

void CarLight_SetOffStandAndNormalLights(){
  //front
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
  // front stand
  CarLedTypeSetValue(2, false);
  CarLedTypeSetValue(5, false);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
}

void CarLight_SetOnNormalLights(){
  // front 
  CarLedTypeSetValue(1, true);
  CarLedTypeSetValue(4, true);
  // front stand
  CarLedTypeSetValue(2, false);
  CarLedTypeSetValue(5, false);
  // rear
  CarLedTypeSetValue(7, true);
  CarLedTypeSetValue(9, true);
}

void CarLight_SetOffNormalLights(){
  // front 
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
    // front stand
  CarLedTypeSetValue(2, false);
  CarLedTypeSetValue(5, false);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
}

void CarLight_SetOnStandLights(){
  // front 
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
    // front stand
  CarLedTypeSetValue(2, true, 3500);
  CarLedTypeSetValue(5, true, 3500);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
}

// ========================================================================================
// Blinkers
// ========================================================================================

void CarLight_SetOffBlinkers(){
  CarLedTypeSetValue(0, false);
  CarLedTypeSetValue(8, false);
  CarLedTypeSetValue(3, false);
  CarLedTypeSetValue(6, false);
}

void CarLight_SetOnBlinkers(){
  CarLedTypeSetValue(0, true);
  CarLedTypeSetValue(8, true);
  CarLedTypeSetValue(3, true);
  CarLedTypeSetValue(6, true);
}

void CarLight_SetOnBlinkersLeft(){
  // left
  CarLedTypeSetValue(0, true);
  CarLedTypeSetValue(8, true);
  // right
  CarLedTypeSetValue(3, false);
  CarLedTypeSetValue(6, false);
}

void CarLight_SetOnBlinkersRight(){
  // left
  CarLedTypeSetValue(0, false);
  CarLedTypeSetValue(8, false);
  // right
  CarLedTypeSetValue(3, true);
  CarLedTypeSetValue(6, true);
}

void CarLedTypeSetValue(int portNumber, bool goOn) {
  CarLedTypeSetValue(portNumber, goOn, 0);
}
// ========================================================================================
// Set the value to the target LED Car light.

// maxValueOn = is an inverted value 0 is full on and 4095 is off
void CarLedTypeSetValue(int portNumber, bool goOn, int16_t maxValueOn) {

  int index = CarLedsGetIndex(portNumber);
  int16_t stepBrightness = 4096 / mCarLights_FadeSteps;
  int16_t temp = 0;

  //mLEDs[index].noChanged = stepBrightness;
  mLEDs[index].lastOnValue = mLEDs[index].off;
  
  if(goOn) {
    // setup LED on
    if(mLEDs[index].off > 0) {
      temp = (int16_t)mLEDs[index].off - stepBrightness;
    }

    if(temp > stepBrightness + maxValueOn) {
      //mLEDs[index].on = temp;
      mLEDs[index].off = temp;
    }
    else {
      mLEDs[index].off = maxValueOn;
    }

    // abgeändert in Else 
//    if(temp < 0) {
//      mLEDs[index].on = 0;
//    }
  }
  else {

    if(mLEDs[index].off < 4096) {
      temp = mLEDs[index].off + stepBrightness;
    }

    if(temp < 4096) {
      mLEDs[index].off = temp;
    }
    else {
      mLEDs[index].off = 4095;
    }

//    if(mLEDs[index].off >= 4096) {
//      mLEDs[index].off = 4095;
//    }
  }

  mLEDs[index].noChanged = temp;
  mLEDs[index].on = 0;
}

// ========================================================================================
//
int CarLedsGetIndex(int portNumber) {

  for(int index = 0; index < mLEDs_Size; index++) {
    if(mLEDs[index].portNumber == portNumber) {
      return index;
    }
  }
}
