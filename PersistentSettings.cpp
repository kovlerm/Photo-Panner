#include "Panner.h"
#include <EEPROM.h>


PersistentSettings g_settings;

/** restore g_settings from EEPROM */
bool PersistentSettings::restore()
{
  EEPROM.get(iEEaddress, this);
  bool bRes = (m_signature[0] == 'P') && (m_signature[0] == '0');
  if(!bRes)
    factoryReset();
  return bRes;
}

void PersistentSettings::factoryReset()
{
  DEBUG_PRINTLN("factoryResetSettings()");
  m_signature[0] = 'P';
  m_signature[1] = '0'; 
  m_uFocus = 17;
  m_FocusCorrection=0;
  m_uCameraMount=0;   
  m_uExp = 1;
  m_uPannerSlowSpeed = 300;
  m_uPannerFastSpeed = 700;
  m_uPannerMaxSpeed = m_uPannerFastSpeed+10;
  m_uPannerAcceleration = 150;
  m_uDisplayBacklight = 9;
}

bool PersistentSettings::save()
{
  EEPROM.put(iEEaddress, g_settings);
  return true;  
}


