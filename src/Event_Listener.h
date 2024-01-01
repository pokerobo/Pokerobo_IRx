#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

#include "Display_Adapter.h"
#include "Event_Trigger.h"
#include "RF24_Controller.h"
#include "Program_Collection.h"

#define DASHBOARD_FLOW_CONFIGURATION        1
#define DASHBOARD_FLOW_EXECUTION            2

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
    int executeProgram_(JoystickAction* action);
    int leaveProgram_(JoystickAction* action);
    void changeFlow_(uint8_t flow);
  private:
    DisplayAdapter* _displayAdapter = NULL;
    EventTrigger* _eventTrigger = NULL;
    RF24Controller* _rf24Controller = NULL;
    ProgramCollection* _programCollection = NULL;
    uint8_t _flow = DASHBOARD_FLOW_EXECUTION;
};

#endif
