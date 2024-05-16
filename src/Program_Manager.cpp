#include "Program_Manager.h"

#define PROGRAM_MENU_TOGGLE_BUTTON          MASK_ANALOG_BUTTON

ProgramManager::ProgramManager() {
  _programCollection = new ProgramCollection();
}

void ProgramManager::set(InputListener* inputListener) {
  _inputListener = inputListener;
};

void ProgramManager::set(DisplayAdapter* displayAdapter) {
  _displayAdapter = displayAdapter;
}

void ProgramManager::set(HangingDetector* hangingDetector) {
  _hangingDetector = hangingDetector;
};

bool ProgramManager::add(ProgramCapsule* programCapsule) {
  return _programCollection->add(programCapsule);
}

int ProgramManager::enterDashboard_(MasterContext* context, JoystickAction* action) {
  if (_displayAdapter != NULL) {
    _displayAdapter->render(_programCollection);
  }
  return 0;
}

int ProgramManager::processDashboard_(MasterContext* context, JoystickAction* action) {
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

int ProgramManager::leaveDashboard_(MasterContext* context, JoystickAction* action) {
  _programCollection->setFocusAsCurrent();
  return 0;
}

int ProgramManager::enterProgram_(MasterContext* context, JoystickAction* action) {
  return 0;
}

int ProgramManager::executeProgram_(MasterContext* context, JoystickAction* action, MovingCommand* command) {
  return _programCollection->getCurrentItem()->check(action, command);
}

int ProgramManager::leaveProgram_(MasterContext* context, JoystickAction* action) {
  return 0;
}

void ProgramManager::changeFlow_(uint8_t flow) {
  if (flow == _flow) return;
  _flow = flow;
  if (_displayAdapter != NULL) {
    _displayAdapter->clear();
  }
}

void ProgramManager::begin() {
}

int ProgramManager::check() {
  MasterContext context;
  JoystickAction action;
  MovingCommand command;

  int ok = _inputListener->read(&context, &action, &command);

  if (_hangingDetector != NULL) {
    _hangingDetector->check(ok == 1);
  }

  if (ok != 1) {
    return ok;
  }

  return wait_(move_(&context, &action, &command));
}

int ProgramManager::wait_(int state) {
  delay(10);
  return state;
}

int ProgramManager::move_(MasterContext* context, JoystickAction* action, MovingCommand* command) {
  uint16_t pressingFlags = action->getPressingFlags();
  uint16_t togglingFlags = action->getTogglingFlags();
  if ((togglingFlags & PROGRAM_MENU_TOGGLE_BUTTON)) {
    switch(_flow) {
      case DASHBOARD_FLOW_DISPATCHING:
        leaveProgram_(context, action);
        changeFlow_(DASHBOARD_FLOW_CONFIGURING);
        return enterDashboard_(context, action);
      case DASHBOARD_FLOW_CONFIGURING:
        leaveDashboard_(context, action);
        changeFlow_(DASHBOARD_FLOW_DISPATCHING);
        return enterProgram_(context, action);
    }
  }

  switch(_flow) {
    case DASHBOARD_FLOW_CONFIGURING:
      return processDashboard_(context, action);
    case DASHBOARD_FLOW_DISPATCHING:
      return executeProgram_(context, action, command);
  }
}
