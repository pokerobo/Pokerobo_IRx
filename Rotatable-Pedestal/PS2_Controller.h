// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#include <PS2X_lib.h>

#ifndef __PS2_CONTROLLER_H__
#define __PS2_CONTROLLER_H__

#define MIN_BOUND_X      5
#define MIN_BOUND_Y      5

#define NUM_RANGE_X      10
#define NUM_RANGE_Y      10

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
    void onStartButtonPressed(void (*function)());
    void onDPadButtonPressed(void (*function)(uint16_t));
    void onLeftJoystickChanged(void (*function)(int, int));
    void onRightJoystickChanged(void (*function)(int, int));
    int check();
  protected:
    int processStartButtonPress();
    int processPadButtonPress(uint16_t button, const char buttonLabel[]);
    int processJoystickButton(byte, byte, void (*function)(int, int), const char c[]);
  private:
    PS2X ps2x;
    bool debugEnabled;
    int errorCode;
    bool errorDisplayed;
    byte ps2Type;
    byte vibrate;
    void (*user_onStartButtonPressed)();
    void (*user_onDPadButtonPressed)(uint16_t);
    void (*user_onLeftJoystickChanged)(int, int);
    void (*user_onRightJoystickChanged)(int, int);
};

#endif
