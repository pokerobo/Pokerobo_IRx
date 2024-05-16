#include "Joystick_Action.h"

#ifndef __POKEROBO_RC_CLIENT_H__
#if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V1
const uint8_t JoystickAction::messageSize = 0 // SIGNATURE
    + sizeof(uint16_t) // pressingFlags
    + sizeof(uint16_t) // Joystick-X
    + sizeof(uint16_t) // Joystick-Y
    + sizeof(uint32_t);
#else
const uint8_t JoystickAction::messageSize = 0 // SIGNATURE
    + sizeof(uint16_t) // pressingFlags
    + sizeof(uint16_t) // togglingFlags
    + sizeof(uint16_t) // Joystick-X
    + sizeof(uint16_t) // Joystick-Y
    + sizeof(uint32_t);
#endif

JoystickAction::JoystickAction(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  update(buttons, x, y, extras);
}

void JoystickAction::update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras) {
  _pressingFlags = buttons;
  _x = x;
  _y = y;
  _extras = extras;
}

JoystickAction::JoystickAction(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras) {
  update(x, y, pressingFlags, togglingFlags, extras);
}

void JoystickAction::update(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras) {
  _x = x;
  _y = y;
  _pressingFlags = pressingFlags;
  _togglingFlags = togglingFlags;
  _extras = extras;
}

uint16_t JoystickAction::getPressingFlags() {
  return _pressingFlags;
}

uint16_t JoystickAction::getTogglingFlags() {
  return _togglingFlags;
}

uint16_t JoystickAction::getX() {
  return _x;
}

uint16_t JoystickAction::getY() {
  return _y;
}

uint32_t JoystickAction::getExtras() {
  return _extras;
}

uint8_t JoystickAction::length() {
  return messageSize;
}

bool decodeMessage(uint8_t* msg, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras);
bool decodeMessage(uint8_t* msg, char* cmd,
    uint16_t* x, uint16_t* y,
    uint16_t* pressingFlags,
    uint16_t* togglingFlags,
    uint32_t* extras);

void* JoystickAction::deserialize(uint8_t* buf) {
  if (buf == NULL) {
    return NULL;
  }
  #if __JOYSTICK_MESSAGE_STRUCTURE__ == CONTROL_PACKET_V1
  decodeMessage(buf, NULL, &(_pressingFlags), &(_x), &(_y), &(_extras));
  #else
  decodeMessage(buf, NULL, &(_x), &(_y), &(_pressingFlags), &(_togglingFlags), &(_extras));
  #endif

  return this;
}

bool decodeMessage(uint8_t* msg, char* cmd, uint16_t* buttons, uint16_t* x, uint16_t* y, uint32_t* extras) {
  if (cmd != NULL) {
    if (msg[0] != cmd[0] || msg[1] != cmd[1]) {
      return false;
    }
    msg += 2;
  }
  *buttons = decodeInteger(&msg[0], 2);
  *x = decodeInteger(&msg[2], 2);
  *y = decodeInteger(&msg[4], 2);
  *extras = decodeInteger(&msg[6], 4);
  return true;
}

bool decodeMessage(uint8_t* msg, char* cmd,
    uint16_t* x, uint16_t* y,
    uint16_t* pressingFlags,
    uint16_t* togglingFlags,
    uint32_t* extras) {
  if (cmd != NULL) {
    if (msg[0] != cmd[0] || msg[1] != cmd[1]) {
      return false;
    }
    msg += 2;
  }
  *x = decodeInteger(&msg[0], 2);
  *y = decodeInteger(&msg[2], 2);
  *pressingFlags = decodeInteger(&msg[4], 2);
  *togglingFlags = decodeInteger(&msg[6], 2);
  *extras = decodeInteger(&msg[8], 4);
  return true;
}
#endif//__POKEROBO_RC_CLIENT_H__
