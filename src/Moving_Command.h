#ifndef __MOVING_COMMAND_H__
#define __MOVING_COMMAND_H__

#include "Commons.h"

class MovingCommand {
  public:
    MovingCommand(int leftSpeed=0, byte leftDirection=0, int rightSpeed=0, byte rightDirection=0);
    void update(int leftSpeed, byte leftDirection, int rightSpeed, byte rightDirection);
    int getLeftSpeed();
    byte getLeftDirection();
    int getRightSpeed();
    byte getRightDirection();
  private:
    int _LeftSpeed;
    byte _LeftDirection;
    int _RightSpeed;
    byte _RightDirection;
};

#endif
