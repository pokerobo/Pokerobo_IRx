#include "PS2_Controller.h"

PS2Controller_::PS2Controller_() {
  debugEnabled = true;
  errorCode = 0;
  errorDisplayed = false;
  ps2Type = 0;
  vibrate = 0;
}

void PS2Controller_::begin() {
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
      char b_[5];
      debugLog("Invalid", " ", "Controller", " ", "type", ": ", itoa(ps2Type, b_, 10));
#endif
      NULL;
  }
};

bool PS2Controller_::hasError() {
  return errorCode != 0;
};

void PS2Controller_::showError() {
  if (!errorDisplayed) {
    debugLog("Error, terminated!");
    errorDisplayed = true;
  }
};

void PS2Controller_::reload() {
  begin();
};

int PS2Controller_::loop() {
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

//-------------------------------------------------------------------------------------------------

void PS2Controller::set(EventTrigger* eventTrigger) {
  _eventTrigger = eventTrigger;
};

int PS2Controller::check() {
  ps2x.read_gamepad(false, vibrate); // disable vibration of the controller
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
  int lstatus = processJoystickChange(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY), 'L');
  //
  int rstatus = processJoystickChange(ps2x.Analog(PSS_RX), ps2x.Analog(PSS_RY), 'R');
};

int PS2Controller::processStartButtonPress() {
  if (!_onStartButtonPressed && !_eventTrigger) {
    return 0;
  }
  if(ps2x.Button(PSB_START)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "START", " is pushed");
    }
#endif
    if (_onStartButtonPressed) {
      _onStartButtonPressed();
    } else {
      _eventTrigger->processStartButtonPressedEvent();
    }
    return PSB_START;
  }
  return 0;
}

int PS2Controller::processSelectButtonPress() {
  if (!_onSelectButtonPressed && !_eventTrigger) {
    return 0;
  }
  if(ps2x.Button(PSB_SELECT)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "SELECT", " is pushed");
    }
#endif
    if (_onSelectButtonPressed) {
      _onSelectButtonPressed();
    } else {
      _eventTrigger->processSelectButtonPressedEvent();
    }
    return PSB_SELECT;
  }
  return 0;
}

int PS2Controller::processDPadUpButtonPress() {
  if (!_onDPadUpButtonPressed && !_eventTrigger) {
    return 0;
  }
  if(ps2x.Button(PSB_PAD_UP)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "UP", " is pushed");
    }
#endif
    if (_onDPadUpButtonPressed) {
      _onDPadUpButtonPressed();
    } else {
      _eventTrigger->processDPadUpButtonPressedEvent();
    }
    return PSB_PAD_UP;
  }
  return 0;
}

int PS2Controller::processDPadRightButtonPress() {
  if (!_onDPadRightButtonPressed && !_eventTrigger) {
    return 0;
  }
  if(ps2x.Button(PSB_PAD_RIGHT)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "RIGHT", " is pushed");
    }
#endif
    if (_onDPadRightButtonPressed) {
      _onDPadRightButtonPressed();
    } else {
      _eventTrigger->processDPadRightButtonPressedEvent();
    }
    return PSB_PAD_RIGHT;
  }
  return 0;
}

int PS2Controller::processDPadDownButtonPress() {
  if (!_onDPadDownButtonPressed && !_eventTrigger) {
    return 0;
  }
  if(ps2x.Button(PSB_PAD_DOWN)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "DOWN", " is pushed");
    }
#endif
    if (_onDPadDownButtonPressed) {
      _onDPadDownButtonPressed();
    } else {
      _eventTrigger->processDPadDownButtonPressedEvent();
    }
    return PSB_PAD_DOWN;
  }
  return 0;
}

int PS2Controller::processDPadLeftButtonPress() {
  if (!_onDPadLeftButtonPressed) {
    return 0;
  }
  if(ps2x.Button(PSB_PAD_LEFT)) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "LEFT", " is pushed");
    }
#endif
    if (_onDPadLeftButtonPressed) {
      _onDPadLeftButtonPressed();
    } else {
      _eventTrigger->processDPadLeftButtonPressedEvent();
    }
    return PSB_PAD_LEFT;
  }
  return 0;
}

bool PS2Controller::isJoystickChanged(int nJoyX, int nJoyY) {
  #if defined(PS2_JOYSTICK_CHECKING_CHANGE)
  return nJoyX >= PS2_JOYSTICK_DEADZONE_X || nJoyX <= -PS2_JOYSTICK_DEADZONE_X ||
      nJoyY >= PS2_JOYSTICK_DEADZONE_Y || nJoyY <= -PS2_JOYSTICK_DEADZONE_Y;
  #else
  return true;
  #endif
}

int PS2Controller::adjustJoystickX(int nJoyX) {
  return map(nJoyX, 0, 255, PS2_JOYSTICK_RANGE_X, -PS2_JOYSTICK_RANGE_X);
}

int PS2Controller::adjustJoystickY(int nJoyY) {
  return map(nJoyY, 0, 255, PS2_JOYSTICK_RANGE_Y, -PS2_JOYSTICK_RANGE_Y);
}

int PS2Controller::processJoystickChange(int nJoyX, int nJoyY, const char label) {
  nJoyX = adjustJoystickX(nJoyX);
  nJoyY = adjustJoystickY(nJoyY);

#if defined(PS2_JOYSTICK_CHECKING_CHANGE)
  if (!isJoystickChanged(nJoyX, nJoyY)) {
    return 0;
  }
#endif

#if __RUNNING_LOG_ENABLED__
  if (debugEnabled) {
    char l_[2] = { label, '\0' };
    debugLog("PS2", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ");
    char x_[7], y_[7];
    debugLog(" - ", "X", ": ", itoa(nJoyX, x_, 10));
    debugLog(" - ", "Y", ": ", itoa(nJoyY, y_, 10));
  }
#endif

  if (label == 'L') {
    if (_onLeftJoystickChanged) {
      _onLeftJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else if (_eventTrigger != NULL) {
      _eventTrigger->processLeftJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

  if (label == 'R') {
    if (_onRightJoystickChanged) {
      _onRightJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else if (_eventTrigger != NULL) {
      _eventTrigger->processRightJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

#if __RUNNING_LOG_ENABLED__
  if (debugEnabled) {
    char l_[2] = { label, '\0' };
    debugLog("PS2", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ", "not registered");
  }
#endif

  return -1;
};

//-------------------------------------------------------------------------------------------------

void PS2Kontroller::set(EventProcessor* eventProcessor) {
  _eventProcessor = eventProcessor;
};

void PS2Kontroller::set(MovingResolver* movingResolver) {
  _movingResolver = movingResolver;
};

int PS2Kontroller::read(JoystickAction* action, MovingCommand* command) {
  ps2x.read_gamepad(false, vibrate); // disable vibration of the controller

  uint16_t buttons = 0;

  if(ps2x.Button(PSB_START)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "START", " is pushed");
    }
    #endif
    buttons |= MASK_START_BUTTON;
  }

  if(ps2x.Button(PSB_SELECT)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "SELECT", " is pushed");
    }
    #endif
    buttons |= MASK_SELECT_BUTTON;
  }

  if(ps2x.Button(PSB_PAD_UP)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "UP", " is pushed");
    }
    #endif
    buttons |= MASK_UP_BUTTON;
  }

  if(ps2x.Button(PSB_PAD_RIGHT)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "RIGHT", " is pushed");
    }
    #endif
    buttons |= MASK_RIGHT_BUTTON;
  }

  if(ps2x.Button(PSB_PAD_DOWN)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "DOWN", " is pushed");
    }
    #endif
    buttons |= MASK_DOWN_BUTTON;
  }

  if(ps2x.Button(PSB_PAD_LEFT)) {
    #if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PSB", "_", "PAD", "_", "LEFT", " is pushed");
    }
    #endif
    buttons |= MASK_LEFT_BUTTON;
  }

  int nJoyX = ps2x.Analog(PSS_LX); // read x-joystick
  int nJoyY = ps2x.Analog(PSS_LY); // read y-joystick

  uint16_t lJoyX = map(nJoyX, 0, 255, 0, 1024);
  uint16_t lJoyY = map(nJoyY, 0, 255, 0, 1024);

  action->update(buttons, lJoyX, lJoyY, 0);

  if (_movingResolver != NULL && command != NULL) {
    _movingResolver->resolve(command, lJoyX, lJoyY);
  }

  return 1;
}

int PS2Kontroller::check() {
  JoystickAction action;
  MovingCommand command;

  int ok = read(&action, &command);

  if (ok == 1) {
    if (_eventProcessor != NULL) {
      _eventProcessor->processEvents(&action, &command);
      return 0xff;
    }

    uint16_t pressed = processButtonPress(action.getPressingFlags());
    if (pressed) {
      return pressed;
    }

    return processJoystickChange(action.getX(), action.getY(), 'L');
  }

  return ok;
}

bool PS2Kontroller::isJoystickChanged(int nJoyX, int nJoyY) {
  #if defined(PS2_JOYSTICK_CHECKING_CHANGE)
  return nJoyX >= PS2_JOYSTICK_DEADZONE_X || nJoyX <= -PS2_JOYSTICK_DEADZONE_X ||
      nJoyY >= PS2_JOYSTICK_DEADZONE_Y || nJoyY <= -PS2_JOYSTICK_DEADZONE_Y;
  #else
  return true;
  #endif
}

int PS2Kontroller::adjustJoystickX(int nJoyX) {
  return map(nJoyX, 0, 1024, PS2_JOYSTICK_RANGE_X, -PS2_JOYSTICK_RANGE_X);
}

int PS2Kontroller::adjustJoystickY(int nJoyY) {
  return map(nJoyY, 0, 1024, PS2_JOYSTICK_RANGE_Y, -PS2_JOYSTICK_RANGE_Y);
}
