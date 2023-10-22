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
    void onStartButtonPressed(void (*function)());
    void onSelectButtonPressed(void (*function)());
    void onAnalogButtonPressed(void (*function)());
    void onDPadUpButtonPressed(void (*function)());
    void onDPadRightButtonPressed(void (*function)());
    void onDPadDownButtonPressed(void (*function)());
    void onDPadLeftButtonPressed(void (*function)());
    void onLeftJoystickChanged(void (*function)(int, int));
  protected:
    uint16_t processButtonPress(uint16_t buttons);
    int processJoystickChange(int, int, void (*function)(int, int), const char label);
  private:
    uint64_t address = 0xE8E8F0F0E1LL;
    bool debugEnabled;
    void (*user_onStartButtonPressed)();
    void (*user_onSelectButtonPressed)();
    void (*user_onAnalogButtonPressed)();
    void (*user_onDPadUpButtonPressed)();
    void (*user_onDPadRightButtonPressed)();
    void (*user_onDPadDownButtonPressed)();
    void (*user_onDPadLeftButtonPressed)();
    void (*user_onLeftJoystickChanged)(int, int);
};

uint32_t decodeInteger(uint8_t* arr, int length);

#endif
