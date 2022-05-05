// ========================================================================================
// Description:       Initialisieren der LEDs und Inputs
// ========================================================================================

void BootSequenz_Setup() {
  if(mSerialMonitor) {
    Serial.begin(115200);
  }

  RcInput_Setup();
  BootSequenz_CarLights();
  BootSequenz_RGBs();
}

void BootSequenz_CarLights() {

  SerialMonitorHelper_Print("Start PCA for all LEDs");
  
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

  SerialMonitorHelper_Print("Start RGB Strips for Pin 2, 3, 4");

  mPixels_Status.begin();
  mPixels_Roof.begin();
  mPixels_Bumper.begin();

  Status_On();

  // --------------------------------------------------------------------------------------
  // all off

  // bumper
  for(uint8_t index = 0; index < mCountRgbLeds2; index++) {
    mPixels_Bumper.setPixelColor(index, 0);
  }

  mPixels_Bumper.show();

  // roof
  for(uint8_t index = 0; index < mCountRgbLeds1; index++) {
    mPixels_Roof.setPixelColor(index, 0);
  }
  
  mPixels_Roof.show();
  

  // --------------------------------------------------------------------------------------
  // boot sequence roof - Fadein
//  while(!mRoof_GoOnline_Finish) {
//    UpdateTimeUp(false);
//    Roof_GoOnline();
//    Roof_Update();
//  }
//  mRoof_GoOnline_Finish = false;
//  
//  // boot sequence roof - Fadeout
//  while(!mRoof_GoOnline_Finish) {
//    UpdateTimeUp(false);
//    Roof_GoOnline_Fadeout();
//    Roof_Update();
//  }

  // --------------------------------------------------------------------------------------
  // boot sequence bumper - Fadein
//  while(!mBumper_GoOnline_Finish) {
//    UpdateTimeUp(false);
//    Bumper_GoOnline();
//    Bumper_Update();
//  }
//  mBumper_GoOnline_Finish = false;
//
//  // boot sequence roof - Fadeout
//  while(!mBumper_GoOnline_Finish) {
//    UpdateTimeUp(false);
//    Bumper_GoOnline_Fadeout();
//    Bumper_Update();
//  }
}
