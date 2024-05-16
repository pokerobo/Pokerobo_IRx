#include "RF24_Listener.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24* _radio = NULL;

RF24Listener::RF24Listener(uint64_t address, bool debugEnabled) {
  _radio = new RF24(__RF24_CE_PIN__, __RF24_CSN_PIN__);  // CE, CSN
  _address = address;
  _debugEnabled = debugEnabled;
}

void RF24Listener::begin() {
  _radio->begin();
  // _radio->setRetries(5, 15);
  _radio->openReadingPipe(0, _address);
  _radio->startListening();
}

bool RF24Listener::available() {
  bool tx_ok, tx_fail, rx_ready;
  _radio->whatHappened(tx_ok, tx_fail, rx_ready);
  bool ok = _radio->available();
  #if __DEBUG_LOG_RF24_LISTENER__
  if (isDebugEnabled()) {
    Serial.print("tx_ok"), Serial.print(':'), Serial.print(tx_ok), Serial.print(';'),
    Serial.print("tx_fail"), Serial.print(':'), Serial.print(tx_fail), Serial.print(';'),
    Serial.print("rx_ready"), Serial.print(':'), Serial.print(rx_ready), Serial.print(';'),
    Serial.print("available()"), Serial.print(':'), Serial.print(ok),
    Serial.println();
  }
  #endif
  if (!ok) {
    bool connected = _radio->isChipConnected();
    if (!connected) {
      debugLog("RF24 is not connected");
    }
  }
  if (_hangingDetector) {
    _hangingDetector->check(ok);
  }
  return ok;
}

int RF24Listener::read(MasterContext* context, JoystickAction* action, MovingCommand* command) {
  if (!available()) {
    return 0;
  }
  uint8_t msg[__RF24_MESSAGE_LENGTH__] = {0};
  _radio->read(&msg, sizeof(msg));

  bool ok = false;

  if (msg[0] == 'J' && msg[1] == 'S') {
    ok = true;
    action->deserialize(msg + 2);
    command->deserialize(msg + 2 + JoystickAction::messageSize);
  } else {
    uint8_t offset = 0;

    if (context != NULL) {
      context->deserialize(msg + offset);
    }
    offset += MasterContext::messageSize;

    ok = true;

    if (action != NULL) {
      action->deserialize(msg + offset);
    }
    offset += JoystickAction::messageSize;

    if (command != NULL) {
      command->deserialize(msg + offset);
    }
  }

  #if __DEBUG_LOG_RF24_LISTENER__
  if (isDebugEnabled()) {
    if (ok) {
      char c_[11], b_[7], t_[7], x_[7], y_[7];
      debugLog("#", ltoa(action->getExtras(), c_, 10), " - ",
          "pressing", "Flags", ": ", itoa(action->getPressingFlags(), b_, 10), "; ",
          "toggling", "Flags", ": ", itoa(action->getTogglingFlags(), t_, 10), "; ",
          "X", ": ", itoa(action->getX(), x_, 10), "; ",
          "Y", ": ", itoa(action->getY(), y_, 10));
    } else {
      
    }
  }
  #endif

  return ok ? 1 : -1;
}

int RF24Listener::loop() {
  MasterContext context;
  JoystickAction action;
  MovingCommand command;

  int ok = read(&context, &action, &command);

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

bool RF24Listener::isDebugEnabled() {
  return _debugEnabled;
}

bool RF24Listener::isJoystickChanged(int nJoyX, int nJoyY) {
  #if defined(RF24_JOYSTICK_CHECKING_CHANGE)
  return nJoyX >= RF24_JOYSTICK_DEADZONE_X || nJoyX <= -RF24_JOYSTICK_DEADZONE_X ||
      nJoyY >= RF24_JOYSTICK_DEADZONE_Y || nJoyY <= -RF24_JOYSTICK_DEADZONE_Y;
  #else
  return true;
  #endif
}

int RF24Listener::adjustJoystickX(int nJoyX) {
  return map(nJoyX, 0, 1024, -RF24_JOYSTICK_RANGE_X, RF24_JOYSTICK_RANGE_X);
}

int RF24Listener::adjustJoystickY(int nJoyY) {
  return map(nJoyY, 0, 1024, -RF24_JOYSTICK_RANGE_Y, RF24_JOYSTICK_RANGE_Y);
}

void RF24Listener::set(HangingDetector* hangingDetector) {
  _hangingDetector = hangingDetector;
};

void RF24Listener::set(EventProcessor* eventProcessor) {
  _eventProcessor = eventProcessor;
};
