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
  // setup pins and settings: 
  // GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  errorCode = ps2x.config_gamepad(GAMEPAD_CLOCK, GAMEPAD_COMMAND, GAMEPAD_ATTENTION, GAMEPAD_DATA, false, false);

  // Check for error
  if(errorCode == 0) {
#if __PS2INIT_LOG_ENABLED__
    debugLog("Controller", " ", "found, configured successful");
#endif
  }
  else if(errorCode == 1) {
#if __PS2INIT_LOG_ENABLED__
    debugLog("Controller", " ", "not found, check wiring or reset the Arduino");
#endif
  }
  else if(errorCode == 2) {
#if __PS2INIT_LOG_ENABLED__
    debugLog("Controller", " ", "found but not accepting commands");
#endif
  }
  else if(errorCode == 3) {
#if __PS2INIT_LOG_ENABLED__
    debugLog("Controller", " ", "refusing to enter Pressures mode."); // may not support it
#endif
  }
  //
  // Check for the type of controller
  ps2Type = ps2x.readType();
  switch(ps2Type) {
    case 0:
#if __PS2INIT_LOG_ENABLED__
      debugLog("Unknown", " ", "Controller", " ", "type");
#endif
      break;
    case 1:
#if __PS2INIT_LOG_ENABLED__
      debugLog("DualShock", " ", "Controller", " ", "Found");
#endif
      break;
    case 2:
#if __PS2INIT_LOG_ENABLED__
      debugLog("GuitarHero", " ", "Controller", " ", "Found");
#endif
      break;
    default:
#if __PS2INIT_LOG_ENABLED__
      debugLog("Invalid", " ", "Controller", " ", "type", ": ", ps2Type);
#endif
      NULL;
  }
};

bool PS2Controller::hasError() {
  return errorCode != 0;
};

void PS2Controller::showError() {
  if (!errorDisplayed) {
    debugLog("Error, terminated!");
    errorDisplayed = true;
  }
};

void PS2Controller::reload() {
  begin();
};

void PS2Controller::setOnStartButtonPressed(void (*function)()) {
  _onStartButtonPressed = function;
};

void PS2Controller::setOnSelectButtonPressed(void (*function)()) {
  _onSelectButtonPressed = function;
};

void PS2Controller::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void PS2Controller::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void PS2Controller::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void PS2Controller::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void PS2Controller::setOnnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
};

void PS2Controller::setOnRightJoystickChanged(void (*function)(int, int)) {
  _onRightJoystickChanged = function;
};

int PS2Controller::loop() {
  if(hasError()) { //skip loop if no controller found
    showError();
    debugLog("Reload ...");
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
  uint16_t selectButtonPressed = processSelectButtonPress();
  if (selectButtonPressed) {
    return selectButtonPressed;
  }
  //
  // Perform movements based on D-pad buttons
  //
  uint16_t buttonPressed = 0;
  buttonPressed |= processDPadUpButtonPress();
  buttonPressed |= processDPadRightButtonPress();
  buttonPressed |= processDPadDownButtonPress();
  buttonPressed |= processDPadLeftButtonPress();

  //
  if (buttonPressed > 0) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("buttonPressed"), Serial.print(": "), Serial.println(buttonPressed, HEX);
    }
#endif
    return buttonPressed;
  }
  //
  int lstatus = processJoystickChange(PSS_LX, PSS_LY, _onLeftJoystickChanged, 'L');
  //
  int rstatus = processJoystickChange(PSS_RX, PSS_RY, _onRightJoystickChanged, 'R');
};

int PS2Controller::processStartButtonPress() {
  uint16_t button = PSB_START;
  if (!_onStartButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "START", " is pushed");
    }
#endif
    _onStartButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processSelectButtonPress() {
  uint16_t button = PSB_SELECT;
  if (!_onSelectButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "SELECT", " is pushed");
    }
#endif
    _onSelectButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadUpButtonPress() {
  uint16_t button = PSB_PAD_UP;
  if (!_onDPadUpButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "PAD_", "UP", " is pushed");
    }
#endif
    _onDPadUpButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadRightButtonPress() {
  uint16_t button = PSB_PAD_RIGHT;
  if (!_onDPadRightButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "PAD_", "RIGHT", " is pushed");
    }
#endif
    _onDPadRightButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadDownButtonPress() {
  uint16_t button = PSB_PAD_DOWN;
  if (!_onDPadDownButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "PAD_", "DOWN", " is pushed");
    }
#endif
    _onDPadDownButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadLeftButtonPress() {
  uint16_t button = PSB_PAD_LEFT;
  if (!_onDPadLeftButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB_", "PAD_", "LEFT", " is pushed");
    }
#endif
    _onDPadLeftButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processJoystickChange(byte xKey, byte yKey, void (*onChange)(int, int), const char label) {
  int nJoyX = ps2x.Analog(xKey); // read x-joystick
  int nJoyY = ps2x.Analog(yKey); // read y-joystick
  //
  nJoyX = map(nJoyX, 0, 255, NUM_RANGE_X, -NUM_RANGE_X);
  nJoyY = map(nJoyY, 0, 255, NUM_RANGE_Y, -NUM_RANGE_Y);
  //
  if (nJoyX >= MIN_BOUND_X || nJoyX <= -MIN_BOUND_X || nJoyY >= MIN_BOUND_Y || nJoyY <= -MIN_BOUND_Y) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PS2", "Controller", "::", "process", "JoystickChange", "()", " - ", label, ": ");
      debugLog(" - ", "X", ": ", nJoyX);
      debugLog(" - ", "Y", ": ", nJoyY);
    }
#endif
    if (onChange) {
      onChange(nJoyX, nJoyY);
      return 1;
    } else {
#if __RUNNING_LOG_ENABLED__
      if (debugEnabled) {
        debugLog("PS2", "Controller", "::", "process", "JoystickChange", "()", " - ", label, ": ", "not registered");
      }
#endif
      return -1;
    }
  }
  return 0;
};
