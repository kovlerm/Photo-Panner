/********************************************************\
 *                                                      *
 * Simple automated video panner                        *
 *                                                      *
 * Alex Sokolsky                                        *
 *                                                      *
 *   v0.1  6/23/2016                                    *
 *                                                      *
\********************************************************/
#include <vector>
#include <string>
#include <map>
#include <climits>

#include <ILI9341_t3.h>
#include <Wire.h>
#include <AccelStepper.h>


#include "Selector.h" 

/** CHAR is signed on AVR and unsigned on ARM!  WTF! */
typedef signed char schar_t;

const uint8_t pinFocus = 1;
const uint8_t pinCamera = 2;

//#define NODEBUG 1
#include "Trace.h"
//#include "ThumbStick.h"
#include "Keypad.h"
#include "Stepper.h"
#include "Camera.h"
#include "CommandInterpreter.h"
#include "PersistentSettings.h"
#include "Views.h"
#include "Views2.h"
//#include "SerialCommand.h"



