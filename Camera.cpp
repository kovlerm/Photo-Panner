#include "Panner.h"
#include "Camera.h"
Camera::Camera() 
{
    CameraPin=pinCamera;
    FocusPin=pinFocus;
    pinMode (CameraPin, OUTPUT);
    pinMode (FocusPin, OUTPUT);
    digitalWrite(CameraPin, LOW);
    digitalWrite(FocusPin, LOW);
}


void Camera::ShootOn()
{
  b_shoot=true;
  digitalWrite(CameraPin, HIGH);
}

void Camera::ShootOff()
{
  b_shoot=false;
  digitalWrite(CameraPin, LOW);
}

void Camera::FocusOn()
{
  b_shoot=true;
  digitalWrite(FocusPin, HIGH);
}

void Camera::FocusOff()
{
  b_shoot=false;
  digitalWrite(FocusPin, LOW);
}

bool Camera::isShooting() // mode 
{
    return b_shoot;
};  


#ifdef DEBUG
void Camera::DUMP(const char *szText /*= 0*/) const
{
  if(szText != 0) {
    DEBUG_PRINT(szText);
  }
  DEBUG_PRINT(" b_shoot="); DEBUG_PRINTDEC((int)b_shoot);
  
  DEBUG_PRINT(" b_focus="); DEBUG_PRINTDEC((int)b_focus);   
  DEBUG_PRINTLN("");
}
#endif




