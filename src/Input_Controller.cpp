#include "Input_Controller.h"

#ifndef CLICKING_FLAGS
#define CLICKING_FLAGS            0
#endif//CLICKING_FLAGS

void InputController::setOnAnalogButtonPressed(void (*function)()) {
  _onAnalogButtonPressed = function;
};

void InputController::setOnStartButtonPressed(void (*function)()) {
  _onStartButtonPressed = function;
};

void InputController::setOnSelectButtonPressed(void (*function)()) {
  _onSelectButtonPressed = function;
};

void InputController::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void InputController::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void InputController::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void InputController::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void InputController::setOnDPadButtonPressed(void (*function)(uint16_t)) {
  _onDPadButtonPressed = function;
};

void InputController::setOnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
};

void InputController::setOnRightJoystickChanged(void (*function)(int, int)) {
  _onRightJoystickChanged = function;
};

bool InputController::checkButtonPress(uint16_t pressed, uint16_t mask) {
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

uint16_t InputController::processButtonPress(uint16_t pressed) {
  uint16_t checked = 0;

  if(checkButtonPress(pressed, MASK_START_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "START", " is pushed");
    }
    #endif
    if (_onStartButtonPressed != NULL) {
      _onStartButtonPressed();
      checked |= MASK_START_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_SELECT_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "SELECT", " is pushed");
    }
    #endif
    if (_onSelectButtonPressed != NULL) {
      _onSelectButtonPressed();
      checked |= MASK_SELECT_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_ANALOG_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "ANALOG", " is pushed");
    }
    #endif
    if (_onAnalogButtonPressed != NULL) {
      _onAnalogButtonPressed();
      checked |= MASK_ANALOG_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_UP_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "UP", " is pushed");
    }
    #endif
    if (_onDPadUpButtonPressed != NULL) {
      _onDPadUpButtonPressed();
      checked |= MASK_UP_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_RIGHT_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "RIGHT", " is pushed");
    }
    #endif
    if (_onDPadRightButtonPressed != NULL) {
      _onDPadRightButtonPressed();
      checked |= MASK_RIGHT_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_DOWN_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "DOWN", " is pushed");
    }
    #endif
    if (_onDPadDownButtonPressed != NULL) {
      _onDPadDownButtonPressed();
      checked |= MASK_DOWN_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_LEFT_BUTTON)) {
    #if __INPUT_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "LEFT", " is pushed");
    }
    #endif
    if (_onDPadLeftButtonPressed != NULL) {
      _onDPadLeftButtonPressed();
      checked |= MASK_LEFT_BUTTON;
    }
  }

  return checked;
}

int InputController::processJoystickChange(int nJoyX, int nJoyY, char label) {
  nJoyX = adjustJoystickX(nJoyX);
  nJoyY = adjustJoystickY(nJoyY);

  if (!isJoystickChanged(nJoyX, nJoyY)) {
    return 0;
  }

  #if __INPUT_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { label, '\0' };
    debugLog("Input", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ");
    char x_[7], y_[7];
    debugLog(" - ", "X", ": ", itoa(nJoyX, x_, 10));
    debugLog(" - ", "Y", ": ", itoa(nJoyY, y_, 10));
  }
  #endif

  if (label == 'L') {
    if (_onLeftJoystickChanged) {
      _onLeftJoystickChanged(nJoyX, nJoyY);
      return 1;
    }
  }

  if (label == 'R') {
    if (_onRightJoystickChanged) {
      _onRightJoystickChanged(nJoyX, nJoyY);
      return 1;
    }
  }

  #if __INPUT_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { label, '\0' };
    debugLog("Input", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ", "not registered");
  }
  #endif

  return -1;
}
