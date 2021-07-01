// ========================================================================================
//      Meine Welt in meinem Kopf
// ========================================================================================
// Projekt:       RC Crawler Beleuchtung
// Author:        Johannes P. Langner
// Controller:    Wemos
// Receiver:      WLAN, PWM
// Actor:         RGB LEDs WS2812b, Piozo-Summer
// Description:   Licht und Hupe Steuern.
// Stand:         28.05.2021
// ========================================================================================

#include <Adafruit_NeoPixel.h>

// ========================================================================================
// Debuggen
bool mDebugOn = true;

// ========================================================================================
// ws2812b
// Strip Dach
const int mPinRgbStripe1 = 3;
const int mCountRgbLeds1 = 24;   
const int mTailRgbLeds1 = 40;

// Instanz initialisieren
// Parameter Angaben:
// 1. Anzahl für verwendeter RGB LEDs
// 2. Auf welchen Pin soll verwendet werden.
// 3. Auswahl zwischen RGB, GRB oder BRG und verwendeter Takt 400KHz oder 800KHz
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
int mMoveLightArray_1_ColorSwitch = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("Start RGB Strip");
  mPixels1.begin();

  AnimateInitialRgbLight();
}

void loop() {
  SetMoving();
  
  UpdateRgbLights();
  delay(20);
}
