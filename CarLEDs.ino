// ========================================================================================
// Set first RGB LED on Front Bumper.
void CarLEDsInit() {
  for(int chanel = 0; chanel < 16; chanel++) {
    mCarLights.setChannelDutyCycle(chanel, 50, 50);
  }

}

void CarLEDsNextChannel(){
  for(int chanel = 0; chanel < 12; chanel++) {
    mCarLights.setChannelDutyCycle(chanel, 100, 0);
  }
  
  if(mCarLightsChannel < 15) {
    mCarLightsChannel++;
  }
  else {
    mCarLightsChannel = 0;
  }

  mCarLights.setChannelDutyCycle(mCarLightsChannel, 0, 100);
  
  if(mSerialMonitor) {
    Serial.print("Set Channel: ");
    Serial.println(mCarLightsChannel, DEC);
  }
}

void CarLEDsSetOffStandAndNormalLights(){
  //front
//  mCarLights.setChannelDutyCycle(1, 100, 0);
//  mCarLights.setChannelDutyCycle(4, 100, 0);
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
  // rear
//  mCarLights.setChannelDutyCycle(7, 100, 0);
//  mCarLights.setChannelDutyCycle(9, 100, 0);
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
  // front stand
//  mCarLights.setChannelDutyCycle(2, 50, 0);
//  mCarLights.setChannelDutyCycle(5, 50, 0);
  CarLedTypeSetValue(2, false);
  CarLedTypeSetValue(5, false);
}

void CarLEDsSetOnNormalLights(){
  // front 
//  mCarLights.setChannelDutyCycle(1, 0, 100);
//  mCarLights.setChannelDutyCycle(4, 0, 100);
  CarLedTypeSetValue(1, true);
  CarLedTypeSetValue(4, true);
  // rear
//  mCarLights.setChannelDutyCycle(7, 0, 100);
//  mCarLights.setChannelDutyCycle(9, 0, 100);
  CarLedTypeSetValue(7, true);
  CarLedTypeSetValue(9, true);
}

void CarLEDsSetOffNormalLights(){
  // front 
//  mCarLights.setChannelDutyCycle(1, 100, 0);
//  mCarLights.setChannelDutyCycle(4, 100, 0);
  CarLedTypeSetValue(1, false);
  CarLedTypeSetValue(4, false);
  // rear
//  mCarLights.setChannelDutyCycle(7, 100, 0);
//  mCarLights.setChannelDutyCycle(9, 100, 0);
  CarLedTypeSetValue(7, false);
  CarLedTypeSetValue(9, false);
}

void CarLEDsSetOnStandLights(){
//  mCarLights.setChannelDutyCycle(2, 0, 100);
//  mCarLights.setChannelDutyCycle(5, 0, 100);
  CarLedTypeSetValue(2, true);
  CarLedTypeSetValue(5, true);
}

void CarLEDsSetOffBlinkers(){
//  mCarLights.setChannelDutyCycle(0, 100, 0);
//  mCarLights.setChannelDutyCycle(8, 100, 0);
//  mCarLights.setChannelDutyCycle(3, 100, 0);
//  mCarLights.setChannelDutyCycle(6, 100, 0);
  CarLedTypeSetValue(0, false);
  CarLedTypeSetValue(8, false);
  CarLedTypeSetValue(3, false);
  CarLedTypeSetValue(6, false);
}

void CarLEDsSetOnBlinkers(){
//  mCarLights.setChannelDutyCycle(0, 0, 100);
//  mCarLights.setChannelDutyCycle(8, 0, 100);
//  mCarLights.setChannelDutyCycle(3, 0, 100);
//  mCarLights.setChannelDutyCycle(6, 0, 100);
  CarLedTypeSetValue(0, true);
  CarLedTypeSetValue(8, true);
  CarLedTypeSetValue(3, true);
  CarLedTypeSetValue(6, true);
}

void CarLEDsSetOnBlinkersLeft(){
  // left
//  mCarLights.setChannelDutyCycle(3, 0, 100);
//  mCarLights.setChannelDutyCycle(6, 0, 100);
  CarLedTypeSetValue(3, true);
  CarLedTypeSetValue(6, true);
  // right
//  mCarLights.setChannelDutyCycle(0, 100, 0);
//  mCarLights.setChannelDutyCycle(8, 100, 0);
  CarLedTypeSetValue(0, false);
  CarLedTypeSetValue(8, false);
}

void CarLEDsSetOnBlinkersRight(){
  // left
//  mCarLights.setChannelDutyCycle(3, 100, 0);
//  mCarLights.setChannelDutyCycle(6, 100, 0);
  CarLedTypeSetValue(3, false);
  CarLedTypeSetValue(6, false);
  // right
//  mCarLights.setChannelDutyCycle(0, 0, 100);
//  mCarLights.setChannelDutyCycle(8, 0, 100);
  CarLedTypeSetValue(0, true);
  CarLedTypeSetValue(8, true);
}

//void CarLEDsSetOnFrontBumper(){
//  mCarLights.setChannelDutyCycle(10, 0, 100);
//  mCarLights.setChannelDutyCycle(11, 0, 100);
//}
//
//void CarLEDsSetOnRearBumper(){
//  mCarLights.setChannelDutyCycle(12, 0, 100);
//  mCarLights.setChannelDutyCycle(13, 0, 100);
//}

void CarLedTypeSetValue(int index, bool goOn) {
  if(goOn) {
    if(mLEDs[index].off > 0) {
      mLEDs[index].off--;
    }

    if(mLEDs[index].on < mLEDs[index].maxOn) {
      mLEDs[index].on++;
    }

    if(mLEDs[index].on == mLEDs[index].maxOn) {
      mUp = false;
    }
  }
  else {
    if(mLEDs[index].off < 100) {
      mLEDs[index].off++;
    }

    if(mLEDs[index].on > 0) {
      mLEDs[index].on--;
    }

    if(mLEDs[index].off == 100) {
      mUp = true;
    }
  }
}

void CarLEDsUpdate() {
  for(int index = 0; index < 10; index++) {
    mCarLights.setChannelDutyCycle(index, mLEDs[index].off, mLEDs[index].on);
  }
}
