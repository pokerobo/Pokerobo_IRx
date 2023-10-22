#ifndef __RF24_CONTROLLER_H__
#define __RF24_CONTROLLER_H__

#define __RF24_BINARY_ENCODING__ 1
#define __RF24_MESSAGE_LENGTH__ 32

#include "Commons.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MIN_BOUND_X      32
#define MIN_BOUND_Y      32

#define NUM_RANGE_X      128
#define NUM_RANGE_Y      128

class RF24Controller {
  public:
    RF24Controller();
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
    uint16_t processButtonPress(uint16_t buttons);
    int processJoystickChange(int, int, void (*function)(int, int), const char label);
  private:
    uint64_t address = 0xE8E8F0F0E1LL;
    bool debugEnabled;
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
