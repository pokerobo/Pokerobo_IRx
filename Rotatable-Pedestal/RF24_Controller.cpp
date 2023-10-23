#include "RF24_Controller.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BIT_UP_BUTTON     1U << 0
#define BIT_RIGHT_BUTTON  1U << 1
#define BIT_DOWN_BUTTON   1U << 2
#define BIT_LEFT_BUTTON   1U << 3
#define BIT_START_BUTTON  1U << 4
#define BIT_SELECT_BUTTON 1U << 5
#define BIT_ANALOG_BUTTON 1U << 6

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

int RF24Controller::loop() {
  if (radio.available()) {
    uint8_t msg[__RF24_MESSAGE_LENGTH__] = {0};
    radio.read(&msg, sizeof(msg));

    bool ok = false;
    uint16_t buttons;
    uint16_t jX, jY;
    uint32_t count;

#if __RF24_BINARY_ENCODING__
    if (msg[0] == 'J' && msg[1] == 'S') {
      ok = true;
      buttons = decodeInteger(&msg[2], 2);
      jX = decodeInteger(&msg[4], 2);
      jY = decodeInteger(&msg[6], 2);
      count = decodeInteger(&msg[8], 4);
    }
#else
    char cmdId;
    sscanf(msg, "%c,%d,%d,%d,%d", &cmdId, &buttons, &jX, &jY, &count);
    if (cmdId == 'J') {
      ok = true;
    }
#endif

#if __RUNNING_LOG_ENABLED__
    char c_[11], b_[7], x_[7], y_[7];
    debugLog("#", ltoa(count, c_, 10), " - ", "Buttons", ": ", itoa(buttons, b_, 10),
        "; ", "X", ": ", itoa(jX, x_, 10),
        "; ", "Y", ": ", itoa(jY, y_, 10));
#endif

    if (!ok) {
      return -1;
    }

    uint16_t pressed = processButtonPress(buttons);
    if (pressed) {
      return pressed;
    }

    return processJoystickChange(jX, jY, _onLeftJoystickChanged, 'L');
  }
  return 0;
}

uint16_t RF24Controller::processButtonPress(uint16_t pressed) {
  uint16_t checked = 0;

  if(pressed & BIT_START_BUTTON && _onStartButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "START", " is pushed");
    }
#endif
    _onStartButtonPressed();
    checked |= BIT_START_BUTTON;
  }

  if(pressed & BIT_SELECT_BUTTON && _onSelectButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "SELECT", " is pushed");
    }
#endif
    _onSelectButtonPressed();
    checked |= BIT_SELECT_BUTTON;
  }

  if(pressed & BIT_ANALOG_BUTTON && _onAnalogButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "ANALOG", " is pushed");
    }
#endif
    _onAnalogButtonPressed();
    checked |= BIT_ANALOG_BUTTON;
  }

  if(pressed & BIT_UP_BUTTON && _onDPadUpButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "UP", " is pushed");
    }
#endif
    _onDPadUpButtonPressed();
    checked |= BIT_UP_BUTTON;
  }

  if(pressed & BIT_RIGHT_BUTTON && _onDPadRightButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "RIGHT", " is pushed");
    }
#endif
    _onDPadRightButtonPressed();
    checked |= BIT_RIGHT_BUTTON;
  }

  if(pressed & BIT_DOWN_BUTTON && _onDPadDownButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "DOWN", " is pushed");
    }
#endif
    _onDPadDownButtonPressed();
    checked |= BIT_DOWN_BUTTON;
  }

  if(pressed & BIT_LEFT_BUTTON && _onDPadLeftButtonPressed) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      debugLog("JOY", "_", "PAD", "_", "LEFT", " is pushed");
    }
#endif
    _onDPadLeftButtonPressed();
    checked |= BIT_LEFT_BUTTON;
  }

  return checked;
}

int RF24Controller::processJoystickChange(int nJoyX, int nJoyY, void (*onChange)(int, int), const char label) {

  nJoyX = map(nJoyX, 0, 1024, NUM_RANGE_X, -NUM_RANGE_X);
  nJoyY = map(nJoyY, 0, 1024, -NUM_RANGE_Y, NUM_RANGE_Y);

  if (nJoyX >= MIN_BOUND_X || nJoyX <= -MIN_BOUND_X || nJoyY >= MIN_BOUND_Y || nJoyY <= -MIN_BOUND_Y) {
#if __RUNNING_LOG_ENABLED__
    if (_debugEnabled) {
      char l_[2] = { label, '\0' };
      debugLog("RF24", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ");
      char x_[7], y_[7];
      debugLog(" - ", "X", ": ", itoa(nJoyX, x_, 10));
      debugLog(" - ", "Y", ": ", itoa(nJoyY, y_, 10));
    }
#endif
    if (onChange) {
      onChange(nJoyX, nJoyY);
      return 1;
    } else {
#if __RUNNING_LOG_ENABLED__
      if (_debugEnabled) {
        char l_[2] = { label, '\0' };
        debugLog("RF24", "Controller", "::", "process", "JoystickChange", "()", " - ", l_, ": ", "not registered");
      }
#endif
      return -1;
    }
  }
  return 0;
}

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

void RF24Controller::setOnnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
}

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
