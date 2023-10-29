#include "Pedestal_Group.h"

PedestalGroup::PedestalGroup(PedestalHandler* pedestalHandlers[]) {
  for (int i=0; i<PEDESTALS_MAX; i++) {
    if (pedestalHandlers[i] != NULL) {
      _pedestalHandlers[_pedestalsTotal] = pedestalHandlers[i];
      _pedestalsTotal += 1;
    }
  }

#if __LOADING_LOG_ENABLED__
  char _pedestalsTotal_[7];
  debugLog("PedestalGroup", "()", " - ", "total", ": ", itoa(_pedestalsTotal, _pedestalsTotal_, 10));

#endif
  _sceneDirection = true;
  _sceneStep = -1;
}

void PedestalGroup::begin() {
  PedestalHandler::init();
  for (int i=0; i<_pedestalsTotal; i++) {
    if (_pedestalHandlers[i] == NULL) continue;
    _pedestalHandlers[i]->begin(
        HORIZONTAL_MIN_ANGLE,
        HORIZONTAL_MAX_ANGLE,
        VERTICAL_MIN_ANGLE,
        VERTICAL_MAX_ANGLE);
  }
}

bool PedestalGroup::change(int hDelta, int vDelta) {
  for (int i=0; i<_pedestalsTotal; i++) {
    _pedestalHandlers[i]->change(hDelta, vDelta);
  }
}

void PedestalGroup::setHorizontalPosition(int hPos) {
  for (int i=0; i<_pedestalsTotal; i++) {
    _pedestalHandlers[i]->setHorizontalPosition(hPos);
  }
}

void PedestalGroup::setVerticalPosition(int vPos) {
  for (int i=0; i<_pedestalsTotal; i++) {
    _pedestalHandlers[i]->setVerticalPosition(vPos);
  }
}

void PedestalGroup::reset() {
  for (int i=0; i<_pedestalsTotal; i++) {
    _pedestalHandlers[i]->reset();
  }
}

void PedestalGroup::processStartButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processStartButtonPressedEventFor(_pedestalHandlers[i]);
  }
}

void PedestalGroup::processStartButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->reset();
}

void PedestalGroup::processAnalogButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processAnalogButtonPressedEventFor(_pedestalHandlers[i]);
  }
}

void PedestalGroup::processAnalogButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  // do something here
}

void PedestalGroup::processDPadUpButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processDPadUpButtonPressedEventFor(_pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  debugLog("main", "()", " - ", "UP", " is pushed");
#endif
}

void PedestalGroup::processDPadUpButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->verticalServoUp();
}

void PedestalGroup::processDPadRightButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processDPadRightButtonPressedEventFor(_pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  debugLog("main", "()", " - ", "RIGHT", " is pushed");
#endif
}

void PedestalGroup::processDPadRightButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->horizontalServoRight();
}

void PedestalGroup::processDPadDownButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processDPadDownButtonPressedEventFor(_pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  debugLog("main", "()", " - ", "DOWN", " is pushed");
#endif
}

void PedestalGroup::processDPadDownButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->verticalServoDown();
}

void PedestalGroup::processDPadLeftButtonPressedEvent() {
  for (int i=0; i<_pedestalsTotal; i++) {
    processDPadLeftButtonPressedEventFor(_pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  debugLog("main", "()", " - ", "LEFT", " is pushed");
#endif
}

void PedestalGroup::processDPadLeftButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->horizontalServoLeft();
}

void PedestalGroup::processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  for (int i=0; i<_pedestalsTotal; i++) {
    processLeftJoystickChangeEventFor(_pedestalHandlers[i], nJoyX, nJoyY);
  }
}

void PedestalGroup::processLeftJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY) {
  if (pedestalHandler == NULL) return;
  bool changed = pedestalHandler->change(nJoyX, nJoyY);
  if (changed) {
#if __RUNNING_LOG_ENABLED__
    debugLog("main", "()", " - ", "process", "Left", "JoystickChange", "Event", "()", " is called");
#endif
  }
}

void PedestalGroup::processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
  for (int i=0; i<_pedestalsTotal; i++) {
    processRightJoystickChangeEventFor(_pedestalHandlers[i], nJoyX, nJoyY);
  }
}

void PedestalGroup::processRightJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY) {
  if (pedestalHandler == NULL) return;
  // do something here
}

void PedestalGroup::autoDance() {
  if (_sceneStep < 0) {
    _sceneStep = 0;
    reset();
#if __RUNNING_LOG_ENABLED__
    debugLog("PedestalGroup", "::", "autoDance", "()", " - ", "Starting");
#endif
    return;
  }
  if (_sceneStep == 0) {
    _sceneDirection = true;
  }
  if (_sceneStep == _sceneStepsTotal - 1) {
    _sceneDirection = false;
  }
  if (_sceneDirection) {
    _sceneStep += 1;
  } else {
    _sceneStep -= 1;
  }
#if __RUNNING_LOG_ENABLED__
  char _step_[7];
  debugLog("PedestalGroup", "::", "autoDance", "()", " - ", "step", ": ", itoa(_sceneStep, _step_, 10));
#endif
  setHorizontalPosition(_sceneHPos[_sceneStep]);
  setVerticalPosition(_sceneVPos[_sceneStep]);
}
