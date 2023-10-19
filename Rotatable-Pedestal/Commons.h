#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <Arduino.h>

#define __HCPCA9685_ENABLED__   1

#define __LOADING_LOG_ENABLED__ 0
#define __RUNNING_LOG_ENABLED__ 0

#define __PS2_LOGGING_ENABLED__ 0

#define CONTROLLER_IR     1
#define CONTROLLER_PS2    2
#define CONTROLLER_RF24   24
#define CONTROLLER CONTROLLER_RF24

class FlagChecker {
  public:
    FlagChecker();
    bool isAvailable();
  private:
    bool enabled;
    uint32_t limit;
};

#endif
