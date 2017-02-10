#include "Panner.h"
#include "Selector.h"

SELECTOR::SELECTOR(){};

bool SELECTOR::loop(unsigned long int ulNow, unsigned int bC, unsigned int bZ, int JoyX, int JoyY) {
    bool b_event=false;
    
    if (!bZ && !bC) {
        // Momentary reading 
        if (JoyY>10) {
          onMUp(JoyY); b_event=true;
          if(!lastUp) onMUpStart();
          mUp=1; 
          if (mDown) onMDownStop(); 
          mDown=0;
        }
        else if (JoyY<-10) {
          onMDown(-JoyY); b_event=true;
          if(!lastDown) onMDownStart();
          if (mUp) onMUpStop(); 
          mUp=0; 
          mDown=1;
        }
        else {
          if (mUp) {onMUpStop(); b_event=true;};
          if (mDown) {onMDownStop();b_event=true;};
          mUp=0; 
          mDown=0;
        };
        
        if (JoyX>10) {
          onMRight(JoyX); b_event=true;
          if(!lastRight) onMRightStart();
          mRight=1; 
          if (mLeft) onMLeftStop();
          mLeft=0;
        }
        else if (JoyX<-10) {
          onMLeft(-JoyX); b_event=true;
          if(!lastLeft) onMLeftStart();
          if (mRight) onMRightStop();
          mRight=0; 
          mLeft=1;
        }
        else {
          if (mLeft) {onMLeftStop(); b_event=true;};
          if (mRight) {onMRightStop(); b_event=true;};
          mRight=0; 
          mLeft=0;
        };
    };

    // Logical events
    if (JoyY>40) {bUp=1; bDown=0;}
    else if (JoyY<-30) {bUp=0; bDown=1;}
    else {bUp=0; bDown=0;};
    
    if (JoyX>40) {bRight=1; bLeft=0;}
    else if (JoyX<-40) {bRight=0; bLeft=1;}
    else {bRight=0; bLeft=0;};

    if (JoyY>40) {bUp=1; bDown=0;}
    else if (JoyY<-40) {bUp=0; bDown=1;}
    else {bUp=0; bDown=0;};
    
    
    // Events  
    if (bC && bZ && (!lastC || !lastZ)) { //press down
      onCZ();
      onCZ_Down();
      b_event=true;
      //set timers
      tRCZ=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLCZ=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLCZ=0;
    }
    else if ((!bC || !bZ)  && lastC && lastZ) {lastC=false;  lastZ=false; onCZ_Up();b_event=true;} //unpressed

    
    else if (bC && !lastC) { //press down
      onC();
      onC_Down();
      b_event=true;
      //set timers
      tRC=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLC=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLC=0;
    }
    else if (!bC && lastC) {onC_Up();b_event=true;} //unpressed
     
    else if (bZ && !lastZ) { //press down
      onZ();
      onZ_Down();
      b_event=true;
      //set timers
      tRZ=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLZ=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLZ=0;
    }
    else if (!bZ && lastZ) {DEBUG_PRINTLN("onZ_Up()"); onZ_Up(); b_event=true;}; //unpressed
    
    
    if (bRight && !lastRight) {
      if (bC) onCRight();
      else if (bZ) onZRight();
      else onRight();
      //set timers
      tRRight=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLRight=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLRight=0;
    };
    if (bLeft && !lastLeft) {
      if (bC) onCLeft();
      else if (bZ) onZLeft();
      else onLeft();
      tRLeft=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLLeft=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLLeft=0; 
    };
    if (bUp && !lastUp) {
      if (bC) onCUp();
      else if (bZ) onZUp();
      else onUp(); 
      tRUp=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLUp=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLUp=0;      
    };
    if (bDown && !lastDown) {
     if (bC) onCDown();
      else if (bZ) onZDown();
      else onDown(); 
      tRDown=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLDown=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLDown=0;           
    };

    
//++++++++++++++++++++++++>>>>
    // Long Events Call 
    bLC=bLZ=bLCZ=bLLeft=bLRight=bLUp=bLDown=0;
    bRC=bRZ=bRCZ=bRLeft=bRRight=bRUp=bRDown=0;
    if (!fLC && bC && tLC-ulNow<0) { // long key timer event
      bLC=1;
      fLC=1;
    };
    if (bC && tRC<ulNow) {         // repeat timer event
      bRC=1;
      tRC=ulNow+s_iDebounceDelay;    // first time repeat timer
    };
   
    if (!fLZ && bZ && tLZ<ulNow) { // long key timer event
      bLZ=1;
      fLZ=1;
    };
    if (bZ && tRZ<ulNow) {         // repeat timer event
      bRZ=1;
      tRZ=ulNow+s_iDebounceDelay;    // first time repeat timer
    };
 
    if (!fLCZ && bC && bZ && tLCZ-ulNow<0) { // long key timer event
      bLCZ=1;
      fLCZ=1;
    };
    if (bC && bZ && tRCZ<ulNow) {         // repeat timer event
      bRCZ=1;
      tRCZ=ulNow+s_iDebounceDelay;    // first time repeat timer
    };

    if (!fLLeft && bLeft && tLLeft<ulNow) { // long key timer event
      bLLeft=1;
      fLLeft=1;
    };
    if (bLeft && tRLeft<ulNow) {         // repeat timer event
      bRLeft=1;
      tRLeft=ulNow+s_iDebounceDelay;    // first time repeat timer
    };

    if (!fLRight && bRight && tLRight<ulNow) { // long key timer event
      bLRight=1;
      fLRight=1;
    };
    if (bRight && tRRight<ulNow) {         // repeat timer event
      bRRight=1;
      tRRight=ulNow+s_iDebounceDelay;    // first time repeat timer
    }; 

    if (!fLDown && bDown && tLDown<ulNow) { // long key timer event
      bLDown=1;
      fLDown=1;
    };
    if (bDown && tRDown<ulNow) {         // repeat timer event
      bRDown=1;
      tRDown=ulNow+s_iDebounceDelay;    // first time repeat timer
    }; 

    if (!fLUp && bUp && tLUp<ulNow) { // long key timer event
      bLUp=1;
      fLUp=1;
    };
    if (bUp && tRUp<ulNow) {         // repeat timer event
      bRUp=1;
      tRUp=ulNow+s_iDebounceDelay;    // first time repeat timer
    };
    
    if (bC) {
      b_event=true; 
      if (bLLeft) onCLongLeft();
      else if (bLRight) onCLongRight();
      else if (bLUp) onCLongUp();
      else if (bLDown) onCLongDown();
      else if (bLC && !bLeft && !bRight && !bUp && !bDown ) onLongC();

      if (bRLeft) onCRepLeft();
      else if (bRRight) onCRepRight();
      else if (bRUp) onCRepUp();
      else if (bRDown) onCRepDown();
      else if (bRC && !bLeft && !bRight && !bUp && !bDown ) onRepC();
     }
    else if (bZ) {
      b_event=true;
      if (bLLeft) onZLongLeft();
      else if (bLRight) onZLongRight();
      else if (bLUp) onZLongUp();
      else if (bLDown) onZLongDown();
      else if (bLZ && !bLeft && !bRight && !bUp && !bDown ) onLongZ();

      if (bRLeft) onZRepLeft();
      else if (bRRight) onZRepRight();
      else if (bRUp) onZRepUp();
      else if (bRDown) onZRepDown();
      else if (bRZ && !bLeft && !bRight && !bUp && !bDown ) onRepZ();
    }
    else if (bLLeft) onLongLeft();
    else if (bLRight) onLongRight();
    else if (bLUp) onLongUp();
    else if (bLDown) onLongDown();
    else if (bRLeft) onRepLeft();
    else if (bRRight) onRepRight();
    else if (bRUp) onRepUp();
    else if (bRDown) onRepDown();


    lastC=bC;
    lastZ=bZ;  
    lastJoyX=JoyX;
    lastJoyY=JoyY;    

    
    lastLeft=bLeft;
    lastRight=bRight;
    lastUp=bUp;
    lastDown=bDown;
    
    return b_event;
  };
