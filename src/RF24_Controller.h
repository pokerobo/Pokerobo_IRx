#ifndef __RF24_CONTROLLER_H__
#define __RF24_CONTROLLER_H__

#include "Commons.h"

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
#define __RF24_DEFAULT_ADDRESS__  0xE8E8F0F0E1LL
#endif

#ifndef __RF24_CE_PIN__
#define __RF24_CE_PIN__           9
#endif

#ifndef __RF24_CSN_PIN__
#define __RF24_CSN_PIN__          10
#endif

#define MIN_BOUND_X               32
#define MIN_BOUND_Y               32

#define NUM_RANGE_X               128
#define NUM_RANGE_Y               128

#define CLICKING_FLAGS            0

class RF24Controller {
  public:
    RF24Controller(uint64_t address=__RF24_DEFAULT_ADDRESS__, bool debugEnabled=true);
    void begin();
    int loop();
    void setOnStartButtonPressed(void (*function)());
    void setOnSelectButtonPressed(void (*function)());
    void setOnAnalogButtonPressed(void (*function)());
    void setOnDPadUpButtonPressed(void (*function)());
    void setOnDPadRightButtonPressed(void (*function)());
    void setOnDPadDownButtonPressed(void (*function)());
    void setOnDPadLeftButtonPressed(void (*function)());
    void setOnnLeftJoystickChanged(void (*function)(int, int));
  protected:
    bool available();
    bool checkButtonPress(uint16_t pressed, uint16_t mask);
    uint16_t processButtonPress(uint16_t buttons);
    int processJoystickChange(int, int, void (*function)(int, int), const char label);
  private:
    uint64_t _address = __RF24_DEFAULT_ADDRESS__;
    bool _debugEnabled;
    uint16_t _pressFlag;
    void (*_onStartButtonPressed)();
    void (*_onSelectButtonPressed)();
    void (*_onAnalogButtonPressed)();
    void (*_onDPadUpButtonPressed)();
    void (*_onDPadRightButtonPressed)();
    void (*_onDPadDownButtonPressed)();
    void (*_onDPadLeftButtonPressed)();
    void (*_onLeftJoystickChanged)(int, int);
};

uint32_t decodeInteger(uint8_t* arr, int length);

#endif
