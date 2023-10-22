#include "IR_Controller.h"
#include "IR_Code_Mapping.h"
#include <IRremote.hpp>

#define RECV_PIN 2

#define BIT_UP_BUTTON       1UL << 0
#define BIT_RIGHT_BUTTON    1UL << 1
#define BIT_DOWN_BUTTON     1UL << 2
#define BIT_LEFT_BUTTON     1UL << 3
#define BIT_OK_BUTTON       1UL << 4
#define BIT_ASTERISK_BUTTON 1UL << 5
#define BIT_SHARP_BUTTON    1UL << 6
#define BIT_DIGIT0_BUTTON   1UL << 7
#define BIT_DIGIT1_BUTTON   1UL << 8
#define BIT_DIGIT2_BUTTON   1UL << 9
#define BIT_DIGIT3_BUTTON   1UL << 10
#define BIT_DIGIT4_BUTTON   1UL << 11
#define BIT_DIGIT5_BUTTON   1UL << 12
#define BIT_DIGIT6_BUTTON   1UL << 13
#define BIT_DIGIT7_BUTTON   1UL << 14
#define BIT_DIGIT8_BUTTON   1UL << 15
#define BIT_DIGIT9_BUTTON   1UL << 16

#define BIT_DIGITS_BUTTON BIT_DIGIT0_BUTTON |\
                          BIT_DIGIT1_BUTTON |\
                          BIT_DIGIT2_BUTTON |\
                          BIT_DIGIT3_BUTTON |\
                          BIT_DIGIT4_BUTTON |\
                          BIT_DIGIT5_BUTTON |\
                          BIT_DIGIT6_BUTTON |\
                          BIT_DIGIT7_BUTTON |\
                          BIT_DIGIT8_BUTTON |\
                          BIT_DIGIT9_BUTTON

uint32_t detectButtonPress(IRData, IRCodeMapper*);

IRController::IRController() {
  _debugEnabled = true;
  _irCodeMapper = NULL;
}

IRController::IRController(IRCodeMapper* irCodeMapper) {
  _debugEnabled = true;
  _irCodeMapper = irCodeMapper;
}

void IRController::begin() {
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  //
  debugLog("IR is now running on Pin ", RECV_PIN);
}

int IRController::loop() {
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

void IRController::setCodeMapper(IRCodeMapper* irCodeMapper) {
  _irCodeMapper = irCodeMapper;
}

uint32_t IRController::processButtonPress(uint32_t pressed) {
  uint32_t checked = 0;

  if(pressed & BIT_OK_BUTTON && user_onOkButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Ok", "ButtonPressed", "()", " is called");
    }
#endif
    user_onOkButtonPressed();
    checked |= BIT_OK_BUTTON;
  }

  if(pressed & BIT_UP_BUTTON && user_onDPadUpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "DPad", "Up", "ButtonPressed", "()", " is called");
    }
#endif
    user_onDPadUpButtonPressed();
    checked |= BIT_UP_BUTTON;
  }

  if(pressed & BIT_RIGHT_BUTTON && user_onDPadRightButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "DPad", "Right", "ButtonPressed", "()", " is called");
    }
#endif
    user_onDPadRightButtonPressed();
    checked |= BIT_RIGHT_BUTTON;
  }

  if(pressed & BIT_DOWN_BUTTON && user_onDPadDownButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "DPad", "Down", "ButtonPressed", "()", " is called");
    }
#endif
    user_onDPadDownButtonPressed();
    checked |= BIT_DOWN_BUTTON;
  }

  if(pressed & BIT_LEFT_BUTTON && user_onDPadLeftButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "DPad", "Left", "ButtonPressed", "()", " is called");
    }
#endif
    user_onDPadLeftButtonPressed();
    checked |= BIT_LEFT_BUTTON;
  }

  if(pressed & BIT_ASTERISK_BUTTON && user_onAsteriskButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Asterisk", "ButtonPressed", "()", " is called");
    }
#endif
    user_onAsteriskButtonPressed();
    checked |= BIT_ASTERISK_BUTTON;
  }

  if(pressed & BIT_SHARP_BUTTON && user_onSharpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Sharp", "ButtonPressed", "()", " is called");
    }
#endif
    user_onSharpButtonPressed();
    checked |= BIT_SHARP_BUTTON;
  }

  if(pressed & BIT_DIGITS_BUTTON && user_onDigitButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Digit", "ButtonPressed", "()", " is called");
    }
#endif
    user_onDigitButtonPressed(pressed & BIT_DIGITS_BUTTON);
    checked |= (pressed & BIT_DIGITS_BUTTON);
  }

  if(pressed && user_onAnyButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("on", "Any", "ButtonPressed", "()", " is called");
    }
#endif
    user_onAnyButtonPressed(pressed);
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
      debugLog("Buttons", ": ", pos, " is pressed");
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

void IRController::setOnOkButtonPressed(void (*function)()) {
  user_onOkButtonPressed = function;
};

void IRController::setOnDPadUpButtonPressed(void (*function)()) {
  user_onDPadUpButtonPressed = function;
};

void IRController::setOnDPadRightButtonPressed(void (*function)()) {
  user_onDPadRightButtonPressed = function;
};

void IRController::setOnDPadDownButtonPressed(void (*function)()) {
  user_onDPadDownButtonPressed = function;
};

void IRController::setOnDPadLeftButtonPressed(void (*function)()) {
  user_onDPadLeftButtonPressed = function;
};

void IRController::setOnAsteriskButtonPressed(void (*function)()) {
  user_onAsteriskButtonPressed = function;
};

void IRController::setOnSharpButtonPressed(void (*function)()) {
  user_onSharpButtonPressed = function;
};

void IRController::setOnDigitButtonPressed(void (*function)(uint32_t)) {
  user_onDigitButtonPressed = function;
};

void IRController::setOnAnyButtonPressed(void (*function)(uint32_t)) {
  user_onAnyButtonPressed = function;
};

void show(IRData decodedIRData, HardwareSerial *serial) {
  switch (decodedIRData.protocol) {
    case NEC: Serial.println("NEC"); break;
    case SONY: Serial.println("SONY"); break;
    case RC5: Serial.println("RC5"); break;
    case RC6: Serial.println("RC6"); break;
    case SHARP: Serial.println("SHARP"); break;
    case JVC: Serial.println("JVC"); break;
    case SAMSUNG: Serial.println("SAMSUNG"); break;
    case LG: Serial.println("LG"); break;
    case WHYNTER: Serial.println("WHYNTER"); break;
    case PANASONIC: Serial.println("PANASONIC"); break;
    case DENON: Serial.println("DENON"); break;
    case UNKNOWN: Serial.println("UNKNOWN"); break;
    default:
      Serial.print("Unknown protocol [");
      Serial.print(decodedIRData.protocol);
      Serial.println("]");
  }
}
