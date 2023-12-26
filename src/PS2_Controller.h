// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#ifndef __PS2_CONTROLLER_H__
#define __PS2_CONTROLLER_H__

#include "Commons.h"
#include "Input_Controller.h"
#include "Event_Trigger.h"
#include <PS2X_lib.h>

#ifndef __PS2INIT_LOG_ENABLED__
#define __PS2INIT_LOG_ENABLED__ (__LOADING_LOG_ENABLED__ || 0)
#endif

#define PS2_JOYSTICK_DEADZONE_X   64
#define PS2_JOYSTICK_DEADZONE_Y   64

#define PS2_JOYSTICK_RANGE_X      255
#define PS2_JOYSTICK_RANGE_Y      255

#define DEBUG_DEFAULT       true

#define GAMEPAD_ATTENTION   10
#define GAMEPAD_COMMAND     11
#define GAMEPAD_DATA        12
#define GAMEPAD_CLOCK       13

class PS2Controller_: InputController {
  public:
    PS2Controller_();
    void begin();
    int loop();
    bool hasError();
    void showError();
    void reload();
    virtual int check();
  protected:
    PS2X ps2x;
    bool debugEnabled;
    int errorCode;
    bool errorDisplayed;
    byte ps2Type;
    byte vibrate;
    using InputController::_onStartButtonPressed;
    using InputController::_onSelectButtonPressed;
    using InputController::_onDPadUpButtonPressed;
    using InputController::_onDPadRightButtonPressed;
    using InputController::_onDPadDownButtonPressed;
    using InputController::_onDPadLeftButtonPressed;
    using InputController::_onLeftJoystickChanged;
    using InputController::_onRightJoystickChanged;
};

class PS2Controller: public PS2Controller_ {
  public:
    PS2Controller(): PS2Controller_() {};
    void set(EventTrigger* eventTrigger);
    int check();
  private:
    EventTrigger* _eventTrigger = NULL;
    bool isJoystickChanged(int, int);
    int processStartButtonPress();
    int processSelectButtonPress();
    int processDPadUpButtonPress();
    int processDPadRightButtonPress();
    int processDPadDownButtonPress();
    int processDPadLeftButtonPress();
    int processJoystickChange(byte, byte, const char label);
};

#endif
