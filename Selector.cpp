#include "Selector.h"

SELECTOR::SELECTOR(){};

void SELECTOR::upd(unsigned long int ulNow, unsigned int bC, unsigned int bZ, int JoyX, int JoyY) {
    if (!bZ && !bC) {
        // Momentary reading 
        if (JoyY>40) {
          onMUp(JoyY);
          if(!lastUp) onMUpStart();
          mUp=1; 
          if (mDown) onMDownStop(); 
          mDown=0;
        }
        else if (JoyY<-40) {
          onMDown(-JoyY); 
          if(!lastDown) onMDownStart();
          if (mUp) onMUpStop(); 
          mUp=0; 
          mDown=1;
        }
        else {
          if (mUp) onMUpStop();
          if (mDown) onMDownStop();
          mUp=0; 
          mDown=0;
        };
        
        if (JoyX>40) {
          onMRight(JoyX); 
          if(!lastRight) onMRightStart();
          mRight=1; 
          if (mLeft) onMLeftStop();
          mLeft=0;
        }
        else if (JoyX<-40) {
          onMLeft(-JoyX); 
          if(!lastLeft) onMLeftStart();
          if (mRight) onMRightStop();
          mRight=0; 
          mLeft=1;
        }
        else {
          if (mLeft) onMLeftStop();
          if (mRight) onMRightStop();
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
    if (bC && !lastC) { //press down
      onC();
      onC_Down();
      //set timers
      tRC=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLC=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLC=0;
    }
    else if (!bC && lastC) onC_Up(); //unpressed
     
    if (bZ && !lastZ) { //press down
      onZ();
      onZ_Down();
      //set timers
      tRZ=ulNow+s_iAutoRepeatDelay; // first time repeat timer
      tLZ=ulNow+s_iLongKeyDelay;    // first time long key timer
      fLZ=0;
    }
    else if (!bZ && lastZ) onZ_Up(); //unpressed
    
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
    bLC=bLZ=bLLeft=bLRight=bLUp=bLDown=0;
    bRC=bRZ=bRLeft=bRRight=bRUp=bRDown=0;
    if (!bLC && bC && tLC-ulNow<0) { // long key timer event
      bLC=1;
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
    
  };
