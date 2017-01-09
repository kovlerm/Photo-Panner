#ifndef CAMERA_H
#define CAMERA_H

class Camera
{  

public:
  Camera(); // copy from permanent settings
  Camera(uint16_t,uint16_t,uint16_t); // Focus (mm), Mount 0-Portrait/1-Landscape,  Exposure in sec
  
  void ShootOn();  // Run shooter for miliseconds pin2
  void ShootOff(); // Run shooter for miliseconds pin2
  void FocusOn();  // Enforce focusing            pin1 
  void FocusOff(); // Enforce focusing            pin1 
   
  bool isShooting();    // mode 
  
  /** Lens Focus Distance mm */
  uint16_t uFocus;
  /** Camera mounted in position 0-Portrait, 1-Landscape (Will get from sensor later) */
  uint16_t uMount;  
  /** Exposure in msec*/
  unsigned long ulExp;  
  /** Number of shots for bracketing */
  uint16_t uBrk;  
  
#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif

  
private:
  bool b_shoot = false; // shooting mode -T otherwise false
  bool b_focus = false; // shooting mode -T otherwise false
  
  int CameraPin   =  2;      
  int FocusPin   =  1;
  
};


#endif

