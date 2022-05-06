
int8_t mShowSingleLedAndIndex_Index = 0;        // That used for tests the LEDs positions.
int16_t mCarLights_FadeSteps = 5;               // steps to fade-in and fade-out.


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
  SerialMonitorHelper_Print("CarLight offline!");

  for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
    mCarLights.setPWM(pwmnum, 0, 4095);
  }
}

// ========================================================================================
// set all LEDs on
void CarLight_On() {
  SerialMonitorHelper_Print("CarLight on!");

  for (uint8_t pwmnum=0; pwmnum < mLEDs_Size; pwmnum++) {  
    CarLedTypeSetValue(mLEDs[pwmnum].portNumber, true);
  }
}

// ========================================================================================
// set all LEDs off
void CarLight_Off(){
  SerialMonitorHelper_Print("CarLight off!");
  
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
  CarLedTypeSetValue(2, false, 3500, 20);
  CarLedTypeSetValue(5, false, 3500, 20);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
  // number plate
  CarLedTypeSetValue(12, false, 3500, 30);
}

void CarLight_SetOnNormalLights(){
  // front 
  CarLedTypeSetValue(1, true);
  CarLedTypeSetValue(4, true);
  // front stand
  CarLedTypeSetValue(2, false, 3500, 20);
  CarLedTypeSetValue(5, false, 3500, 20);
  // rear
  CarLedTypeSetValue(7, true);
  CarLedTypeSetValue(9, true);
  // number plate
  CarLedTypeSetValue(12, true, 3500, 30);
}

void CarLight_SetOffNormalLights(){
  // front 
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
    // front stand
  CarLedTypeSetValue(2, false, 3500, 20);
  CarLedTypeSetValue(5, false, 3500, 20);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
  // number plate
  CarLedTypeSetValue(12, false, 3500, 30);
}

void CarLight_SetOnStandLights(){
  // front 
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
    // front stand
  CarLedTypeSetValue(2, true, 3500, 20);
  CarLedTypeSetValue(5, true, 3500, 20);
  // rear
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
  // number plate
  CarLedTypeSetValue(12, true, 3500, 30);
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

void CarLight_NumberPlate_On(){
  CarLedTypeSetValue(12, true, 3500, 4);
}

void CarLight_NumberPlate_Off(){
  CarLedTypeSetValue(12, false);
}

// ========================================================================================
// Set the value to the target LED Car light.
// ----------------------------------------------------------------------------------------
// portNumber = LED Number
// goOn       = true for on, false for going to off.
void CarLedTypeSetValue(int portNumber, bool goOn) {
  CarLedTypeSetValue(portNumber, goOn, 0, 1);
}

// ========================================================================================
// Set the value to the target LED Car light.
// ----------------------------------------------------------------------------------------
// portNumber = LED Number
// goOn       = true for on, false for going to off.
// maxValueOn = is an inverted value 0 is full on and 4095 is off
//              - 0 is on
//              - 4095 is off
// multiplyStep = default => 1
void CarLedTypeSetValue(int portNumber, bool goOn, int16_t maxValueOn, int16_t multiplyStep) {

  int index = CarLedsGetIndex(portNumber);
  int16_t stepBrightness = 4096 / (mCarLights_FadeSteps * multiplyStep);
  int16_t temp = 0;

  //mLEDs[index].noChanged = stepBrightness;
  mLEDs[index].lastOnValue = mLEDs[index].off;
  
  if(goOn) {
    // setup LED on

    // the value is higher than '0'
    if(mLEDs[index].off > 0) {
      // reduce low level value 
      temp = (int16_t)mLEDs[index].off - stepBrightness;
    }

    // is prepare temp value greater than 
    if(temp > stepBrightness + maxValueOn) {
      // set new value for low level
      mLEDs[index].off = temp;
    }
    else {
      // is same or lower, set max value
      mLEDs[index].off = maxValueOn;
      //temp = maxValueOn;
    }
  }
  else {

    // setup LED off

    // level value is lower than max
    if(mLEDs[index].off < 4096) {
      // prepare new low level value
      temp = mLEDs[index].off + stepBrightness;
    }

    // prepare low level value is lower than max
    if(temp < 4096) {
      // set new value for low level
      mLEDs[index].off = temp;
    }
    else {
      // set max value for off
      mLEDs[index].off = 4095;
      //temp = 4095;
    }
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
