#ifndef __MOVING_RESOLVER_H__
#define __MOVING_RESOLVER_H__

#include "Commons.h"
#include "Moving_Command.h"

class MovingResolver {
  public:
    MovingCommand* resolve(MovingCommand* packet, int x, int y, int coeff=3, bool rotatable=false);
};

#endif
