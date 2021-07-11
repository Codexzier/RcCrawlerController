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
  mCarLights.setChannelDutyCycle(1, 100, 0);
  mCarLights.setChannelDutyCycle(4, 100, 0);
  // rear
  mCarLights.setChannelDutyCycle(7, 100, 0);
  mCarLights.setChannelDutyCycle(9, 100, 0);
  // front stand
  mCarLights.setChannelDutyCycle(2, 100, 0);
  mCarLights.setChannelDutyCycle(5, 100, 0);
}

void CarLEDsSetOnNormalLights(){
  // front 
  mCarLights.setChannelDutyCycle(1, 0, 100);
  mCarLights.setChannelDutyCycle(4, 0, 100);
  // rear
  mCarLights.setChannelDutyCycle(7, 0, 100);
  mCarLights.setChannelDutyCycle(9, 0, 100);
}

void CarLEDsSetOnStandLights(){
  mCarLights.setChannelDutyCycle(2, 0, 100);
  mCarLights.setChannelDutyCycle(5, 0, 100);
}

void CarLEDsSetOffBlinkers(){
  mCarLights.setChannelDutyCycle(0, 100, 0);
  mCarLights.setChannelDutyCycle(8, 100, 0);
  mCarLights.setChannelDutyCycle(3, 100, 0);
  mCarLights.setChannelDutyCycle(6, 100, 0);
}

void CarLEDsSetOnBlinkers(){
  mCarLights.setChannelDutyCycle(0, 0, 100);
  mCarLights.setChannelDutyCycle(8, 0, 100);
  mCarLights.setChannelDutyCycle(3, 0, 100);
  mCarLights.setChannelDutyCycle(6, 0, 100);
}

void CarLEDsSetOnBlinkersLeft(){
  // left
  mCarLights.setChannelDutyCycle(3, 0, 100);
  mCarLights.setChannelDutyCycle(6, 0, 100);
  // right
  mCarLights.setChannelDutyCycle(0, 100, 0);
  mCarLights.setChannelDutyCycle(8, 100, 0);
}

void CarLEDsSetOnBlinkersRight(){
  // left
  mCarLights.setChannelDutyCycle(3, 100, 0);
  mCarLights.setChannelDutyCycle(6, 100, 0);
  // right
  mCarLights.setChannelDutyCycle(0, 0, 100);
  mCarLights.setChannelDutyCycle(8, 0, 100);
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
