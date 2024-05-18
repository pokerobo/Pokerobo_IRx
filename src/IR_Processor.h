#ifndef __POKEROPO_IRX_IR_PROCESSOR_H__
#define __POKEROPO_IRX_IR_PROCESSOR_H__

#include "IR_Commons.h"

class IRProcessor {
  public:
    virtual uint32_t processButtonPress(uint32_t buttons);
    void setDebugEnabled(bool enabled);
    void set(IRDebugLogger* logger);
  protected:
    bool _debugEnabled = false;
    IRDebugLogger* _logger = NULL;
    bool isDebugEnabled();
};

class IREventDispatcher: public IRProcessor {
  public:
    uint32_t processButtonPress(uint32_t buttons);
    virtual void processOkButtonPressedEvent();
    virtual void processDPadUpButtonPressedEvent();
    virtual void processDPadRightButtonPressedEvent();
    virtual void processDPadDownButtonPressedEvent();
    virtual void processDPadLeftButtonPressedEvent();
    virtual void processAsteriskButtonPressedEvent();
    virtual void processSharpButtonPressedEvent();
    virtual void processDigitButtonsPressedEvent(uint32_t buttons);
    virtual void processOtherButtonsPressedEvent(uint16_t buttons);
};

class IREventTrigger: public IRProcessor {
  public:
    IREventTrigger(IREventDispatcher* eventDispatcher=NULL) {
      _eventDispatcher = eventDispatcher;
    }
    uint32_t processButtonPress(uint32_t buttons);
    void setOnOkButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnAsteriskButtonPressed(void (*function)());
    void setOnSharpButtonPressed(void (*function)());
    void setOnDigitButtonsPressed(void (*function)(uint32_t));
    void setOnOtherButtonsPressed(void (*function)(uint16_t));
  private:
    void (*_onOkButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onAsteriskButtonPressed)();
    void (*_onSharpButtonPressed)();
    void (*_onDigitButtonsPressed)(uint32_t buttons);
    void (*_onOtherButtonsPressed)(uint16_t buttons);
    IREventDispatcher* _eventDispatcher = NULL;
};

#endif
