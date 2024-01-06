#ifndef __JOYSTICK_ACTION_H__
#define __JOYSTICK_ACTION_H__

#include "Commons.h"
#include "Message_Exchange.h"

#define CONTROL_PACKET_V1                 1
#define CONTROL_PACKET_V2                 2

#ifndef __JOYSTICK_MESSAGE_STRUCTURE__
#define __JOYSTICK_MESSAGE_STRUCTURE__    CONTROL_PACKET_V1
#endif//__JOYSTICK_MESSAGE_STRUCTURE__

class JoystickAction {
  public:
    static const uint8_t messageSize;
    JoystickAction(uint16_t buttons=0, uint16_t x=0, uint16_t y=0, uint32_t extras=0);
    JoystickAction(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras);
    void update(uint16_t buttons, uint16_t x, uint16_t y, uint32_t extras);
    void update(uint16_t x, uint16_t y, uint16_t pressingFlags, uint16_t togglingFlags, uint32_t extras);
    uint16_t getPressingFlags();
    uint16_t getTogglingFlags();
    uint16_t getX();
    uint16_t getY();
    uint32_t getExtras();
    void* deserialize(uint8_t* buf);
  private:
    uint16_t _pressingFlags = 0;
    uint16_t _togglingFlags = 0;
    uint16_t _x = 0;
    uint16_t _y = 0;
    uint32_t _extras = 0;
};

#endif
