#ifndef __POKEROBO_CAR_PROGRAM_MANAGER_H__
#define __POKEROBO_CAR_PROGRAM_MANAGER_H__

#include "Display_Adapter.h"
#include "Input_Listener.h"
#include "Program_Collection.h"

#define DASHBOARD_FLOW_CONFIGURING            1
#define DASHBOARD_FLOW_DISPATCHING            2

class ProgramManager {
  public:
    ProgramManager();
    void set(InputListener* inputListener);
    void set(DisplayAdapter* displayAdapter);
    void set(HangingDetector* hangingDetector);
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
    InputListener* _inputListener = NULL;
    DisplayAdapter* _displayAdapter = NULL;
    HangingDetector* _hangingDetector = NULL;
    ProgramCollection* _programCollection = NULL;
    uint8_t _flow = DASHBOARD_FLOW_DISPATCHING;
};

#endif
