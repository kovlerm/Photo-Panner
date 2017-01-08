#ifndef CAMERA_H
#define CAMERA_H

class Camera
{  
public:
  Camera();
  
  void ShootOn();  // Run shooter for miliseconds pin2
  void ShootOff(); // Run shooter for miliseconds pin2
  void FocusOn();  // Enforce focusing            pin1 
  void FocusOff(); // Enforce focusing            pin1 
   
  bool isShooting();    // mode 
  
  void DUMP(const char *szText /*= 0*/) const;
  
private:
  bool b_shoot = false; // shooting mode -T otherwise false
  bool b_focus = false; // shooting mode -T otherwise false
  
  int CameraPin   =  2;      
  int FocusPin   =  1;
  
};


#endif

