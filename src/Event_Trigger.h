#ifndef __EVENT_TRIGGER_H__
#define __EVENT_TRIGGER_H__

#include "Commons.h"

#if (CONTROLLER_CARBOT)
#include "Carbot_Handler.h"
#endif

#include "Pedestal_Group.h"

#define PROGRAM_CARDUINO_STATE_IDLE         1
#define PROGRAM_CARDUINO_STATE_CARDUINO     2
#define PROGRAM_CARDUINO_STATE_PEDESTAL     4

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

class EventTrigger {
  public:
    void begin();
#if (CONTROLLER_CARBOT)
    void set(CarbotHandler* carbotHandler);
#endif
    void set(PedestalGroup* pedestalGroup);
    int check();
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
#if (CONTROLLER_CARBOT)
    CarbotHandler* _carbotHandler = NULL;
#endif
    PedestalGroup* _pedestalGroup = NULL;
    uint8_t _currentState = PROGRAM_CARDUINO_STATE_IDLE;
};

#endif
