
volatile unsigned long mRcInput_PulseInTimeBegin_A = micros();
volatile unsigned long mRcInput_PulseInTimeEnd_A = micros();
volatile bool mRcInput_NewPulseDurationAvailable_A = false;

volatile unsigned long mRcInput_PulseInTimeBegin_B = micros();
volatile unsigned long mRcInput_PulseInTimeEnd_B = micros();
volatile bool mRcInput_NewPulseDurationAvailable_B = false;

volatile unsigned long mRcInput_PulseInTimeBegin_C = micros();
volatile unsigned long mRcInput_PulseInTimeEnd_C = micros();
volatile bool mRcInput_NewPulseDurationAvailable_C = false;

volatile unsigned long mRcInput_PulseInTimeBegin_D = micros();
volatile unsigned long mRcInput_PulseInTimeEnd_D = micros();
volatile bool mRcInput_NewPulseDurationAvailable_D = false;

void RcInputPinInterrupt_A()
{
  if (digitalRead(PIN_INPUT_A) == HIGH) {
    mRcInput_PulseInTimeBegin_A = micros(); // start measuring
  }
  else {
    mRcInput_PulseInTimeEnd_A = micros();  // stop measuring
    mRcInput_NewPulseDurationAvailable_A = true;
  }
}

void RcInputPinInterrupt_B()
{
  if (digitalRead(PIN_INPUT_B) == HIGH) {
    mRcInput_PulseInTimeBegin_B = micros(); // start measuring
  }
  else {
    mRcInput_PulseInTimeEnd_B = micros();  // stop measuring
    mRcInput_NewPulseDurationAvailable_B = true;
  }
}

void RcInputPinInterrupt_C()
{
  if (digitalRead(PIN_INPUT_C) == HIGH) {
    mRcInput_PulseInTimeBegin_C = micros(); // start measuring
  }
  else {
    mRcInput_PulseInTimeEnd_C = micros();  // stop measuring
    mRcInput_NewPulseDurationAvailable_C = true;
  }
}

void RcInputPinInterrupt_D()
{
  if (digitalRead(PIN_INPUT_D) == HIGH) {
    mRcInput_PulseInTimeBegin_D = micros(); // start measuring
  }
  else {
    mRcInput_PulseInTimeEnd_D = micros();  // stop measuring
    mRcInput_NewPulseDurationAvailable_D = true;
  }
}

void RcInput_Setup(){

  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);
  pinMode(PIN_INPUT_C, INPUT);
  pinMode(PIN_INPUT_D, INPUT);

//  pinMode(PIN_INPUT_A, INPUT);
//  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_A),
//                  RcInputPinInterrupt_A,
//                  CHANGE);
//
//  pinMode(PIN_INPUT_B, INPUT);
//  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_B),
//                  RcInputPinInterrupt_B,
//                  CHANGE);
//
//  pinMode(PIN_INPUT_C, INPUT);
//  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_C),
//                  RcInputPinInterrupt_C,
//                  CHANGE);
//
//  pinMode(PIN_INPUT_D, INPUT);
//  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_D),
//                  RcInputPinInterrupt_D,
//                  CHANGE); 
}

void RcInputsReadInputs() {

//  if (mRcInput_NewPulseDurationAvailable_A) {
//    mRcInput_NewPulseDurationAvailable_A = false;
//    mReadValueA = mRcInput_PulseInTimeEnd_A - mRcInput_PulseInTimeBegin_A;
//  }
//
//  if (mRcInput_NewPulseDurationAvailable_B) {
//    mRcInput_NewPulseDurationAvailable_B = false;
//    mReadValueA = mRcInput_PulseInTimeEnd_B - mRcInput_PulseInTimeBegin_B;
//  }
//
//  if (mRcInput_NewPulseDurationAvailable_C) {
//    mRcInput_NewPulseDurationAvailable_C = false;
//    mReadValueA = mRcInput_PulseInTimeEnd_C - mRcInput_PulseInTimeBegin_C;
//  }
//
//  if (mRcInput_NewPulseDurationAvailable_D) {
//    mRcInput_NewPulseDurationAvailable_D = false;
//    mReadValueA = mRcInput_PulseInTimeEnd_D - mRcInput_PulseInTimeBegin_D;
//  }

  
  mReadValueA = pulseIn(PIN_INPUT_A, HIGH);
  mReadValueB = pulseIn(PIN_INPUT_B, HIGH);
  mReadValueC = pulseIn(PIN_INPUT_C, HIGH);
  mReadValueD = 1000; //pulseIn(PIN_INPUT_D, HIGH);
  
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
