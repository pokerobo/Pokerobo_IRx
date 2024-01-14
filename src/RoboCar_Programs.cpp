#include "RoboCar_Programs.h"

//-------------------------------------------------------------------------------------------------

RemoteControlCar::RemoteControlCar(char* title,
    DisplayAdapter* displayAdapter,
    RoboCarHandler* roboCarHandler) {
  _title = title;
  _displayAdapter = displayAdapter;
  _roboCarHandler = roboCarHandler;
}

void RemoteControlCar::set(DisplayAdapter* displayAdapter) {
  _displayAdapter = displayAdapter;
}

void RemoteControlCar::set(RoboCarHandler* roboCarHandler) {
  _roboCarHandler = roboCarHandler;
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

  if (_roboCarHandler) {
    _roboCarHandler->move(command);
  }

  showSpeedometer_(action, command);

  return 0;
}

int RemoteControlCar::close() {
  if (_roboCarHandler != NULL) {
    _roboCarHandler->turnOff();
  }

  return 0;
}

void RemoteControlCar::showSpeedometer_(JoystickAction* action, MovingCommand* command) {
  if (_displayAdapter != NULL) {
    _displayAdapter->render(0, 0, _title);
    char text[16] = {};
    sprintf(text, fmt, command->getLeftSpeed(), command->getRightSpeed());
    _displayAdapter->render(0, 1, text);
  }
}

DisplayAdapter* RemoteControlCar::getDisplayAdapter() {
  return _displayAdapter;
}

RoboCarHandler* RemoteControlCar::getRoboCarHandler() {
  return _roboCarHandler;
}

//-------------------------------------------------------------------------------------------------

int LineFollowingCar::check(void* action_, void* command_) {
  JoystickAction* action = (JoystickAction*) action_;
  MovingCommand* command = (MovingCommand*) command_;

  return RemoteControlCar::check(action_, command_);
}

//-------------------------------------------------------------------------------------------------

void DancingPuppetCar::set(DisplayAdapter* displayAdapter) {
  RemoteControlCar::set(displayAdapter);
}

void DancingPuppetCar::set(RoboCarHandler* roboCarHandler) {
  RemoteControlCar::set(roboCarHandler);
}

void DancingPuppetCar::set(PedestalGroup* pedestalGroup, bool autoPedestal) {
  _pedestalGroup = pedestalGroup;
  _autoPedestal = autoPedestal;
}

int DancingPuppetCar::begin() {
  return RemoteControlCar::begin();
}

int DancingPuppetCar::check(void* action_, void* command_) {
  JoystickAction* action = (JoystickAction*) action_;
  MovingCommand* command = (MovingCommand*) command_;

  if (_autoPedestal) {
    if (_pedestalGroup != NULL) {
      _pedestalGroup->autoDance();
    }
  }

  return RemoteControlCar::check(action_, command_);
}

int DancingPuppetCar::close() {\
  if (_pedestalGroup != NULL) {
    _pedestalGroup->reset();
  }

  return RemoteControlCar::close();
}
