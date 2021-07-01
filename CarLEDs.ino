// ========================================================================================
// Set first RGB LED on Front Bumper.
void CarLEDsInit() {
  for(int chanel = 0; chanel < 16; chanel++) {
    mCarLights.setPWM(chanel, 0, 0);
  }

  mCarLights.setPWM(0, 0, 4096);
}

void CarLEDsNextChannel(){
  if(mCarLightsChannel < 15) {
    mCarLightsChannel++;
  }
  else {
    mCarLightsChannel = 0;
  }

  if(mSerialMonitor) {
    Serial.print("Set Channel: ");
    Serial.println(mCarLightsChannel, DEC);
  }
}

void CarLEDsSetOffStandAndNormalLights(){
  mCarLights.setPWM(0, 0, 0);
  mCarLights.setPWM(1, 0, 0);
  mCarLights.setPWM(2, 0, 0);
  mCarLights.setPWM(3, 0, 0);
  mCarLights.setPWM(4, 0, 0);
  mCarLights.setPWM(5, 0, 0);
}

void CarLEDsSetOnNormalLights(){
  mCarLights.setPWM(0, 0, 4096);
  mCarLights.setPWM(1, 0, 4096);
  mCarLights.setPWM(2, 0, 4096);
  mCarLights.setPWM(3, 0, 4096);
}

void CarLEDsSetOnStandLights(){
  mCarLights.setPWM(4, 0, 4096);
  mCarLights.setPWM(5, 0, 4096);
}

void CarLEDsSetOffBlinkers(){
  mCarLights.setPWM(6, 0, 0);
  mCarLights.setPWM(7, 0, 0);
  mCarLights.setPWM(8, 0, 0);
  mCarLights.setPWM(9, 0, 0);
}

void CarLEDsSetOnBlinkers(){
  mCarLights.setPWM(6, 0, 4096);
  mCarLights.setPWM(7, 0, 4096);
  mCarLights.setPWM(8, 0, 4096);
  mCarLights.setPWM(9, 0, 4096);
}

void CarLEDsSetOnBlinkersLeft(){
  mCarLights.setPWM(6, 0, 4096);
  mCarLights.setPWM(7, 0, 4096);
  mCarLights.setPWM(8, 0, 0);
  mCarLights.setPWM(9, 0, 0);
}

void CarLEDsSetOnBlinkersRight(){
  mCarLights.setPWM(6, 0, 0);
  mCarLights.setPWM(7, 0, 0);
  mCarLights.setPWM(8, 0, 4096);
  mCarLights.setPWM(9, 0, 4096);
}

void CarLEDsSetOnFrontBumper(){
  mCarLights.setPWM(10, 0, 4096);
  mCarLights.setPWM(11, 0, 4096);
}

void CarLEDsSetOnRearBumper(){
  mCarLights.setPWM(12, 0, 4096);
  mCarLights.setPWM(13, 0, 4096);
}
