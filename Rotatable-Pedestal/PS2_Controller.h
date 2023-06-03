// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#include <PS2X_lib.h>

#ifndef __PS2_CONTROLLER_H__
#define __PS2_CONTROLLER_H__

#define DELTA_X      5
#define DELTA_Y      5

#define GAMEPAD_ATTENTION   10
#define GAMEPAD_COMMAND     11
#define GAMEPAD_DATA        12
#define GAMEPAD_CLOCK       13

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int errorCode = 0;
bool errorDisplayed = false;
byte ps2Type = 0;
byte vibrate = 0;

void initializePS2() {
  //
  // setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  errorCode = ps2x.config_gamepad(GAMEPAD_CLOCK, GAMEPAD_COMMAND, GAMEPAD_ATTENTION, GAMEPAD_DATA, false, false);

  // Check for error
  if(errorCode == 0) {
    Serial.println("Found Controller, configured successful");
  }
  else if(errorCode == 1) {
    Serial.println("No controller found, check wiring or reset the Arduino");
  }
  else if(errorCode == 2) {
    Serial.println("Controller found but not accepting commands");
  }
  else if(errorCode == 3) {
    Serial.println("Controller refusing to enter Pressures mode, may not support it.");
  }
  //
  // Check for the type of controller
  ps2Type = ps2x.readType();
  switch(ps2Type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
    default:
      Serial.print("Invalid Controller type: ");
      Serial.println(ps2Type);
  }
}

#endif
