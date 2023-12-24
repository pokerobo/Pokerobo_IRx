#ifndef __EVENT_TRIGGER_H__
#define __EVENT_TRIGGER_H__

#include "Commons.h"

#include "Joystick_Action.h"
#include "Moving_Command.h"

#if (CONTROLLER_ROBOCAR)
#include "RoboCar_Handler.h"
#endif

#if (CONTROLLER_PEDESTAL)
#include "Pedestal_Group.h"
#endif

#define PROGRAM_CARDUINO_STATE_IDLE         1
#define PROGRAM_CARDUINO_STATE_CARDUINO     2
#define PROGRAM_CARDUINO_STATE_PEDESTAL     4

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

#define RF24_JOYSTICK_DEADZONE_X  32
#define RF24_JOYSTICK_DEADZONE_Y  32

#define RF24_JOYSTICK_RANGE_X     255
#define RF24_JOYSTICK_RANGE_Y     255

#define CLICKING_FLAGS            0

class EventTrigger {
  public:
    void begin();
    int check();
#if (CONTROLLER_PEDESTAL)
    void set(PedestalGroup* pedestalGroup);
#endif
#if (CONTROLLER_ROBOCAR)
    void set(RoboCarHandler* roboCarHandler);
#endif
    void setOnStartButtonPressed(void (*function)());
    void setOnSelectButtonPressed(void (*function)());
    void setOnAnalogButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnLeftJoystickChanged(void (*function)(int, int));
    void setOnRightJoystickChanged(void (*function)(int, int));
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
#if CLICKING_FLAGS
    uint16_t _clickingTrail;
#endif
    bool checkButtonPress(uint16_t pressed, uint16_t mask);
    uint16_t processButtonPress(uint16_t buttons);
    bool isJoystickChanged(int, int);
    int processJoystickChange(int, int, char);
  private:
    bool _debugEnabled = true;
    uint8_t _currentState = PROGRAM_CARDUINO_STATE_CARDUINO;
    #if (CONTROLLER_PEDESTAL)
    PedestalGroup* _pedestalGroup = NULL;
    #endif
    #if (CONTROLLER_ROBOCAR)
    RoboCarHandler* _roboCarHandler = NULL;
    #endif
    void (*_onStartButtonPressed)();
    void (*_onSelectButtonPressed)();
    void (*_onAnalogButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onLeftJoystickChanged)(int, int);
    void (*_onRightJoystickChanged)(int, int);
};

#endif
