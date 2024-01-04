#include "Joystick_Action.h"

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
