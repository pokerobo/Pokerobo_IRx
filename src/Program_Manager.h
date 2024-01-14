#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__

#include "Display_Adapter.h"
#include "Input_Listener.h"
#include "Program_Collection.h"

#define DASHBOARD_FLOW_CONFIGURING            1
#define DASHBOARD_FLOW_DISPATCHING            2

class ProgramManager {
  public:
    ProgramManager();
    void set(DisplayAdapter* displayAdapter);
    void set(InputListener* inputListener);
    bool add(ProgramCapsule* programCapsule);
    void begin();
    int check();
  protected:
    int enterDashboard_(MasterContext* context, JoystickAction* action);
    int processDashboard_(MasterContext* context, JoystickAction* action);
    int leaveDashboard_(MasterContext* context, JoystickAction* action);
    int enterProgram_(MasterContext* context, JoystickAction* action);
    int executeProgram_(MasterContext* context, JoystickAction* action, MovingCommand* command);
    int leaveProgram_(MasterContext* context, JoystickAction* action);
    void changeFlow_(uint8_t flow);
    int move_(MasterContext* context, JoystickAction* action, MovingCommand* command);
    int wait_(int state);
  private:
    DisplayAdapter* _displayAdapter = NULL;
    InputListener* _inputListener = NULL;
    ProgramCollection* _programCollection = NULL;
    uint8_t _flow = DASHBOARD_FLOW_DISPATCHING;
};

#endif
