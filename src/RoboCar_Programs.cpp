#include "RoboCar_Programs.h"

//-------------------------------------------------------------------------------------------------

RemoteControlCar::RemoteControlCar(char* title,
    DisplayAdapter* displayAdapter,
    RoboCarHandler* roboCarHandler) {
  _title = title;
  _displayAdapter = displayAdapter;
  _roboCarHandler = roboCarHandler;
}

uint8_t RemoteControlCar::getId() {
  return 1;
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

bool RemoteControlCar::isDebugEnabled() {
  return _debugEnabled;
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

uint8_t LineFollowingCar::getId() {
  return 2;
}

void LineFollowingCar::set(DisplayAdapter* displayAdapter) {
  RemoteControlCar::set(displayAdapter);
}

void LineFollowingCar::set(RoboCarHandler* roboCarHandler) {
  RemoteControlCar::set(roboCarHandler);
}

void LineFollowingCar::set(LineDetector* lineDetector) {
  _lineDetector = lineDetector;
}

int LineFollowingCar::check(void* action_, void* command_) {
  JoystickAction* action = (JoystickAction*) action_;
  MovingCommand* command = (MovingCommand*) command_;

  if (_lineDetector != NULL) {
    uint8_t lineSignals = _lineDetector->read();

    if (isDebugEnabled()) {
      Serial.print("LineSignals: "), Serial.print(lineSignals, BIN), Serial.println();
    }

    // transforms the 10001 -> "-|||-", 1: white(-); 0: black(|)
    char lineText[16] = { 0 };
    for (int i=0; i<5; i++) {
      if ((lineSignals >> i) & 0b1) {
        lineText[i] = '-';
      } else {
        lineText[i] = '|';
      }
    }

    // show the line signals to the screen
    if (getDisplayAdapter() != NULL) {
      getDisplayAdapter()->render(5, 0, lineText);
    }
  }

  return RemoteControlCar::check(action_, command_);
}

void LineFollowingCar::showSpeedometer_(JoystickAction* action, MovingCommand* command) {
  if (getDisplayAdapter() != NULL) {
    char text[16] = {};
    sprintf(text, fmt, command->getLeftSpeed(), command->getRightSpeed());
    getDisplayAdapter()->render(0, 1, text);
  }
}

//-------------------------------------------------------------------------------------------------

uint8_t DancingPuppetCar::getId() {
  return 3;
}

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

int DancingPuppetCar::close() {
  if (_pedestalGroup != NULL) {
    _pedestalGroup->reset();
  }

  return RemoteControlCar::close();
}
