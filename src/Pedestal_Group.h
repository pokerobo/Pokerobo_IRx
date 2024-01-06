#ifndef __PEDESTAL_GROUP_H__
#define __PEDESTAL_GROUP_H__

#include "Pedestal_Handler.h"

#define PEDESTALS_MAX                 8
#define PEDESTAL_DANCING_STEPS_MAX    32

#ifndef PEDESTAL_WAITING_MAX
#define PEDESTAL_WAITING_MAX          100
#endif//PEDESTAL_WAITING_MAX

class PedestalGroup {
  public:
    PedestalGroup(PedestalHandler* pedestalHandlers[]);
    void begin();
    void reset();
    void verticalServoUp();
    void horizontalServoRight();
    void verticalServoDown();
    void horizontalServoLeft();
    void changeByJoystick(int nJoyX, int nJoyY);
    bool change(int hUnits, int vUnits);
    void setHorizontalPosition(int hPos);
    void setVerticalPosition(int vPos);
    void autoDance();
  private:
    uint8_t _pedestalsTotal = 0;
    PedestalHandler* _pedestalHandlers[PEDESTALS_MAX] = {};
    void verticalServoUpFor(PedestalHandler *pedestalHandler);
    void horizontalServoRightFor(PedestalHandler *pedestalHandler);
    void verticalServoDownFor(PedestalHandler *pedestalHandler);
    void horizontalServoLeftFor(PedestalHandler *pedestalHandler);
    void changeByJoystickFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY);
    WaitingCounter _waitingCounter;
    bool _sceneDirection = true;
    int _sceneStep = -1;
    int _sceneStepsTotal = 19;
    int8_t _sceneHPos[PEDESTAL_DANCING_STEPS_MAX] = {
      30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120
    };
    int8_t _sceneVPos[PEDESTAL_DANCING_STEPS_MAX] = {
      30, 35, 40, 45, 50, 45, 40, 35, 30, 30, 30, 35, 40, 45,  50,  45,  40,  33,  30
    };
};

#endif
