#include "IR_Listener.h"
#include "IR_Mapping_Code.h"
#include <IRremote.hpp>

#ifndef RECV_PIN
#define RECV_PIN 2
#endif

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

uint32_t detectButtonPress(IRData, IRCodeMapper*);

IRListener::IRListener(IRCodeMapper* irCodeMapper) {
  _debugEnabled = true;
  _irCodeMapper = irCodeMapper;
}

void IRListener::begin() {
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  //
  #if __LOADING_LOG_ENABLED__
  if (_debugEnabled) {
    char buf[7];
    debugLog("IR is now running on Pin", " ", itoa(RECV_PIN, buf, 10));
  }
  #endif
}

int IRListener::loop() {
  if (!IrReceiver.decode()) {
    return 0;
  }

  uint16_t buttons = detectButtonPress(IrReceiver.decodedIRData, _irCodeMapper);
  uint16_t checked = processButtonPress(buttons);

  IrReceiver.resume();

  if (checked) {
    return 1;
  }

  return -1;
}

void IRListener::setCodeMapper(IRCodeMapper* irCodeMapper) {
  _irCodeMapper = irCodeMapper;
}

void IRListener::set(EventDispatcher* eventDispatcher) {
  _eventDispatcher = eventDispatcher;
};

uint32_t IRListener::processButtonPress(uint32_t pressed) {
  uint32_t checked = 0;

  if(pressed & IR_MASK_OK_BUTTON && _onOkButtonPressed) {
    #if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Ok", "ButtonPressed", "()", " is called");
    }
    #endif
    _onOkButtonPressed();
    checked |= IR_MASK_OK_BUTTON;
  }

  if(pressed & IR_MASK_UP_BUTTON) {
    if (_eventDispatcher || _onDPadUpButtonPressed) {
      #if __RUNNING_LOG_ENABLED__
      if (_debugEnabled) {
        debugLog("on", "DPad", "Up", "ButtonPressed", "()", " is called");
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
      #if __RUNNING_LOG_ENABLED__
      if (_debugEnabled) {
        debugLog("on", "DPad", "Right", "ButtonPressed", "()", " is called");
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
      #if __RUNNING_LOG_ENABLED__
      if (_debugEnabled) {
        debugLog("on", "DPad", "Down", "ButtonPressed", "()", " is called");
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
      #if __RUNNING_LOG_ENABLED__
      if (_debugEnabled) {
        debugLog("on", "DPad", "Left", "ButtonPressed", "()", " is called");
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

  if(pressed & IR_MASK_ASTERISK_BUTTON && _onAsteriskButtonPressed) {
    #if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Asterisk", "ButtonPressed", "()", " is called");
    }
    #endif
    _onAsteriskButtonPressed();
    checked |= IR_MASK_ASTERISK_BUTTON;
  }

  if(pressed & IR_MASK_SHARP_BUTTON && _onSharpButtonPressed) {
    #if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Sharp", "ButtonPressed", "()", " is called");
    }
    #endif
    _onSharpButtonPressed();
    checked |= IR_MASK_SHARP_BUTTON;
  }

  if(pressed & IR_MASK_DIGITS_BUTTON && _onDigitButtonPressed) {
    #if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Digit", "ButtonPressed", "()", " is called");
    }
    #endif
    _onDigitButtonPressed(pressed & IR_MASK_DIGITS_BUTTON);
    checked |= (pressed & IR_MASK_DIGITS_BUTTON);
  }

  if(pressed && _onAnyButtonPressed) {
    #if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Any", "ButtonPressed", "()", " is called");
    }
    #endif
    _onAnyButtonPressed(pressed);
    checked |= pressed;
  }

  return checked;
}

uint32_t detectButtonPress(IRData decodedIRData, IRCodeMapper* _irCodeMapper) {
  uint32_t buttons = 0;

  if (_irCodeMapper != NULL) {
    uint8_t pos = _irCodeMapper->getPosition((uint8_t)decodedIRData.protocol, decodedIRData.command);
    if (_irCodeMapper->isValid(pos)) {
      buttons |= (1UL << pos);
      #if __RUNNING_LOG_ENABLED__
      char pos_[7];
      debugLog("Buttons", ": ", itoa(pos, pos_, 10), " is pushed");
      #endif
      return buttons;
    } else {
      #if __RUNNING_LOG_ENABLED__
      IrReceiver.printIRResultShort(&Serial);
      #endif
    }
  }

  return buttons;
}

void IRListener::setOnOkButtonPressed(void (*function)()) {
  _onOkButtonPressed = function;
};

void IRListener::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void IRListener::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void IRListener::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void IRListener::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void IRListener::setOnAsteriskButtonPressed(void (*function)()) {
  _onAsteriskButtonPressed = function;
};

void IRListener::setOnSharpButtonPressed(void (*function)()) {
  _onSharpButtonPressed = function;
};

void IRListener::setOnDigitButtonPressed(void (*function)(uint32_t)) {
  _onDigitButtonPressed = function;
};

void IRListener::setOnAnyButtonPressed(void (*function)(uint32_t)) {
  _onAnyButtonPressed = function;
};

void show(IRData decodedIRData, HardwareSerial *serial) {
  switch (decodedIRData.protocol) {
    case NEC: Serial.println("NEC"); break;
    case SONY: Serial.println("SONY"); break;
    case RC5: Serial.print("RC"), Serial.println("5"); break;
    case RC6: Serial.print("RC"), Serial.println("6"); break;
    case SHARP: Serial.println("SHARP"); break;
    case JVC: Serial.println("JVC"); break;
    case SAMSUNG: Serial.println("SAMSUNG"); break;
    case LG: Serial.println("LG"); break;
    case WHYNTER: Serial.println("WHYNTER"); break;
    case PANASONIC: Serial.println("PANASONIC"); break;
    case DENON: Serial.println("DENON"); break;
    case UNKNOWN: Serial.println("UNKNOWN"); break;
    default:
      Serial.print("Unknown [");
      Serial.print(decodedIRData.protocol);
      Serial.println("]");
  }
}
