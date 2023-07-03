#include "PS2_Controller.h"

PS2Controller::PS2Controller() {
  debugEnabled = true;
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

void PS2Controller::onStartButtonPressed(void (*function)()) {
  user_onStartButtonPressed = function;
};

void PS2Controller::onDPadButtonPressed(void (*function)(uint16_t)) {
  user_onDPadButtonPressed = function;
};

void PS2Controller::onLeftJoystickChanged(void (*function)(int, int)) {
  user_onLeftJoystickChanged = function;
};

void PS2Controller::onRightJoystickChanged(void (*function)(int, int)) {
  user_onRightJoystickChanged = function;
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
  //
  //
  uint16_t startButtonPressed = processStartButtonPress();
  if (startButtonPressed) {
    return startButtonPressed;
  }
  //
  // Perform movements based on D-pad buttons
  //
  uint16_t buttonPressed = 0;
  //
  // MOVE FORWARD
  buttonPressed |= processPadButtonPress(PSB_PAD_UP, "PSB_PAD_UP");
  // MOVE BACK
  buttonPressed |= processPadButtonPress(PSB_PAD_DOWN, "PSB_PAD_DOWN");
  // TURN LEFT
  buttonPressed |= processPadButtonPress(PSB_PAD_LEFT, "PSB_PAD_LEFT");
  // TURN RIGHT
  buttonPressed |= processPadButtonPress(PSB_PAD_RIGHT, "PSB_PAD_RIGHT");
  //
  if (buttonPressed > 0) {
#ifdef __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("buttonPressed flag: ");
      Serial.println(buttonPressed, HEX);
    }
#endif
    return buttonPressed;
  }
  //
  int lstatus = processJoystickButton(PSS_LX, PSS_LY, user_onLeftJoystickChanged, "Left Joystick");
  //
  int rstatus = processJoystickButton(PSS_RX, PSS_RY, user_onRightJoystickChanged, "Right Joystick");
};

int PS2Controller::processStartButtonPress() {
  uint16_t button = PSB_START;
  if (!user_onStartButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#ifdef __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_START");
      Serial.println(" is pushed");
    }
#endif
    user_onStartButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processPadButtonPress(uint16_t button, const char buttonLabel[]) {
  if (!user_onDPadButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#ifdef __RUNNING_LOG_ENABLED__
    if (debugEnabled && buttonLabel) {
      Serial.print(buttonLabel);
      Serial.println(" is pushed");
    }
#endif
    user_onDPadButtonPressed(button);
    return button;
  }
  return 0;
}

int PS2Controller::processJoystickButton(byte xKey, byte yKey, void (*onChange)(int, int), const char label[]) {
  int nJoyX = ps2x.Analog(xKey); // read x-joystick
  int nJoyY = ps2x.Analog(yKey); // read y-joystick
  //
  nJoyX = map(nJoyX, 0, 255, NUM_RANGE_X, -NUM_RANGE_X);
  nJoyY = map(nJoyY, 0, 255, NUM_RANGE_Y, -NUM_RANGE_Y);
  //
  if (nJoyX >= MIN_BOUND_X || nJoyX <= -MIN_BOUND_X || nJoyY >= MIN_BOUND_Y || nJoyY <= -MIN_BOUND_Y)
  {
#ifdef __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PS2Controller::");
      Serial.print("processJoystickButton() - ");
      Serial.print(label);
      Serial.println(": ");
      Serial.print("- X: ");
      Serial.println(nJoyX);
      Serial.print("- Y: ");
      Serial.println(nJoyY);
    }
#endif
    if (onChange) {
      onChange(nJoyX, nJoyY);
      return 1;
    } else {
#ifdef __RUNNING_LOG_ENABLED__
      if (debugEnabled) {
        Serial.print("PS2Controller::");
        Serial.print("processJoystickButton() - ");
        Serial.print(label);
        Serial.print(": ");
        Serial.println("event listener has not registered");
      }
#endif
      return -1;
    }
  }
  return 0;
};
