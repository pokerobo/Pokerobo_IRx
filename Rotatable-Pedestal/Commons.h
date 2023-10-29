#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <Arduino.h>

#define __HCPCA9685_ENABLED__   1

#define __LOADING_LOG_ENABLED__ 1
#define __RUNNING_LOG_ENABLED__ 0

#define __PS2_LOGGING_ENABLED__ 1

#define CONTROLLER_IR                   0
#define CONTROLLER_IR_DEVICE_PANASONIC  0
#define CONTROLLER_IR_DEVICE_SONY       0

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

void debugLog(char* s0);
void debugLog(char* s0, char* s1);
void debugLog(char* s0, char* s1, char* s2);
void debugLog(char* s0, char* s1, char* s2, char* s3);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14, char* s15);
#endif
