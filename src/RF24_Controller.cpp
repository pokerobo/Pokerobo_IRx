#include "RF24_Controller.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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

int RF24Controller::read(JoystickAction* action, MovingCommand* command) {
  if (!available()) {
    return 0;
  }
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

  #if __RF24_RUNNING_LOG__
  char c_[11], b_[7], x_[7], y_[7];
  debugLog("#", ltoa(count, c_, 10), " - ", "Buttons", ": ", itoa(buttons, b_, 10),
      "; ", "X", ": ", itoa(jX, x_, 10),
      "; ", "Y", ": ", itoa(jY, y_, 10));
  #endif

  if (!ok) {
    return -1;
  }

  action->update(buttons, jX, jY, count);
  command->update(leftWeight, leftDirection, rightWeight, rightDirection);

  return 1;
}

int RF24Controller::loop() {
  JoystickAction action;
  MovingCommand command;

  int ok = read(&action, &command);

  if (ok == 1) {
    if (_eventProcessor != NULL) {
      _eventProcessor->processEvents(&action, &command);
      return 0xff;
    }

    uint16_t pressed = processButtonPress(action.getPressingFlags());
    if (pressed) {
      return pressed;
    }

    return processJoystickChange(action.getX(), action.getY(), 'L');
  }

  return ok;
}

bool RF24Controller::isDebugEnabled() {
  return _debugEnabled;
}

bool RF24Controller::isJoystickChanged(int nJoyX, int nJoyY) {
  #if defined(RF24_JOYSTICK_CHECKING_CHANGE)
  return nJoyX >= RF24_JOYSTICK_DEADZONE_X || nJoyX <= -RF24_JOYSTICK_DEADZONE_X ||
      nJoyY >= RF24_JOYSTICK_DEADZONE_Y || nJoyY <= -RF24_JOYSTICK_DEADZONE_Y;
  #else
  return true;
  #endif
}

int RF24Controller::adjustJoystickX(int nJoyX) {
  return map(nJoyX, 0, 1024, -RF24_JOYSTICK_RANGE_X, RF24_JOYSTICK_RANGE_X);
}

int RF24Controller::adjustJoystickY(int nJoyY) {
  return map(nJoyY, 0, 1024, -RF24_JOYSTICK_RANGE_Y, RF24_JOYSTICK_RANGE_Y);
}

void RF24Controller::set(HangingDetector* hangingDetector) {
  _hangingDetector = hangingDetector;
};

void RF24Controller::set(EventProcessor* eventProcessor) {
  _eventProcessor = eventProcessor;
};
