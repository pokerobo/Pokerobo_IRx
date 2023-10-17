#ifndef __IR_CONTROLLER_H__
#define __IR_CONTROLLER_H__

#include "Commons.h"

class IRController {
  public:
    IRController();
    void begin();
    void setOnOkButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnAsteriskButtonPressed(void (*function)());
    void setOnSharpButtonPressed(void (*function)());
    void setOnDigitButtonPressed(void (*function)(uint32_t));
    void setOnAnyButtonPressed(void (*function)(uint32_t));
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
    void (*user_onAsteriskButtonPressed)();
    void (*user_onSharpButtonPressed)();
    void (*user_onDigitButtonPressed)(uint32_t buttons);
    void (*user_onAnyButtonPressed)(uint32_t buttons);
};

#endif
