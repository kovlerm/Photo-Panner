/**
 * Project-specific Views and Dialogs
 */

/**
 * Settings view class
 */
class SettingsView : public View
{
  KeyValueListWidget m_settings;
  /** confirmation dialog */
  MessageBox m_resetConfirmation;
  
public:  
  SettingsView();
  
  /** analog keyboard APIs where vk is one of VK_xxx */
  //bool onKeyDown(uint8_t vk);
  bool onKeyAutoRepeat(uint8_t vk);
  /** Long press on central click pops up a Reset confirmation dialog */
  bool onLongKeyDown(uint8_t vk);
  bool onKeyUp(uint8_t vk);

  /** also handles WaypointDefinitionDialog results */
  void onActivate(View *pPrevActive);
};

class MySettingsView : public View
{
  KeyValueListWidget m_settings;
  
  
  /** confirmation dialog */
  MessageBox m_resetConfirmation;
  
public:  
  MySettingsView();
  
  /** analog keyboard APIs where vk is one of VK_xxx */
  //bool onKeyDown(uint8_t vk);
  bool onKeyAutoRepeat(uint8_t vk);
  /** Long press on central click pops up a Reset confirmation dialog */
  bool onLongKeyDown(uint8_t vk);
  bool onKeyUp(uint8_t vk);

  /** also handles WaypointDefinitionDialog results */
  void onActivate(View *pPrevActive);
 private:
  ListSpinnerWidget lwFocus;   // list of focuses
  ListSpinnerWidget lwPos;     // list of focuses 
};

 
class WaypointDefinitionDialog : public ModalDialog
{ 
public:  
  ListWidget m_wpoints;
  
  WaypointDefinitionDialog();
  /**
   * to scroll though waypoint list
   */
  bool onKeyUp(uint8_t vk);
  /**
   * to arrange and fill widgets
   */
  void onActivate(View *pPrevActive);
};

/**
 * Direct control view class
 * Use joystick to pan the camera
 * Drop waypoints by pressing middle joystick button - pops up WaypointDefinitionDialog
 */
class ControlView : public View
{
  /** confirmation dialog */
  WaypointDefinitionDialog m_WPontDlg;
  /** extra message to guide the user - displayed at the bottom */
  std::string m_strMessage;
  
public:  
  ControlView();
  
  /** analog keyboard APIs where vk is one of VK_xxx */
  /** start panning */
  bool onKeyDown(uint8_t vk);
  /** start faster panning */
  bool onLongKeyDown(uint8_t vk);
  /** Controlled panning */
  bool onJoy(uint8_t vk, int);
  /** stop panning */
  bool onKeyUp(uint8_t vk);

  /** repaint client area */
  void updateClient(unsigned long now);
  /** 
   * to be called from the main loop on the active view.  
   * Do nothing by default. Return TRUE to update display
   */
  boolean loop(unsigned long now);
  /** also handles WaypointDefinitionDialog results */
  void onActivate(View *pPrevActive);
};

/**
 * Waypoints view class
 * Show the waypoints defined in the Control View
 * Go to the selected waypoint.
 * Delete one by long press of middle key
 */
class WaypointsView : public View
{
  ListWidget m_wpoints;
  //ListSpinnerWidget m_wpoints;
  MessageBox m_deleteConfirmation;
  
public:  
  WaypointsView();
  
  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyDown(uint8_t vk);
  bool onLongKeyDown(uint8_t vk);
  bool onKeyUp(uint8_t vk);

  /**
   * redraw the view
   */
  void updateClient(unsigned long now);
  /**
   * to fill the widgets
   */
  void onActivate(View *pPrevActive);
  boolean loop(unsigned long now);
};

/**
 * Panorama view class
 * Show the 2 waypoints (corners) defined in the Control View
 * Go to the selected waypoint.
 * Redefined corner
 * Calculate all middle waypoints (based on zoom) make program based on exposure
 * 
 
 
 */
class PanoramaView : public View
{
  ListWidget m_wpoints;
  //ListSpinnerWidget m_wpoints;
  MessageBox m_deleteConfirmation;
   KeyValueListWidget m_panorama;
  
public:  
  PanoramaView();
  
  bool onKeyAutoRepeat(uint8_t vk);
  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyDown(uint8_t vk);
  //bool onLongKeyDown(uint8_t vk);
  bool onKeyUp(uint8_t vk);

  /**
   * redraw the view
   */
  void updateClient(unsigned long now);
  /**
   * to fill the widgets
   */
  void onActivate(View *pPrevActive);
  boolean loop(unsigned long now);
  
private:
  float fMtrx;
  float fAngCam;
  float fAngPan;
  int iShots;
  int iStep;
  ListSpinnerWidget lwPos;     // list of focuses 
  ListSpinnerWidget lwBrk;     // list of focuses 
};


class EditView : public View
{
  KeyValueListWidget m_steps;
  
public:  
  EditView();
  
  /** analog keyboard APIs where vk is one of VK_xxx */
  //bool onKeyDown(uint8_t vk);
  bool onKeyAutoRepeat(uint8_t vk);
  bool onKeyUp(uint8_t vk);
  bool onLongKeyDown(uint8_t vk);

  void onActivate(View *pPrevActive);
private:
  /** go through commands and populate the steps widget */
  void populateWidget(const Command *pCmds, KeyValueListWidget &steps);
  /** walk through the steps widget and save commands */
  bool saveProgram(KeyValueListWidget &steps, Command cmds[]);
};


class RunPView : public View
{
  
public:  
  RunPView();

  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyUp(uint8_t vk);

  void updateClient(unsigned long now);

  void onActivate(View *pPrevActive);
  /** 
   * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
   */
  boolean loop(unsigned long now);
};



class RunView : public View
{
  
public:  
  RunView();

  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyUp(uint8_t vk);

  void updateClient(unsigned long now);

  void onActivate(View *pPrevActive);
  /** 
   * to be called from the main loop on the active view.  Do nothing by default. Return TRUE to update display
   */
  boolean loop(unsigned long now);
};

class PausedRunView : public View
{
  
public:  
  PausedRunView();

  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyUp(uint8_t vk);

  void updateClient(unsigned long now);

  void onActivate(View *pPrevActive);
};

class AboutView : public View
{
  ListWidget m_text;
  
public:  
  AboutView();

  /** analog keyboard APIs where vk is one of VK_xxx */
  bool onKeyUp(uint8_t vk);
  void onActivate(View *pPrevActive);
};

extern SettingsView g_settingsView;
extern MySettingsView g_mysettingsView;
extern ControlView g_controlView;
extern WaypointsView g_waypointsView;
extern EditView g_editView;
extern RunView g_runView;
extern PausedRunView g_pausedRunView;
extern AboutView g_aboutView;

