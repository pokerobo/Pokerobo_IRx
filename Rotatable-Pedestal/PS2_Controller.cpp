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
#if __PS2INIT_LOG_ENABLED__
    Serial.println("Found Controller, configured successful");
#endif
  }
  else if(errorCode == 1) {
#if __PS2INIT_LOG_ENABLED__
    Serial.println("No controller found, check wiring or reset the Arduino");
#endif
  }
  else if(errorCode == 2) {
#if __PS2INIT_LOG_ENABLED__
    Serial.println("Controller found but not accepting commands");
#endif
  }
  else if(errorCode == 3) {
#if __PS2INIT_LOG_ENABLED__
    Serial.println("Controller refusing to enter Pressures mode, may not support it.");
#endif
  }
  //
  // Check for the type of controller
  ps2Type = ps2x.readType();
  switch(ps2Type) {
    case 0:
#if __PS2INIT_LOG_ENABLED__
      Serial.println("Unknown Controller type");
#endif
      break;
    case 1:
#if __PS2INIT_LOG_ENABLED__
      Serial.println("DualShock Controller Found");
#endif
      break;
    case 2:
#if __PS2INIT_LOG_ENABLED__
      Serial.println("GuitarHero Controller Found");
#endif
      break;
    default:
#if __PS2INIT_LOG_ENABLED__
      Serial.print("Invalid Controller type"), Serial.print(": "), Serial.println(ps2Type);
#endif
      NULL;
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
  begin();
};

void PS2Controller::onStartButtonPressed(void (*function)()) {
  user_onStartButtonPressed = function;
};

void PS2Controller::onSelectButtonPressed(void (*function)()) {
  user_onSelectButtonPressed = function;
};

void PS2Controller::onDPadUpButtonPressed(void (*function)()) {
  user_onDPadUpButtonPressed = function;
};

void PS2Controller::onDPadRightButtonPressed(void (*function)()) {
  user_onDPadRightButtonPressed = function;
};

void PS2Controller::onDPadDownButtonPressed(void (*function)()) {
  user_onDPadDownButtonPressed = function;
};

void PS2Controller::onDPadLeftButtonPressed(void (*function)()) {
  user_onDPadLeftButtonPressed = function;
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
      Serial.print("buttonPressed"), Serial.print(": ");
      Serial.println(buttonPressed, HEX);
    }
#endif
    return buttonPressed;
  }
  //
  int lstatus = processJoystickChange(PSS_LX, PSS_LY, user_onLeftJoystickChanged, 'L');
  //
  int rstatus = processJoystickChange(PSS_RX, PSS_RY, user_onRightJoystickChanged, 'R');
};

int PS2Controller::processStartButtonPress() {
  uint16_t button = PSB_START;
  if (!user_onStartButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("START");
      Serial.println(" is pushed");
    }
#endif
    user_onStartButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processSelectButtonPress() {
  uint16_t button = PSB_SELECT;
  if (!user_onSelectButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("SELECT");
      Serial.println(" is pushed");
    }
#endif
    user_onSelectButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadUpButtonPress() {
  uint16_t button = PSB_PAD_UP;
  if (!user_onDPadUpButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("PAD_"), Serial.print("UP");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadUpButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadRightButtonPress() {
  uint16_t button = PSB_PAD_RIGHT;
  if (!user_onDPadRightButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("PAD_"), Serial.print("RIGHT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadRightButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadDownButtonPress() {
  uint16_t button = PSB_PAD_DOWN;
  if (!user_onDPadDownButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("PAD_"), Serial.print("DOWN");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadDownButtonPressed();
    return button;
  }
  return 0;
}

int PS2Controller::processDPadLeftButtonPress() {
  uint16_t button = PSB_PAD_LEFT;
  if (!user_onDPadLeftButtonPressed) {
    return 0;
  }
  if(ps2x.Button(button)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("PSB_"), Serial.print("PAD_"), Serial.print("LEFT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadLeftButtonPressed();
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
      Serial.print("PS2Controller"), Serial.print("::");
      Serial.print("processJoystickChange"), Serial.print("()"), Serial.print(" - ");
      Serial.print(label);
      Serial.println(": ");
      Serial.print(" - "), Serial.print("X"), Serial.print(": ");
      Serial.println(nJoyX);
      Serial.print(" - "), Serial.print("Y"), Serial.print(": ");
      Serial.println(nJoyY);
    }
#endif
    if (onChange) {
      onChange(nJoyX, nJoyY);
      return 1;
    } else {
#if __RUNNING_LOG_ENABLED__
      if (debugEnabled) {
        Serial.print("PS2Controller"), Serial.print("::");
        Serial.print("processJoystickChange"), Serial.print("()"), Serial.print(" - ");
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
