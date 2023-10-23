#ifndef __PEDESTAL_GROUP_H__
#define __PEDESTAL_GROUP_H__

#include "Pedestal_Handler.h"

#define PEDESTALS_MAX   8

class PedestalGroup {
  public:
    PedestalGroup(PedestalHandler* pedestalHandlers[]);
    void begin();
    void processStartButtonPressedEvent();
    void processAnalogButtonPressedEvent();
    void processDPadUpButtonPressedEvent();
    void processDPadRightButtonPressedEvent();
    void processDPadDownButtonPressedEvent();
    void processDPadLeftButtonPressedEvent();
    void processLeftJoystickChangeEvent(int nJoyX, int nJoyY);
    void processRightJoystickChangeEvent(int nJoyX, int nJoyY);
    bool change(int hUnits, int vUnits);
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
};

#endif
