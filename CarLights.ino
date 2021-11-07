
int8_t mShowSingleLedAndIndex_Index = 0;        // That used for tests the LEDs positions.
int16_t mCarLights_FadeSteps = 50;               // steps to fade-in and fade-out.


// ========================================================================================
// Test. used it for configuratiion the mapping array.
void CarLight_ShowSingleLedAndIndex(){

  if(mShowSingleLedAndIndex_Index < 12) {
    mShowSingleLedAndIndex_Index++;
  }
  else {
    mShowSingleLedAndIndex_Index = 6;
  }

  Serial.print("LED INDEX: "); Serial.println(mShowSingleLedAndIndex_Index, DEC);

  mCarLights.setPWM(mShowSingleLedAndIndex_Index, 0, 0);
  delay(4000);
  mCarLights.setPWM(mShowSingleLedAndIndex_Index, 4096, 0);
}

// ========================================================================================
// Test dimming
void CarLight_ShowDimming() {

    int8_t steps = 100;
uint16_t lastOn = 0;
Serial.println("FADEIN");

  for(uint16_t brightness = 1; brightness < steps; brightness++) {
    uint16_t on = map(brightness, 0, steps, 0, 4096);
    if(lastOn > on) {
      on = lastOn;
    }
    lastOn = on;
    Serial.print("FADEIN LED Brightness: "); Serial.println(on, DEC);
    for (uint8_t pwmnum=0; pwmnum < mLEDs_Size; pwmnum++) {
      
      mCarLights.setPWM(
        mLEDs[pwmnum].portNumber, 
        on, 
        mLEDs[pwmnum].off);
    }
    delay(30);
  }
  
Serial.print("to on Last LED Brightness: "); Serial.println(lastOn, DEC);
  delay(1000);

Serial.println("FADEOUT");
  for(uint16_t brightness = 0; brightness < steps; brightness++) {
    uint16_t on = map(brightness, 0, steps, 4096, 0);
    if(lastOn < on) {
      on = lastOn;
    }
lastOn = on;
    Serial.print("FADEOUT LED Brightness: "); Serial.println(on, DEC);
    for (uint8_t pwmnum=0; pwmnum < mLEDs_Size; pwmnum++) {
      
      mCarLights.setPWM(
        mLEDs[pwmnum].portNumber, 
        on, 
        mLEDs[pwmnum].off);

      //delay(10);
    }
    delay(30);
  }
  
  Serial.print("to off Last LED Brightness: "); Serial.println(lastOn, DEC);
  delay(1000);
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
    if(mSerialMonitor) {
      Serial.print("CarLight INDEX: "); 
      Serial.print(index, DEC);
      Serial.print(", Mapped Port: ");
      Serial.print(mLEDs[index].portNumber, DEC);
      Serial.print(", LED Value: ");
      Serial.print(mLEDs[index].on, DEC);
      Serial.print(", Step value: ");
      Serial.println(mLEDs[index].noChanged, DEC);
    }

    if(mLEDs[index].lastOnValue != mLEDs[index].on) {
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
  CarLedTypeSetValue(2, true);
  CarLedTypeSetValue(5, true);
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

// ========================================================================================
// Set the value to the target LED Car light.
void CarLedTypeSetValue(int portNumber, bool goOn) {

  int index = CarLedsGetIndex(portNumber);
  int16_t stepBrightness = 4096 / mCarLights_FadeSteps;
  int temp = 0;

  //mLEDs[index].noChanged = stepBrightness;
  mLEDs[index].lastOnValue = mLEDs[index].on;
  
  if(!goOn) {
    // setup LED on
    if(mLEDs[index].on > 0) {
      temp = (int16_t)mLEDs[index].on - stepBrightness;
    }

    if(temp > 0) {
      mLEDs[index].on = temp;
    }

    if(temp < 0) {
      mLEDs[index].on = 0;
    }
  }
  else {

    if(mLEDs[index].on < 4096) {
      temp = mLEDs[index].on + stepBrightness;
    }

    if(temp < 4096) {
      mLEDs[index].on = temp;  
    }
  }

  mLEDs[index].noChanged = temp;
  mLEDs[index].off = 0;
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
