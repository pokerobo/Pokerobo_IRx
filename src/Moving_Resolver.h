#ifndef __MOVING_RESOLVER_H__
#define __MOVING_RESOLVER_H__

#include "Commons.h"
#include "Moving_Command.h"

class MovingResolver {
  public:
    MovingCommand* resolve(MovingCommand* packet, int x, int y);
  private:
    int _coeff = 3;
    bool _rotatable = false;
};

#endif
