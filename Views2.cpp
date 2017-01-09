#include "Panner.h"
#include "BatteryMonitor.h"
//#include <EEPROM.h>
#include <font_LiberationSans.h>
#include <font_AwesomeF000.h>
#include <font_AwesomeF080.h>
#include <font_AwesomeF100.h>
#include <font_AwesomeF200.h>

/**
 * App-Specific Views (Top Level Windows) Implementation
 */

/**
 * Globals: views
 */
SettingsView g_settingsView;
MySettingsView g_mysettingsView;
ControlView g_controlView;
WaypointsView g_waypointsView;
PanoramaView g_panoramaView;
EditView g_editView;
RunView g_runView;
RunPView g_runPView;
PausedRunView g_pausedRunView;
AboutView g_aboutView;

/**
 * Globals: commands to run at startup
 */
static Command cmds[] = 
{
  {chPan,     cmdSetMaxSpeed, 20},
  {chPan,     cmdSetAcceleration, 10},
  {chControl, cmdControlBeginLoop, 0},
    {chPan,     cmdGoToWaypoint, 1},                 // go right
    {chControl, cmdControlWaitForCompletion,  500000},  // wait for the movement to be completed for 50 sec
    {chControl, cmdControlRest,  20000},  // rest for 10 sec
    {chPan,     cmdGoToWaypoint, 2},                // go left
    {chControl, cmdControlWaitForCompletion,  500000},  // wait for the movement to be completed for 50 sec
    {chControl, cmdControlRest,  20000},  // rest for 10 sec
  {chControl, cmdControlEndLoop, 0},
  {chControl, cmdControlNone,    0}
};


/**
 * Globals: commands to run at startup
 */
static Command pan_cmds[200];

static const char szRest[] = "Rest";
static const char szWaitForCompletion[] = "Wait";
static const char szBeginLoop[] = "Begin Loop";
static const char szEndLoop[] = "End Loop";
static const char szGo[] = "Go";
static const char szGoTo[] = "GoTo";
static const char szSetMaxSpeed[] = "Max Speed";
static const char szSetAcceleration[] = "Acceleration";

static const char szPanSlowSpeed[] = "Pan Slow Speed";
static const char szPanMaxSpeed[] = "Pan Max Speed";
static const char szPanAcceleration[] = "Pan Acceleration";
static const char szBacklight[] = "Backlight";
static const char szExposure[] = "Exposure";
static const char szFocus[] = "Lens Focus";
static const char szFocusCorrection[] = "Focus Adjustment";
static const char szPos[] = "Position";
static const char szBrk[] = "Bracketing";

/**
 * 
 */
 
SettingsView::SettingsView() : 
  View("Settings", &AwesomeF000_16, "Z"/* i */,  &AwesomeF000_16, "[\x7D"/* up/down */, 0, szOK),
  //View("Settings", &AwesomeF000_16, "Z"/* i */,  &AwesomeF080_16, "\x5C", 0, szOK),
  m_resetConfirmation(szConfirmation, MB_OKCANCEL)
{
  m_settings.hasBorder(false);
  addChild(&m_settings);
  m_resetConfirmation.m_strMessage = "Reset all settings?";
}

/** Long press on central click pops up a Reset confirmation dialog */
bool SettingsView::onLongKeyDown(uint8_t vk)
{
  if(vk == VK_SEL) {
    DEBUG_PRINTLN("SettingsView::onKeyDown(VK_SEL): reset settings!");
    activate(&m_resetConfirmation);
    return true;
  }
  return false;
}

bool SettingsView::onKeyAutoRepeat(uint8_t vk)
{
  switch(vk) {
    case VK_UP: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_UP)");
      //m_settings.setCurValue(m_settings.getCurValue() + 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_DOWN)");
      //m_settings.setCurValue(m_settings.getCurValue() - 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      break;
    }
    default:
      return false;
  }  
  return true;
}

bool SettingsView::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_LEFT): jump to a prev editable field");
      m_settings.advanceSelection(-1);
      break;
    case VK_RIGHT:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_RIGHT or VK_SEL): jump to a next editable field");
      m_settings.advanceSelection(1);
      break;
    case VK_SEL:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_SEL):");
      // move focus to the next editable item
      if(m_settings.advanceSelection() == LB_ERR) {
        m_settings.setCurSel(0);
        m_settings.advanceSelection();
      }
      break;
      
    case VK_UP: {
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_UP)");
      //m_settings.setCurValue(m_settings.getCurValue() + 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      // immediate settings update!
      m_lcd.setBacklight(m_settings.getNumericValue(szBacklight));
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_DOWN)");
      //m_settings.setCurValue(m_settings.getCurValue() - 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      // immediate settings update!
      m_lcd.setBacklight(m_settings.getNumericValue(szBacklight));
      break;
    }
    case VK_SOFTA:
      // switch to About view
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_SOFTA): switch to my setting view");
      activate(&g_mysettingsView); 
      break;
    case VK_SOFTB: {
      // save settings!     
      g_settings.m_uPannerSlowSpeed = m_settings.getNumericValue(szPanSlowSpeed);
      g_settings.m_uPannerFastSpeed = 3 * g_settings.m_uPannerSlowSpeed;
      g_pPanner->setMaxSpeed(g_settings.m_uPannerMaxSpeed = m_settings.getNumericValue(szPanMaxSpeed));
      g_pPanner->setAcceleration(g_settings.m_uPannerAcceleration = m_settings.getNumericValue(szPanAcceleration));
      m_lcd.setBacklight(g_settings.m_uDisplayBacklight = m_settings.getNumericValue(szBacklight));
      g_settings.save();
      //    
      activate(((g_pPreviousView == &g_aboutView) || (g_pPreviousView == &m_resetConfirmation)) ? 
               &g_controlView : 
               g_pPreviousView);
      break;
    }
    default:
      return false;
  }  
  return true;
}

/** also handles m_resetConfirmation results */
void SettingsView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("SettingsView::onActivate()");
  if((pPrevActive == &m_resetConfirmation) && (m_resetConfirmation.getResult() == IDOK))
  {
    // the Reset Confirmation MessageBox was just closed.  Reset confirmed!
    g_settings.factoryReset();
    g_pPanner->setMaxSpeed(g_settings.m_uPannerMaxSpeed);
    g_pPanner->setAcceleration(g_settings.m_uPannerAcceleration);
  }
  // clear the listBox
  m_settings.clear();
  // fill m_settings
  {
    m_settings.push_back(" Direct Control");
    m_settings.push_back(szPanSlowSpeed, (long)g_settings.m_uPannerSlowSpeed);
    m_settings.push_back(" Runtime");
    m_settings.push_back(szPanMaxSpeed, (long)g_pPanner->getMaxSpeed());
    m_settings.push_back(szPanAcceleration, (long)g_pPanner->getAcceleration());
    m_settings.push_back(" Display");
    m_settings.push_back(szBacklight, (long)m_lcd.getBacklight(), m_lcd.getBacklightMin(), m_lcd.getBacklightMax());
    m_settings.setCurSel(1);
  }
  g_pPanner->enable(false);
}

MySettingsView::MySettingsView() : 
  View("My Settings", &AwesomeF000_16, "Z"/* i */,  &AwesomeF000_16, "[\x7D"/* up/down */, 0, szOK),
  //View("Settings", &AwesomeF000_16, "Z"/* i */,  &AwesomeF080_16, "\x5C", 0, szOK),
  m_resetConfirmation(szConfirmation, MB_OKCANCEL)
{
  m_settings.hasBorder(false);
  addChild(&m_settings);
  m_resetConfirmation.m_strMessage = "Reset all settings?";
  lwFocus.m_items={"16","17","24","35","50","70","120","200","500"};
  lwFocus.setCurSel(0);
  lwPos.m_items={"Landscape","Portrait"};
  lwPos.setCurSel(0);
 }

/** Long press on central click pops up a Reset confirmation dialog */
bool MySettingsView::onLongKeyDown(uint8_t vk)
{
  if(vk == VK_SEL) {
    DEBUG_PRINTLN("SettingsView::onKeyDown(VK_SEL): reset settings!");
    activate(&m_resetConfirmation);
    return true;
  }
  return false;
}

bool MySettingsView::onKeyAutoRepeat(uint8_t vk)
{
  switch(vk) {
    case VK_UP: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_UP)");
      //m_settings.setCurValue(m_settings.getCurValue() + 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_DOWN)");
      //m_settings.setCurValue(m_settings.getCurValue() - 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      break;
    }
    default:
      return false;
  }  
  return true;
}

bool MySettingsView::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_LEFT): jump to a prev editable field");
      m_settings.advanceSelection(-1);
      break;
    case VK_RIGHT:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_RIGHT or VK_SEL): jump to a next editable field");
      m_settings.advanceSelection(1);
      break;
    case VK_SEL:
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_SEL):");
      // move focus to the next editable item
      if(m_settings.advanceSelection() == LB_ERR) {
        m_settings.setCurSel(0);
        m_settings.advanceSelection();
      }
      break;
      
    case VK_UP: {
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_UP)");
      //m_settings.setCurValue(m_settings.getCurValue() + 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      // immediate settings update!
      m_lcd.setBacklight(m_settings.getNumericValue(szBacklight));
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_DOWN)");
      //m_settings.setCurValue(m_settings.getCurValue() - 1);
      ListSpinnerWidget *p = m_settings.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      // immediate settings update!
      m_lcd.setBacklight(m_settings.getNumericValue(szBacklight));
      break;
    }
    case VK_SOFTA:
      // switch to About view
      DEBUG_PRINTLN("SettingsView::onKeyUp(VK_SOFTA): switch to About view");
      activate(&g_aboutView);
      break;
    case VK_SOFTB: {
      // save settings!  
      g_settings.m_uFocus = atoi(lwFocus.m_items[m_settings.getValue(szFocus)->getCurSel()].c_str()) ; 
      g_settings.m_FocusCorrection=m_settings.getNumericValue(szFocusCorrection);
      g_pCam->uFocus=g_settings.m_uFocus+g_settings.m_FocusCorrection;
      g_pCam->uMount=g_settings.m_uCameraMount = m_settings.getValue(szPos)->getCurSel();      
      g_settings.m_uExp = m_settings.getNumericValue(szExposure); 
      g_pCam->ulExp=g_settings.m_uExp*1000;
      g_settings.m_uPannerFastSpeed = 3 * g_settings.m_uPannerSlowSpeed;
      g_pPanner->setMaxSpeed(g_settings.m_uPannerMaxSpeed = m_settings.getNumericValue(szPanMaxSpeed));
      g_pPanner->setAcceleration(g_settings.m_uPannerAcceleration = m_settings.getNumericValue(szPanAcceleration));
      m_lcd.setBacklight(g_settings.m_uDisplayBacklight = m_settings.getNumericValue(szBacklight));
      g_settings.save();
      //    
      activate(((g_pPreviousView == &g_aboutView) || (g_pPreviousView == &m_resetConfirmation)) ? 
               &g_controlView : 
               g_pPreviousView);
      break;
    }
    default:
      return false;
  }  
  return true;
}

/** also handles m_resetConfirmation results */
void MySettingsView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("MySettingsView::onActivate()");
  if((pPrevActive == &m_resetConfirmation) && (m_resetConfirmation.getResult() == IDOK))
  {
    // the Reset Confirmation MessageBox was just closed.  Reset confirmed!
    g_settings.factoryReset();
    g_pPanner->setMaxSpeed(g_settings.m_uPannerMaxSpeed);
    g_pPanner->setAcceleration(g_settings.m_uPannerAcceleration);
  }
  // clear the listBox
  m_settings.clear();
  // fill m_settings
  {
    m_settings.push_back(" Camera Settings");
    
    for( unsigned int a = 0; a < sizeof(lwFocus.m_items); a = a + 1 )
    {
       if (atoi(lwFocus.m_items[a].c_str())==g_settings.m_uFocus) {lwFocus.setCurSel(a);break;}
    };
    m_settings.push_back(szFocus, lwFocus);
    m_settings.push_back(szFocusCorrection, (long)g_settings.m_FocusCorrection); 
    
    lwPos.setCurSel(g_settings.m_uCameraMount);
    m_settings.push_back(szPos, lwPos);
    
    m_settings.push_back(" ");
    m_settings.push_back(szExposure, (long)g_settings.m_uExp); 
    m_settings.push_back(" Direct Control");
    m_settings.push_back(szPanSlowSpeed, (long)g_settings.m_uPannerSlowSpeed);
    m_settings.push_back(" Runtime");
    m_settings.push_back(szPanMaxSpeed, (long)g_pPanner->getMaxSpeed());
    m_settings.push_back(szPanAcceleration, (long)g_pPanner->getAcceleration());
    m_settings.push_back(" Display");
    m_settings.push_back(szBacklight, (long)m_lcd.getBacklight(), m_lcd.getBacklightMin(), m_lcd.getBacklightMax());
    m_settings.setCurSel(1);
  }
  g_pPanner->enable(false);
}



/**
 * Waypoint confirmation dialog
 */
WaypointDefinitionDialog::WaypointDefinitionDialog() : 
  ModalDialog("Define WayPoint", MB_OKCANCEL),
  m_wpoints(smSingleSelection)
{
  addChild(&m_wpoints);
  // set font and text for navigation label
  m_fontNav = &AwesomeF000_16;
  m_szNavLabel = "\x7D";
}

/** 
 * fill m_wpoints from  std::map<std::string, long> g_pPanner->m_wayPoints; 
 * szName is the name of the selected waypoint if not NULL
 */
static int16_t fill(ListWidget &list, const std::map<std::string, long> &wayPoints, const char szName[4])
{
  char szWaypoint[5];
  if(szName == 0)
    szWaypoint[0] = '\0';
  else {
    memcpy(szWaypoint, szName, 4);
    szWaypoint[4] = 0;
  }
  // clear the listBox
  list.clear();
  int16_t i = 0;
  for(auto const& x : wayPoints)
  {
    DEBUG_PRINT("Key="); DEBUG_PRINT(x.first.c_str()); DEBUG_PRINT(" Val="); DEBUG_PRINTDEC(x.second); DEBUG_PRINTLN("");
    char s[80];
    sprintf(s, "%s: %li", x.first.c_str(), x.second);
    list.push_back(s);
    if(x.first.compare(szWaypoint) == 0)
      list.setCurSel(i);
    i++;
  }
  DEBUG_PRINT("fill()=>"); DEBUG_PRINTDEC(i); DEBUG_PRINTLN("");
  return i;
}

static bool newWaypointLabel(char szLabel[2], const std::map<std::string, long> &wayPoints)
{
  for(char ch = 'A'; ch < 'Z'; ch++)
  {
    szLabel[0] = ch;
    if(wayPoints.count(szLabel) == 0)
      return true;    
  }
  return false;
}

void WaypointDefinitionDialog::onActivate(View *pPrevActive)
{
  // support the default behaviour
  ModalDialog::onActivate(pPrevActive);
  DEBUG_PRINTLN("WaypointDefinitionDialog::onActivate()");

  // stretch the list box over the half of the client area
  RECT r = m_rectClient;
  int16_t w = r.width() / 4;
  r.top++;
  r.left += w;
  r.right -= w;
  r.bottom--;
  m_wpoints.setPosition(r); 

  // fill ListWidget m_wpoints from  std::map<std::string, long> g_pPanner->m_wayPoints;
  fill(m_wpoints, g_pPanner->m_wayPoints, 0);
  // add new label
  {
    char s[] = " ";
    newWaypointLabel(s, g_pPanner->m_wayPoints);
    m_wpoints.m_items.push_back(s);
  }
  // and select it by default
  m_wpoints.setCurSel(m_wpoints.m_items.size() - 1);
}

bool WaypointDefinitionDialog::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_DOWN: 
      m_wpoints.advanceSelection(1);
      break;
    case VK_UP: 
      m_wpoints.advanceSelection(-1);
      break;
    case VK_SEL:
      vk = VK_SOFTB;
      // fall through!
    default:
      return ModalDialog::onKeyUp(vk); // to handle OK/Cancel
  }
  return true;
}

/**
 *  Direct Control View Class Implementation
 */
ControlView::ControlView() :
  View("Direct Control", &AwesomeF000_16, "\x13",  /* settings icon */  &AwesomeF000_16, "SAT" /* S[T */, 0, "WPoints")
{
}

/** 
 * To be called from the main loop on the active view.  
 * Do nothing by default. Return TRUE to update display
 */
boolean ControlView::loop(unsigned long now)
{
  g_pPanner->runSpeed();
  return (g_pPanner->speed() != 0.0);
}

/** analog keyboard APIs where vk is one of VK_xxx */
bool ControlView::onKeyDown(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
      // start pan left
      DEBUG_PRINTLN("ControlView::onKeyDown(VK_LEFT): start pan left");
      g_pPanner->setSpeed((float)g_settings.m_uPannerSlowSpeed);
      break;
    case VK_RIGHT:
      // start pan right
      g_pPanner->setSpeed((float) - g_settings.m_uPannerSlowSpeed);
      DEBUG_PRINTLN("ControlView::onKeyDown(VK_RIGHT): start pan right");
      break;
    default:
      return false;
  }
  return true;
}

/** analog keyboard APIs where vk is one of VK_xxx */
bool ControlView::onJoy(uint8_t vk, int v)
{
  switch(vk) {
    case VK_LEFT:
      // start pan left
      DEBUG_PRINTLN("ControlView::onKeyDown(VK_LEFT): start pan left");
      g_pPanner->setSpeed((float)v*(float)g_settings.m_uPannerFastSpeed/100.0);
      break;
    case VK_RIGHT:
      // start pan right
      g_pPanner->setSpeed((float)v*(float)-g_settings.m_uPannerFastSpeed/100.0);
      DEBUG_PRINTLN("ControlView::onKeyDown(VK_RIGHT): start pan right");
      break;
    default:
      return false;
  }
  return true;
}

bool ControlView::onLongKeyDown(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
      // start fast pan left
      DEBUG_PRINTLN("ControlView::onLongKeyDown(VK_LEFT): start fast pan left");
      g_pPanner->setSpeed((float)g_settings.m_uPannerFastSpeed);
      break;
    case VK_RIGHT:
      // start fast pan right
      DEBUG_PRINTLN("ControlView::onLongKeyDown(VK_RIGHT): start fast pan right");
      g_pPanner->setSpeed((float) - g_settings.m_uPannerFastSpeed);
      break;
    default:
      return false;
  }
  return true;
}

bool ControlView::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
    case VK_RIGHT:
      // stop pan
      DEBUG_PRINTLN("ControlView::onKeyUp(VK_LEFT or VK_RIGHT): stop pan");
      g_pPanner->setSpeed(0);
      break;
    case VK_SOFTA:
      // switch to Settings view
      DEBUG_PRINTLN("ControlView::onKeyUp(VK_SOFTA): switch to Settings view");
      activate(&g_mysettingsView);
      break;
    case VK_SOFTB:
      // switch to WayPoints view
      DEBUG_PRINTLN("ControlView::onKeyUp(VK_SOFTB): switch to WayPoints view");
      activate(&g_waypointsView);
      break;
    case VK_SEL:
      DEBUG_PRINTLN("ControlView::onKeyUp(VK_SEL): define a waipoint in the dialog");
      activate(&m_WPontDlg);
      break;
    default:
      return false;
  }
  return true;
}

void ControlView::updateClient(unsigned long now)
{
  //DEBUG_PRINTLN("ControlView::updateClient()");
  updateClientRunOrPaused(now, false, m_strMessage.c_str());
}

void ControlView::onActivate(View *pPrevActive)
{
  View::onActivate(pPrevActive);
  
  DEBUG_PRINTLN("ControlView::onActivate()");
  if((pPrevActive == &m_WPontDlg) && (m_WPontDlg.getResult() == IDOK))
  {
    // the dialog was just closed, selection confirmed!
    int16_t iSel = m_WPontDlg.m_wpoints.getCurSel();
    if(iSel != LB_ERR) {
      //DEBUG_PRINT("Selected: "); DEBUG_PRINTLN(m_WPontDlg.m_wpoints.m_items[iSel].c_str());
      long cp = g_pPanner->currentPosition();
      //DEBUG_PRINT("CurrentPos: "); DEBUG_PRINTDEC(cp); DEBUG_PRINTLN("");
      std::string strSelectedText = m_WPontDlg.m_wpoints.m_items[iSel];
      std::string str = strSelectedText.substr(0, 1);      
      g_pPanner->m_wayPoints[str] = cp;
      
      m_strMessage = "Way point '" + str + "' defined!";
    }
  }
  else
  {
    m_strMessage = ""; // "Add wpoint by middle click";
  }
  g_pPanner->enable(true);
}

/**
 *  WaypointsView Class Implementation
 */
WaypointsView::WaypointsView() : 
  View("WayPoints", 0, "Control", &AwesomeF000_16, "w[x" /* "\x7D"*/ , 0, "Panoram"),
  //m_wpoints(smSingleSelection),
  m_deleteConfirmation(szConfirmation, MB_OKCANCEL)
{
  addChild(&m_wpoints);
}

/**
 * analog keyboard APIs where vk is one of VK_xxx 
 */
bool WaypointsView::onKeyDown(uint8_t vk)
{
  switch(vk) {
    case VK_SEL: {
      DEBUG_PRINTLN("WaypointsView::onKeyDown(VK_SEL): move the panner to this waypoint!");
      int16_t iSel = m_wpoints.getCurSel();
      if(LB_ERR != iSel) {
        std::string s = m_wpoints.m_items[iSel];
        std::string ss = s.substr(0, 1);
        g_pPanner->moveToWayPoint(ss.c_str());
      }
      break;
    }
    default:
      return false;
  }
  return true;
}

bool WaypointsView::onLongKeyDown(uint8_t vk)
{
  switch(vk) {
    case VK_SEL: {
      DEBUG_PRINTLN("WaypointsView::onLongKeyDown(VK_SEL): delete this waypoint?");
      int16_t iSel = m_wpoints.getCurSel();
      if(LB_ERR != iSel) {
        std::string s = m_wpoints.m_items[iSel];
        std::string ss = s.substr(0, 1);
        m_deleteConfirmation.m_strMessage = "Delete the waypoint '" + ss + "'?";
        activate(&m_deleteConfirmation);
      }
      break;
    }
    default:
      return false;
  }
  return true;
}

bool WaypointsView::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_RIGHT: 
    case VK_DOWN: 
      m_wpoints.advanceSelection(1);
      break;
    case VK_LEFT: 
    case VK_UP: 
      m_wpoints.advanceSelection(-1);
      break;

    case VK_SOFTA:
      DEBUG_PRINTLN("WaypointsView::onKeyUp(VK_SOFTA): switch to Control view");
      activate(&g_controlView);
      break;
    case VK_SOFTB:
      // switch to Edit view
      DEBUG_PRINTLN("WaypointsView::onKeyUp(VK_SOFTB): switch to Edit view");
      //activate(&g_editView);
      activate(&g_panoramaView);
      break;
    case VK_SEL:
      DEBUG_PRINTLN("WaypointsView::onKeyUp(VK_SEL)");
      // do nothing here!  
      break;
    default:
      return false;
  }
  return true;
}

void WaypointsView::updateClient(unsigned long now)
{
  View::updateClient(now);
  //DEBUG_PRINTLN("WaypointsView::updateClient()");
  uint16_t x = m_rectClient.width() / 4;             // position of the first ':'
  RECT rClip = m_rectClient;
  rClip.right -= rClip.width() / 3;
  m_lcd.setClipRect(rClip);
  // do show position and speed
  uint16_t y = m_rectClient.top + m_lcd.fontLineSpace();
  printKeyVal(x, y, "Pan", g_pPanner->currentPosition());
  y += m_lcd.fontLineSpace() + 2;
  printKeyVal(x, y, "Speed", (long)g_pPanner->speed());
  
  m_lcd.setClipRect(m_rectClient);
  //m_wpoints.draw();
}

void WaypointsView::onActivate(View *pPrevActive)
{
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("WaypointsView::onActivate()");

  // stretch the list box over the entire client area - 1/2 of it!
  RECT r = m_rectClient;
  int16_t w = r.width() / 3;
  r.left = r.right - w;
  r.top++;
  r.bottom--;
  m_wpoints.setPosition(r);
  

  if((pPrevActive == &m_deleteConfirmation) && (m_deleteConfirmation.getResult() == IDOK))
  {
    // the dialog was just closed, deletion confirmed!
    int16_t iSel = m_wpoints.getCurSel();
    if(iSel != LB_ERR) {      
      std::string strSelectedText = m_wpoints.m_items[iSel];
      std::string str = strSelectedText.substr(0, 1);      
      g_pPanner->m_wayPoints.erase(str);
    }
  }
  // fill ListWidget m_wpoints from  std::map<std::string, long> g_pPanner->m_wayPoints;
  fill(m_wpoints, g_pPanner->m_wayPoints, 0);

  g_pPanner->enable(true);
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean WaypointsView::loop(unsigned long now)
{
  g_pPanner->run();
  return (g_pPanner->speed() != 0.0);
}


/**
 *  WaypointsView Class Implementation
 */
PanoramaView::PanoramaView() : 
  View("PanoramaView", 0, "WPoints", &AwesomeF000_16, "w[x" /* "\x7D"*/ , 0, "Program"),
  //m_wpoints(smSingleSelection),
  m_deleteConfirmation(szConfirmation, MB_OKCANCEL)
{
  addChild(&m_wpoints);
  m_panorama.hasBorder(false);
  addChild(&m_panorama);
  lwPos.m_items={"L","P"};
  lwPos.setCurSel(0);
  lwBrk.m_items={"No","3","5","7"};
  lwBrk.setCurSel(0);
}

bool PanoramaView::onKeyAutoRepeat(uint8_t vk)
{
  switch(vk) {
    case VK_UP: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_UP)");
      //m_settings.setCurValue(m_settings.getCurValue() + 1);
      ListSpinnerWidget *p = m_panorama.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("SettingsView::onKeyAutoRepeat(VK_DOWN)");
      //m_settings.setCurValue(m_settings.getCurValue() - 1);
      ListSpinnerWidget *p = m_panorama.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      break;
    }
    default:
      return false;
  }  
  return true;
}

/**
 * analog keyboard APIs where vk is one of VK_xxx 
 */
bool PanoramaView::onKeyDown(uint8_t vk)
{
  switch(vk) {
    case VK_SEL: {
      DEBUG_PRINTLN("PanoramaView::onKeyDown(VK_SEL): move the panner to this waypoint!");
      int16_t iSel = m_wpoints.getCurSel();
      if(LB_ERR != iSel) {
        std::string s = m_wpoints.m_items[iSel];
        std::string ss = s.substr(0, 1);
        g_pPanner->moveToWayPoint(ss.c_str());
      }
      break;
    }
    default:
      return false;
  }
  return true;
}



bool PanoramaView::onKeyUp(uint8_t vk)
{
  switch(vk) {
    case VK_LEFT:
        m_panorama.advanceSelection(-1); 
        break;    
    case VK_RIGHT:
        m_panorama.advanceSelection(1); 
        break;      
    case VK_DOWN: {
        ListSpinnerWidget *p = m_panorama.getCurValue();
        if(p != 0) p->advanceSelection(-1);
        break;
    }    
    case VK_UP: {
        ListSpinnerWidget *p = m_panorama.getCurValue();
        if(p != 0) p->advanceSelection(1);
        break;
    }    
    case VK_SOFTA:
        DEBUG_PRINTLN("PanoramaView::onKeyUp(VK_SOFTA): switch to Control view");
        activate(&g_waypointsView);
        break;
    case VK_SOFTB:
        // switch to Edit view
        DEBUG_PRINTLN("PanoramaView::onKeyUp(VK_SOFTB): switch to Edit view");
        activate(&g_editView);
        break;
    case VK_SEL: {
      g_pCam->uFocus = m_panorama.getNumericValue(szFocus); 
      g_pCam->uMount= m_panorama.getValue(szPos)->getCurSel();      
      g_pCam->ulExp=m_panorama.getNumericValue(szExposure)*1000;  
      uint16_t u_cp=m_panorama.getValue(szBrk)->getCurSel();;
      g_pCam->uBrk= u_cp*2+1;       
      DEBUG_PRINTLN("Bracketing");
      DEBUG_PRINTDEC(g_pCam->uBrk);
      DEBUG_PRINTLN(" ");
      DEBUG_PRINTLN("u_cp");
      DEBUG_PRINTDEC(u_cp);
      DEBUG_PRINTLN(" ");       
    
      /**  Calculate angles     */
  
      // Camera view angle
      fMtrx=36.0;
      if (g_pCam->uMount) fMtrx=24.0;
      fAngCam=2.0*atan(fMtrx/(2*(g_pCam->uFocus)));
      
      fAngCam*=180/3.1415926535897;
      DEBUG_PRINTLN("Camera Angle");
      DEBUG_PRINTDEC(fAngCam);
      DEBUG_PRINTLN(" ");
      // Panorama angle
      fAngPan=0.36*abs(g_pPanner->m_wayPoints["A"]-g_pPanner->m_wayPoints["B"]);
      DEBUG_PRINTLN("Panorama Angle");
      DEBUG_PRINTDEC(fAngPan);
      DEBUG_PRINTLN(" ");
      // step angle
      iShots=(fAngCam+fAngPan)/((1.0-0.3)*fAngCam)+1;
      DEBUG_PRINTLN("Number of SHots");
      DEBUG_PRINTDEC(iShots);
      DEBUG_PRINTLN(" ");
      // step delta
      iStep=(g_pPanner->m_wayPoints["B"]-g_pPanner->m_wayPoints["A"])/iShots;
      DEBUG_PRINTLN("Number of steps");
      DEBUG_PRINTDEC(iStep);
      DEBUG_PRINTLN(" ");
      // Create Program 
      int step=0;
      //test A={.m_szParam={'A'}};   Test for Union needs to be removed
      pan_cmds[step] = {chPan,     cmdSetMaxSpeed, (unsigned long)g_pPanner->getMaxSpeed()};
      pan_cmds[step++] = {chPan,     cmdSetAcceleration, 100};
      pan_cmds[step++] = {chControl, cmdControlBeginLoop, 0};
      //pan_cmds[step++] = {chPan,     cmdGoToWaypoint, {.m_lPosition='A'}};                        // go to left corner
      pan_cmds[step] = {chPan,     cmdGoToWaypoint, 1}; pan_cmds[step].m_szParam[0]='A'; step++;    // go to left corner
      pan_cmds[step++] = {chControl, cmdControlWaitForCompletion,  50000};                         // wait for the movement to be completed for 50 sec
      pan_cmds[step++] = {chControl, cmdControlRest,  200};                                         // deep breath before shooting
      pan_cmds[step++] = {chPan, cmdShootOn,  0};                                                   // fire
      // loop for bracketing
      DEBUG_PRINTLN("Bracketing loop");
      for( int b = 0; b < g_pCam->uBrk; b = b + 1 ) {
          DEBUG_PRINTDEC(b+1);
          DEBUG_PRINT(" Exposure "); 
          if (g_pCam->ulExp == 0) { // exposure managed by camera < 1 sec
            pan_cmds[step++] = {chControl, cmdControlRest, 200};                                       // very quick exposure
            pan_cmds[step++] = {chPan, cmdShootOff, 0};
            pan_cmds[step++] = {chControl, cmdControlRest, 2000};                                      // wait 2 sec
          }
          else { 
            unsigned long uCExp;
            uint16_t myPowerOfTwo = (uint16_t) (1 << abs(b-u_cp));
            if (b<u_cp) uCExp=g_pCam->ulExp/myPowerOfTwo;
            else if (b==u_cp) uCExp=g_pCam->ulExp;  
            else uCExp=g_pCam->ulExp*myPowerOfTwo;     
            DEBUG_PRINTDEC(uCExp);
            DEBUG_PRINTLN(" ");             
            pan_cmds[step++] = {chControl, cmdControlRest, uCExp};                                // exposure
            pan_cmds[step++] = {chPan, cmdShootOff, 0};                                                   // gun down
          } 
      }      
      for( int a = 0; a < iShots; a = a + 1 ) {
        //pan_cmds[step++] = {chPan,     cmdGo, {.m_lPosition=(long)iStep}};                        //go to next point of shooting
        pan_cmds[step] = {chPan,     cmdGo, 0}; pan_cmds[step].m_lPosition=(long) iStep; step++;    //go to next point of shooting
        pan_cmds[step++] = {chControl, cmdControlWaitForCompletion,  50000};                        // wait for the movement to be completed for 5 sec
        pan_cmds[step++] = {chControl, cmdControlRest,  200};                                       // deep breath before shooting
        pan_cmds[step++] = {chPan, cmdShootOn,  0};                                                 // fire
        // loop for bracketing
        
      
        for( int b = 0; b < g_pCam->uBrk; b = b + 1 ) {
        
            if (g_pCam->ulExp == 0) { // exposure managed by camera < 1 sec
                pan_cmds[step++] = {chControl, cmdControlRest, 200};                                       // very quick exposure
                pan_cmds[step++] = {chPan, cmdShootOff, 0};
                pan_cmds[step++] = {chControl, cmdControlRest, 2000};                                      // wait 2 sec
            }
            else { 
                unsigned long uCExp;
                uint16_t myPowerOfTwo = (uint16_t) (1 << abs(b-u_cp));
                if (b<u_cp) uCExp=g_pCam->ulExp/myPowerOfTwo;
                else if (b==u_cp) uCExp=g_pCam->ulExp;  
                else uCExp=g_pCam->ulExp*myPowerOfTwo; 
                                
                pan_cmds[step++] = {chControl, cmdControlRest, uCExp};                                // exposure
                pan_cmds[step++] = {chPan, cmdShootOff, 0};                                                   // gun down
            } 
        }                                                    // gun down  
      };
      pan_cmds[step++] = {chControl, cmdControlRest,  1000}; 
      pan_cmds[step++] = {chControl, cmdControlEndLoop, 0};
      pan_cmds[step++] = {chControl, cmdControlNone,    0};
      DEBUG_PRINTLN("PanoramaView::onKeyUp(VK_SEL)");
      // Run Program 
      activate(&g_runPView);
      break;
    }  
    default:
      return false;
  }
  return true;
}

void PanoramaView::updateClient(unsigned long now)
{
  View::updateClient(now);
  //DEBUG_PRINTLN("WaypointsView::updateClient()");
  /* DEBUG_PRINTLN("Main Rectangle:");
  DEBUG_PRINT("   Left:");
  DEBUG_PRINTDEC(m_rectClient.left);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("   Right:");
  DEBUG_PRINTDEC(m_rectClient.right);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("   Top:");
  DEBUG_PRINTDEC(m_rectClient.top);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT(" Bottom:");
  DEBUG_PRINTDEC(m_rectClient.bottom);
  DEBUG_PRINTLN(" "); */
  
  RECT rClip = m_rectClient;
  rClip.left = rClip.right- m_rectClient.width() / 3;
  rClip.top =  rClip.top+ m_rectClient.height() / 2+3;
  m_lcd.setClipRect(rClip);
  
  // do show position and speed
  
  /* DEBUG_PRINTLN("Rectangle N:");
  DEBUG_PRINT("   Left:");
  DEBUG_PRINTDEC(rClip.left);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("   Right:");
  DEBUG_PRINTDEC(rClip.right);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("   Top:");
  DEBUG_PRINTDEC(rClip.top);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT(" Bottom:");
  DEBUG_PRINTDEC(rClip.bottom);
  DEBUG_PRINTLN(" "); */
  
  
  uint16_t x = rClip.right-rClip.width()/2;             // position of the first ':'
  uint16_t y = rClip.top + m_lcd.fontLineSpace();
  DEBUG_PRINT("   X:");
  DEBUG_PRINTDEC(x);
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("   Y:");
  DEBUG_PRINTDEC(y);
  DEBUG_PRINTLN(" ");
  printKeyVal(x, y, "Pan", g_pPanner->currentPosition());
  y += m_lcd.fontLineSpace() + 2;
  printKeyVal(x, y, "Spd", (long)g_pPanner->speed());
  
  m_lcd.setClipRect(m_rectClient);
  //m_wpoints.draw();
}

void PanoramaView::onActivate(View *pPrevActive)
{
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("PanoramaView::onActivate()");
  RECT r1 = m_rectClient;
  int16_t w = r1.width() * 0.4;
  r1.right = r1.right - w;
  
  r1.top++;
  r1.bottom--;
  m_panorama.setPosition(r1);
  // clear the listBox
  m_panorama.clear();
  // fill m_settings
  {
    m_panorama.push_back(" Camera Settings");
    m_panorama.push_back(szFocus, (long)(g_pCam->uFocus));
    lwPos.setCurSel(g_pCam->uMount);
    m_panorama.push_back(szPos, lwPos); 
    m_panorama.push_back(szExposure, (long)g_pCam->ulExp/1000); 
    m_panorama.push_back(szBrk, lwBrk);
    lwPos.setCurSel(g_pCam->uBrk/2);
    m_panorama.setCurSel(1);
  }
  

  // stretch the list box over the entire client area - 1/2 of it!
  RECT r2 = m_rectClient;
  w = r2.width() / 3;
  int16_t h = r2.height() / 2;
  r2.left = r2.right - w;
  r2.top++;
  r2.bottom = r2.bottom - h;
  r2.bottom--;
  m_wpoints.setPosition(r2);
  
  
  

  if((pPrevActive == &m_deleteConfirmation) && (m_deleteConfirmation.getResult() == IDOK))
  {
    // the dialog was just closed, deletion confirmed!
    int16_t iSel = m_wpoints.getCurSel();
    if(iSel != LB_ERR) {      
      std::string strSelectedText = m_wpoints.m_items[iSel];
      std::string str = strSelectedText.substr(0, 1);      
      g_pPanner->m_wayPoints.erase(str);
    }
  }
  // fill ListWidget m_wpoints from  std::map<std::string, long> g_pPanner->m_wayPoints;
  //fill(m_wpoints, g_pPanner->m_wayPoints, 0);
  
  m_wpoints.clear();
  char s[80];
  bool b_defnd=true;
  if (g_pPanner->m_wayPoints.find("A") != g_pPanner->m_wayPoints.end() ) {
    sprintf(s, "C1: %li", g_pPanner->m_wayPoints["A"]);
  }
  else {
    memcpy(s,"N/A",4);
    b_defnd=false;
  };  
  m_wpoints.push_back(s);
  if (g_pPanner->m_wayPoints.find("B") != g_pPanner->m_wayPoints.end() ) {    
    sprintf(s, "C2: %li", g_pPanner->m_wayPoints["B"]);
  }
  else {
    memcpy(s,"N/A",4);
    b_defnd=false;
  };   
  m_wpoints.push_back(s);   
  m_wpoints.setCurSel(0);
  if  (b_defnd) {
    g_pPanner->enable(true);
      // Move to Corner 1
      g_pPanner->setSpeed((float)g_settings.m_uPannerMaxSpeed);
      g_pPanner->moveToWayPoint("A");
  }    
  
  
  
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean PanoramaView::loop(unsigned long now)
{
  g_pPanner->run();
  return (g_pPanner->speed() != 0.0);
}


/**
 * EditView class implementation
 */
EditView::EditView() : 
  View("Program", 0, "WPoints", &AwesomeF000_16, "G" /* up/down */, &AwesomeF000_16, "K"/* Run */),
  m_steps()
{
  m_steps.hasBorder(false);
  addChild(&m_steps);
}

/*void EditView::onKeyDown(uint8_t vk)
{
  DEBUG_PRINT("EditView::onKeyDown ");
  DEBUG_PRINTDEC(vk);
  DEBUG_PRINTLN("");
}*/

bool EditView::onLongKeyDown(uint8_t vk)
{
  switch(vk)
  {
    case VK_SEL:
      DEBUG_PRINTLN("EditView::onLongKeyDown(VK_SEL): back to settings");
      activate(&g_settingsView);
      break;
    case VK_SOFTA:
      DEBUG_PRINTLN("EditView::onLongKeyDown(VK_SOFTA): back to control");
      activate(&g_controlView);
      break;
    default:
      //DEBUG_PRINT("EditView::onLongKeyDown "); DEBUG_PRINTDEC(vk);  DEBUG_PRINTLN("");
      return false;
  }
  return true;
}

bool EditView::onKeyAutoRepeat(uint8_t vk)
{
  switch(vk) {
    case VK_UP: {
      DEBUG_PRINTLN("EditView::onKeyAutoRepeat(VK_UP)");
      ListSpinnerWidget *p = m_steps.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      break;
    }
    case VK_DOWN: {
      DEBUG_PRINTLN("EditView::onKeyAutoRepeat(VK_DOWN)");
      ListSpinnerWidget *p = m_steps.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      break;
    }
    default:
      return false;
  }  
  return true;
}

bool EditView::onKeyUp(uint8_t vk)
{
  switch(vk)
  {
    case VK_UP: {
      // increase the editable field value
      DEBUG_PRINTLN("EditView::onKeyUp(VK_UP)");
      //m_steps.setCurValue(m_steps.getCurValue() + 1);      
      ListSpinnerWidget *p = m_steps.getCurValue();
      if(p != 0)
        p->advanceSelection(1);
      break;
    }
    case VK_DOWN: {
      // decrease the editable field value
      DEBUG_PRINTLN("EditView::onKeyUp(VK_DOWN)");
      //m_steps.setCurValue(m_steps.getCurValue() - 1);
      ListSpinnerWidget *p = m_steps.getCurValue();
      if(p != 0)
        p->advanceSelection(-1);
      break;
    } 
    case VK_LEFT:
      // decrease the editable field value
      DEBUG_PRINTLN("EditView::onKeyUp(VK_LEFT): jump to a prev editable field");
      m_steps.advanceSelection(-1);
      break;
    case VK_RIGHT:
      DEBUG_PRINTLN("EditView::onKeyUp(VK_RIGHT or VK_SEL): jump to a next editable field");
      m_steps.advanceSelection(1);
      break;
    case VK_SEL:
      DEBUG_PRINTLN("EditView::onKeyUp(VK_RIGHT or VK_SEL): jump to a next editable field");
      if(m_steps.advanceSelection(1) == LB_ERR)
        m_steps.setCurSel(0);
      break;
    case VK_SOFTA:
      DEBUG_PRINTLN("EditView::onKeyUp(VK_SOFTA): jump to the waypoints view");
      //activate(&g_waypointsView);
      activate(&g_panoramaView);
      break;      
    case VK_SOFTB:
      DEBUG_PRINTLN("EditView::onKeyUp(VK_SOFTB): switch to Run view");
      // save the program
      if(saveProgram(m_steps, cmds))
      {
        activate(&g_runView);
      }
      else
      {
        DEBUG_PRINTLN("EditView::onKeyUp(VK_SOFTB): Can't run this!");
        ;
      
      }
      break;      
    default:
      //DEBUG_PRINT("EditView::onKeyUp "); DEBUG_PRINTDEC(vk); DEBUG_PRINTLN("");
      return false;
  }
  return true;
}

/** 
 * go through commands and populate the steps widget 
 */
void EditView::populateWidget(const Command *pCmds, KeyValueListWidget &steps)
{
  DEBUG_PRINTLN("EditView::populateWidget");
  steps.clear();
  for(;pCmds != 0; pCmds++)
  {
    pCmds->DUMP("pCmds=");
    switch(pCmds->m_channel)
    {
      case chControl:
        switch(pCmds->m_command)
        {
          case cmdControlNone:
            // graceful completion!
            return;
          case cmdControlRest:
            steps.push_back(szRest, pCmds->m_uValue / 1000);
            break;
          case cmdControlWaitForCompletion:
            steps.push_back(szWaitForCompletion, pCmds->m_uValue / 1000);
            break;
          case cmdControlBeginLoop:
            steps.push_back(szBeginLoop);
            break;
          case cmdControlEndLoop:
            steps.push_back(szEndLoop);
            break;
          default:
            pCmds->DUMP("populateWidget() ABNORMAL EXIT in chControl!");
            return;
        }
        break;
      case chPan:
        switch(pCmds->m_command)
        {
          case cmdControlNone:
            // graceful completion!
            return;
          case cmdGo:
            steps.push_back(szGo, pCmds->m_lPosition);
            break;
          case cmdGoTo:
            steps.push_back(szGoTo, pCmds->m_lPosition);
            break;
          case cmdGoToWaypoint: {
            ListSpinnerWidget wayPoints;
            // fill ListWidget m_wpoints from  std::map<std::string, long> g_pPanner->m_wayPoints;
            fill(wayPoints, g_pPanner->m_wayPoints, pCmds->m_szParam);
            steps.push_back(szGoTo, wayPoints);
            break;
          }
          case cmdSetMaxSpeed:
            steps.push_back(szSetMaxSpeed, pCmds->m_uValue);
            break;
          case cmdSetAcceleration:
            steps.push_back(szSetAcceleration, pCmds->m_uValue);
            break;
          default:
            pCmds->DUMP("populateWidget() ABNORMAL EXIT in chPan!");
            return;
        }
        break;
      default:
        pCmds->DUMP("populateWidget() ABNORMAL EXIT in unknown channel!");
        return;
    }    
  }  
}

/** walk through the steps widget and save commands */
bool EditView::saveProgram(KeyValueListWidget &steps, Command cmds[])
{
  DEBUG_PRINTLN("EditView::saveProgram");
  for(size_t i = 0; i < m_steps.m_items.size(); i++)
  {
    cmds[i].DUMP("cmds[i] before");
    switch(cmds[i].m_channel)
    {
      case chControl:
        switch(cmds[i].m_command)
        {
          case cmdControlNone:
            // graceful completion!
            return true;
          case cmdControlRest:
          case cmdControlWaitForCompletion:
            cmds[i].m_uValue = 1000l * (unsigned long)steps.getNumericValue((int16_t)i);
            break;
          case cmdControlBeginLoop:
            //
            break;
          case cmdControlEndLoop:
            //
            break;
          default:
            cmds[i].DUMP("saveProgram() ABNORMAL EXIT in chControl - unknown command!");
            return false;
        }
        break;
      case chPan:
        switch(cmds[i].m_command)
        {
          case cmdControlNone:
            // graceful completion!
            return true;
          case cmdGo:
          case cmdGoTo:
            cmds[i].m_lPosition = (long)steps.getNumericValue((int16_t)i);
            break;
          case cmdGoToWaypoint: {
            ListSpinnerWidget *p = &steps.m_values[i];
            int16_t iSel = p->getCurSel();
            if((iSel < 0) || (iSel >= (int16_t)p->m_items.size()))
              return false;
            cmds[i].m_szParam[0] = p->m_items[iSel][0];
            cmds[i].m_szParam[1] = '\0';
            break;
          }
          case cmdSetMaxSpeed:
          case cmdSetAcceleration:
            cmds[i].m_lPosition = (unsigned long)steps.getNumericValue((int16_t)i);
            break;
          default:
            cmds[i].DUMP("populateWidget() ABNORMAL EXIT in chPan - unknown command!");
            return false;
        }
        break;
      default:
        cmds[i].DUMP("saveProgram() ABNORMAL EXIT - unknown channel!");
        return false;
    }    
    cmds[i].DUMP("cmds[i] after ");
  }
  return true;
}

void EditView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("EditView::onActivate()");

  // fill the m_steps from cmds
  populateWidget(cmds, m_steps);

  g_pPanner->enable(false);  
}

/**
 *  Run View Class Implementation
 */
RunView::RunView() : 
  View("Run", &AwesomeF000_16, "L"/* Pause */, 0, 0, &AwesomeF000_16, "M" /* Stop */)
{
}

bool RunView::onKeyUp(uint8_t vk)
{
  switch(vk)
  {
    case VK_SOFTA:
      DEBUG_PRINTLN("RunView::onKeyUp(VK_SOFTA): switch to Pause view");
      activate(&g_pausedRunView);
      break;
    case VK_SOFTB:
      DEBUG_PRINTLN("RunView::onKeyUp(VK_SOFTB): stop and back to edit");
      activate(&g_editView);
      break;
    default:
      return false;
  }
  return true;
}

/**
 *  display Interpreter status
 */
void RunView::updateClient(unsigned long now)
{
  DEBUG_PRINTLN("RunView::updateClient()");
  updateClientRunOrPaused(now, true);
  // if battery is low - pause the execution which will turn off motors
  //if(g_batteryMonitor.getGauge() < 5)
  //  activate(&g_pausedRunView);
}

void RunView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("RunView::onActivate()");
  
  g_pPanner->enable(true);
  // (re)start command interpreter
  if(g_ci.isPaused())
    g_ci.resumeRun();
  else
    g_ci.beginRun(cmds);
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean RunView::loop(unsigned long now)
{
  if(!g_ci.isRunning())
    ;
  else if(g_ci.continueRun(now))
    ;
  else
    g_ci.endRun();
  return needsUpdate(now);
}

/**
 *  Run View Class Implementation
 */
RunPView::RunPView() : 
  View("Run Panorama", &AwesomeF000_16, "L"/* Pause */, 0, 0, &AwesomeF000_16, "M" /* Stop */)
{
}

bool RunPView::onKeyUp(uint8_t vk)
{
  switch(vk)
  {
    case VK_SOFTA:
      DEBUG_PRINTLN("RunView::onKeyUp(VK_SOFTA): switch to Pause view");
      activate(&g_pausedRunView);
      break;
    case VK_SOFTB:
      DEBUG_PRINTLN("RunView::onKeyUp(VK_SOFTB): stop and back to Panorama");
      activate(&g_panoramaView);
      break;
    default:
      return false;
  }
  return true;
}

/**
 *  display Interpreter status
 */
void RunPView::updateClient(unsigned long now)
{
  DEBUG_PRINTLN("RunView::updateClient()");
  updateClientRunOrPaused(now, true);
  // if battery is low - pause the execution which will turn off motors
  //if(g_batteryMonitor.getGauge() < 5)
  //  activate(&g_pausedRunView);
}

void RunPView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("RunView::onActivate()");
  
  g_pPanner->enable(true);
  // (re)start command interpreter
  if(g_ci.isPaused())
    g_ci.resumeRun();
  else
    g_ci.beginRun(pan_cmds); 
}

/** 
 * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
 */
boolean RunPView::loop(unsigned long now)
{
  if(!g_ci.isRunning())
    ;
  else if(g_ci.continueRun(now))
    ;
  else
    g_ci.endRun();
  return needsUpdate(now);
}


/**
 *  Run View Class Implementation
 */
PausedRunView::PausedRunView() : 
  View("Paused", &AwesomeF000_16, "K"/* Run*/, 0, 0, &AwesomeF000_16, "M"/* Stop*/)
{
}

bool PausedRunView::onKeyUp(uint8_t vk)
{
  switch(vk)
  {
    case VK_SOFTA:
      DEBUG_PRINTLN("PausedRunView::onKeyUp(VK_SOFTA): switch to Run view");
      // resume program execution here!
      activate(&g_runView);
      break;
    case VK_SOFTB:
      DEBUG_PRINTLN("PausedRunView::onKeyUp(VK_SOFTB): stop and switch to Run view");
      // stop program execution here!
      activate(g_pPrevPreviousView);
      
      break;
    default:
      return false;
  }
  return true;
}


/**
 *  display Interpreter status
 */
void PausedRunView::updateClient(unsigned long now)
{
  DEBUG_PRINTLN("PausedRunView::updateClient()");
  updateClientRunOrPaused(now, true);
}

void PausedRunView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("PausedRunView::onActivate()");
  if(g_ci.isRunning())
    g_ci.pauseRun();
  g_pPanner->enable(false);
}

/**
 *  About View Class Implementation
 */
const char *aboutViewLines[] = {
  "Panner v0.5.0",
  "(c) 2015-2016 Alex Sokolsky",
  "",
  "Thank you to:",
  "Dean Blackketter for a fork of ILI9341_t3",
  "Paul Stoffregen for ILI9341_t3",
  "Paul Stoffregen for Teensy 3.x",
  "Mike McCauley for AccelStepper",
  "",
  "This software is licensed under the terms",
  "of the GNU Public License, V2."
};

AboutView::AboutView() : 
  View("About", 0, 0, &AwesomeF000_16, "\x7D" /* up/down */, 0, szOK),
  m_text(smNoSelection, &LiberationSans_12)
{
  m_text.hasBorder(false);
  addChild(&m_text);
  for(size_t i = 0; i <  (sizeof(aboutViewLines)/ sizeof(aboutViewLines[0])); i++)
    m_text.m_items.push_back(aboutViewLines[i]);
}

bool AboutView::onKeyUp(uint8_t vk)
{
  switch(vk)
  {
    case VK_DOWN:
      DEBUG_PRINTLN("AboutView::onKeyUp(VK_DOWN)");
      m_text.scroll(1);
      break;
    case VK_UP:
      DEBUG_PRINTLN("AboutView::onKeyUp(VK_UP)");
      m_text.scroll(-1);
      break;
    case VK_SOFTA:
    case VK_SOFTB:
      DEBUG_PRINTLN("AboutView::onKeyUp(VK_SOFTB): back to control view");
      activate(g_pPreviousView);
      break;      
    default:
      return false;
  }
  return true;
}

void AboutView::onActivate(View *pPrevActive)
{
  // support the default behaviour
  View::onActivate(pPrevActive);
  DEBUG_PRINTLN("AboutView::onActivate()");
  
  m_text.scroll(-100); // reset first displayed position
  g_pPanner->enable(false);  
}

