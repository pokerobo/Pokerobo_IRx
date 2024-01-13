#include "Event_Dispatcher.h"

#define PEDESTAL_RANGE_X          6
#define PEDESTAL_RANGE_Y          6

//-------------------------------------------------------------------------------------------------

#if (CONTROLLER_PEDESTAL)
void EventDispatcher::set(PedestalGroup* pedestalGroup) {
  _pedestalGroup = pedestalGroup;
}
#endif

#if (CONTROLLER_ROBOCAR)
void EventDispatcher::set(RoboCarHandler* roboCarHandler) {
  _roboCarHandler = roboCarHandler;
}
#endif

void EventDispatcher::setOnStartButtonPressed(void (*function)()) {
  _onStartButtonPressed = function;
};

void EventDispatcher::setOnSelectButtonPressed(void (*function)()) {
  _onSelectButtonPressed = function;
};

void EventDispatcher::setOnAnalogButtonPressed(void (*function)()) {
  _onAnalogButtonPressed = function;
};

void EventDispatcher::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void EventDispatcher::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void EventDispatcher::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void EventDispatcher::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void EventDispatcher::setOnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
}

void EventDispatcher::setOnRightJoystickChanged(void (*function)(int, int)) {
  _onRightJoystickChanged = function;
};

//-------------------------------------------------------------------------------------------------

void EventDispatcher::begin() {
}

int EventDispatcher::loop() {
  return autoplay();
}

int EventDispatcher::autoplay() {
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      #if (CONTROLLER_PEDESTAL)
      if (_pedestalGroup != NULL) {
        _pedestalGroup->autoDance();
      }
      #endif
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      break;
  }
  return 0;
}

int EventDispatcher::next() {
  int result = 0;
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      #if (CONTROLLER_ROBOCAR)
      if (_roboCarHandler != NULL) {
        _roboCarHandler->turnOn();
      }
      #endif
      _currentState = PROGRAM_CARDUINO_STATE_CARDUINO;
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      #if (CONTROLLER_ROBOCAR)
      if (_roboCarHandler != NULL) {
        _roboCarHandler->turnOff();
      }
      #endif
      _currentState = PROGRAM_CARDUINO_STATE_PEDESTAL;
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      #if (CONTROLLER_PEDESTAL)
      if (_pedestalGroup != NULL) {
        _pedestalGroup->reset();
      }
      #endif
      _currentState = PROGRAM_CARDUINO_STATE_IDLE;
      break;
  }
  return result;
}

//-------------------------------------------------------------------------------------------------

void EventDispatcher::processEvents(JoystickAction* action, MovingCommand* command) {
  if (action == NULL) {
    return;
  }

  uint16_t pressed = processButtonPress(action->getPressingFlags());
  if (pressed) {
    return pressed;
  }

  if (_currentState == PROGRAM_CARDUINO_STATE_CARDUINO && command != NULL) {
    #if (CONTROLLER_ROBOCAR)
    if (_roboCarHandler) {
      _roboCarHandler->move(command);
    }
    #endif
  }

  return processJoystickChange(action->getX(), action->getY(), 'L');
}

bool EventDispatcher::checkButtonPress(uint16_t pressed, uint16_t mask) {
  #if CLICKING_FLAGS
  if (pressed & mask) {
    _clickingTrail |= mask;
  } else {
    if (_clickingTrail & mask) {
      _clickingTrail &= (~mask);
      return true;
    }
  }
  return false;
  #else
  return pressed & mask;
  #endif
}

uint16_t EventDispatcher::processButtonPress(uint16_t pressed) {
  uint16_t checked = 0;

  if(checkButtonPress(pressed, MASK_START_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "START", " is pushed");
    }
    #endif
    if (_onStartButtonPressed != NULL) {
      _onStartButtonPressed();
    } else {
      processStartButtonPressedEvent();
    }
    checked |= MASK_START_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_SELECT_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "SELECT", " is pushed");
    }
    #endif
    if (_onSelectButtonPressed != NULL) {
      _onSelectButtonPressed();
    } else {
      processSelectButtonPressedEvent();
    }
    checked |= MASK_SELECT_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_ANALOG_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "ANALOG", " is pushed");
    }
    #endif
    if (_onAnalogButtonPressed != NULL) {
      _onAnalogButtonPressed();
    } else {
      processAnalogButtonPressedEvent();
    }
    checked |= MASK_ANALOG_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_UP_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "UP", " is pushed");
    }
    #endif
    if (_onDPadUpButtonPressed != NULL) {
      _onDPadUpButtonPressed();
    } else {
      processDPadUpButtonPressedEvent();
    }
    checked |= MASK_UP_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_RIGHT_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "RIGHT", " is pushed");
    }
    #endif
    if (_onDPadRightButtonPressed != NULL) {
      _onDPadRightButtonPressed();
    } else {
      processDPadRightButtonPressedEvent();
    }
    checked |= MASK_RIGHT_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_DOWN_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "DOWN", " is pushed");
    }
    #endif
    if (_onDPadDownButtonPressed != NULL) {
      _onDPadDownButtonPressed();
    } else {
      processDPadDownButtonPressedEvent();
    }
    checked |= MASK_DOWN_BUTTON;
  }

  if(checkButtonPress(pressed, MASK_LEFT_BUTTON)) {
    #if __DISPATCHER_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "LEFT", " is pushed");
    }
    #endif
    if (_onDPadLeftButtonPressed != NULL) {
      _onDPadLeftButtonPressed();
    } else {
      processDPadLeftButtonPressedEvent();
    }
    checked |= MASK_LEFT_BUTTON;
  }

  return checked;
}

bool EventDispatcher::isJoystickChanged(int nJoyX, int nJoyY) {
  return nJoyX >= RF24_JOYSTICK_DEADZONE_X || nJoyX <= -RF24_JOYSTICK_DEADZONE_X ||
      nJoyY >= RF24_JOYSTICK_DEADZONE_Y || nJoyY <= -RF24_JOYSTICK_DEADZONE_Y;
}

int EventDispatcher::processJoystickChange(int nJoyX, int nJoyY, char label) {

  nJoyX = map(nJoyX, 0, 1024, -RF24_JOYSTICK_RANGE_X, RF24_JOYSTICK_RANGE_X);
  nJoyY = map(nJoyY, 0, 1024, -RF24_JOYSTICK_RANGE_Y, RF24_JOYSTICK_RANGE_Y);

  #if defined(RF24_JOYSTICK_CHECKING_CHANGE)
  if (!isJoystickChanged(nJoyX, nJoyY)) {
    return 0;
  }
  #endif

  #if __DISPATCHER_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { 'L', '\0' };
    debugLog("Event", "Trigger", "::", "process", "JoystickChange", "()", " - ", l_, ": ");
    char x_[7], y_[7];
    debugLog(" - ", "X", ": ", itoa(nJoyX, x_, 10));
    debugLog(" - ", "Y", ": ", itoa(nJoyY, y_, 10));
  }
  #endif

  if (label == 'L') {
    if (_onLeftJoystickChanged) {
      _onLeftJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else {
      processLeftJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

  if (label == 'R') {
    if (_onRightJoystickChanged) {
      _onRightJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else {
      processRightJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

  #if __DISPATCHER_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { 'L', '\0' };
    debugLog("Event", "Trigger", "::", "process", "JoystickChange", "()", " - ", l_, ": ", "not registered");
  }
  #endif

  return -1;
}

//-------------------------------------------------------------------------------------------------

void EventDispatcher::processStartButtonPressedEvent() {
  next();
}

void EventDispatcher::processSelectButtonPressedEvent() {
}

void EventDispatcher::processAnalogButtonPressedEvent() {
}

void EventDispatcher::processDPadUpButtonPressedEvent() {
  #if (CONTROLLER_PEDESTAL)
  if (_pedestalGroup != NULL) {
    _pedestalGroup->verticalServoUp();
  }
  #endif
}

void EventDispatcher::processDPadRightButtonPressedEvent() {
  #if (CONTROLLER_PEDESTAL)
  if (_pedestalGroup != NULL) {
    _pedestalGroup->horizontalServoRight();
  }
  #endif
}

void EventDispatcher::processDPadDownButtonPressedEvent() {
  #if (CONTROLLER_PEDESTAL)
  if (_pedestalGroup != NULL) {
    _pedestalGroup->verticalServoDown();
  }
  #endif
}

void EventDispatcher::processDPadLeftButtonPressedEvent() {
  #if (CONTROLLER_PEDESTAL)
  if (_pedestalGroup != NULL) {
    _pedestalGroup->horizontalServoLeft();
  }
  #endif
}

void EventDispatcher::processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      #if (CONTROLLER_ROBOCAR)
      if (_roboCarHandler != NULL) {
        _roboCarHandler->move(nJoyX, nJoyY);
      }
      #endif
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      #if (CONTROLLER_PEDESTAL)
      if (_pedestalGroup != NULL) {
        nJoyX = map(nJoyX, -255, 255, PEDESTAL_RANGE_X, -PEDESTAL_RANGE_X);
        nJoyY = map(nJoyY, -255, 255, PEDESTAL_RANGE_Y, -PEDESTAL_RANGE_Y);
        _pedestalGroup->changeByJoystick(nJoyX, nJoyY);
      }
      #endif
      break;
  }
}

void EventDispatcher::processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
}
