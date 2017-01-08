/** EEPROM address where persistent settings are stored */
const int iEEaddress = 0;

/**
 * Persistent settings to be stored in EEPROM
 */
struct PersistentSettings
{ 
  /** signature */
  union {
  uint16_t m_uSignature;
  char m_signature[2];
  };
  /** Lens Focus Distance mm */
  uint16_t m_uFocus;
  /** Focus Correction for zoom lens */
  int16_t m_FocusCorrection;
  /** Camera mounted in position 0-Portrait, 1-Landscape (Will get from sensor later) */
  int16_t m_uCameraMount;  
  
  /** Exposure in sec*/
  uint16_t m_uExp;
  /** Panner slow speed in the direct control screen */
  uint16_t m_uPannerSlowSpeed;
  /** Panner speed in the direct control screen */
  uint16_t m_uPannerFastSpeed;
  /** Panner max speed */
  uint16_t m_uPannerMaxSpeed;
  /** Panner acceleration */
  uint16_t m_uPannerAcceleration;
  /** Dsiplay backlight in the range 0..10 */
  uint16_t m_uDisplayBacklight;

  /** reset settings to factory defaults */
  void factoryReset();
  /** restore g_settings from EEPROM */
  bool restore();
  /** save g_settings into EEPROM */
  bool save();

 
};

extern PersistentSettings g_settings;

