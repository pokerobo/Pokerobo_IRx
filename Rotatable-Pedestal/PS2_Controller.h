// PS2 Tank by Igor Fonseca @2019
// Controls a robotic tank using a PS2 joystick, using D-pad buttons
// based on an example using the PS2X library by Bill Porter 2011
// All text above must be included in any redistribution.

// include libraries
#include <PS2X_lib.h>

#ifndef __PS2_CONTROLLER_H__
#define __PS2_CONTROLLER_H__

#define DELTA_X      5
#define DELTA_Y      5

#define GAMEPAD_ATTENTION   10
#define GAMEPAD_COMMAND     11
#define GAMEPAD_DATA        12
#define GAMEPAD_CLOCK       13

static void (*user_onDPadButtonPressed)(uint16_t);

class PS2Controller {
  public:
    PS2Controller();
    void begin();
    bool hasError();
    void showError();
    void reload();
    void onDPadButtonPressed(void (*function)(uint16_t));
    int check();
  private:
    PS2X ps2x;
    int errorCode;
    bool errorDisplayed;
    byte ps2Type;
    byte vibrate;
};

#endif
