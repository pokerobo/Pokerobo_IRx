#include "Event_Trigger.h"

#define PEDESTAL_RANGE_X          6
#define PEDESTAL_RANGE_Y          6

void EventTrigger::begin() {
}

int EventTrigger::check() {
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      _pedestalGroup->autoDance();
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      break;
  }
  return 0;
}

int EventTrigger::next() {
  int result = 0;
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      _carbotHandler->turnOn();
      _currentState = PROGRAM_CARDUINO_STATE_CARDUINO;
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      _carbotHandler->turnOff();
      _currentState = PROGRAM_CARDUINO_STATE_PEDESTAL;
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      _pedestalGroup->reset();
      _currentState = PROGRAM_CARDUINO_STATE_IDLE;
      break;
  }
  return result;
}

#if (CONTROLLER_CARBOT)
void EventTrigger::set(CarbotHandler* carbotHandler) {
  _carbotHandler = carbotHandler;
}
#endif

void EventTrigger::set(PedestalGroup* pedestalGroup) {
  _pedestalGroup = pedestalGroup;
}

void EventTrigger::processStartButtonPressedEvent() {
  next();
}

void EventTrigger::processSelectButtonPressedEvent() {
}

void EventTrigger::processAnalogButtonPressedEvent() {
}

void EventTrigger::processDPadUpButtonPressedEvent() {
  _pedestalGroup->processDPadUpButtonPressedEvent();
}

void EventTrigger::processDPadRightButtonPressedEvent() {
  _pedestalGroup->processDPadRightButtonPressedEvent();
}

void EventTrigger::processDPadDownButtonPressedEvent() {
  _pedestalGroup->processDPadDownButtonPressedEvent();
}

void EventTrigger::processDPadLeftButtonPressedEvent() {
  _pedestalGroup->processDPadLeftButtonPressedEvent();
}

void EventTrigger::processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  switch(_currentState) {
    case PROGRAM_CARDUINO_STATE_IDLE:
      break;
    case PROGRAM_CARDUINO_STATE_CARDUINO:
      _carbotHandler->move(nJoyX, nJoyY);
      break;
    case PROGRAM_CARDUINO_STATE_PEDESTAL:
      nJoyX = map(nJoyX, -255, 255, PEDESTAL_RANGE_X, -PEDESTAL_RANGE_X);
      nJoyY = map(nJoyY, -255, 255, PEDESTAL_RANGE_Y, -PEDESTAL_RANGE_Y);
      _pedestalGroup->processLeftJoystickChangeEvent(nJoyX, nJoyY);
      break;
  }
}

void EventTrigger::processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
  _pedestalGroup->processRightJoystickChangeEvent(nJoyX, nJoyY);
}
