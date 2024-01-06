#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

#include "Display_Adapter.h"
#include "Event_Trigger.h"
#include "RF24_Controller.h"
#include "Program_Collection.h"

#define DASHBOARD_FLOW_CONFIGURING            1
#define DASHBOARD_FLOW_DISPATCHING            2

class EventListener {
  public:
    EventListener();
    void set(DisplayAdapter* displayAdapter);
    void set(EventTrigger* eventTrigger);
    void set(RF24Controller* rf24Controller);
    bool add(ProgramCapsule* programCapsule);
    void begin();
    int check();
  protected:
    int enterDashboard_(JoystickAction* action);
    int processDashboard_(JoystickAction* action);
    int leaveDashboard_(JoystickAction* action);
    int enterProgram_(JoystickAction* action);
    int executeProgram_(JoystickAction* action, MovingCommand* command);
    int leaveProgram_(JoystickAction* action);
    void changeFlow_(uint8_t flow);
    int move_(JoystickAction* action, MovingCommand* command);
    int wait_(int state);
  private:
    DisplayAdapter* _displayAdapter = NULL;
    EventTrigger* _eventTrigger = NULL;
    RF24Controller* _rf24Controller = NULL;
    ProgramCollection* _programCollection = NULL;
    uint8_t _flow = DASHBOARD_FLOW_DISPATCHING;
};

#endif
