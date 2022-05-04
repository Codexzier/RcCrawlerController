// ========================================================================================
// Description:       Blinker Steuerung und animation
// ========================================================================================

// ========================================================================================
// allgemeine blinker Variablen
bool mChangeBlinkerOnOff = false;
unsigned long mBlinkerLastCurrentTime = 0;

// ========================================================================================
// set the flashing for turn left or right
// ----------------------------------------------------------------------------------------
// inputValue       = signal input value from rx
// minValue         = signal minimum value
// maxValue         = signal maximum value
// middleValue      = signal middle value
// changeDirection  = invert set target result
void CarBlinker_SetTurnSignal(int inputValue, int minValue, int maxValue, int middleValue, bool changeDirection) {
  
  // straigt ahead
  if(inputValue > middleValue - mDeathbandPlusMinus && 
     inputValue < middleValue + mDeathbandPlusMinus) {
  
    CarLight_SetOffBlinkers();
    return;
  }

  if(mCurrentMillis - mBlinkerLastCurrentTime > 500 || mBlinkerLastCurrentTime < 0) {
    mChangeBlinkerOnOff = !mChangeBlinkerOnOff;
    mBlinkerLastCurrentTime = mCurrentMillis;

    if(mChangeBlinkerOnOff) {
      // TODO: Sollte ein anderer Off Wert gesetzt werden.
    }

    if(mSerialMonitor) {
      //Serial.print("Change blinker: ");
      //Serial.println(mChangeBlinkerOnOff, BIN);
    }
  }

  if(mChangeBlinkerOnOff) {
    CarLight_SetOffBlinkers();

    if(mSerialMonitor) {
      //Serial.println("Blinker OFF");
    }
    return;
  }
  
  // set left
  if(inputValue >= middleValue + mDeathbandPlusMinus) {

    if(changeDirection) {
      CarLight_SetOnBlinkersRight();
    }
    else {
      CarLight_SetOnBlinkersLeft();
    }

    if(mSerialMonitor) {
      //Serial.println("Blinker LEFT");
    }
    return;
  }

  // set right
  if(inputValue <= middleValue - mDeathbandPlusMinus) {

     if(changeDirection) {
      CarLight_SetOnBlinkersLeft();
    }
    else {
      CarLight_SetOnBlinkersRight();
    }

    if(mSerialMonitor) {
      //Serial.println("Blinker RIGHT");
    }
    return;
  }
}
