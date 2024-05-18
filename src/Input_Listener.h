#ifndef __POKEROBO_CAR_INPUT_LISTENER_H__
#define __POKEROBO_CAR_INPUT_LISTENER_H__

#include "Commons.h"

#include "Context_Packet.h"
#include "Joystick_Action.h"
#include "Moving_Command.h"

class InputListener {
  public:
    virtual int read(MasterContext* context, JoystickAction* action, MovingCommand* command);
};

#endif
