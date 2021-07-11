// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       RC Crawler Beleuchtung
// Author:        Johannes P. Langner
// Controller:    Arduino Mini
// Receiver:      FrSky RX8R
// Actor:         RGB LEDs WS2812b, Piozo-Summer, PCA9685
// Description:   Licht und Hupe Steuern.
// Stand:         28.06.2021
// ========================================================================================

#include <Adafruit_NeoPixel.h>
#include <PCA9685.h>
//#include <Adafruit_PWMServoDriver.h>

// ========================================================================================
// Debuggen
bool mSerialMonitor = false;            // Set this value true, for show all Value on Serial Monitor.

// ========================================================================================
// PCA9685 - all LEDs (14)
//  - Front Lights (2)
//  - Front Stand Lights (2)
//  - Front Bumper Lights  (2)
//  - Front Left/Rigth Flashs  (2)
//  - Rear Lights  (2)
//  - Rear Left/Right Flashs (2)
//  - Rear Bumper Lights (2)

//Adafruit_PWMServoDriver mCarLights = Adafruit_PWMServoDriver();
PCA9685 mCarLights;

int mCarLightsChannel = 0;

// ========================================================================================
// ws2812b - Status RGB
const int mPinRgbStripe0 = 2;
const int mCountRgbLeds0 = 1;  

// 1. count of using RGB LEDs
// 2. used on pin number
// 3. Choose RGB, GRB oder BRG and used clock 400KHz oder 800KHz
Adafruit_NeoPixel mPixels0 = Adafruit_NeoPixel(mCountRgbLeds0, mPinRgbStripe0, NEO_GRB + NEO_KHZ800);

// ========================================================================================
// ws2812b - Stripe Roof
const int mPinRgbStripe1 = 3;
const int mCountRgbLeds1 = 20;   
const int mTailRgbLeds1 = 40;
int mPixel_1_AnimationMode = 0;

Adafruit_NeoPixel mPixels1 = Adafruit_NeoPixel(mCountRgbLeds1, mPinRgbStripe1, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
int mMoveLightArray_1_Red[mCountRgbLeds1];
int mMoveLightArray_1_Green[mCountRgbLeds1];
int mMoveLightArray_1_Blue[mCountRgbLeds1];
int mMoveLightArray_1_IndexRed = 0;
int mMoveLightArray_1_IndexGreen = 0;
int mMoveLightArray_1_IndexBlue = 0;
bool mMoveLightArray_1_ForwardRed = true;
bool mMoveLightArray_1_ForwardGreen = true;
bool mMoveLightArray_1_ForwardBlue = true;
int mMoveLightArray_1_ColorSwitch = 8;



// ========================================================================================
// ws2812b - Stripe Front Bumper
const int mPinRgbStripe2 = 4;
const int mCountRgbLeds2 = 10;   
const int mTailRgbLeds2 = 40;
int mPixel_2_AnimationMode = 0;

Adafruit_NeoPixel mPixels2 = Adafruit_NeoPixel(mCountRgbLeds2, mPinRgbStripe2, NEO_GRB + NEO_KHZ800);

// RGB LEDs Array State
int mMoveLightArray_2_Red[mCountRgbLeds2];
int mMoveLightArray_2_Green[mCountRgbLeds2];
int mMoveLightArray_2_Blue[mCountRgbLeds2];

// ========================================================================================
// ws2812b - Stripe Unknow 1
const int mPinRgbStripe3 = 5;
const int mCountRgbLeds3 = 1;   
int mPixel_3_AnimationMode = 0;

Adafruit_NeoPixel mPixels3 = Adafruit_NeoPixel(mCountRgbLeds3, mPinRgbStripe3, NEO_GRB + NEO_KHZ800);

// ========================================================================================
// ws2812b - Stripe Unknow 2
const int mPinRgbStripe4 = 6;
const int mCountRgbLeds4 = 1;
int mPixel_4_AnimationMode = 0;

Adafruit_NeoPixel mPixels4 = Adafruit_NeoPixel(mCountRgbLeds4, mPinRgbStripe4, NEO_GRB + NEO_KHZ800);

// ========================================================================================
// RC PWM Input signal for Light Mods
#define PIN_INPUT_A A0                    // pwm input 1 for motor A
#define PIN_INPUT_B A1                    // pwm input 2 for motor B
#define PIN_INPUT_C A2                    // pwm input 3 for motor C
#define PIN_INPUT_D A3                    // pwm input 4 for motor D

boolean mVehicleModeSteerInvert = false;  // invert steering direction, only tracked vehicle 
int mDeathbandPlusMinus = 20;             // Tolerance range in which no reaction should take 
                                          // place when sticks are in the middle position.
int mThresholdValue = 200;                // Threshold distance to minimum, middle and maximum value.

int mInputMiddleA = 1500;                 // middel state of the input signal. Example: stick on neutral position
int mInputMinA = 987;                     // minimum value of input signal. Example: stick is move complete down
int mInputMaxA = 1988;                    // minimum value of input signal. Example: stick is move complete up

int mInputMiddleB = 1490;                 // middel state of the input signal. Example: stick on neutral position
int mInputMinB = 981;                     // minimum value of input signal. Example: stick is move complete down
int mInputMaxB = 1999;                    // minimum value of input signal. Example: stick is move complete up

int mInputMiddleC = 1490;                 // middel state of the input signal. Example: stick on neutral position
int mInputMinC = 981;                     // minimum value of input signal. Example: stick is move complete down
int mInputMaxC = 1999;                    // minimum value of input signal. Example: stick is move complete up

int mInputMiddleD = 1490;                 // middel state of the input signal. Example: stick on neutral position
int mInputMinD = 981;                     // minimum value of input signal. Example: stick is move complete down
int mInputMaxD = 1999;                    // minimum value of input signal. Example: stick is move complete up

int mReadValueA = 0;                      // input signal result from A
int mReadValueB = 0;                      // input signal result from B
int mReadValueC = 0;                      // input signal result from C
int mReadValueD = 0;                      // input signal result from D

                                          // Setup input values to target control
const int mInputValueA_To = 1;            // 1 => Blinker
const int mInputValueB_To = 2;            // 2 => Stand Lights On and Normal Light
const int mInputValueC_To = 4;            // 3 => Strips On Headlight and animation lights
const int mInputValueD_To = 3;            // 4 => Change Strip Light Mode and all LEDs

                                          // TODO: Save last used Strip Light Mode

int mCurrentMillis;                       // aktuelle verstrichende Zeit.

void setup() {
  
  if(mSerialMonitor) {
    Serial.begin(115200);
  }

  if(mSerialMonitor) {
    Serial.println("Start RGB Strips for Pin 2, 3, 4, 5, 6");
  }

  mCarLights.setupSingleDevice(Wire, 0x40);
//  mCarLights.begin();
//  mCarLights.setOscillatorFrequency(27000000);
//  mCarLights.setPWMFreq(1600);
  Wire.setClock(400000);
  CarLEDsInit();
  
  mPixels0.begin();
  mPixels1.begin();
  mPixels2.begin();
  mPixels3.begin();
  mPixels4.begin();

  // Status RGB LED (Orange)
  StatusStateOn();

  // Roof RGB LED ()
  //RoofStateOn();
  for(int index = 0; index < mCountRgbLeds0; index++) {
    mPixels1.setPixelColor(index, mPixels1.Color(0, 0, 0));
  }
  mPixels1.setPixelColor(0, mPixels1.Color(0, 204, 0));
  mPixels1.show();
  
  // Bumper RGB LED (Yellow)
  //BumperStateOn();
  for(int index = 0; index < mCountRgbLeds2; index++) {
    mPixels2.setPixelColor(index, mPixels2.Color(200, 0, 0));
  }
  mPixels2.setPixelColor(0, mPixels1.Color(200, 0, 0));
  mPixels2.show();

  // Unknow 1 (hell blau)
  for(int index = 0; index < mCountRgbLeds1; index++) {
    mPixels3.setPixelColor(index, mPixels3.Color(0, 0, 0));
  }
  mPixels3.setPixelColor(0, mPixels1.Color(0, 204, 255));
  mPixels3.show();
  // Unknow 2 (lila)
  for(int index = 0; index < mCountRgbLeds4; index++) {
    mPixels4.setPixelColor(index, mPixels4.Color(0, 0, 0));
  }
  mPixels4.setPixelColor(0, mPixels1.Color(156, 0, 255));
  mPixels4.show();
  
  if(mSerialMonitor) {
    Serial.println("Setup Inputs");
  }
  pinMode(PIN_INPUT_A, INPUT);
  pinMode(PIN_INPUT_B, INPUT);
  pinMode(PIN_INPUT_C, INPUT);
  pinMode(PIN_INPUT_D, INPUT);
  
  //AnimateInitialRgbLight();
  int vDuty = 0;
  while(true) {
    for(int chanel = 0; chanel < 16; chanel++) {
      mCarLights.setChannelDutyCycle(chanel, vDuty, 100 - vDuty);
    }

    vDuty++;
    if(vDuty >= 100) {
      break;
    }
  }
  delay(3000);



//  while(true) {
//    RoofAnimationFadeOut();
//    RoofUpdateRgbLights();
//
//    BumperAnimationFadeOut();
//    BumperUpdateRgbLights();
//
//    if(mMoveLightArray_1_Red[0] < 20 && mMoveLightArray_2_Red[0] < 20) {
//      break;
//    }
//    delay(100);
//  }
//  while(true) {
//
//    
//    CarLEDsNextChannel();
//    delay(5000);
//  }
}



void loop() {

  // update time
  mCurrentMillis = millis();
  
  // read all input receiver signals
  RcInputsReadInputs();

  // switch the LEDs on or off by the input values from the receiver.
  RxInputSetValueToTargetLights();
}
