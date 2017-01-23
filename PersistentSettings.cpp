#include "Panner.h"
#include <EEPROM.h>


PersistentSettings g_settings;

/** restore g_settings from EEPROM */
bool PersistentSettings::restore()
{
  //EEPROM.get(iEEaddress, this);
  EEPROM.get(iEEaddress, g_settings);
  bool bRes = (m_signature[0] == 'P') && (m_signature[1] == '0');
  DEBUG_PRINTLN("Signature");
      DEBUG_PRINTDEC(m_uSignature);
      DEBUG_PRINTLN(" ");
  if(!bRes)
    factoryReset();
  DEBUG_PRINTLN("Signature");
      DEBUG_PRINTDEC(m_uSignature);
      DEBUG_PRINTLN(" ");  
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
  m_uPannerSlowSpeed = 50;
  m_uPannerFastSpeed = 550;
  m_uPannerMaxSpeed = m_uPannerFastSpeed+100;
  m_uPannerAcceleration = 250;
  m_uDisplayBacklight = 9;
}

bool PersistentSettings::save()
{
  EEPROM.put(iEEaddress, g_settings);
  DEBUG_PRINTLN("Saved");
  return true;  
}


