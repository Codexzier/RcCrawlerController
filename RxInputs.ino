void RcInputsReadInputs() {
  mReadValueA = pulseIn(PIN_INPUT_A, HIGH);
  mReadValueB = pulseIn(PIN_INPUT_B, HIGH);
  mReadValueC = pulseIn(PIN_INPUT_C, HIGH);
  mReadValueD = pulseIn(PIN_INPUT_D, HIGH);
  //mReadValueE = pulseIn(PIN_INPUT_E, HIGH);

  if(mSerialMonitor) {
    Serial.print("INPUT A: "); Serial.print(mReadValueA);
    Serial.print("\tINPUT B: "); Serial.print(mReadValueB);
    Serial.print("\tINPUT C: "); Serial.print(mReadValueC);
    Serial.print("\tINPUT D: "); Serial.print(mReadValueD);
    //Serial.print("\tINPUT E: "); Serial.print(mReadValueE);
    Serial.println("\t");
    delay(100);
  }
}
