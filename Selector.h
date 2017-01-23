#ifndef Selector_h
#define Selector_h

// Class SELECTOR call callbacks events
class SELECTOR
 {
public:  

// Callbacks

  virtual void onC() {};            // C pressed and unpressed 
  virtual void onC_Up() {};          // C unpressed
  virtual void onC_Down() {};        // C pressed
  virtual void onRepC() {};      // C pressed 
  virtual void onLongC() {};        // C Long pressed
  
  virtual void onZ() {};            // Z pressed
  virtual void onZ_Up() {};          // Z unpressed
  virtual void onZ_Down() {};        // Z pressed
  virtual void onRepZ() {};      // Z pressed 
  virtual void onLongZ() {};        // Z Long pressed
  
  virtual void onLeft() {};         // Left pressed
  virtual void onRight() {};        // Right pressed
  virtual void onUp() {};           // Up pressed
  virtual void onDown() {};         // Down pressed

  virtual void onCZ() {};            // C pressed and unpressed 
  virtual void onCZ_Up() {};          // C unpressed
  virtual void onCZ_Down() {};       // C pressed
  virtual void onRepCZ() {};         // C pressed 
  virtual void onLongCZ() {};        // C Long pressed

  virtual void onMLeft(int) {};         // Left pressed
  virtual void onMRight(int) {};        // Right pressed
  virtual void onMUp(int) {};           // Up pressed
  virtual void onMDown(int) {};         // Down pressed

  virtual void onMLeftStart() {};         // Left pressed
  virtual void onMRightStart() {};        // Right pressed
  virtual void onMUpStart() {};           // Up pressed
  virtual void onMDownStart() {};         // Down pressed
  
  virtual void onMLeftStop() {};         // Left unpressed
  virtual void onMRightStop() {};        // Right unpressed
  virtual void onMUpStop() {};           // Up unpressed
  virtual void onMDownStop() {};         // Down unpressed


  virtual void onLongLeft() {};     // Left Long pressed
  virtual void onLongRight()  {};   // Right long pressed
  virtual void onLongUp() {};       // Up Long pressed
  virtual void onLongDown() {};     // Down Long pressed

  virtual void onRepLeft() {};     // Left Repeat pressed
  virtual void onRepRight()  {};   // Right Repeat pressed
  virtual void onRepUp() {};       // Up Repeat pressed
  virtual void onRepDown() {};     // Down Repeat pressed
  
  virtual void onCLeft() {};
  virtual void onCRight() {};
  virtual void onCUp() {};
  virtual void onCDown() {};

  virtual void onCLongLeft() {};
  virtual void onCLongRight() {};
  virtual void onCLongUp() {};
  virtual void onCLongDown() {};

  virtual void onCRepLeft() {};
  virtual void onCRepRight() {};
  virtual void onCRepUp() {};
  virtual void onCRepDown() {};

  virtual void onZLeft() {};
  virtual void onZRight() {};
  virtual void onZUp() {};
  virtual void onZDown() {};

  virtual void onZLongLeft() {};
  virtual void onZLongRight() {};
  virtual void onZLongUp() {};
  virtual void onZLongDown() {};

  virtual void onZRepLeft() {};
  virtual void onZRepRight() {};
  virtual void onZRepUp() {};
  virtual void onZRepDown() {};

  // timer, C state (0-1), Z state (0-1), JoyX value (0-100), JoyY value (0-100) 
  bool loop(unsigned long int, unsigned int, unsigned int, int, int);
    
  SELECTOR();

  private:
  /** delay in ms to debounce */
  static const int s_iDebounceDelay = 200;
  /** delay in ms to autorepeat */
  static const int s_iAutoRepeatDelay = 500;
  /** delay in ms before the long key is fired */
  static const int s_iLongKeyDelay = 2000;
  
  unsigned int lastC=0;
  unsigned int lastZ=0;
  unsigned int lastJoyX=0;
  unsigned int lastJoyY=0;

 // Logical momentary events
  unsigned int mLeft=0;
  unsigned int mRight=0;
  unsigned int mUp=0;
  unsigned int mDown=0;

  // Logical events
  unsigned int bLeft=0;
  unsigned int bRight=0;
  unsigned int bUp=0;
  unsigned int bDown=0;

 // repeat timers
  unsigned long tRLeft=0;
  unsigned long tRRight=0;
  unsigned long tRUp=0;
  unsigned long tRDown=0;
  unsigned long tRC=0;
  unsigned long tRZ=0;
  unsigned long tRCZ=0;

// Long Event timers
  unsigned long tLLeft=0;
  unsigned long tLRight=0;
  unsigned long tLUp=0;
  unsigned long tLDown=0;
  unsigned long tLC=0;
  unsigned long tLZ=0;
  unsigned long tLCZ=0;  

  // Logical Long events
  unsigned int bLLeft=0;
  unsigned int bLRight=0;
  unsigned int bLUp=0;
  unsigned int bLDown=0;
  unsigned int bLC=0;
  unsigned int bLZ=0;
  unsigned int bLCZ=0;
  
  // flags
  unsigned int fLLeft=0;
  unsigned int fLRight=0;
  unsigned int fLUp=0;
  unsigned int fLDown=0;
  unsigned int fLC=0;
  unsigned int fLZ=0;
  unsigned int fLCZ=0;

  unsigned int bRLeft=0;
  unsigned int bRRight=0;
  unsigned int bRUp=0;
  unsigned int bRDown=0;
  unsigned int bRC=0;
  unsigned int bRZ=0;
  unsigned int bRCZ=0;
  
  unsigned int lastLeft=0;
  unsigned int lastRight=0;
  unsigned int lastUp=0;
  unsigned int lastDown=0;
 };
#endif 
