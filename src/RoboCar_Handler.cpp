#include "RoboCar_Handler.h"

// Motor A connections

#define IN_2    8
#define IN_1    7
#define EN_A    6

// Motor B connections
#define IN_3    5
#define IN_4    4
#define EN_B    3

#define ROBOCAR_DEADZONE_BOUND_X    52
#define ROBOCAR_DEADZONE_BOUND_Y    52

int RoboCarHandler::begin() {
  // Set all the motor control pins to outputs
	pinMode(EN_A, OUTPUT);
	pinMode(EN_B, OUTPUT);
	pinMode(IN_1, OUTPUT);
	pinMode(IN_2, OUTPUT);
	pinMode(IN_3, OUTPUT);
	pinMode(IN_4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
	digitalWrite(IN_3, LOW);
	digitalWrite(IN_4, LOW);
}

void RoboCarHandler::set(MovingResolver* movingResolver) {
  _movingResolver = movingResolver;
}

bool RoboCarHandler::isActive() {
  return _active;
}

void RoboCarHandler::turnOn() {
  _active = true;
  #if __ROBOCAR_RUNNING_LOG__
  if (_debugEnabled) {
    debugLog("RoboCarHandler", "::", "turnOn", "()");
  }
  #endif
}

void RoboCarHandler::turnOff() {
  #if __ROBOCAR_RUNNING_LOG__
  if (_debugEnabled) {
    debugLog("RoboCarHandler", "::", "turnOff", "()");
  }
  #endif
  stop();
  _active = false;
}

void RoboCarHandler::flip() {
  if (isActive()) {
    turnOff();
  } else {
    turnOn();
  }
}

void RoboCarHandler::move(int x, int y) {
  if (!_movingResolver) {
    return;
  }

  if (!_active) {
    x = y = 0;
  }

  MovingCommand packet;
  _movingResolver->resolve(&packet, x, y);
  move(&packet);
}

void RoboCarHandler::move(MovingCommand* packet) {
  if (packet == NULL) {
    return;
  }

  uint8_t in1Val = LOW;
  uint8_t in2Val = LOW;
  uint8_t in3Val = LOW;
  uint8_t in4Val = LOW;

  switch (packet->getLeftDirection()) {
    case 1:
      in1Val = HIGH;
      break;
    case 2:
      in2Val = HIGH;
      break;
  }

  switch (packet->getRightDirection()) {
    case 1:
      in3Val = HIGH;
      break;
    case 2:
      in4Val = HIGH;
      break;
  }

  int enaVal = packet->getLeftSpeed();
  int enbVal = packet->getRightSpeed();

  #if __ROBOCAR_RUNNING_LOG__
  if (_debugEnabled) {
    char num_[7];
    debugLog(" - ", "active", ": ", _active ? "On" : "Off");
    debugLog(" - ", "IN_1", ": ", itoa(in1Val, num_, 10));
    debugLog(" - ", "IN_2", ": ", itoa(in2Val, num_, 10));
    debugLog(" - ", "IN_3", ": ", itoa(in3Val, num_, 10));
    debugLog(" - ", "IN_4", ": ", itoa(in4Val, num_, 10));

    debugLog(" - ", "EN_A", ": ", itoa(enaVal, num_, 10));
    debugLog(" - ", "EN_B", ": ", itoa(enbVal, num_, 10));
  }
  #endif

  digitalWrite(IN_1, in1Val);
  digitalWrite(IN_2, in2Val);
  digitalWrite(IN_3, in3Val);
  digitalWrite(IN_4, in4Val);

  analogWrite(EN_A, enaVal);
  analogWrite(EN_B, enbVal);
}

void RoboCarHandler::stop() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, 0);

  #if __ROBOCAR_RUNNING_LOG__
  if (_debugEnabled) {
    debugLog("RoboCarHandler", "::", "stop", "()");
  }
  #endif
}
