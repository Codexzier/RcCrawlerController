unsigned long mRxInputs_LastCurrentTime = 0;
// TODO: kann auch zentralsiiert werden
unsigned long mRcInputs_Speed = 50;

boolean RcInputs_CurrentTimeup(){
  
  if(mCurrentMillis - mRxInputs_LastCurrentTime < mRcInputs_Speed) {
    return true;
  }
  mRxInputs_LastCurrentTime = mCurrentMillis;
  return false;
}


void RcInput_Setup(){

  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);
  pinMode(PIN_INPUT_C, INPUT);
  pinMode(PIN_INPUT_D, INPUT);
  pinMode(PIN_INPUT_E, INPUT);
}

void RcInputsReadInputs() {

  if(RcInputs_CurrentTimeup()) {
    return;
  }

  // Nur einmal pro iteration
  // alle müssen nicht Zeitnah gesetzt werden, 
  // reicht ein aktualisieren der einzelnen Werte
  switch(mSwitchToNextInput) {
    case(0): { mReadValueA = pulseIn(PIN_INPUT_A, HIGH); mSwitchToNextInput++; break; }
    case(1): { mReadValueB = pulseIn(PIN_INPUT_B, HIGH); mSwitchToNextInput++; break; }
    case(2): { mReadValueC = pulseIn(PIN_INPUT_C, HIGH); mSwitchToNextInput++; break; }
    case(3): { mReadValueD = pulseIn(PIN_INPUT_D, HIGH); mSwitchToNextInput++; break; }
    default: { mReadValueE = pulseIn(PIN_INPUT_E, HIGH); mSwitchToNextInput = 0; break; }
  }
  
//  mReadValueA = pulseIn(PIN_INPUT_A, HIGH);
//  mReadValueB = pulseIn(PIN_INPUT_B, HIGH);
//  mReadValueC = pulseIn(PIN_INPUT_C, HIGH);
//  mReadValueD = pulseIn(PIN_INPUT_D, HIGH);
//  mReadValueE = pulseIn(PIN_INPUT_E, HIGH);

  if(mSerialMonitor) {
    Serial.print("INPUT A: "); Serial.print(mReadValueA);
    Serial.print("\tINPUT B: "); Serial.print(mReadValueB);
    Serial.print("\tINPUT C: "); Serial.print(mReadValueC);
    Serial.print("\tINPUT D: "); Serial.print(mReadValueD);
    Serial.print("\tINPUT E: "); Serial.print(mReadValueE);
    Serial.println("\t");
    delay(100);
  }
}
