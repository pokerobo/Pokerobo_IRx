#include "Input_Controller.h"

void InputController::setOnStartButtonPressed(void (*function)()) {
  _onStartButtonPressed = function;
};

void InputController::setOnSelectButtonPressed(void (*function)()) {
  _onSelectButtonPressed = function;
};

void InputController::setOnDPadUpButtonPressed(void (*function)()) {
  _onDPadUpButtonPressed = function;
};

void InputController::setOnDPadRightButtonPressed(void (*function)()) {
  _onDPadRightButtonPressed = function;
};

void InputController::setOnDPadDownButtonPressed(void (*function)()) {
  _onDPadDownButtonPressed = function;
};

void InputController::setOnDPadLeftButtonPressed(void (*function)()) {
  _onDPadLeftButtonPressed = function;
};

void InputController::setOnDPadButtonPressed(void (*function)(uint16_t)) {
  _onDPadButtonPressed = function;
};

void InputController::setOnLeftJoystickChanged(void (*function)(int, int)) {
  _onLeftJoystickChanged = function;
};

void InputController::setOnRightJoystickChanged(void (*function)(int, int)) {
  _onRightJoystickChanged = function;
};
