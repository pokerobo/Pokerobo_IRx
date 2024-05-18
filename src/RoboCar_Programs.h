#ifndef __POKEROBO_CAR_ROBOCAR_PROGRAMS_H__
#define __POKEROBO_CAR_ROBOCAR_PROGRAMS_H__

#include "Commons.h"
#include "Display_Adapter.h"
#include "Program_Capsule.h"
#include "Joystick_Action.h"
#include "Moving_Command.h"

#include "Line_Detector.h"
#include "Pedestal_Group.h"
#include "RoboCar_Handler.h"

class RemoteControlCar: public ProgramCapsule {
  public:
    RemoteControlCar(char* title,
        DisplayAdapter* displayAdapter=NULL,
        RoboCarHandler* roboCarHandler=NULL);
    uint8_t getId();
    void set(DisplayAdapter* displayAdapter);
    void set(RoboCarHandler* roboCarHandler);
    virtual char* getTitle();
    virtual int begin();
    virtual int check(void* action, void* command);
    virtual int close();
  protected:
    virtual void showSpeedometer_(JoystickAction* action, MovingCommand* command);
    DisplayAdapter* getDisplayAdapter();
    RoboCarHandler* getRoboCarHandler();
    bool isDebugEnabled();
  private:
    char* _title;
    DisplayAdapter* _displayAdapter = NULL;
    RoboCarHandler* _roboCarHandler = NULL;
    bool _debugEnabled = true;
};

class LineFollowingCar: public RemoteControlCar {
  public:
    LineFollowingCar(char* title,
        DisplayAdapter* displayAdapter=NULL,
        RoboCarHandler* roboCarHandler=NULL): RemoteControlCar(title,
            displayAdapter,
            roboCarHandler) {};
    uint8_t getId();
    void set(DisplayAdapter* displayAdapter);
    void set(RoboCarHandler* roboCarHandler);
    void set(LineDetector* lineDetector);
    int check(void* action, void* command);
  protected:
    void showSpeedometer_(JoystickAction* action, MovingCommand* command);
  private:
    LineDetector* _lineDetector = NULL;
};

class DancingPuppetCar: public RemoteControlCar {
  public:
    DancingPuppetCar(char* title,
        DisplayAdapter* displayAdapter=NULL,
        RoboCarHandler* roboCarHandler=NULL,
        PedestalGroup* pedestalGroup=NULL,
        bool autoPedestal=false): RemoteControlCar(title,
            displayAdapter,
            roboCarHandler) {
              _pedestalGroup = pedestalGroup;
              _autoPedestal = autoPedestal;
            };
    uint8_t getId();
    void set(DisplayAdapter* displayAdapter);
    void set(RoboCarHandler* roboCarHandler);
    void set(PedestalGroup* pedestalGroup, bool autoPedestal=false);
    int begin();
    int check(void* action, void* command);
    int close();
  private:
    PedestalGroup* _pedestalGroup = NULL;
    bool _autoPedestal = false;
};

#endif
