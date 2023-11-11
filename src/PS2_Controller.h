// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#ifndef __PS2_CONTROLLER_H__
#define __PS2_CONTROLLER_H__

#include "Commons.h"
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

class PS2Controller {
  public:
    PS2Controller();
    void begin();
    int loop();
    bool hasError();
    void showError();
    void reload();
    void set(EventTrigger* eventTrigger);
    void setOnStartButtonPressed(void (*function)());
    void setOnSelectButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnnLeftJoystickChanged(void (*function)(int, int));
    void setOnRightJoystickChanged(void (*function)(int, int));
    int check();
  protected:
    bool isJoystickChanged(int, int);
    int processStartButtonPress();
    int processSelectButtonPress();
    int processDPadUpButtonPress();
    int processDPadRightButtonPress();
    int processDPadDownButtonPress();
    int processDPadLeftButtonPress();
    int processJoystickChange(byte, byte, const char label);
  private:
    PS2X ps2x;
    bool debugEnabled;
    int errorCode;
    bool errorDisplayed;
    byte ps2Type;
    byte vibrate;
    EventTrigger* _eventTrigger = NULL;
    void (*_onStartButtonPressed)();
    void (*_onSelectButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onDPadButtonPressed)(uint16_t);
    void (*_onLeftJoystickChanged)(int, int);
    void (*_onRightJoystickChanged)(int, int);
};

#endif
