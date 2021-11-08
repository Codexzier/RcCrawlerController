// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       RC Crawler Beleuchtung V2
// Author:        Johannes P. Langner
// Controller:    Arduino Mini Pro
// Receiver:      FrSky RX8R
// Actor:         RGB LEDs WS2812b, PCA9685
// Description:   Licht Steuerung
// Stand:         24.10.2021
// ========================================================================================

#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>

// ========================================================================================
// Debuggen
bool mSerialMonitor = false;                // Set this value true, for show all Value 
                                           // on Serial Monitor.

// ========================================================================================
// LED Control with PWM driver.

Adafruit_PWMServoDriver mCarLights = Adafruit_PWMServoDriver();

typedef struct {
  uint16_t portNumber;                         // PWM Port Number from 0 to 15
  uint16_t off;                                // keeps the current set off state
  uint16_t on;                                 // keeps the current set on state
  uint16_t maxOn;                              // limits the maximum value that can be switched on
  uint16_t noChanged;                          // indicates whether the light condition has changed
  bool up;                                     // sets or retrieves the state whether the value is set up or down.
  uint16_t lastOnValue;                        // value from the last setup
} carLedType;

// Description:
// The mapping seems a bit complicated.
// For each LED a setting is stored and which value was currently set.
// A problem with the PWM driver PCA 9685, 
// you need the last state to realize a reasonable transition from fade in and fade out.
int8_t mLEDs_Size = 11;
carLedType mLEDs[] = {
  { 1, 0,0, 100, 0},   // Front Left           Portnumber  1
  { 4, 0,0, 100, 0},   // Front Right          Portnumber  4
  { 2, 0,0, 100, 0},   // Front Stand Left     Portnumber  2
  { 5, 0,0, 100, 0},   // Front Stand Right    Portnumber  5
  { 0, 0,0, 100, 0},   // Front Blinker Left   Portnumber  0
  { 3, 0,0, 100, 0},   // Front Blinker Right  Portnumber  3
  { 9, 0,0, 100, 0},   // Rear  Left           Portnumber  9 
  { 6, 0,0, 100, 0},   // Rear  Right          Portnumber  6
  { 8, 0,0, 100, 0},   // Rear  Blinker Left   Portnumber  8
  { 7, 0,0, 100, 0},   // Rear  Blinker Right  Portnumber  7
  {12, 0,0, 100, 0}    // Rear  number plate   Portnumber 12
};

// ========================================================================================
// ws2812b - Status RGB
const uint16_t mPinRgbStripe0 = 2;
const int16_t mCountRgbLeds0 = 1;  

// 1. count of using RGB LEDs
// 2. used on pin number
// 3. Choose RGB, GRB oder BRG and used clock 400KHz oder 800KHz
Adafruit_NeoPixel mPixels0 = Adafruit_NeoPixel(mCountRgbLeds0, mPinRgbStripe0, NEO_GRB + NEO_KHZ800);

// ========================================================================================
// ws2812b - Stripe Roof
const uint16_t mPinRgbStripe1 = 4;
const uint16_t mCountRgbLeds1 = 20;   
const uint16_t mTailRgbLeds1 = 40;
int16_t mRgbBrightnessMaxValue1 = 0;

Adafruit_NeoPixel mPixels1 = Adafruit_NeoPixel(mCountRgbLeds1, mPinRgbStripe1, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
uint8_t mMoveLightArray_1_Red[mCountRgbLeds1];
uint8_t mMoveLightArray_1_Green[mCountRgbLeds1];
uint8_t mMoveLightArray_1_Blue[mCountRgbLeds1];
uint8_t mMoveLightArray_1_IndexRed = 0;
uint8_t mMoveLightArray_1_IndexGreen = 0;
uint8_t mMoveLightArray_1_IndexBlue = 0;
bool mMoveLightArray_1_ForwardRed = true;
bool mMoveLightArray_1_ForwardGreen = true;
bool mMoveLightArray_1_ForwardBlue = true;
int8_t mMoveLightArray_1_ColorSwitch = 8;


// ========================================================================================
// ws2812b - Stripe Front Bumper
const uint8_t mPinRgbStripe2 = 5;
const uint8_t mCountRgbLeds2 = 11;   
const uint8_t mTailRgbLeds2 = 40;
uint8_t mPixel_2_AnimationMode = 0;
uint8_t mRgbBrightnessMaxValue2 = 0;

Adafruit_NeoPixel mPixels2 = Adafruit_NeoPixel(mCountRgbLeds2, mPinRgbStripe2, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
uint8_t mMoveLightArray_2_Red[mCountRgbLeds2];
uint8_t mMoveLightArray_2_Green[mCountRgbLeds2];
uint8_t mMoveLightArray_2_Blue[mCountRgbLeds2];


// ========================================================================================
// RC PWM Input signal for Light Mods
#define PIN_INPUT_A A0                         // pwm input 1 for wheel
#define PIN_INPUT_B A1                         // pwm input 2 for LED
#define PIN_INPUT_C A2                         // pwm input 3 for LED RGB
#define PIN_INPUT_D A3                         // pwm input 4 for Helligkeit
//#define PIN_INPUT_E A6                       // pwm input 5 for drive

boolean mVehicleModeSteerInvert = false;       // invert steering direction, only tracked vehicle 
uint16_t mDeathbandPlusMinus = 20;             // Tolerance range in which no reaction should take 
                                               // place when sticks are in the middle position.
uint16_t mThresholdValue = 200;                // Threshold distance to minimum, middle and maximum value.

uint16_t mInputMiddleA = 1500;                 // middel state of the input signal. Example: stick on neutral position
uint16_t mInputMinA = 987;                     // minimum value of input signal. Example: stick is move complete down
uint16_t mInputMaxA = 1988;                    // minimum value of input signal. Example: stick is move complete up

uint16_t mInputMiddleB = 1490;                 // middel state of the input signal. Example: stick on neutral position
uint16_t mInputMinB = 981;                     // minimum value of input signal. Example: stick is move complete down
uint16_t mInputMaxB = 1999;                    // minimum value of input signal. Example: stick is move complete up

uint16_t mInputMiddleC = 1490;                 // middel state of the input signal. Example: stick on neutral position
uint16_t mInputMinC = 981;                     // minimum value of input signal. Example: stick is move complete down
uint16_t mInputMaxC = 1999;                    // minimum value of input signal. Example: stick is move complete up

uint16_t mInputMiddleD = 1490;                 // middel state of the input signal. Example: stick on neutral position
uint16_t mInputMinD = 981;                     // minimum value of input signal. Example: stick is move complete down
uint16_t mInputMaxD = 1999;                    // minimum value of input signal. Example: stick is move complete up

uint16_t mInputMiddleE = 1500;                 // middel state of the input signal. Example: stick on neutral position
uint16_t mInputMinE = 1000;                    // minimum value of input signal. Example: stick is move complete down
uint16_t mInputMaxE = 2000;                    // minimum value of input signal. Example: stick is move complete up

uint16_t mReadValueA = 0;                      // input signal result from A
uint16_t mReadValueB = 0;                      // input signal result from B
uint16_t mReadValueC = 0;                      // input signal result from C
uint16_t mReadValueD = 0;                      // input signal result from D
uint16_t mReadValueE = 0;                      // input signal result from E

// TODO: Save last used Strip Light Mode



// ========================================================================================
// Timings and other things
unsigned long mCurrentMillis;                       // aktuelle verstrichende Zeit.
unsigned long mLastMillis;                       // aktuelle verstrichende Zeit.

uint16_t mActiveCount = 0;

// ========================================================================================
void setup() {
  if(mSerialMonitor) {
    Serial.begin(115200);
  }

  if(mSerialMonitor) {
    Serial.println("Start RGB Strips for Pin 2, 3, 4");
  }

  mPixels0.begin();
  mPixels1.begin();
  mPixels2.begin();

  if(mSerialMonitor) {
    Serial.println("Start PCA for all LEDs");
  }

  mCarLights.begin();
  mCarLights.setOscillatorFrequency(27000000);
  mCarLights.setPWMFreq(1600);
  Wire.setClock(400000);

 
  if(mSerialMonitor) {
    Serial.println("Setup Inputs");
  }

  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);
  pinMode(PIN_INPUT_C, INPUT);
  pinMode(PIN_INPUT_D, INPUT);

  Status_On();
  Roof_Offline();
  Bumper_Offline();
  
  CarLight_Off();
  CarLight_Update();
  delay(1000);
}


long mTimeToChange = 0;
bool mChangeLightsToOnOrOff = true;

// ========================================================================================
void loop() {

  //CarLight_ShowSingleLedAndIndex();
  //CarLight_ShowDimming();
  //CarBlinker_SetTurnSignal(1000, mInputMinA, mInputMaxA, mInputMiddleA);
//  Serial.print("C: "); Serial.print(mActiveCount, DEC);
//  Serial.print(", mTimeToChange: "); Serial.println(mTimeToChange, DEC);
  mActiveCount++;
  UpdateTimeUp(false);

mTimeToChange++;

   if(mTimeToChange > 20000) {
      mChangeLightsToOnOrOff = !mChangeLightsToOnOrOff;
      mTimeToChange = 0;
  
      if(mSerialMonitor) {
        Serial.print("Change Light to: ");
        Serial.println(mChangeLightsToOnOrOff, BIN);
      }
    }
//    if(mCurrentMillis - mTimeToChange < 0) {
//      mTimeToChange = 0;
//    }

  if(mChangeLightsToOnOrOff) {
    //CarLight_SetOnStandLightOrDriveLight(1000, mInputMinB, mInputMaxB, mInputMiddleB);
    //CarBlinker_SetTurnSignal(1000, mInputMinA, mInputMaxA, mInputMiddleA);
    Bumper_SetAnimationMod(2000, mInputMinC, mInputMaxC, mInputMiddleC);
    Roof_SetAnimationMod(2000, mInputMinC, mInputMaxC, mInputMiddleC);
  }
  else {
    //CarLight_SetOnStandLightOrDriveLight(2000, mInputMinB, mInputMaxB, mInputMiddleB);
    //CarBlinker_SetTurnSignal(2000, mInputMinA, mInputMaxA, mInputMiddleA);
    Bumper_SetAnimationMod(2000, mInputMinC, mInputMaxC, mInputMiddleC);
    Roof_SetAnimationMod(2000, mInputMinC, mInputMaxC, mInputMiddleC);
  }
//  if(mChangeLightsToOnOrOff) {
//    CarLight_On();
//    mPixels1.setPixelColor(0, mPixels1.Color(110, 110, 0));
//    mPixels1.show();
//  }
//  else {
//    CarLight_Off();
//    mPixels1.setPixelColor(0, mPixels1.Color(255, 0, 0));
//    mPixels1.show();
//  }

  Bumper_Update();
  Roof_Update();
  CarLight_Update();

//Serial.println("---------------------------------------------");
  //delay(1);
  return;

  // update rgb led stats
  Bumper_Update();
  Roof_Update();
  CarLight_Update();
  
  // read PWM signals from Receiver
  RcInputsReadInputs();
  
  // if the receiver is not on,
  // the current time goes up.
  if(UpdateTimeUp(true)) {
    
    CarLight_SetOnBlinkers();
    CarLight_Update();
    delay(1000);
    CarLight_SetOffBlinkers();
    CarLight_Update();
    delay(100);

    Roof_Offline();
    Bumper_Offline();

    delay(100);
    UpdateTimeUp(false);
    return;
  }

  // blinker
  CarBlinker_SetTurnSignal(mReadValueA, mInputMinA, mInputMaxA, mInputMiddleA);

  // lights
  CarLight_SetOnStandLightOrDriveLight(mReadValueB, mInputMinB, mInputMaxB, mInputMiddleB);

  // roof and bumper rgb lights
  Bumper_SetAnimationMod(mReadValueC, mInputMinC, mInputMaxC, mInputMiddleC);
  Roof_SetAnimationMod(mReadValueC, mInputMinC, mInputMaxC, mInputMiddleC);
}

// ========================================================================================
bool UpdateTimeUp(bool f){
  mCurrentMillis = millis();

  boolean result = false;
  if(mCurrentMillis - mLastMillis > 1000) {
    mLastMillis = mCurrentMillis;
    result = true;
  }
  
  if(result && f) {
    if(mSerialMonitor) {
      Serial.println("long iteration. Set all in offline state!");
    }
  }
  return result;
}
