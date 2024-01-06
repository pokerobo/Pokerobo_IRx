#include "Moving_Command.h"

MovingCommand::MovingCommand(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  update(leftSpeed, leftDirection, rightSpeed, rightDirection);
}

void MovingCommand::update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection) {
  _LeftSpeed = leftSpeed;
  _LeftDirection = leftDirection;
  _RightSpeed = rightSpeed;
  _RightDirection = rightDirection;
}

int MovingCommand::getLeftSpeed() {
  return _LeftSpeed;
}

byte MovingCommand::getLeftDirection() {
  return _LeftDirection;
}

int MovingCommand::getRightSpeed() {
  return _RightSpeed;
}

byte MovingCommand::getRightDirection() {
  return _RightDirection;
}

void* MovingCommand::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }

  uint8_t directionFlags = buf[0];
  _LeftDirection = directionFlags & 0b0011;
  _RightDirection = (directionFlags & 0b1100) >> 2;
  _LeftSpeed = buf[1];
  _RightSpeed = buf[2];

  return this;
}
