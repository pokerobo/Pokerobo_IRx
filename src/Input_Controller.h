#ifndef __INPUT_CONTROLLER_H__
#define __INPUT_CONTROLLER_H__

#include "Commons.h"

#define MASK_UP_BUTTON     1U << 0
#define MASK_RIGHT_BUTTON  1U << 1
#define MASK_DOWN_BUTTON   1U << 2
#define MASK_LEFT_BUTTON   1U << 3
#define MASK_START_BUTTON  1U << 4
#define MASK_SELECT_BUTTON 1U << 5
#define MASK_ANALOG_BUTTON 1U << 6

class InputController {
  public:
    void setOnAnalogButtonPressed(void (*function)());
    void setOnStartButtonPressed(void (*function)());
    void setOnSelectButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnDPadButtonPressed(void (*function)(uint16_t));
    void setOnLeftJoystickChanged(void (*function)(int, int));
    void setOnRightJoystickChanged(void (*function)(int, int));
  protected:
    bool checkButtonPress(uint16_t pressed, uint16_t mask);
    uint16_t processButtonPress(uint16_t buttons);
    int processJoystickChange(int, int, char);
    virtual bool isJoystickChanged(int, int);
    virtual int adjustJoystickX(int nJoyX);
    virtual int adjustJoystickY(int nJoyY);
    void (*_onAnalogButtonPressed)();
    void (*_onStartButtonPressed)();
    void (*_onSelectButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onDPadButtonPressed)(uint16_t);
    void (*_onLeftJoystickChanged)(int, int);
    void (*_onRightJoystickChanged)(int, int);
  private:
    uint16_t _clickingTrail;
};

#endif
