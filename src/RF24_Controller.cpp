#include "RF24_Controller.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MASK_UP_BUTTON     1U << 0
#define MASK_RIGHT_BUTTON  1U << 1
#define MASK_DOWN_BUTTON   1U << 2
#define MASK_LEFT_BUTTON   1U << 3
#define MASK_START_BUTTON  1U << 4
#define MASK_SELECT_BUTTON 1U << 5
#define MASK_ANALOG_BUTTON 1U << 6

RF24 radio(__RF24_CE_PIN__, __RF24_CSN_PIN__);  // CE, CSN

RF24Controller::RF24Controller(uint64_t address, bool debugEnabled) {
  _address = address;
  _debugEnabled = debugEnabled;
}

void RF24Controller::begin() {
  radio.begin();
  radio.openReadingPipe(0, _address);
  radio.startListening();
}

bool RF24Controller::available() {
  bool tx_ds, tx_df, rx_dr;
  radio.whatHappened(tx_ds, tx_df, rx_dr);
  bool ok = radio.available();
  if (!ok) {
    bool connected = radio.isChipConnected();
    if (!connected) {
      debugLog("RF24 is not connected");
    }
  }
  if (_hangingDetector) {
    _hangingDetector->check(ok);
  }
  return ok;
}

int RF24Controller::loop() {
  if (available()) {
    uint8_t msg[__RF24_MESSAGE_LENGTH__] = {0};
    radio.read(&msg, sizeof(msg));

    bool ok = false;
    uint16_t buttons;
    uint16_t jX, jY;
    uint32_t count;

    uint8_t directionFlags;
    uint8_t leftDirection;
    uint8_t rightDirection;
    uint8_t leftWeight;
    uint8_t rightWeight;

#if __RF24_BINARY_ENCODING__
    if (msg[0] == 'J') {
      if (msg[1] == 'S') {
        ok = true;
        buttons = decodeInteger(&msg[2], 2);
        jX = decodeInteger(&msg[4], 2);
        jY = decodeInteger(&msg[6], 2);
        count = decodeInteger(&msg[8], 4);
        directionFlags = msg[12];
        leftDirection = directionFlags & 0b0011;
        rightDirection = (directionFlags & 0b1100) >> 2;
        leftWeight = msg[13];
        rightWeight = msg[14];
      }
    }
#else
    char cmdId;
    sscanf(msg, "%c,%d,%d,%d,%d", &cmdId, &buttons, &jX, &jY, &count);
    if (cmdId == 'J') {
      ok = true;
    }
#endif

#if __RF24_RUNNING_LOG__
    char c_[11], b_[7], x_[7], y_[7];
    debugLog("#", ltoa(count, c_, 10), " - ", "Buttons", ": ", itoa(buttons, b_, 10),
        "; ", "X", ": ", itoa(jX, x_, 10),
        "; ", "Y", ": ", itoa(jY, y_, 10));
#endif

    if (!ok) {
      return -1;
    }

    if (_eventTrigger != NULL) {
      JoystickAction action(buttons, jX, jY, count);
      MovingCommand command(leftWeight, leftDirection, rightWeight, rightDirection);
      _eventTrigger->processEvents(&action, &command);
      return 0xff;
    }

    uint16_t pressed = processButtonPress(buttons);
    if (pressed) {
      return pressed;
    }

    return processJoystickChange(jX, jY, 'L');
  }
  return 0;
}

bool RF24Controller::checkButtonPress(uint16_t pressed, uint16_t mask) {
#if CLICKING_FLAGS
  if (pressed & mask) {
    _clickingTrail |= mask;
  } else {
    if (_clickingTrail & mask) {
      _clickingTrail &= (~mask);
      return true;
    }
  }
  return false;
#else
  return pressed & mask;
#endif
}

uint16_t RF24Controller::processButtonPress(uint16_t pressed) {
  uint16_t checked = 0;

  if(checkButtonPress(pressed, MASK_START_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "START", " is pushed");
    }
#endif
    if (_onStartButtonPressed != NULL) {
      _onStartButtonPressed();
      checked |= MASK_START_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_SELECT_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "SELECT", " is pushed");
    }
#endif
    if (_onSelectButtonPressed != NULL) {
      _onSelectButtonPressed();
      checked |= MASK_SELECT_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_ANALOG_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "ANALOG", " is pushed");
    }
#endif
    if (_onAnalogButtonPressed != NULL) {
      _onAnalogButtonPressed();
      checked |= MASK_ANALOG_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_UP_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "UP", " is pushed");
    }
#endif
    if (_onDPadUpButtonPressed != NULL) {
      _onDPadUpButtonPressed();
      checked |= MASK_UP_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_RIGHT_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "RIGHT", " is pushed");
    }
#endif
    if (_onDPadRightButtonPressed != NULL) {
      _onDPadRightButtonPressed();
      checked |= MASK_RIGHT_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_DOWN_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "DOWN", " is pushed");
    }
#endif
    if (_onDPadDownButtonPressed != NULL) {
      _onDPadDownButtonPressed();
      checked |= MASK_DOWN_BUTTON;
    }
  }

  if(checkButtonPress(pressed, MASK_LEFT_BUTTON)) {
#if __RF24_RUNNING_LOG__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "LEFT", " is pushed");
    }
#endif
    if (_onDPadLeftButtonPressed != NULL) {
      _onDPadLeftButtonPressed();
      checked |= MASK_LEFT_BUTTON;
    }
  }

  return checked;
}

bool RF24Controller::isJoystickChanged(int nJoyX, int nJoyY) {
  return nJoyX >= RF24_JOYSTICK_DEADZONE_X || nJoyX <= -RF24_JOYSTICK_DEADZONE_X ||
      nJoyY >= RF24_JOYSTICK_DEADZONE_Y || nJoyY <= -RF24_JOYSTICK_DEADZONE_Y;
}

int RF24Controller::processJoystickChange(int nJoyX, int nJoyY, char label) {

  nJoyX = map(nJoyX, 0, 1024, -RF24_JOYSTICK_RANGE_X, RF24_JOYSTICK_RANGE_X);
  nJoyY = map(nJoyY, 0, 1024, -RF24_JOYSTICK_RANGE_Y, RF24_JOYSTICK_RANGE_Y);

#if defined(RF24_JOYSTICK_CHECKING_CHANGE)
  if (!isJoystickChanged(nJoyX, nJoyY)) {
    return 0;
  }
#endif

#if __RF24_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { 'L', '\0' };
    debugLog("RF24", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ");
    char x_[7], y_[7];
    debugLog(" - ", "X", ": ", itoa(nJoyX, x_, 10));
    debugLog(" - ", "Y", ": ", itoa(nJoyY, y_, 10));
  }
#endif

  if (label == 'L') {
    if (_onLeftJoystickChanged) {
      _onLeftJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else if (_eventTrigger != NULL) {
      _eventTrigger->processLeftJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

  if (label == 'R') {
    if (_onRightJoystickChanged) {
      _onRightJoystickChanged(nJoyX, nJoyY);
      return 1;
    } else if (_eventTrigger != NULL) {
      _eventTrigger->processRightJoystickChangeEvent(nJoyX, nJoyY);
      return 1;
    }
  }

#if __RF24_RUNNING_LOG__
  if (_debugEnabled) {
    char l_[2] = { 'L', '\0' };
    debugLog("RF24", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ", "not registered");
  }
#endif

  return -1;
}

void RF24Controller::set(HangingDetector* hangingDetector) {
  _hangingDetector = hangingDetector;
};

void RF24Controller::set(EventTrigger* eventTrigger) {
  _eventTrigger = eventTrigger;
};

void RF24Controller::setOnStartButtonPressed(void (*function)()) {
  _onStartButtonPressed = function;
};

void RF24Controller::setOnSelectButtonPressed(void (*function)()) {
  _onSelectButtonPressed = function;
};

void RF24Controller::setOnAnalogButtonPressed(void (*function)()) {
  _onAnalogButtonPressed = function;
};

void RF24Controller::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void RF24Controller::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void RF24Controller::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void RF24Controller::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void RF24Controller::setOnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
}

void RF24Controller::setOnRightJoystickChanged(void (*function)(int, int)) {
  _onRightJoystickChanged = function;
};

uint32_t decodeInteger(uint8_t* arr, int length) {
  uint32_t a0 = arr[0];
  uint32_t a1 = arr[1];
  uint32_t a2 = arr[2];
  uint32_t a3 = arr[3];
  if (length == 2) {
    return (a1 << 8) | a0;
  }
  if (length == 4) {
    return (a3 << 24) | (a2 << 16) | (a1 << 8) | a0;
  }
  return 0;
}
