#ifndef __EVENT_PROCESSOR_H__
#define __EVENT_PROCESSOR_H__

#include "Commons.h"

#include "Joystick_Action.h"
#include "Moving_Command.h"

class EventProcessor {
  public:
    virtual void processEvents(JoystickAction* action, MovingCommand* command);
};

#endif
