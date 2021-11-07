bool mChangeBlinkerOnOff = false;
long mBlinkerLastCurrentTime;

// ========================================================================================
// set the flashing 
void CarBlinker_SetTurnSignal(int inputValue, int minValue, int maxValue, int middleValue) {
  // straigt ahead
  if(inputValue > middleValue - mDeathbandPlusMinus && 
     inputValue < middleValue + mDeathbandPlusMinus) {
  
    CarLight_SetOffBlinkers();
    return;
  }

  if(mCurrentMillis - mBlinkerLastCurrentTime > 500) {
    mChangeBlinkerOnOff = !mChangeBlinkerOnOff;
    mBlinkerLastCurrentTime = mCurrentMillis;

    if(mBlinkerLastCurrentTime < 0) {
      mBlinkerLastCurrentTime = 0;
    }

    if(mSerialMonitor) {
      Serial.print("Change blinker: ");
      Serial.println(mChangeBlinkerOnOff, BIN);
    }
  }

  
  
  if(mChangeBlinkerOnOff) {
    CarLight_SetOffBlinkers();

    if(mSerialMonitor) {
      Serial.println("Blinker OFF");
    }
    return;
  }
  
  // set left
  if(inputValue >= middleValue + mDeathbandPlusMinus) {
    CarLight_SetOnBlinkersLeft();

    if(mSerialMonitor) {
      Serial.println("Blinker LEFT");
    }
    return;
  }

  // set right
  if(inputValue <= middleValue - mDeathbandPlusMinus) {
    CarLight_SetOnBlinkersRight();

    if(mSerialMonitor) {
      Serial.println("Blinker RIGHT");
    }
    return;
  }

  
}
