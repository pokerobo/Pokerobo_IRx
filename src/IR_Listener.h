#ifndef __IR_LISTENER_H__
#define __IR_LISTENER_H__

#include "Commons.h"
#include "IR_Mapping_Code.h"
#include "Event_Trigger.h"

class IRListener {
  public:
    IRListener(IRCodeMapper* irCodeMapper=NULL);
    void begin();
    void setCodeMapper(IRCodeMapper* irCodeMapper);
    void set(EventTrigger* eventTrigger);
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
    bool _debugEnabled;
    IRCodeMapper* _irCodeMapper = NULL;
    EventTrigger* _eventTrigger = NULL;
    void (*_onOkButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onAsteriskButtonPressed)();
    void (*_onSharpButtonPressed)();
    void (*_onDigitButtonPressed)(uint32_t buttons);
    void (*_onAnyButtonPressed)(uint32_t buttons);
};

#endif
