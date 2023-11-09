#ifndef __PEDESTAL_GROUP_H__
#define __PEDESTAL_GROUP_H__

#include "Pedestal_Handler.h"

#define PEDESTALS_MAX                 8
#define PEDESTAL_DANCING_STEPS_MAX    32

#ifndef PEDESTAL_WAITING_MAX
#define PEDESTAL_WAITING_MAX          1
#endif//PEDESTAL_WAITING_MAX

class PedestalGroup {
  public:
    PedestalGroup(PedestalHandler* pedestalHandlers[]);
    void begin();
    void reset();
    void processStartButtonPressedEvent();
    void processAnalogButtonPressedEvent();
    void processDPadUpButtonPressedEvent();
    void processDPadRightButtonPressedEvent();
    void processDPadDownButtonPressedEvent();
    void processDPadLeftButtonPressedEvent();
    void processLeftJoystickChangeEvent(int nJoyX, int nJoyY);
    void processRightJoystickChangeEvent(int nJoyX, int nJoyY);
    bool change(int hUnits, int vUnits);
    void setHorizontalPosition(int hPos);
    void setVerticalPosition(int vPos);
    void autoDance();
  private:
    uint8_t _pedestalsTotal = 0;
    PedestalHandler* _pedestalHandlers[PEDESTALS_MAX] = {};
    void processStartButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processAnalogButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processDPadUpButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processDPadRightButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processDPadDownButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processDPadLeftButtonPressedEventFor(PedestalHandler *pedestalHandler);
    void processLeftJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY);
    void processRightJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY);
    uint8_t _waitingLimit = PEDESTAL_WAITING_MAX;
    uint8_t _waitingCount = 0;
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
