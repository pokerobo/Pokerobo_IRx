#ifndef __EVENT_TRIGGER_H__
#define __EVENT_TRIGGER_H__

#include "Commons.h"

#include "Joystick_Action.h"
#include "Moving_Command.h"

#if (CONTROLLER_ROBOCAR)
#include "RoboCar_Handler.h"
#endif

#include "Pedestal_Group.h"

#define PROGRAM_CARDUINO_STATE_IDLE         1
#define PROGRAM_CARDUINO_STATE_CARDUINO     2
#define PROGRAM_CARDUINO_STATE_PEDESTAL     4

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

class EventTrigger {
  public:
    void begin();
    int check();
    void set(PedestalGroup* pedestalGroup);
#if (CONTROLLER_ROBOCAR)
    void set(RoboCarHandler* roboCarHandler);
#endif
    void processEvents(JoystickAction* action, MovingCommand* command);
    void processStartButtonPressedEvent();
    void processSelectButtonPressedEvent();
    void processAnalogButtonPressedEvent();
    void processDPadUpButtonPressedEvent();
    void processDPadRightButtonPressedEvent();
    void processDPadDownButtonPressedEvent();
    void processDPadLeftButtonPressedEvent();
    void processLeftJoystickChangeEvent(int nJoyX, int nJoyY);
    void processRightJoystickChangeEvent(int nJoyX, int nJoyY);
  protected:
    int next();
  private:
#if (CONTROLLER_ROBOCAR)
    RoboCarHandler* _roboCarHandler = NULL;
#endif
    PedestalGroup* _pedestalGroup = NULL;
    uint8_t _currentState = PROGRAM_CARDUINO_STATE_IDLE;
};

#endif
