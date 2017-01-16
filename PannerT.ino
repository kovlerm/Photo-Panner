/********************************************************
 *  Photo panner and smart timer                        *    
 *                                                      *    
 *  By M.Kovler                                         *    
 *                                                      *    
 *  Branch of:                                          * 
 *  Simple automated video panner                       *
 *  Alex Sokolsky                                       *    
 *                                                      *
 *    v0.1  1/7/2017                                    *    
 *                                                      *
 ********************************************************/
#include "Wire.h"
extern TwoWire Wire1;
#include "Panner.h"
#include "BatteryMonitor.h"
#include <wiinunchuck.h>


//#define NODEBUG 1

/**
    Necessary for STL to link
*/
extern "C" {
  int _getpid() {
    return -1;
  }
  int _kill(int pid, int sig) {
    return -1;
  }
}


/**
   Globals: reading battery voltage on A0 (through divider offcourse)
*/
BatteryMonitor g_batteryMonitor(A0);

const uint8_t pinPanStep = 3;
  const uint8_t pinPanEnable = 0;
const uint8_t pinPanDirection = 6;

int varx,vary; // wii joystick position


class g_wiiJoy: public SELECTOR {
 /** define some values for button (key) scan codes */
const uint8_t VK_NONE  = 0;
const uint8_t VK_RIGHT = 1;
const uint8_t VK_LEFT  = 2;
const uint8_t VK_UP    = 4;
const uint8_t VK_DOWN  = 8;
const uint8_t VK_SEL   = 16;
/** Custom context specific soft buttons */
const uint8_t VK_SOFTA = 32;
const uint8_t VK_SOFTB = 64;

  /* vIEW METHODS
  virtual bool onKeyDown(uint8_t vk);
  virtual bool onKeyAutoRepeat(uint8_t vk);
  virtual bool onLongKeyDown(uint8_t vk);
  virtual bool onKeyUp(uint8_t vk);
  virtual bool onKeyInactive();
  */

  // void onC() {Serial.println();Serial.println("PRESSED C"); Serial.println();};            // C pressed
  void onZ_Up() {Serial.println();Serial.println("PRESSED Z"); Serial.println(); View::g_pActiveView->onKeyUp(VK_SEL);};            // Z Unpressed
  void onZ_Down() {Serial.println();Serial.println("PRESSED Z"); Serial.println(); View::g_pActiveView->onKeyDown(VK_SEL);};          // Z pressed
  // void onLongC() {Serial.println();Serial.println("PRESSED Long C"); Serial.println();};       // C Long pressed
  void onLongZ() {Serial.println();Serial.println("PRESSED Long Z"); Serial.println(); View::g_pActiveView->onLongKeyDown(VK_SEL);};       // Z Long pressed SEL Long
   
  
  // void onLeft() {Serial.println();Serial.println("Left"); Serial.println(); };             // Left pressed
  // void onRight() {Serial.println();Serial.println("Right"); Serial.println();};           // Right pressed
  // void onUp() {Serial.println();Serial.println("Up"); Serial.println();  };                 // Up pressed
  // void onDown() {Serial.println();Serial.println("Down"); Serial.println(); };             // Down pressed
   
  // momentary    
  void onMLeft(int v) {
    //Serial.println();Serial.print("MLeft Start  "); Serial.println(v);
    View::g_pActiveView->onJoy(VK_LEFT,v); 
  };      // Left pressed
  void onMRight(int v) {Serial.println();Serial.print("MRight Start  "); Serial.println(v);View::g_pActiveView->onJoy(VK_RIGHT,v); };         // Right pressed
  void onMUp(int v) {Serial.println();Serial.print("MUp Start  "); Serial.println(v);View::g_pActiveView->onJoy(VK_UP,v); };            // Up pressed
  void onMDown(int v) {Serial.println();Serial.print("MDown Start  "); Serial.println(v);View::g_pActiveView->onJoy(VK_DOWN,v); };          // Down pressed
  
  
  void onMLeftStart() {Serial.println();Serial.println("MLeft Start  "); View::g_pActiveView->onKeyDown(VK_LEFT); };             // Left pressed
  void onMRightStart() {Serial.println();Serial.println("MRight Start"); View::g_pActiveView->onKeyDown(VK_RIGHT); };          // Right pressed
  void onMUpStart() {Serial.println();Serial.println("MUp Start"); View::g_pActiveView->onKeyDown(VK_UP); };                  // Up pressed
  void onMDownStart() {Serial.println();Serial.println("MDown Start"); View::g_pActiveView->onKeyDown(VK_DOWN);};              // Down pressed

  void onMLeftStop() {Serial.println();Serial.println("MLeft Stop"); Serial.println(); View::g_pActiveView->onKeyUp(VK_LEFT);};            // Left unpressed
  void onMRightStop() {Serial.println();Serial.println("MRight Stop"); Serial.println(); View::g_pActiveView->onKeyUp(VK_RIGHT);};         // Right pressed
  void onMUpStop() {Serial.println();Serial.println("MUp Stop"); Serial.println();  View::g_pActiveView->onKeyUp(VK_UP);};                 // Up pressed
  void onMDownStop() {Serial.println();Serial.println("MDown Stop"); Serial.println(); View::g_pActiveView->onKeyUp(VK_DOWN);};                 // Down pressed
   
  void onRepLeft() {
    //Serial.println();Serial.println("Left Repeat"); Serial.println(); 
    View::g_pActiveView->onKeyAutoRepeat(VK_LEFT);
  };     // Left Repeat pressed
  void onRepRight()  {Serial.println();Serial.println("Right Repeat"); Serial.println(); View::g_pActiveView->onKeyAutoRepeat(VK_RIGHT);};   // Right Repeat pressed
  void onRepUp() {Serial.println();Serial.println("Up Repeat"); Serial.println(); View::g_pActiveView->onKeyAutoRepeat(VK_UP);};       // Up Repeat pressed
  void onRepDown() {Serial.println();Serial.println("Down Repeat"); Serial.println(); View::g_pActiveView->onKeyAutoRepeat(VK_DOWN);};     // Down Repeat pressed
  
  void onCLeft() {Serial.println();Serial.println("CLeft"); Serial.println(); View::g_pActiveView->onKeyUp(VK_SOFTA);};             // Left pressed
  void onCRight() {Serial.println();Serial.println("CRight"); Serial.println();View::g_pActiveView->onKeyUp(VK_SOFTB);};           // Right pressed
  void onCUp() {Serial.println();Serial.println("CUp"); Serial.println(); View::g_pActiveView->onKeyUp(VK_SOFTA);};                 // Up pressed
  void onCDown() {Serial.println();Serial.println("CDown"); Serial.println(); View::g_pActiveView->onKeyUp(VK_SOFTB);};             // Down pressed

  // void onZLeft() {Serial.println();Serial.println("ZLeft"); Serial.println();};             // Left pressed
  // void onZRight() {Serial.println();Serial.println("ZRight"); Serial.println();};           // Right pressed
  // void onZUp() {Serial.println();Serial.println("ZUp"); Serial.println(); };                 // Up pressed
  // void onZDown() {Serial.println();Serial.println("ZDown"); Serial.println();};             // Down pressed

  void onCLongLeft() {Serial.println();Serial.println("Long CLeft"); Serial.println(); View::g_pActiveView->onLongKeyDown(VK_SOFTA);};             // Left pressed
  void onCLongRight() {Serial.println();Serial.println("Long CRight"); Serial.println(); View::g_pActiveView->onLongKeyDown(VK_SOFTB);};           // Right pressed
  void onCLongUp() {Serial.println();Serial.println("Long CUp"); Serial.println(); View::g_pActiveView->onLongKeyDown(VK_SOFTA);};                 // Up pressed
  void onCLongDown() {Serial.println();Serial.println("Long CDown"); Serial.println(); View::g_pActiveView->onLongKeyDown(VK_SOFTB);};             // Down pressed

  // void onZLongLeft() {Serial.println();Serial.println("Long ZLeft"); Serial.println();};             // Left pressed
  // void onZLongRight() {Serial.println();Serial.println("Long ZRight"); Serial.println();};           // Right pressed
  // void onZLongUp() {Serial.println();Serial.println("Long ZUp"); Serial.println();};                 // Up pressed
  // void onZLongDown() {Serial.println();Serial.println("Long ZDown"); Serial.println();};             // Down pressed

  // void onLongLeft() {Serial.println();Serial.println("Long Left"); Serial.println(); };    // Left Long pressed
  // void onLongRight()  {Serial.println();Serial.println("Long Right"); Serial.println();}; // Right long pressed

  // void onLongUp() {Serial.println();Serial.println("Long Up"); Serial.println();};        // Up Long pressed
  // void onLongDown() {Serial.println();Serial.println("Long Down"); Serial.println();};    // Down Long pressed 
};


 g_wiiJoy W;

 int i_loop=0;
 long l_loop=0;
 unsigned long s_now=0;

/**
   Globals: Main command interpreter
*/
CommandInterpreter g_ci(pinPanStep, pinPanDirection, pinPanEnable);  // pan pinStep, pinDirection, pinEnable

/**
   Globals: Serial Port object
*/
//MySerialCommand g_serialCommandInterpreter;

void setup()
{
  nunchuk_init();
  Serial.begin(115200);

  delay(1000);


    // Joystick calibration
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {
    joy_x=nunchuk_joy_x();
    joy_y=nunchuk_joy_y();
    Serial.print("JoyX:");
    Serial.print(joy_x);
    Serial.print("\t| ");
    Serial.print("JoyY:");
    Serial.print(joy_y);
    Serial.print("\t| ");
    
    nunchuk_calibrate_joy();
    Serial.print("CalX:");
    Serial.print(joy_zerox);
    Serial.print("\t| ");
    Serial.print("CalY:");
    Serial.print(joy_zeroy);
    Serial.print("\t| ");
  } ; 
  
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("Panner test!");

  g_batteryMonitor.update(millis());

  g_settings.restore();

  View::setup();
  View::activate(&g_controlView);

  //g_serialCommandInterpreter.begin();

  // Joystick calibration
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {}; delay(5);
  if (nunchuk_get_data()) {
    joy_x=nunchuk_joy_x();
    joy_y=nunchuk_joy_y();
    Serial.print("JoyX:");
    Serial.print(joy_x);
    Serial.print("\t| ");
    Serial.print("JoyY:");
    Serial.print(joy_y);
    Serial.print("\t| ");
    
    nunchuk_calibrate_joy();
    Serial.print("CalX:");
    Serial.print(joy_zerox);
    Serial.print("\t| ");
    Serial.print("CalY:");
    Serial.print(joy_zeroy);
    Serial.print("\t| ");
  } ; 
}

void loop()
{
  i_loop++;
  
  if (i_loop==102) i_loop=0;
  unsigned long now = millis();
  if (s_now+1000<now ) {
    s_now=now;
    
    DEBUG_PRINT("Loops:");
    DEBUG_PRINTDEC(l_loop);
    DEBUG_PRINTLN(";");
    l_loop=0;
  }     
  l_loop++;

 // Get wii nunchuck data
 
 delay(1);
 
  if (i_loop==100) {
  
  if (nunchuk_get_data() ) {
    // 100 is the distance from 0 on the joystick but safe to take 90 as max value
    varx = nunchuk_cjoy_x(); // nunchuk.analogX is the value of the x-axis
    vary = nunchuk_cjoy_y(); // nunchuk.analogY is the value of the y-axi
  }
  else {
    Serial.println("Cannot communicate to wiichuck"); 
  };
  
    //DEBUG_PRINT("dELAY:");
    //DEBUG_PRINTDEC(millis()-now);
    //DEBUG_PRINTLN(";");
  };
  
  bool bUpdateDisplay = false;
  if (i_loop==100 || i_loop==60 || i_loop==30) {
    W.loop(now, nunchuk_cbutton(),nunchuk_zbutton(),varx,vary);  
  };  
  

  
  if (View::g_pActiveView != 0)
    if (i_loop==100 || i_loop==60 || i_loop==30) bUpdateDisplay = View::g_pActiveView->loop(now);

  if ((i_loop==100 || i_loop==60 || i_loop==30) && g_keyPad.getAndDispatchKey(now))
  {
    bUpdateDisplay = true;
  }
  /*else if(g_serialCommandInterpreter.available())
    {
    DEBUG_PRINTLN("Read a command from serial line!");
    g_serialCommandInterpreter.readAndDispatch();
    bUpdateDisplay = true;
    } */
  else if (g_batteryMonitor.updateMaybe(now))
  {
    bUpdateDisplay = true;
  }
  //if(bUpdateDisplay) g_ci.updateDisplay(now);
  if (bUpdateDisplay && View::g_pActiveView != 0)
    if (i_loop==100 ) View::g_pActiveView->update(now);
}


