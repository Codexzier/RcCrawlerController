// ========================================================================================
// Description:       Initialisieren der LEDs und Inputs
// ========================================================================================

void BootSequenz_Setup() {
  if(mSerialMonitor) {
    Serial.begin(115200);
  }

  // --------------------------------------------------------------------------------------
  BootSequenz_CarLights();
  
  // --------------------------------------------------------------------------------------
  RcInput_Setup();

  // --------------------------------------------------------------------------------------
  BootSequenz_RGBs();
}

void BootSequenz_CarLights() {
  if(mSerialMonitor) {
    Serial.println("Start PCA for all LEDs");
  }

  mCarLights.begin();
  //mCarLights.setOscillatorFrequency(27000000);
  mCarLights.setPWMFreq(1600);
  Wire.setClock(400000);

  // set all pwm outputs off
  for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
    mCarLights.setPWM(pwmnum, 0, 4095);
  }
}

void BootSequenz_RGBs() {
  if(mSerialMonitor) {
    Serial.println("Start RGB Strips for Pin 2, 3, 4");
  }

  mPixels_Status.begin();
  mPixels_Roof.begin();
  mPixels_Bumper.begin();

  Status_On();

  // --------------------------------------------------------------------------------------
  // boot sequence roof - Fadein
  while(!mRoof_GoOnline_Finish) {
    UpdateTimeUp(false);
    Roof_GoOnline();
    Roof_Update();
  }
  mRoof_GoOnline_Finish = false;
  
  // boot sequence roof - Fadeout
  while(!mRoof_GoOnline_Finish) {
    UpdateTimeUp(false);
    Roof_GoOnline_Fadeout();
    Roof_Update();
  }

  // --------------------------------------------------------------------------------------
  // boot sequence bumper - Fadein
  while(!mBumper_GoOnline_Finish) {
    UpdateTimeUp(false);
    Bumper_GoOnline();
    Bumper_Update();
  }
  mBumper_GoOnline_Finish = false;

  // boot sequence roof - Fadeout
  while(!mBumper_GoOnline_Finish) {
    UpdateTimeUp(false);
    Bumper_GoOnline_Fadeout();
    Bumper_Update();
  }
}
