#include "PS2_Controller.h"

PS2Controller::PS2Controller() {
  errorCode = 0;
  errorDisplayed = false;
  ps2Type = 0;
  vibrate = 0;
}

void PS2Controller::begin() {
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
};

bool PS2Controller::hasError() {
  return errorCode != 0;
};

void PS2Controller::showError() {
  if (!errorDisplayed) {
    Serial.println("Error, terminated!");
    errorDisplayed = true;
  }
};

void PS2Controller::reload() {
  return begin();
};

void PS2Controller::onDPadButtonPressed(void (*function)(uint16_t)) {
  user_onDPadButtonPressed = function;
};

void PS2Controller::onLeftJoystickChanged(void (*function)(int, int)) {
  user_onLeftJoystickChanged = function;
};

int PS2Controller::loop() {
  if(hasError()) { //skip loop if no controller found
    showError();
    Serial.println("Reload ...");
    delay(1000);
    reload();
    return 0;
  } else { //DualShock Controller
    return check();
  }
}

int PS2Controller::check() {
  ps2x.read_gamepad(false, vibrate); // disable vibration of the controller
  //
  // Perform movements based on D-pad buttons
  //
  // MOVE FORWARD
  if(ps2x.Button(PSB_PAD_UP)) {
    Serial.println("PSB_PAD_UP is pushed");
    if (!user_onDPadButtonPressed) {
      return -1;
    }
    user_onDPadButtonPressed(PSB_PAD_UP);
    return 1;
  }
  // MOVE BACK
  if(ps2x.Button(PSB_PAD_DOWN)){
    Serial.println("PSB_PAD_DOWN is pushed");
    if (!user_onDPadButtonPressed) {
      return -1;
    }
    user_onDPadButtonPressed(PSB_PAD_DOWN);
    return 1;
  }
  // TURN LEFT
  if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.println("PSB_PAD_LEFT is pushed");
    if (!user_onDPadButtonPressed) {
      return -1;
    }
    user_onDPadButtonPressed(PSB_PAD_LEFT);
    return 1;
  }
  // TURN RIGHT
  if(ps2x.Button(PSB_PAD_RIGHT)){
    Serial.println("PSB_PAD_RIGHT is pushed");
    if (!user_onDPadButtonPressed) {
      return -1;
    }
    user_onDPadButtonPressed(PSB_PAD_RIGHT);
    return 1;
  }
  //
  //
  int nJoyLX = ps2x.Analog(PSS_LX); // read x-joystick
  int nJoyLY = ps2x.Analog(PSS_LY); // read y-joystick
  //
  nJoyLX = map(nJoyLX, 0, 255, -10, 10);
  nJoyLY = map(nJoyLY, 0, 255, 10, -10);
  //
  if (nJoyLX >= MIN_BOUND_X || nJoyLX <= -MIN_BOUND_X || nJoyLY >= MIN_BOUND_Y || nJoyLY <= -MIN_BOUND_Y)
  {
    if (!user_onLeftJoystickChanged) {
      Serial.println("Left Joystick: ");
      Serial.print("- nJoyLX: ");
      Serial.println(nJoyLX);
      Serial.print("- nJoyLY: ");
      Serial.println(nJoyLY);
    } else {
      user_onLeftJoystickChanged(nJoyLX, nJoyLY);
    }
  }
};
