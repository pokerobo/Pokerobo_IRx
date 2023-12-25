#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

#include "Event_Trigger.h"
#include "RF24_Controller.h"

class EventListener {
  public:
    void set(EventTrigger* eventTrigger);
    void set(RF24Controller* rf24Controller);
    void begin();
    int check();
  private:
    EventTrigger* _eventTrigger = NULL;
    RF24Controller* _rf24Controller = NULL;
};

#endif
