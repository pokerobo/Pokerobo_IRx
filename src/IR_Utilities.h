#ifndef __POKEROPO_IRX_IR_UTILITIES_H__
#define __POKEROPO_IRX_IR_UTILITIES_H__

#include "IR_Processor.h"

class IRSerialPrintDispatcher: public IREventDispatcher {
  public:
    using IREventDispatcher::IREventDispatcher;
    void processOkButtonPressedEvent();
    void processDPadUpButtonPressedEvent();
    void processDPadRightButtonPressedEvent();
    void processDPadDownButtonPressedEvent();
    void processDPadLeftButtonPressedEvent();
    void processAsteriskButtonPressedEvent();
    void processSharpButtonPressedEvent();
    void processDigitButtonsPressedEvent(uint32_t buttons);
    void processOtherButtonsPressedEvent(uint16_t buttons);
};

#endif
