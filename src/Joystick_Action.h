#ifndef __JOYSTICK_ACTION_H__
#define __JOYSTICK_ACTION_H__

#include "Commons.h"

class JoystickAction {
  public:
    JoystickAction(uint16_t buttons=0, uint16_t x=0, uint16_t y=0, uint32_t extras=0);
    void update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
    uint16_t getClickingFlags();
    uint16_t getPressingFlags();
    uint16_t getX();
    uint16_t getY();
    uint32_t getExtras();
  private:
    uint16_t _clickingFlags = 0;
    uint16_t _pressingFlags = 0;
    uint16_t _x = 0;
    uint16_t _y = 0;
    uint32_t _extras = 0;
};

#endif
