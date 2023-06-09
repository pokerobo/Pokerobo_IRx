#include <Servo.h>

#ifndef __PEDESTAL_HANDLER_H__
#define __PEDESTAL_HANDLER_H__

#define HORIZONTAL_SERVO_PIN 8
#define VERTICAL_SERVO_PIN 9

static Servo horizontalServo;
static Servo verticalServo;

class PedestalHandler {
  public:
    PedestalHandler();
    void begin();
    void test();

    bool verticalServoUp();
    bool verticalServoDown();
    bool horizontalServoLeft();
    bool horizontalServoRight();  

  private:
    int count;
};

#endif
