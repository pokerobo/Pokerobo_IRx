#ifndef __POKEROBO_CAR_COMMONS_H__
#define __POKEROBO_CAR_COMMONS_H__

#include <Arduino.h>

#define __HCPCA9685_ENABLED__           1

#define __LOADING_LOG_ENABLED__         1
#define __RUNNING_LOG_ENABLED__         1

#define __RF24_RUNNING_LOG__            0
#define __DISPATCHER_RUNNING_LOG__      0
#define __ROBOCAR_RUNNING_LOG__         0
#define __PEDESTAL_RUNNING_LOG__        0

#define  __DEBUG_LOG_RF24_LISTENER__    0

#define __PS2_LOGGING_ENABLED__         1

#define CONTROLLER_PEDESTAL             1
#define CONTROLLER_ROBOCAR              1

#define CONTROLLER_IR                   0
#define CONTROLLER_IR_DEVICE_PANASONIC  0
#define CONTROLLER_IR_DEVICE_SONY       0

#define CONTROLLER_PS2                  2
#define CONTROLLER_RF24                 24
#define CONTROLLER                      CONTROLLER_RF24

#ifndef int_abs
#define int_abs(x) ((x)>0?(x):-(x))
#endif

#ifndef int_min
#define int_min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef int_max
#define int_max(a,b) ((a)>(b)?(a):(b))
#endif

class FlagChecker {
  public:
    FlagChecker();
    bool isAvailable();
  private:
    bool enabled;
    uint32_t limit;
};

class HangingDetector {
  public:
    HangingDetector(void (*trigger)() = NULL, uint16_t limit = 10);
    void begin(void (*trigger)() = NULL, uint16_t limit = 10);
    bool check(bool ok);
    void reset();
  private:
    uint16_t _count = 0;
    uint16_t _limit = 10;
    bool _triggered = false;
    void (*_trigger)();
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
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14, char* s15, char* s16, char* s17, char* s18);
void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14, char* s15, char* s16, char* s17, char* s18, char* s19);

#endif
