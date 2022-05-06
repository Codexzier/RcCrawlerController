// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       RC Crawler Beleuchtung V2
// Author:        Johannes P. Langner
// Controller:    Arduino Mini Pro
// Receiver:      FrSky RX8R
// Actor:         RGB LEDs WS2812b, PCA9685
// Description:   Licht Steuerung
// Stand:         05.05.2022
// ========================================================================================

#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>

// ========================================================================================
// Setup - Parameter for color and Light animations.

// Color Index
// -------------------------
// 0 = Orange
// 1 = gelb
// 2 = türkis
// 3 = hellblau
// 4 = defuse neutral
// 5 = dark tyrkis
// 6 = dark wine red
// 7 = Okka / dark yellow
// 8 = lila
// 9 = rosa

// Animation Index
// -------------------------
// 0 = like Knight Rider / K.I.T.T moving light

uint16_t mRoof_ColorIndex = 8;
uint16_t mRoof_AnimIndex = 0;

uint16_t mBumper_ColorIndex = 8;
uint16_t mBumper_AnimIndex = 0;


// ========================================================================================
// Debuggen
bool mSerialMonitor = false;                  // Set this value true, for show all Value 
                                              // on Serial Monitor.
                                              // WARNING: The runtime in debug is significantly slower.

bool mDemoMode = false;

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
// Wird verwendet, um die Ziel RGB für den nächsten Stand vorzubereiten.
typedef struct {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;

  uint8_t AltRed;
  uint8_t AltGreen;
  uint8_t AltBlue;

  uint8_t TargetRed;
  uint8_t TargetGreen;
  uint8_t TargetBlue;
  
  bool Up;
  bool On;

  uint32_t GetColor() {
    return ((uint32_t)Red << 16) | ((uint32_t)Green <<  8) | Blue;
  }
  
} RgbSetup;


// ========================================================================================
// ws2812b - Status RGB
const uint16_t mPinRgbStripe0 = 2;
const int16_t mCountRgbLeds0 = 1;  

// 1. count of using RGB LEDs
// 2. used on pin number
// 3. Choose RGB, GRB oder BRG and used clock 400KHz oder 800KHz
Adafruit_NeoPixel mPixels_Status = Adafruit_NeoPixel(mCountRgbLeds0, mPinRgbStripe0, NEO_GRB + NEO_KHZ800);

// ========================================================================================
// ws2812b - Stripe Roof
const uint16_t mPinRgbStripe1 = 4;
const uint16_t mCountRgbLeds1 = 20;   
const uint16_t mTailRgbLeds1 = 40;
int16_t mRgbBrightnessMaxValue1 = 0;

Adafruit_NeoPixel mPixels_Roof = Adafruit_NeoPixel(mCountRgbLeds1, mPinRgbStripe1, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
RgbSetup mRgbSetup_Roof[mCountRgbLeds1];

// go online, only used to animate the roof by running setup method
uint8_t mRoof_GoOnline_Index = 0;
boolean mRoof_GoOnline_Finish = false;


// ========================================================================================
// ws2812b - Stripe Front Bumper
const uint8_t mPinRgbStripe2 = 5;
const uint8_t mCountRgbLeds2 = 11;   
const uint8_t mTailRgbLeds2 = 40;
uint8_t mPixel_2_AnimationMode = 0;
uint8_t mRgbBrightnessMaxValue2 = 0;

Adafruit_NeoPixel mPixels_Bumper = Adafruit_NeoPixel(mCountRgbLeds2, mPinRgbStripe2, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
RgbSetup mRgbSetup_Bumper[mCountRgbLeds2];

// go online, only used to animate the roof by running setup method
uint8_t mBumper_GoOnline_Index = 0;
boolean mBumper_GoOnline_Finish = false;


// ========================================================================================
// RC PWM Input signal for Light Mods
#define PIN_INPUT_A A0                         // pwm input 1 for wheel
#define PIN_INPUT_B A1                         // pwm input 2 for LED
#define PIN_INPUT_C A2                         // pwm input 3 for LED RGB
#define PIN_INPUT_D A3                         // pwm input 4 for brightness / animation option
#define PIN_INPUT_E A6                         // pwm input 5 for drive

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

uint8_t mSwitchToNextInput = 0;

// TODO: Save last used Strip Light Mode

uint8_t mInputD_AnimationOption = 0;



// ========================================================================================
// Timings and other things
unsigned long mCurrentMillis;                   // aktuelle Zeit.
unsigned long mLastMillis;                      // verstrichende Zeit.

uint16_t mActiveCount = 0;

// TODO: wird das noch verwendet?
long mTimeToChange = 0;
bool mChangeLightsToOnOrOff = true;

// ========================================================================================
void setup() {
  // setup lights and inputs
  BootSequenz_Setup();
}

// ========================================================================================
void loop() {

  if(mDemoMode) {
    DemoLoop_Run();
    return;
  }

  // update leds
  Bumper_Update();
  Roof_Update();
  CarLight_Update();

  // read PWM signals from Receiver
  RcInputsReadInputs();
  
  // if the receiver is not on,
  // the current time goes up.
  if(UpdateTimeUp(true)) {

    Bumper_SignalLost();
    Roof_SignalLost();

    return;
  }

  SetAnimationOption(mReadValueD);
  
  // blinker
  CarBlinker_SetTurnSignal(mReadValueA, mInputMinA, mInputMaxA, mInputMiddleA, true);

  // lights
  CarLight_SetOnStandLightOrDriveLight(mReadValueB, mInputMinB, mInputMaxB, mInputMiddleB);

  // roof and bumper rgb lights
  Bumper_SetAnimationMod(mReadValueC, mInputMinC, mInputMaxC, mInputMiddleC);
  Roof_SetAnimationMod(mReadValueC, mInputMinC, mInputMaxC, mInputMiddleC);

  // roof lights off (signal is near 1000)
  if(mReadValueC < mInputMinC + mThresholdValue) {
    return;
  }
  
  Roof_Blinker(mReadValueA, mInputMinA, mInputMaxA, mInputMiddleA);
}

void SetAnimationOption(int inputValue) {
  
  if(inputValue < 1100) {

    mInputD_AnimationOption = 1;    
    return;
  }

  if(inputValue > 1100 &&
     inputValue < 1300) {

    mInputD_AnimationOption = 2;
    return;
  }

  if(inputValue > 1300 &&
     inputValue < 1500) {

    mInputD_AnimationOption = 3;
    return;
  }

  if(inputValue > 1500 &&
     inputValue < 1700) {

    mInputD_AnimationOption = 4;
    return;
  }

  // signal is near to 2000 or 1000
  if(inputValue > 1700) {
    mInputD_AnimationOption = 5;
  }
}

// ========================================================================================
// aktualisiert den aktuellen Zeitabstand
// Wenn zulange die Signale nicht ausgewertet wurden, 
// dann soll über andere Funktionen an den LEDs Sichtbar werden, 
// dass der Empfang unterbrochen wurde.
// Gibt true zurück, wenn die maximale Zeit überschritten wurde.
// f = Zeigt auf dem Serialmonitor an, wenn die Zeit überschritten wurde
bool UpdateTimeUp(bool f){
  mCurrentMillis = millis();

  boolean result = false;

  unsigned long timeGone = mCurrentMillis - mLastMillis;
  
  if(timeGone > 1000) {
    result = true;
  }
   mLastMillis = mCurrentMillis;

  if(mSerialMonitor) {
    //Serial.print("Millis: ");
    //Serial.println(timeGone, DEC);
  }
  
  if(result && f) {
    SerialMonitorHelper_Print("long iteration. Set all in offline state!");
  }
  return result;
}
