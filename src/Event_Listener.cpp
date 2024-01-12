#include "Event_Listener.h"

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

ProgramCollection programCollection;

EventListener::EventListener() {
  _programCollection = &programCollection;
}

void EventListener::set(DisplayAdapter* displayAdapter) {
  _displayAdapter = displayAdapter;
}

void EventListener::set(RF24Controller* rf24Controller) {
  _rf24Controller = rf24Controller;
};

bool EventListener::add(ProgramCapsule* programCapsule) {
  return _programCollection->add(programCapsule);
}

int EventListener::enterDashboard_(JoystickAction* action) {
  if (_displayAdapter != NULL) {
    _displayAdapter->render(_programCollection);
  }
  return 0;
}

int EventListener::processDashboard_(JoystickAction* action) {
  uint8_t toggle = action->getTogglingFlags();
  if (toggle & (1U >> 12)) { // LEFT -> BACK
    changeFlow_(DASHBOARD_FLOW_DISPATCHING);
  } else
  if (toggle & (1U >> 13)) { // UP -> PREV
    _programCollection->moveFocusUp();
    if (_displayAdapter != NULL) {
      _displayAdapter->render(_programCollection);
    }
  } else
  if (toggle & (1U >> 14)) { // DOWN -> NEXT
    _programCollection->moveFocusDown();
    if (_displayAdapter != NULL) {
      _displayAdapter->render(_programCollection);
    }
  } else
  if (toggle & (1U >> 15)) { // RIGHT -> SELECT
    _programCollection->setFocusAsCurrent();
    if (_displayAdapter != NULL) {
      _displayAdapter->render(_programCollection);
    }
  }
  return 0;
}

int EventListener::leaveDashboard_(JoystickAction* action) {
  _programCollection->setFocusAsCurrent();
  return 0;
}

int EventListener::enterProgram_(JoystickAction* action) {
  return 0;
}

int EventListener::executeProgram_(JoystickAction* action, MovingCommand* command) {
  return _programCollection->getCurrentItem()->check(action, command);
}

int EventListener::leaveProgram_(JoystickAction* action) {
  return 0;
}

void EventListener::changeFlow_(uint8_t flow) {
  if (flow == _flow) return;
  _flow = flow;
  if (_displayAdapter != NULL) {
    _displayAdapter->clear();
  }
}

void EventListener::begin() {
}

int EventListener::check() {
  JoystickAction action;
  MovingCommand command;

  int ok = _rf24Controller->read(NULL, &action, &command);

  if (ok != 1) {
    return ok;
  }

  return wait_(move_(&action, &command));
}

int EventListener::wait_(int state) {
  delay(10);
  return state;
}

int EventListener::move_(JoystickAction* action, MovingCommand* command) {
  uint16_t pressingFlags = action->getPressingFlags();
  uint16_t togglingFlags = action->getTogglingFlags();
  if ((togglingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    switch(_flow) {
      case DASHBOARD_FLOW_DISPATCHING:
        leaveProgram_(action);
        changeFlow_(DASHBOARD_FLOW_CONFIGURING);
        return enterDashboard_(action);
      case DASHBOARD_FLOW_CONFIGURING:
        leaveDashboard_(action);
        changeFlow_(DASHBOARD_FLOW_DISPATCHING);
        return enterProgram_(action);
    }
  }

  switch(_flow) {
    case DASHBOARD_FLOW_CONFIGURING:
      return processDashboard_(action);
    case DASHBOARD_FLOW_DISPATCHING:
      return executeProgram_(action, command);
  }
}
