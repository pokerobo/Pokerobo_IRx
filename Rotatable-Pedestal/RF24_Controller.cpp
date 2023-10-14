#include "RF24_Controller.h"

#define __BINARY_ENCODING__ 1

#define FLAG_UP     1 << 0
#define FLAG_RIGHT  1 << 1
#define FLAG_DOWN   1 << 2
#define FLAG_LEFT   1 << 3
#define FLAG_START  1 << 4
#define FLAG_SELECT 1 << 5
#define FLAG_ANALOG 1 << 6

RF24 radio(9, 10);  // CE, CSN

RF24Controller::RF24Controller() {
  debugEnabled = true;
}

void RF24Controller::begin() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
}

int RF24Controller::loop() {
  if (radio.available()) {
    uint8_t msg[32] = {0};
    radio.read(&msg, sizeof(msg));

    bool ok = false;
    uint8_t buttons;
    uint16_t jX, jY;
    uint32_t count;

#ifdef __BINARY_ENCODING__
    if (msg[0] == 'J') {
      ok = true;
      buttons = msg[1];
      jX = decodeInteger(&msg[2], 2);
      jY = decodeInteger(&msg[4], 2);
      count = decodeInteger(&msg[6], 4);
    }
#else
    char cmdId;
    sscanf(msg, "%c,%d,%d,%d,%d", &cmdId, &buttons, &jX, &jY, &count);
    if (cmdId == 'J') {
      ok = true;
    }
#endif

#if (__RUNNING_LOG_ENABLED__)
    Serial.print("No: "); Serial.print(count);
    Serial.print("; Buttons: "); Serial.print(buttons);
    Serial.print("; X="); Serial.print(jX);
    Serial.print("; Y="); Serial.print(jY);
    Serial.println();
#endif

    if (!ok) {
      return -1;
    }

    uint8_t pressed = processButtonPress(buttons);
    if (pressed) {
      return pressed;
    }

    return processJoystickChange(jX, jY, user_onLeftJoystickChanged, "Left Joystick");
  }
  return 0;
}

int RF24Controller::processButtonPress(uint8_t pressed) {
  uint8_t checked = 0;

  if(pressed & FLAG_START && user_onStartButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("START");
      Serial.println(" is pushed");
    }
#endif
    user_onStartButtonPressed();
    checked |= FLAG_START;
  }

  if(pressed & FLAG_SELECT && user_onSelectButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("SELECT");
      Serial.println(" is pushed");
    }
#endif
    user_onSelectButtonPressed();
    checked |= FLAG_START;
  }

  if(pressed & FLAG_UP && user_onDPadUpButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("UP");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadUpButtonPressed();
    checked |= FLAG_UP;
  }

  if(pressed & FLAG_RIGHT && user_onDPadRightButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("RIGHT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadRightButtonPressed();
    checked |= FLAG_RIGHT;
  }

  if(pressed & FLAG_DOWN && user_onDPadDownButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("DOWN");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadDownButtonPressed();
    checked |= FLAG_DOWN;
  }

  if(pressed & FLAG_LEFT && user_onDPadLeftButtonPressed) {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("JOY_"), Serial.print("PAD_"), Serial.print("LEFT");
      Serial.println(" is pushed");
    }
#endif
    user_onDPadLeftButtonPressed();
    checked |= FLAG_LEFT;
  }

  return checked;
}

int RF24Controller::processJoystickChange(int nJoyX, int nJoyY, void (*onChange)(int, int), const char label[]) {

  //
  nJoyX = map(nJoyX, 0, 726, NUM_RANGE_X, -NUM_RANGE_X);
  nJoyY = map(nJoyY, 0, 726, NUM_RANGE_Y, -NUM_RANGE_Y);
  //
  if (nJoyX >= MIN_BOUND_X || nJoyX <= -MIN_BOUND_X || nJoyY >= MIN_BOUND_Y || nJoyY <= -MIN_BOUND_Y)
  {
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("RF24Controller::");
      Serial.print("processJoystickChange() - ");
      Serial.print(label);
      Serial.println(": ");
      Serial.print("- X: ");
      Serial.println(nJoyX);
      Serial.print("- Y: ");
      Serial.println(nJoyY);
    }
#endif
    if (onChange) {
      onChange(nJoyX, nJoyY);
      return 1;
    } else {
#if (__RUNNING_LOG_ENABLED__)
      if (debugEnabled) {
        Serial.print("RF24Controller::");
        Serial.print("processJoystickChange() - ");
        Serial.print(label);
        Serial.print(": ");
        Serial.println("event listener has not registered");
      }
#endif
      return -1;
    }
  }
  return 0;
}

void RF24Controller::onStartButtonPressed(void (*function)()) {
  user_onStartButtonPressed = function;
};

void RF24Controller::onSelectButtonPressed(void (*function)()) {
  user_onSelectButtonPressed = function;
};

void RF24Controller::onDPadUpButtonPressed(void (*function)()) {
  user_onDPadUpButtonPressed = function;
};

void RF24Controller::onDPadRightButtonPressed(void (*function)()) {
  user_onDPadRightButtonPressed = function;
};

void RF24Controller::onDPadDownButtonPressed(void (*function)()) {
  user_onDPadDownButtonPressed = function;
};

void RF24Controller::onDPadLeftButtonPressed(void (*function)()) {
  user_onDPadLeftButtonPressed = function;
};

void RF24Controller::onLeftJoystickChanged(void (*function)(int, int)) {
  user_onLeftJoystickChanged = function;
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