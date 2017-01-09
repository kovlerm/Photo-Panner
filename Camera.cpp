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
    uFocus=g_settings.m_uFocus+g_settings.m_FocusCorrection;
    uMount=g_settings.m_uCameraMount;
    ulExp=g_settings.m_uExp*1000000;
}

Camera::Camera(uint16_t fcs,uint16_t mnt,uint16_t exp) // Focus (mm), Mount 0-Portrait/1-Landscape,  Exposure in sec 
{
    CameraPin=pinCamera;
    FocusPin=pinFocus;
    pinMode (CameraPin, OUTPUT);
    pinMode (FocusPin, OUTPUT);
    digitalWrite(CameraPin, LOW);
    digitalWrite(FocusPin, LOW);
    uFocus=fcs;
    uMount=mnt;
    ulExp=exp*1000;
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




