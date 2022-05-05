// ========================================================================================
// Description:       Demo and test Sequenzen
// ========================================================================================


void DemoLoop_Run(){

  // 10 mal alle LEDs nach einander leuchten lassen
  for(int i = 0; i < 10; i++) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      mCarLights.setPWM(pwmnum, 0, 0);
      delay(500);
      mCarLights.setPWM(pwmnum, 0, 4095);
    }
  }

  // verschiedene Signal zustände testen
  uint16_t signal_A_Turn = 2000;
  uint16_t signal_B_Light = 1000;
  uint16_t signal_C_RgbLights = 1000;
  uint16_t signal_D_Animation = 1000;

  SerialMonitorHelper_Print("light off");

  for(int i = 0; i < 1000; i++) {
    DemoLoop_Iteration(signal_A_Turn, signal_B_Light, signal_C_RgbLights, signal_D_Animation);
    delay(2);
  }

  signal_B_Light = 1500;
  SerialMonitorHelper_Print("light stand");
  for(int i = 0; i < 1000; i++) {
    DemoLoop_Iteration(signal_A_Turn, signal_B_Light, signal_C_RgbLights, signal_D_Animation);
    delay(2);
  }

  signal_B_Light = 2000;
  SerialMonitorHelper_Print("light on");
  for(int i = 0; i < 1000; i++) {
    DemoLoop_Iteration(signal_A_Turn, signal_B_Light, signal_C_RgbLights, signal_D_Animation);
    delay(2);
  }
}

void DemoLoop_Iteration(uint16_t signal_A_Turn,
    uint16_t signal_B_Light,
    uint16_t signal_C_RgbLights,
    uint16_t signal_D_Animation) {

  Bumper_Update();
  Roof_Update();
  CarLight_Update();

  delay(100); // Simulate readInput delay

  UpdateTimeUp(true);

  //SetAnimationOption(signal_D_Animation);

  // blinker
  CarBlinker_SetTurnSignal(signal_A_Turn, 1000, 2000, 1500, true);

  // lights
  CarLight_SetOnStandLightOrDriveLight(signal_B_Light, 1000, 2000, 1500);

  // roof and bumper rgb lights
  Bumper_SetAnimationMod(signal_C_RgbLights, 1000, 2000, 1500);
  Roof_SetAnimationMod(signal_C_RgbLights, 1000, 2000, 1500);
  Roof_Blinker(signal_A_Turn, 1000, 2000, 1500);
}
