#include "RoboCar_Programs.h"

//-------------------------------------------------------------------------------------------------

RemoteControlCar::RemoteControlCar(char* title,
    DisplayAdapter* displayAdapter,
    RoboCarHandler* roboCarHandler,
    PedestalGroup* pedestalGroup,
    bool autoPedestal) {
  _title = title;
  _displayAdapter = displayAdapter;
  _roboCarHandler = roboCarHandler;
  _pedestalGroup = pedestalGroup;
  _autoPedestal = autoPedestal;
}

void RemoteControlCar::set(DisplayAdapter* displayAdapter) {
  _displayAdapter = displayAdapter;
}

void RemoteControlCar::set(RoboCarHandler* roboCarHandler) {
  _roboCarHandler = roboCarHandler;
}

void RemoteControlCar::set(PedestalGroup* pedestalGroup, bool autoPedestal) {
  _pedestalGroup = pedestalGroup;
  _autoPedestal = autoPedestal;
}

char* RemoteControlCar::getTitle() {
  return _title;
}

int RemoteControlCar::begin() {
  if (_roboCarHandler != NULL) {
    _roboCarHandler->turnOn();
  }
  return 0;
}

char fmt[] = "L:%4d - R:%4d";

int RemoteControlCar::check(void* action_, void* command_) {
  JoystickAction* action = (JoystickAction*) action_;
  MovingCommand* command = (MovingCommand*) command_;

  if (_autoPedestal) {
    if (_pedestalGroup != NULL) {
      _pedestalGroup->autoDance();
    }
  }

  if (_roboCarHandler) {
    _roboCarHandler->move(command);
  }

  if (_displayAdapter != NULL) {
    _displayAdapter->render(0, 0, _title);
    char text[16] = {};
    sprintf(text, fmt, command->getLeftSpeed(), command->getRightSpeed());
    _displayAdapter->render(0, 1, text);
  }

  return 0;
}

int RemoteControlCar::close() {
  if (_pedestalGroup != NULL) {
    _pedestalGroup->reset();
  }

  if (_roboCarHandler != NULL) {
    _roboCarHandler->turnOff();
  }

  return 0;
}

//-------------------------------------------------------------------------------------------------
