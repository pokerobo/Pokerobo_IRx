#include <Servo.h>

#ifndef __PEDESTAL_HANDLER_H__
#define __PEDESTAL_HANDLER_H__

#define HORIZONTAL_SERVO_PIN  8
#define VERTICAL_SERVO_PIN    9

#define MOVING_AMOUNT         2

static Servo horizontalServo;
static Servo verticalServo;

class PedestalHandler {
  public:
    PedestalHandler();
    PedestalHandler(int hPin, int vPin);
    void begin();
    void test();

    bool verticalServoUp();
    bool verticalServoDown();
    bool horizontalServoLeft();
    bool horizontalServoRight();  

    bool changeHorizontalServo(int hDelta);
    bool changeVerticalServo(int vDelta);
    bool change(int hDelta, int vDelta);

  private:
    int count;
    int horizontalServoPin;
    int verticalServoPin;
};

#endif
