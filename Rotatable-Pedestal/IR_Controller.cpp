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
#define BIT_0_BUTTON        1UL << 7
#define BIT_1_BUTTON        1UL << 8
#define BIT_2_BUTTON        1UL << 9
#define BIT_3_BUTTON        1UL << 10
#define BIT_4_BUTTON        1UL << 11
#define BIT_5_BUTTON        1UL << 12
#define BIT_6_BUTTON        1UL << 13
#define BIT_7_BUTTON        1UL << 14
#define BIT_8_BUTTON        1UL << 15
#define BIT_9_BUTTON        1UL << 16

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
      Serial.print("JOY_"), Serial.print("OK"), Serial.println(" is pushed");
    }
#endif
    user_onOkButtonPressed();
    checked |= BIT_OK_BUTTON;
  }

  if(pressed & BIT_UP_BUTTON && user_onDPadUpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("UP");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadUpButtonPressed();
    checked |= BIT_UP_BUTTON;
  }

  if(pressed & BIT_RIGHT_BUTTON && user_onDPadRightButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("RIGHT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadRightButtonPressed();
    checked |= BIT_RIGHT_BUTTON;
  }

  if(pressed & BIT_DOWN_BUTTON && user_onDPadDownButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("DOWN");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadDownButtonPressed();
    checked |= BIT_DOWN_BUTTON;
  }

  if(pressed & BIT_LEFT_BUTTON && user_onDPadLeftButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("LEFT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadLeftButtonPressed();
    checked |= BIT_LEFT_BUTTON;
  }

  if(pressed && user_onButtonsPressed) {
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      Serial.print("onButtonsPressed()"), Serial.println(" is called");
    }
#endif
    user_onButtonsPressed(pressed);
    checked |= pressed;
  }

  return checked;
}

uint32_t detectButtonPress(IRData decodedIRData) {
  uint32_t buttons = 0;

  switch (decodedIRData.protocol) {
    case SONY:
      switch (decodedIRData.command) {
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
      };
      break;
    case NEC:
      switch(decodedIRData.decodedRawData) {
        case 0xFF9867:
#if __RUNNING_LOG_ENABLED__
          Serial.println("0");
#endif
          buttons |= BIT_0_BUTTON;
          break;
        case 0xFFA25D:
#if __RUNNING_LOG_ENABLED__
          Serial.println("1");
#endif
          buttons |= BIT_1_BUTTON;
          break;
        case 0xFF629D:
#if __RUNNING_LOG_ENABLED__
          Serial.println("2");
#endif
          buttons |= BIT_2_BUTTON;
          break;
        case 0xFFE21D:
#if __RUNNING_LOG_ENABLED__
          Serial.println("3");
#endif
          buttons |= BIT_3_BUTTON;
          break;
        case 0xFF22DD:
#if __RUNNING_LOG_ENABLED__
          Serial.println("4");
#endif
          buttons |= BIT_4_BUTTON;
          break;
        case 0xFF02FD:
#if __RUNNING_LOG_ENABLED__
          Serial.println("5");
#endif
          buttons |= BIT_5_BUTTON;
          break;
        case 0xFFC23D:
#if __RUNNING_LOG_ENABLED__
          Serial.println("6");
#endif
          buttons |= BIT_6_BUTTON;
          break;
        case 0xFFE01F:
#if __RUNNING_LOG_ENABLED__
          Serial.println("7");
#endif
          buttons |= BIT_7_BUTTON;
          break;
        case 0xFFA857:
#if __RUNNING_LOG_ENABLED__
          Serial.println("8");
#endif
          buttons |= BIT_8_BUTTON;
          break;
        case 0xFF906F:
#if __RUNNING_LOG_ENABLED__
          Serial.println("9");
#endif
          buttons |= BIT_9_BUTTON;
          break;
        case 0xFF38C7:
#if __RUNNING_LOG_ENABLED__
          Serial.println("OK");
#endif
          buttons |= BIT_OK_BUTTON;
          break;
        case 0xFF6897:
#if __RUNNING_LOG_ENABLED__
          Serial.println("*");
#endif
          buttons |= BIT_ASTERISK_BUTTON;
          break;
        case 0xFFB04F:
#if __RUNNING_LOG_ENABLED__
          Serial.println("#");
#endif
          buttons |= BIT_SHARP_BUTTON;
          break;
        case 0xFF18E7:
#if __RUNNING_LOG_ENABLED__
          Serial.println("UP");
#endif
          buttons |= BIT_UP_BUTTON;
          break;
        case 0xFF5AA5:
#if __RUNNING_LOG_ENABLED__
          Serial.println("RIGHT");
#endif
          buttons |= BIT_RIGHT_BUTTON;
          break;
        case 0xFF4AB5:
#if __RUNNING_LOG_ENABLED__
          Serial.println("DOWN");
#endif
          buttons |= BIT_DOWN_BUTTON;
          break;
        case 0xFF10EF:
#if __RUNNING_LOG_ENABLED__
          Serial.println("LEFT");
#endif
          buttons |= BIT_LEFT_BUTTON;
          break;
        default:
          NULL;
#if __RUNNING_LOG_ENABLED__
          Serial.println("Unknown Code");
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

void IRController::setOnButtonsPressed(void (*function)(uint32_t)) {
  user_onButtonsPressed = function;
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
