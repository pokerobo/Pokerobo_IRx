#include "IR_Processor.h"

#define IR_MASK_UP_BUTTON       1UL << 0
#define IR_MASK_RIGHT_BUTTON    1UL << 1
#define IR_MASK_DOWN_BUTTON     1UL << 2
#define IR_MASK_LEFT_BUTTON     1UL << 3
#define IR_MASK_OK_BUTTON       1UL << 4
#define IR_MASK_ASTERISK_BUTTON 1UL << 5
#define IR_MASK_SHARP_BUTTON    1UL << 6
#define IR_MASK_DIGIT0_BUTTON   1UL << 7
#define IR_MASK_DIGIT1_BUTTON   1UL << 8
#define IR_MASK_DIGIT2_BUTTON   1UL << 9
#define IR_MASK_DIGIT3_BUTTON   1UL << 10
#define IR_MASK_DIGIT4_BUTTON   1UL << 11
#define IR_MASK_DIGIT5_BUTTON   1UL << 12
#define IR_MASK_DIGIT6_BUTTON   1UL << 13
#define IR_MASK_DIGIT7_BUTTON   1UL << 14
#define IR_MASK_DIGIT8_BUTTON   1UL << 15
#define IR_MASK_DIGIT9_BUTTON   1UL << 16

#define IR_MASK_DIGITS_BUTTON IR_MASK_DIGIT0_BUTTON |\
                          IR_MASK_DIGIT1_BUTTON |\
                          IR_MASK_DIGIT2_BUTTON |\
                          IR_MASK_DIGIT3_BUTTON |\
                          IR_MASK_DIGIT4_BUTTON |\
                          IR_MASK_DIGIT5_BUTTON |\
                          IR_MASK_DIGIT6_BUTTON |\
                          IR_MASK_DIGIT7_BUTTON |\
                          IR_MASK_DIGIT8_BUTTON |\
                          IR_MASK_DIGIT9_BUTTON

//-------------------------------------------------------------------------------------------------

void IRProcessor::set(IRDebugLogger* logger) {
  _logger = logger;
}

void IRProcessor::setDebugEnabled(bool enabled) {
  _debugEnabled = enabled;
}

bool IRProcessor::isDebugEnabled() {
  return _debugEnabled && _logger != NULL;
}

//-------------------------------------------------------------------------------------------------

uint32_t IREventDispatcher::processButtonPress(uint32_t pressed) {
  uint32_t checked = 0;

  if(pressed & IR_MASK_OK_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "Ok", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processOkButtonPressedEvent();
    checked |= IR_MASK_OK_BUTTON;
  }

  if(pressed & IR_MASK_UP_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "DPad", "Up", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processDPadUpButtonPressedEvent();
    checked |= IR_MASK_UP_BUTTON;
  }

  if(pressed & IR_MASK_RIGHT_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "DPad", "Right", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processDPadRightButtonPressedEvent();
    checked |= IR_MASK_RIGHT_BUTTON;
  }

  if(pressed & IR_MASK_DOWN_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "DPad", "Down", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processDPadDownButtonPressedEvent();
    checked |= IR_MASK_DOWN_BUTTON;
  }

  if(pressed & IR_MASK_LEFT_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "DPad", "Left", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processDPadLeftButtonPressedEvent();
    checked |= IR_MASK_LEFT_BUTTON;
  }

  if(pressed & IR_MASK_ASTERISK_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "Asterisk", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processAsteriskButtonPressedEvent();
    checked |= IR_MASK_ASTERISK_BUTTON;
  }

  if(pressed & IR_MASK_SHARP_BUTTON) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "Sharp", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processSharpButtonPressedEvent();
    checked |= IR_MASK_SHARP_BUTTON;
  }

  uint32_t digitButtons = pressed & uint32_t(IR_MASK_DIGITS_BUTTON);
  if(digitButtons) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "Digit", "ButtonPressedEvent", "()", " is called");
    }
    #endif
    processDigitButtonsPressedEvent(digitButtons);
    checked |= (digitButtons);
  }

  uint32_t otherButtons = pressed >> 17;
  if(otherButtons) {
    #if POKEROBO_IRX_DEBUG_ENABLED
    if (isDebugEnabled()) {
      _logger->debug("process", "OtherButtons", "PressedEvent", "()", " is called");
    }
    #endif
    processOtherButtonsPressedEvent(otherButtons);
    checked |= (otherButtons << 17);
  }
}

void IREventDispatcher::processOkButtonPressedEvent() {}
void IREventDispatcher::processDPadUpButtonPressedEvent() {}
void IREventDispatcher::processDPadRightButtonPressedEvent() {}
void IREventDispatcher::processDPadDownButtonPressedEvent() {}
void IREventDispatcher::processDPadLeftButtonPressedEvent() {}
void IREventDispatcher::processAsteriskButtonPressedEvent() {}
void IREventDispatcher::processSharpButtonPressedEvent(){}
void IREventDispatcher::processDigitButtonsPressedEvent(uint32_t buttons) {}
void IREventDispatcher::processOtherButtonsPressedEvent(uint16_t buttons) {}

//-------------------------------------------------------------------------------------------------

uint32_t IREventTrigger::processButtonPress(uint32_t pressed) {
  uint32_t checked = 0;

  if(pressed & IR_MASK_OK_BUTTON) {
    if (_eventDispatcher || _onOkButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "Ok", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onOkButtonPressed) {
        _onOkButtonPressed();
      } else {
        _eventDispatcher->processOkButtonPressedEvent();
      }
      checked |= IR_MASK_OK_BUTTON;
    }
  }

  if(pressed & IR_MASK_UP_BUTTON) {
    if (_eventDispatcher || _onDPadUpButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "DPad", "Up", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onDPadUpButtonPressed) {
        _onDPadUpButtonPressed();
      } else {
        _eventDispatcher->processDPadUpButtonPressedEvent();
      }
      checked |= IR_MASK_UP_BUTTON;
    }
  }

  if(pressed & IR_MASK_RIGHT_BUTTON) {
    if (_eventDispatcher || _onDPadRightButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "DPad", "Right", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onDPadRightButtonPressed) {
        _onDPadRightButtonPressed();
      } else {
        _eventDispatcher->processDPadRightButtonPressedEvent();
      }
      checked |= IR_MASK_RIGHT_BUTTON;
    }
  }

  if(pressed & IR_MASK_DOWN_BUTTON) {
    if (_eventDispatcher || _onDPadDownButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "DPad", "Down", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onDPadDownButtonPressed) {
        _onDPadDownButtonPressed();
      } else {
        _eventDispatcher->processDPadDownButtonPressedEvent();
      }
      checked |= IR_MASK_DOWN_BUTTON;
    }
  }

  if(pressed & IR_MASK_LEFT_BUTTON) {
    if (_eventDispatcher || _onDPadLeftButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "DPad", "Left", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onDPadLeftButtonPressed) {
        _onDPadLeftButtonPressed();
      } else {
        _eventDispatcher->processDPadLeftButtonPressedEvent();
      }
      checked |= IR_MASK_LEFT_BUTTON;
    }
  }

  if(pressed & IR_MASK_ASTERISK_BUTTON) {
    if (_eventDispatcher || _onAsteriskButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "Asterisk", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onAsteriskButtonPressed) {
        _onAsteriskButtonPressed();
      } else {
        _eventDispatcher->processAsteriskButtonPressedEvent();
      }
      checked |= IR_MASK_ASTERISK_BUTTON;
    }
  }

  if(pressed & IR_MASK_SHARP_BUTTON) {
    if (_eventDispatcher || _onSharpButtonPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "Sharp", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onSharpButtonPressed) {
        _onSharpButtonPressed();
      } else {
        _eventDispatcher->processSharpButtonPressedEvent();
      }
      checked |= IR_MASK_SHARP_BUTTON;
    }
  }

  if(pressed & IR_MASK_DIGITS_BUTTON) {
    if (_eventDispatcher || _onDigitButtonsPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "Digit", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onDigitButtonsPressed) {
        _onDigitButtonsPressed(pressed & IR_MASK_DIGITS_BUTTON);
      } else {
        _eventDispatcher->processDigitButtonsPressedEvent(pressed & IR_MASK_DIGITS_BUTTON);
      }
      checked |= (pressed & IR_MASK_DIGITS_BUTTON);
    }
  }

  uint32_t otherButtons = pressed >> 17;
  if(otherButtons) {
    if (_eventDispatcher || _onOtherButtonsPressed) {
      #if POKEROBO_IRX_DEBUG_ENABLED
      if (isDebugEnabled()) {
        _logger->debug("on", "Other", "ButtonPressed", "()", " is called");
      }
      #endif
      if (_onOtherButtonsPressed) {
        _onOtherButtonsPressed(otherButtons);
      } else {
        _eventDispatcher->processOtherButtonsPressedEvent(otherButtons);
      }
      checked |= (otherButtons << 17);
    }
  }

  return checked;
}

void IREventTrigger::setOnOkButtonPressed(void (*function)()) {
  _onOkButtonPressed = function;
};

void IREventTrigger::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void IREventTrigger::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void IREventTrigger::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void IREventTrigger::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void IREventTrigger::setOnAsteriskButtonPressed(void (*function)()) {
  _onAsteriskButtonPressed = function;
};

void IREventTrigger::setOnSharpButtonPressed(void (*function)()) {
  _onSharpButtonPressed = function;
};

void IREventTrigger::setOnDigitButtonsPressed(void (*function)(uint32_t)) {
  _onDigitButtonsPressed = function;
};

void IREventTrigger::setOnOtherButtonsPressed(void (*function)(uint16_t)) {
  _onOtherButtonsPressed = function;
};
