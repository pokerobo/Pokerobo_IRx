#include "Commons.h"

#ifndef __IR_CONTROLLER_H__
#define __IR_CONTROLLER_H__

class IRController {
  public:
    IRController();
    void begin();
    void setOnOkButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnButtonsPressed(void (*function)(uint32_t));
    int loop();
  protected:
    uint32_t processButtonPress(uint32_t buttons);
  private:
    bool debugEnabled;
    void (*user_onOkButtonPressed)();
    void (*user_onDPadUpButtonPressed)();
    void (*user_onDPadRightButtonPressed)();
    void (*user_onDPadDownButtonPressed)();
    void (*user_onDPadLeftButtonPressed)();
    void (*user_onButtonsPressed)(uint32_t command);
};

#endif
