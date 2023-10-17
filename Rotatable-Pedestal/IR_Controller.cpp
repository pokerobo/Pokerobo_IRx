#include "IR_Controller.h"
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

uint32_t detectButtonPress(IRData);

IRController::IRController() {
  debugEnabled = true;
}

void IRController::begin() {
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  //
  Serial.print("IR is now running and waiting for message on Pin ");
  Serial.println(RECV_PIN);
}

int IRController::loop() {
  if (!IrReceiver.decode()) {
    return 0;
  }

  uint16_t buttons = detectButtonPress(IrReceiver.decodedIRData);
  uint16_t checked = processButtonPress(buttons);

  IrReceiver.resume();

  if (checked) {
    return 1;
  }

  return -1;
}

uint32_t IRController::processButtonPress(uint32_t pressed) {
  uint32_t checked = 0;

  if(pressed & BIT_OK_BUTTON && user_onOkButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onOkButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onOkButtonPressed();
    checked |= BIT_OK_BUTTON;
  }

  if(pressed & BIT_UP_BUTTON && user_onDPadUpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onDPadUpButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onDPadUpButtonPressed();
    checked |= BIT_UP_BUTTON;
  }

  if(pressed & BIT_RIGHT_BUTTON && user_onDPadRightButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onDPadRightButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onDPadRightButtonPressed();
    checked |= BIT_RIGHT_BUTTON;
  }

  if(pressed & BIT_DOWN_BUTTON && user_onDPadDownButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onDPadDownButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onDPadDownButtonPressed();
    checked |= BIT_DOWN_BUTTON;
  }

  if(pressed & BIT_LEFT_BUTTON && user_onDPadLeftButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onDPadLeftButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onDPadLeftButtonPressed();
    checked |= BIT_LEFT_BUTTON;
  }

  if(pressed & BIT_ASTERISK_BUTTON && user_onAsteriskButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onAsteriskButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onAsteriskButtonPressed();
    checked |= BIT_ASTERISK_BUTTON;
  }

  if(pressed & BIT_SHARP_BUTTON && user_onSharpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onSharpButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onSharpButtonPressed();
    checked |= BIT_SHARP_BUTTON;
  }

  if(pressed & BIT_DIGITS_BUTTON && user_onDigitButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onDigitButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onDigitButtonPressed(pressed & BIT_DIGITS_BUTTON);
    checked |= (pressed & BIT_DIGITS_BUTTON);
  }

  if(pressed && user_onAnyButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onAnyButtonPressed"), Serial.print("()"), Serial.println(" is called");
    }
#endif
    user_onAnyButtonPressed(pressed);
    checked |= pressed;
  }

  return checked;
}

uint32_t detectButtonPress(IRData decodedIRData) {
  uint32_t buttons = 0;

  switch (decodedIRData.protocol) {
    case SONY:
      switch (decodedIRData.command) {
        case 0x65:
#if __RUNNING_LOG_ENABLED__
          Serial.println("OK");
#endif
          buttons |= BIT_OK_BUTTON;
          break;
        case 0x74:
#if __RUNNING_LOG_ENABLED__
          Serial.println("UP");
#endif
          buttons |= BIT_UP_BUTTON;
          break;
        case 0x33:
#if __RUNNING_LOG_ENABLED__
          Serial.println("RIGHT");
#endif
          buttons |= BIT_RIGHT_BUTTON;
          break;
        case 0x75:
#if __RUNNING_LOG_ENABLED__
          Serial.println("DOWN");
#endif
          buttons |= BIT_DOWN_BUTTON;
          break;
        case 0x34:
#if __RUNNING_LOG_ENABLED__
          Serial.println("LEFT");
#endif
          buttons |= BIT_LEFT_BUTTON;
          break;

        case 0x00:
#if __RUNNING_LOG_ENABLED__
          Serial.println("1");
#endif
          buttons |= BIT_DIGIT1_BUTTON;
          break;
        case 0x01:
#if __RUNNING_LOG_ENABLED__
          Serial.println("2");
#endif
          buttons |= BIT_DIGIT2_BUTTON;
          break;
        case 0x02:
#if __RUNNING_LOG_ENABLED__
          Serial.println("3");
#endif
          buttons |= BIT_DIGIT3_BUTTON;
          break;
        case 0x03:
#if __RUNNING_LOG_ENABLED__
          Serial.println("4");
#endif
          buttons |= BIT_DIGIT4_BUTTON;
          break;
        case 0x04:
#if __RUNNING_LOG_ENABLED__
          Serial.println("5");
#endif
          buttons |= BIT_DIGIT5_BUTTON;
          break;
        case 0x05:
#if __RUNNING_LOG_ENABLED__
          Serial.println("6");
#endif
          buttons |= BIT_DIGIT6_BUTTON;
          break;
        case 0x06:
#if __RUNNING_LOG_ENABLED__
          Serial.println("7");
#endif
          buttons |= BIT_DIGIT7_BUTTON;
          break;
        case 0x07:
#if __RUNNING_LOG_ENABLED__
          Serial.println("8");
#endif
          buttons |= BIT_DIGIT8_BUTTON;
          break;
        case 0x08:
#if __RUNNING_LOG_ENABLED__
          Serial.println("9");
#endif
          buttons |= BIT_DIGIT9_BUTTON;
          break;
        case 0x09:
#if __RUNNING_LOG_ENABLED__
          Serial.println("0");
#endif
          buttons |= BIT_DIGIT0_BUTTON;
          break;

        case 0x3A:
#if __RUNNING_LOG_ENABLED__
          Serial.println("ASTERISK");
#endif
          buttons |= BIT_ASTERISK_BUTTON;
          break;
        case 0x3F:
#if __RUNNING_LOG_ENABLED__
          Serial.println("SHARP");
#endif
          buttons |= BIT_SHARP_BUTTON;
          break;

        default:
          NULL;
#if __RUNNING_LOG_ENABLED__
          IrReceiver.printIRResultShort(&Serial);
#endif
      };
      break;

    case PANASONIC:
      switch (decodedIRData.command) {
        case 0x49:
#if __RUNNING_LOG_ENABLED__
          Serial.println("OK");
#endif
          buttons |= BIT_OK_BUTTON;
          break;
        case 0x34:
#if __RUNNING_LOG_ENABLED__
          Serial.println("UP");
#endif
          buttons |= BIT_UP_BUTTON;
          break;
        case 0x20:
#if __RUNNING_LOG_ENABLED__
          Serial.println("RIGHT");
#endif
          buttons |= BIT_RIGHT_BUTTON;
          break;
        case 0x35:
#if __RUNNING_LOG_ENABLED__
          Serial.println("DOWN");
#endif
          buttons |= BIT_DOWN_BUTTON;
          break;
        case 0x21:
#if __RUNNING_LOG_ENABLED__
          Serial.println("LEFT");
#endif
          buttons |= BIT_LEFT_BUTTON;
          break;

        case 0x10:
#if __RUNNING_LOG_ENABLED__
          Serial.println("1");
#endif
          buttons |= BIT_DIGIT1_BUTTON;
          break;
        case 0x11:
#if __RUNNING_LOG_ENABLED__
          Serial.println("2");
#endif
          buttons |= BIT_DIGIT2_BUTTON;
          break;
        case 0x12:
#if __RUNNING_LOG_ENABLED__
          Serial.println("3");
#endif
          buttons |= BIT_DIGIT3_BUTTON;
          break;
        case 0x13:
#if __RUNNING_LOG_ENABLED__
          Serial.println("4");
#endif
          buttons |= BIT_DIGIT4_BUTTON;
          break;
        case 0x14:
#if __RUNNING_LOG_ENABLED__
          Serial.println("5");
#endif
          buttons |= BIT_DIGIT5_BUTTON;
          break;
        case 0x15:
#if __RUNNING_LOG_ENABLED__
          Serial.println("6");
#endif
          buttons |= BIT_DIGIT6_BUTTON;
          break;
        case 0x16:
#if __RUNNING_LOG_ENABLED__
          Serial.println("7");
#endif
          buttons |= BIT_DIGIT7_BUTTON;
          break;
        case 0x17:
#if __RUNNING_LOG_ENABLED__
          Serial.println("8");
#endif
          buttons |= BIT_DIGIT8_BUTTON;
          break;
        case 0x18:
#if __RUNNING_LOG_ENABLED__
          Serial.println("9");
#endif
          buttons |= BIT_DIGIT9_BUTTON;
          break;
        case 0x19:
#if __RUNNING_LOG_ENABLED__
          Serial.println("0");
#endif
          buttons |= BIT_DIGIT0_BUTTON;
          break;

        case 0x39:
#if __RUNNING_LOG_ENABLED__
          Serial.println("ASTERISK");
#endif
          buttons |= BIT_ASTERISK_BUTTON;
          break;
        case 0x3B:
#if __RUNNING_LOG_ENABLED__
          Serial.println("SHARP");
#endif
          buttons |= BIT_SHARP_BUTTON;
          break;

        default:
          NULL;
#if __RUNNING_LOG_ENABLED__
          IrReceiver.printIRResultShort(&Serial);
#endif
      };
      break;

    case NEC:
      switch(decodedIRData.command) {
        case 0x19:
#if __RUNNING_LOG_ENABLED__
          Serial.println("0");
#endif
          buttons |= BIT_DIGIT0_BUTTON;
          break;
        case 0x45:
#if __RUNNING_LOG_ENABLED__
          Serial.println("1");
#endif
          buttons |= BIT_DIGIT1_BUTTON;
          break;
        case 0x46:
#if __RUNNING_LOG_ENABLED__
          Serial.println("2");
#endif
          buttons |= BIT_DIGIT2_BUTTON;
          break;
        case 0x47:
#if __RUNNING_LOG_ENABLED__
          Serial.println("3");
#endif
          buttons |= BIT_DIGIT3_BUTTON;
          break;
        case 0x44:
#if __RUNNING_LOG_ENABLED__
          Serial.println("4");
#endif
          buttons |= BIT_DIGIT4_BUTTON;
          break;
        case 0x40:
#if __RUNNING_LOG_ENABLED__
          Serial.println("5");
#endif
          buttons |= BIT_DIGIT5_BUTTON;
          break;
        case 0x43:
#if __RUNNING_LOG_ENABLED__
          Serial.println("6");
#endif
          buttons |= BIT_DIGIT6_BUTTON;
          break;
        case 0x07:
#if __RUNNING_LOG_ENABLED__
          Serial.println("7");
#endif
          buttons |= BIT_DIGIT7_BUTTON;
          break;
        case 0x15:
#if __RUNNING_LOG_ENABLED__
          Serial.println("8");
#endif
          buttons |= BIT_DIGIT8_BUTTON;
          break;
        case 0x09:
#if __RUNNING_LOG_ENABLED__
          Serial.println("9");
#endif
          buttons |= BIT_DIGIT9_BUTTON;
          break;
        case 0x1C:
#if __RUNNING_LOG_ENABLED__
          Serial.println("OK");
#endif
          buttons |= BIT_OK_BUTTON;
          break;
        case 0x16:
#if __RUNNING_LOG_ENABLED__
          Serial.println("*");
#endif
          buttons |= BIT_ASTERISK_BUTTON;
          break;
        case 0x0D:
#if __RUNNING_LOG_ENABLED__
          Serial.println("#");
#endif
          buttons |= BIT_SHARP_BUTTON;
          break;
        case 0x18:
#if __RUNNING_LOG_ENABLED__
          Serial.println("UP");
#endif
          buttons |= BIT_UP_BUTTON;
          break;
        case 0x5A:
#if __RUNNING_LOG_ENABLED__
          Serial.println("RIGHT");
#endif
          buttons |= BIT_RIGHT_BUTTON;
          break;
        case 0x52:
#if __RUNNING_LOG_ENABLED__
          Serial.println("DOWN");
#endif
          buttons |= BIT_DOWN_BUTTON;
          break;
        case 0x08:
#if __RUNNING_LOG_ENABLED__
          Serial.println("LEFT");
#endif
          buttons |= BIT_LEFT_BUTTON;
          break;
        default:
          NULL;
#if __RUNNING_LOG_ENABLED__
          Serial.println("NOOP");
#endif
      }
      break;
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
