#include "PS2_Controller.h"

void initializePS2() {
  //
  // setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(GAMEPAD_CLOCK, GAMEPAD_COMMAND, GAMEPAD_ATTENTION, GAMEPAD_DATA, true, true);

  // Check for error
  if(error == 0) {
    Serial.println("Found Controller, configured successful");
  }
  else if(error == 1) {
    Serial.println("No controller found, check wiring or reset the Arduino");
  }
  else if(error == 2) {
    Serial.println("Controller found but not accepting commands");
  }
  else if(error == 3) {
    Serial.println("Controller refusing to enter Pressures mode, may not support it.");
  }
  //
  // Check for the type of controller
  type = ps2x.readType();
  switch(type) {
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
      Serial.println(type);
  }
}
