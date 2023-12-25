#include "Event_Listener.h"

void EventListener::set(EventTrigger* eventTrigger) {
  _eventTrigger = eventTrigger;
};

void EventListener::set(RF24Controller* rf24Controller) {
  _rf24Controller = rf24Controller;
};

void EventListener::begin() {
}

int EventListener::check() {
  JoystickAction action;
  MovingCommand command;

  int ok = _rf24Controller->read(&action, &command);

  if (ok != 1) {
    return ok;
  }

  if (_eventTrigger == NULL) {
    return -1;
  }

  if(_eventTrigger->checkButtonPress(action.getPressingFlags(), MASK_START_BUTTON)) {
    return _eventTrigger->next();
  }

  _eventTrigger->autoplay();

  _eventTrigger->processEvents(&action, &command);

  return ok;
}
