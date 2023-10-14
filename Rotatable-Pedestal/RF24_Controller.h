#ifndef __RF24_CONTROLLER_H__
#define __RF24_CONTROLLER_H__

#include "Commons.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MIN_BOUND_X      3
#define MIN_BOUND_Y      3

#define NUM_RANGE_X      6
#define NUM_RANGE_Y      6

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
    int processButtonPress(uint8_t buttons);
    int processJoystickChange(int, int, void (*function)(int, int), const char c[]);
  private:
    uint64_t address = 0xE8E8F0F0E1LL;
    bool debugEnabled;
    uint16_t debugLimit;
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
