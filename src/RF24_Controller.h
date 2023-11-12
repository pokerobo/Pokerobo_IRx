#ifndef __RF24_CONTROLLER_H__
#define __RF24_CONTROLLER_H__

#include "Commons.h"
#include "Event_Trigger.h"

#ifndef __RF24_LOADING_LOG__
#define __RF24_LOADING_LOG__  __LOADING_LOG_ENABLED__
#endif//__RF24_LOADING_LOG__

#ifndef __RF24_RUNNING_LOG__
#define __RF24_RUNNING_LOG__  __RUNNING_LOG_ENABLED__
#endif//__RF24_RUNNING_LOG__

#ifndef __RF24_BINARY_ENCODING__
#define __RF24_BINARY_ENCODING__  1
#endif

#ifndef __RF24_MESSAGE_LENGTH__
#define __RF24_MESSAGE_LENGTH__   32
#endif

#ifndef __RF24_DEFAULT_ADDRESS__
#define __RF24_DEFAULT_ADDRESS__  0x123456789ABCDEF0LL
#endif

#ifndef __RF24_CE_PIN__
#define __RF24_CE_PIN__           9
#endif

#ifndef __RF24_CSN_PIN__
#define __RF24_CSN_PIN__          10
#endif

#define RF24_JOYSTICK_DEADZONE_X  32
#define RF24_JOYSTICK_DEADZONE_Y  32

#define RF24_JOYSTICK_RANGE_X     255
#define RF24_JOYSTICK_RANGE_Y     255

#define CLICKING_FLAGS            0

class RF24Controller {
  public:
    RF24Controller(uint64_t address=__RF24_DEFAULT_ADDRESS__, bool debugEnabled=true);
    void begin();
    int loop();
    void set(HangingDetector* hangingDetector);
    void set(EventTrigger* eventTrigger);
    void setOnStartButtonPressed(void (*function)());
    void setOnSelectButtonPressed(void (*function)());
    void setOnAnalogButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnLeftJoystickChanged(void (*function)(int, int));
    void setOnRightJoystickChanged(void (*function)(int, int));
  protected:
    bool available();
    bool checkButtonPress(uint16_t pressed, uint16_t mask);
    uint16_t processButtonPress(uint16_t buttons);
    bool isJoystickChanged(int, int);
    int processJoystickChange(int, int, char);
  private:
    uint64_t _address = __RF24_DEFAULT_ADDRESS__;
    bool _debugEnabled;
    uint16_t _clickingTrail;
    HangingDetector* _hangingDetector = NULL;
    EventTrigger* _eventTrigger = NULL;
    void (*_onStartButtonPressed)();
    void (*_onSelectButtonPressed)();
    void (*_onAnalogButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onLeftJoystickChanged)(int, int);
    void (*_onRightJoystickChanged)(int, int);
};

uint32_t decodeInteger(uint8_t* arr, int length);

#endif
